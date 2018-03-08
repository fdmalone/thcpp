from mpi4py import MPI
import time
import numpy

class KMeans:

    def __init__(self, grid, max_it=100, thresh=1e-6, comm=None):

        self.comm = comm
        self.nprocs = comm.Get_size()
        self.rank = comm.Get_rank()
        self.nlocal = len(grid) // self.nprocs
        self.neven = self.nlocal * self.nprocs
        self.nleft = len(grid) - self.neven 
        # distribute grid
        self.grid = self.distribute(grid)
        self.global_grid = grid
        self.max_it = max_it
        self.thresh = thresh

    def distribute(self, array):
        # pad last processor grid with zeros to avoid scatterv
        if self.rank == 0:
            if self.nleft > 0:
                if len(array.shape) == 2:
                    pad_shape = [(0,self.nleft), (0,0)]
                else:
                    pad_shape = (0, self.nleft)
                sendbuf = numpy.pad(array, pad_shape, 'constant')
            else:
                sendbuf = array
        else:
            sendbuf = None
        recvbuf = numpy.empty(array.shape)
        self.comm.Scatter(sendbuf, recvbuf, root=0)
        return recvbuf

    def classify(self, grid, centroids):
        ngs = grid.shape[0]
        nmu = centroids.shape[0]
        X = numpy.zeros(ngs, dtype=numpy.int32)
        X[:] = -1
        # simple double loop for now
        for ni in range(ngs):
            delta = numpy.sum((centroids-grid[ni,:])**2, axis=1)
            X[ni] = numpy.argmin(numpy.sqrt(delta))
        return X

    # calculates the new centroids and measures the distance with
    # respect to the old ones
    def centroids(self, X, nmu):
        ngs = self.grid.shape[0]
        cloc = numpy.zeros((nmu,3), dtype=numpy.float64)
        wloc = numpy.zeros(nmu, dtype=numpy.float64)
        cglobal = numpy.copy(cloc)
        wglobal = numpy.copy(wloc)
        for ni in range(ngs):
            #assert(w[ni] > 0.0)
            wloc[X[ni]] += self.weights[ni]
            cloc[X[ni],:] += self.weights[ni]*self.grid[ni,:]     
        self.comm.Allreduce(wloc, wglobal, op=MPI.SUM) 
        self.comm.Allreduce(cloc, cglobal, op=MPI.SUM) 
        return cglobal / wglobal[:,None]

    # finds the closest point in the grid for every centroid and returns the
    # set of indices
    def map2grid(self, centroids):
        if self.comm.rank == 0:
            X = numpy.sort(self.classify(centroids, self.global_grid))
            # look for repeated
            for i in range(X.shape[0]-1):
                assert( X[i] != X[i+1] ) 
            return X
        else:
            return None

    def kernel(self, weights, centroids):
        # Distribute weights among processors. This is to ensure that
        # weights are padded with zeros for processor with fewer grid
        # points.
        self.t_kmeans = time.time()
        self.weights = self.distribute(weights)
        ngs = self.grid.shape[0] 
        nmu = centroids.shape[0] 
        for t in range(self.max_it):
            # Per processor step
            X = self.classify(self.grid, centroids)
            # Global reduce
            c_new = self.centroids(X, nmu)
            if t == 67:
                print ("NEw: ", c_new)
            d = numpy.linalg.norm(c_new-centroids)
            if d < self.thresh:
                self.t_kmeans = time.time() - self.t_kmeans
                return self.map2grid(c_new)
            centroids = c_new
        self.t_kmeans = time.time() - self.t_kmeans
        return self.map2grid(c_new)
