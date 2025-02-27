// Copyright (C) 2004, 2006 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

#include "IpoptConfig.h"
#include "IpBlas.hpp"

#include <cstring>

// Prototypes for the BLAS routines
extern "C"
{
   /** BLAS Fortran function DDOT */
   double F77_FUNC(ddot, DDOT)(
      ipfint*       n,
      const double* x,
      ipfint*       incX,
      const double* y,
      ipfint*       incY
   );

   /** BLAS Fortran function DNRM2 */
   double F77_FUNC(dnrm2, DNRM2)(
      ipfint*       n,
      const double* x,
      ipfint*       incX
   );

   /** BLAS Fortran function DASUM */
   double F77_FUNC(dasum, DASUM)(
      ipfint*       n,
      const double* x,
      ipfint*       incX
   );

   /** BLAS Fortran function IDAMAX */
   ipfint F77_FUNC(idamax, IDAMAX)(
      ipfint*       n,
      const double* x,
      ipfint*       incX
   );

   /** BLAS Fortran subroutine DCOPY */
   void F77_FUNC(dcopy, DCOPY)(
      ipfint*       n,
      const double* x,
      ipfint*       incX,
      double*       y,
      ipfint*       incY
   );

   /** BLAS Fortran subroutine DAXPY */
   void F77_FUNC(daxpy, DAXPY)(
      ipfint*       n,
      const double* alpha,
      const double* x,
      ipfint*       incX,
      double*       y,
      ipfint*       incY
   );

   /** BLAS Fortran subroutine DSCAL */
   void F77_FUNC(dscal, DSCAL)(
      ipfint*       n,
      const double* alpha,
      const double* x,
      ipfint*       incX
   );

   /** BLAS Fortran subroutine DGEMV */
   void F77_FUNC(dgemv, DGEMV)(
      char*         trans,
      ipfint*       m,
      ipfint*       n,
      const double* alpha,
      const double* a,
      ipfint*       lda,
      const double* x,
      ipfint*       incX,
      const double* beta,
      double*       y,
      ipfint*       incY,
      int           trans_len
   );

   /** BLAS Fortran subroutine DSYMV */
   void F77_FUNC(dsymv, DSYMV)(
      char*         uplo,
      ipfint*       n,
      const double* alpha,
      const double* a,
      ipfint*       lda,
      const double* x,
      ipfint*       incX,
      const double* beta,
      double*       y,
      ipfint*       incY,
      int           uplo_len
   );

   /** BLAS Fortran subroutine DGEMM */
   void F77_FUNC(dgemm, DGEMM)(
      char*         transa,
      char*         transb,
      ipfint*       m,
      ipfint*       n,
      ipfint*       k,
      const double* alpha,
      const double* a,
      ipfint*       lda,
      const double* b,
      ipfint*       ldb,
      const double* beta,
      double*       c,
      ipfint*       ldc,
      int           transa_len,
      int           transb_len
   );

   /** BLAS Fortran subroutine DSYRK */
   void F77_FUNC(dsyrk, DSYRK)(
      char*         uplo,
      char*         trans,
      ipfint*       n,
      ipfint*       k,
      const double* alpha,
      const double* a,
      ipfint*       lda,
      const double* beta,
      double*       c,
      ipfint*       ldc,
      int           uplo_len,
      int           trans_len
   );

   /** BLAS Fortran subroutine DTRSM */
   void F77_FUNC(dtrsm, DTRSM)(
      char*         side,
      char*         uplo,
      char*         transa,
      char*         diag,
      ipfint*       m,
      ipfint*       n,
      const double* alpha,
      const double* a,
      ipfint*       lda,
      const double* b,
      ipfint*       ldb,
      int           side_len,
      int           uplo_len,
      int           transa_len,
      int           diag_len
   );
}

namespace Ipopt
{
Number IpBlasDdot(
   Index         size,
   const Number* x,
   Index         incX,
   const Number* y,
   Index         incY
)
{
   if( incX > 0 && incY > 0 )
   {
      ipfint n = size, INCX = incX, INCY = incY;

      return F77_FUNC(ddot, DDOT)(&n, x, &INCX, y, &INCY);
   }
   else
   {
      Number s = 0.0;

      for( ; size; --size, x += incX, y += incY )
      {
         s += *x * *y;
      }

      return s;
   }
}

Number IpBlasDnrm2(
   Index         size,
   const Number* x,
   Index         incX
)
{
   ipfint n = size, INCX = incX;

   return F77_FUNC(dnrm2, DNRM2)(&n, x, &INCX);
}

Number IpBlasDasum(
   Index         size,
   const Number* x,
   Index         incX
)
{
   ipfint n = size, INCX = incX;

   return F77_FUNC(dasum, DASUM)(&n, x, &INCX);
}

/** interface to FORTRAN routine IDAMAX */
Index IpBlasIdamax(
   Index         size,
   const Number* x,
   Index         incX
)
{
   ipfint n = size, INCX = incX;

   return (Index) F77_FUNC(idamax, IDAMAX)(&n, x, &INCX);
}

/** interface to FORTRAN routine DCOPY */
void IpBlasDcopy(
   Index         size,
   const Number* x,
   Index         incX,
   Number*       y,
   Index         incY
)
{
   if( incX > 0 )
   {
      ipfint N = size, INCX = incX, INCY = incY;

      F77_FUNC(dcopy, DCOPY)(&N, x, &INCX, y, &INCY);
   }
   else if( incY == 1 )
   {
      for( ; size; --size, ++y )
      {
         *y = *x;
      }
   }
   else
   {
      for( ; size; --size, y += incY )
      {
         *y = *x;
      }
   }
}

void IpBlasDaxpy(
   Index         size,
   Number        alpha,
   const Number* x,
   Index         incX,
   Number*       y,
   Index         incY
)
{
   if( incX > 0 )
   {
      ipfint N = size, INCX = incX, INCY = incY;

      F77_FUNC(daxpy, DAXPY)(&N, &alpha, x, &INCX, y, &INCY);
   }
   else if( incY == 1 )
   {
      for( ; size; --size, ++y )
      {
         *y += alpha * *x;
      }
   }
   else
   {
      for( ; size; --size, y += incY )
      {
         *y += alpha * *x;
      }
   }
}

void IpBlasDscal(
   Index   size,
   Number  alpha,
   Number* x,
   Index   incX
)
{
   ipfint N = size, INCX = incX;

   F77_FUNC(dscal, DSCAL)(&N, &alpha, x, &INCX);
}

void IpBlasDgemv(
   bool          trans,
   Index         nRows,
   Index         nCols,
   Number        alpha,
   const Number* A,
   Index         ldA,
   const Number* x,
   Index         incX,
   Number        beta,
   Number*       y,
   Index         incY
)
{
   ipfint M = nCols, N = nRows, LDA = ldA, INCX = incX, INCY = incY;

   char TRANS;
   if( trans )
   {
      TRANS = 'T';
   }
   else
   {
      TRANS = 'N';
   }

   F77_FUNC(dgemv, DGEMV)(&TRANS, &M, &N, &alpha, A, &LDA, x, &INCX, &beta, y, &INCY, 1);
}

void IpBlasDsymv(
   Index         n,
   Number        alpha,
   const Number* A,
   Index         ldA,
   const Number* x,
   Index         incX,
   Number        beta,
   Number*       y,
   Index         incY
)
{
   ipfint N = n, LDA = ldA, INCX = incX, INCY = incY;

   char UPLO = 'L';

   F77_FUNC(dsymv, DSYMV)(&UPLO, &N, &alpha, A, &LDA, x, &INCX, &beta, y, &INCY, 1);
}

void IpBlasDgemm(
   bool          transa,
   bool          transb,
   Index         m,
   Index         n,
   Index         k,
   Number        alpha,
   const Number* A,
   Index         ldA,
   const Number* B,
   Index         ldB,
   Number        beta,
   Number*       C,
   Index         ldC
)
{
   ipfint M = m, N = n, K = k, LDA = ldA, LDB = ldB, LDC = ldC;

   char TRANSA;
   if( transa )
   {
      TRANSA = 'T';
   }
   else
   {
      TRANSA = 'N';
   }
   char TRANSB;
   if( transb )
   {
      TRANSB = 'T';
   }
   else
   {
      TRANSB = 'N';
   }

   F77_FUNC(dgemm, DGEMM)(&TRANSA, &TRANSB, &M, &N, &K, &alpha, A, &LDA, B, &LDB, &beta, C, &LDC, 1, 1);
}

void IpBlasDsyrk(
   bool          trans,
   Index         ndim,
   Index         nrank,
   Number        alpha,
   const Number* A,
   Index         ldA,
   Number        beta,
   Number*       C,
   Index         ldC
)
{
   ipfint N = ndim, K = nrank, LDA = ldA, LDC = ldC;

   char UPLO = 'L';
   char TRANS;
   if( trans )
   {
      TRANS = 'T';
   }
   else
   {
      TRANS = 'N';
   }

   F77_FUNC(dsyrk, DSYRK)(&UPLO, &TRANS, &N, &K, &alpha, A, &LDA, &beta, C, &LDC, 1, 1);
}

void IpBlasDtrsm(
   bool          trans,
   Index         ndim,
   Index         nrhs,
   Number        alpha,
   const Number* A,
   Index         ldA,
   Number*       B,
   Index         ldB
)
{
   ipfint M = ndim, N = nrhs, LDA = ldA, LDB = ldB;

   char SIDE = 'L';
   char UPLO = 'L';
   char TRANSA;
   if( trans )
   {
      TRANSA = 'T';
   }
   else
   {
      TRANSA = 'N';
   }
   char DIAG = 'N';

   F77_FUNC(dtrsm, DTRSM)(&SIDE, &UPLO, &TRANSA, &DIAG, &M, &N, &alpha, A, &LDA, B, &LDB, 1, 1, 1, 1);
}

} // namespace Ipopt
