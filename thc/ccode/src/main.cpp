#include <iostream>
#include <vector>
#include <iomanip>
#include <time.h>
#include <complex>
#include <fftw3.h>
#include <mpi.h>
#include "distributed_matrix.h"
#include "matrix_operations.h"
#include "h5helper.h"
#include "utils.h"

int main(int argc, char* argv[])
{
  int rank, nprocs;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  bool root = (rank == 0);
  double sum1 = 0, sum2 = 0;
  const double one = 1.0, zero = 0.0;
  // Initialise blacs context.
  ContextHandler::BlacsHandler BH;
  // Read aos, grid and density
  //DistributedMatrix::Matrix aoR("supercell_atomic_orbitals.h5", "aoR", )
  //DistributedMatrix::Matrix grid("supercell_atomic_orbitals.h5", "real_space_grid",
                                 //block_rows, block_cols, ctxt, root_ctxt, ccyc_ctxt,
                                 //rank)
  //DistributedMatrix::Matrix density("supercell_atomic_orbitals.h5", "density",
                                    //block_rows, block_cols, ctxt, root_ctxt, ccyc_ctxt,
                                    //rank)
  DistributedMatrix::Matrix CZt("thc_data.h5", "CZt", BH.Root, rank);
  //DistributedMatrix::Matrix CCt("thc_data.h5", "CCt", BH.Root, rank);
  //CZt.scatter_block_cyclic(ctxt);
  //CCt.scatter_block_cyclic(ctxt);

  //double tlsq = clock();
  //if (root) {
    //std::cout << "Performing least squares solve." << std::endl;
    //std::cout << MatrixOperations::vector_sum(CZt.global_data) << " " << MatrixOperations::vector_sum(CCt.global_data) << " " << std::endl;
  //}
  //double local_sum, global_sum;
  //CZt.gather_block_cyclic(ctxt);
  //local_sum = MatrixOperations::vector_sum(CZt.local_data);
  //MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  //if (root) std::cout << "REDUCE : " << global_sum << " " << CZt.ncols << std::endl;
  //DistributedMatrix::Matrix C(CCt.nrows, CZt.ncols, block_rows, block_cols, ctxt, root_ctxt);
  //std::cout << C.nrows << " " << C.ncols << " " << CZt.ncols << " " << CZt.nrows << std::endl;
  //MatrixOperations::product(CCt, CZt, C);
  //C.gather_block_cyclic(ctxt);
  //if (root) std::cout << "MM: " << MatrixOperations::vector_sum(C.global_data) << std::endl;
  //MatrixOperations::least_squares(CCt, CZt);
  //CZt.gather_block_cyclic(ctxt);
  //tlsq = clock() - tlsq;
  //if (root) {
    //std::cout << "Time for least squares solve : " << tlsq / CLOCKS_PER_SEC << " seconds" << std::endl;
    //std::cout << "SUM: " << MatrixOperations::vector_sum(CZt.global_data) << std::endl;
    //H5Helper::write_interpolating_points(CZt.global_data, CZt.nrows, CZt.ncols);
  //}
  //DistributedMatrix::Matrix CZ(CZt.ncols, CZt.nrows, block_rows, block_cols, ctxt, root_ctxt, ccyc_ctxt);
  //MatrixOperations::transpose(CZt, CZ);
  //// will now have matrix in C order (nmu/nprocs, ngs)
  //CZ.redistribute_to_column_cyclic(ctxt);
  //// FFT interpolating vectors
  //if (root) {
    //std::cout << "Performing FFT of interpolating functions." << std::endl;
    //std::cout << "DIMS: " << CZ.nrows << " " << CZ.ncols << " " << CZ.ccyc_nrows << " " << CZ.ccyc_ncols << std::endl;
  //}
  //fftw_plan p;
  //int ngs = CZ.nrows;
  //int ng = (int)pow(ngs, 1.0/3.0);
  //int offset = ngs;
  //if (root) {
    //std::cout << "offset between grid points: " << offset << std::endl;
    //std::cout << "ngs: " << ng << std::endl;
  //}
  //for (int i = 0; i < CZ.ccyc_ncols; i++) {
    //std::vector<std::complex<double> > complex_data = UTILS::convert_double_to_complex(CZ.ccyc_data.data()+i*offset, ngs);
    //if (root) {
      //std::cout << "Performing FFT " << i+1 << " of " <<  CZ.ccyc_ncols << " " << i*offset << " " << complex_data.size() << std::endl;
    //}
    //p = fftw_plan_dft_3d(ng, ng, ng, reinterpret_cast<fftw_complex*> (complex_data.data()),
                         //reinterpret_cast<fftw_complex*>(CZ.fft_data.data()+i*offset),
                         //FFTW_FORWARD, FFTW_ESTIMATE);
    //fftw_execute(p);
    //fftw_destroy_plan(p);
  //}
  //CZ.gather_fft(ccyc_ctxt);
  //if (root) {
    //// The data which was in FTN order has been transposed, but we want to print in C
    //// order, so reverse dimensions.
    //H5Helper::write_fft(CZ.cglobal_data, CZ.ncols, CZ.nrows);
  //}
  //rfftwnd_one_real_to_complex(p, CZ.ccyc_data.data(), CZ.fft_data.data());
  MPI_Finalize();
}
