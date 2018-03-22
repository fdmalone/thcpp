#ifndef DISTRIBUTED_MATRIX_H
#define DISTRIBUTED_MATRIX_H 
#include <iostream>
#include <vector>

// Operations for matrices distributed using scalapack.
// Asumes column major (Fortran) order for layout.
// Thin wrapper to distribute data to and from block cyclic form.
namespace DistributedMatrix
{
  class Matrix
  {
    public:
      Matrix(int nrows, int ncols, int block_rows, int block_cols, int ctxt, int root_ctxt);
      Matrix(std::string filename, std::string name, int block_rows, int block_cols, int ctxt, int root_ctxt, int rank);
      ~Matrix();
      void gather_block_cyclic(int ctxt);
      void scatter_block_cyclic(int ctxt);
      void initialise_discriptors(int ctxt, int root_ctxt);
      // global matrix dimensions
      int nrows;
      int ncols;
      // Block sizes.
      int block_nrows;
      int block_ncols;
      // processor grid.
      int proc_nrows;
      int proc_ncols;
      // Location in processor grid. 
      int proc_row;
      int proc_col;
      // local number of rows and columns.
      int local_nrows;
      int local_ncols;
      // local leading dimension.
      int lld;
      // Variables for fancy selection of data. Not used set to 0 or 1.
      int izero;
      int init_row_idx, init_col_idx;
      // Variable for fortran interface.
      int info;
      // Global and local data stores for matrix.
      std::vector<double> global_data, local_data;
      std::vector<int> desc_global, desc_local;
  };
}
#endif
