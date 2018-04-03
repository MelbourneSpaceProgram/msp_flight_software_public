/*     ----------------------------------------------------------------
*
*                                astMath.cpp
*
*    this file contains miscellaneous math functions.
*
*                          Companion code for
*             Fundamentals of Astrodynamics and Applications
*                                  2013
*                            by David Vallado
*
*       (w) 719-573-2600, email dvallado@agi.com, davallado@gmail.com
*

*       ----------------------------------------------------------------      */

#include <external/sgp4/ast_math.h>

namespace astMath {

/* -----------------------------------------------------------------------------
 *
 *                           procedure matvecmult
 *
 *  this procedure multiplies a 3x3 matrix and a 3x1 vector together.
 *
 *  author        : david vallado                  719-573-2600    1 mar 2001
 *
 *  inputs          description                    range / units
 *    mat         - 3 x 3 matrix
 *    vec         - vector
 *
 *  outputs       :
 *    vecout      - vector result of mat * vec
 *
 *  locals        :
 *    row         - row index
 *    col         - column index
 *    ktr         - index
 *
 *  coupling      :
 * ---------------------------------------------------------------------------
 */

void matvecmult(std::vector<std::vector<double> > mat,
                //          double mat[3][3],
                double vec[3], double vecout[3]) {
    int row, ktr;

    for (row = 0; row <= 2; row++) {
        vecout[row] = 0.0;
        for (ktr = 0; ktr <= 2; ktr++)
            vecout[row] = vecout[row] + mat[row][ktr] * vec[ktr];
    }
}

/* -----------------------------------------------------------------------------
 *
 *                           procedure mattrans
 *
 *  this procedure finds the transpose of a matrix.
 *
 *  author        : david vallado                  719-573-2600    1 mar 2001
 *
 *  inputs          description                    range / units
 *    mat1        - matrix number 1
 *    mat1r       - matrix number 1 rows
 *    mat1c       - matrix number 1 columns
 *
 *  outputs       :
 *    mat2        - matrix result of transpose mat2
 *
 *  locals        :
 *    row         - row index
 *    col         - column index
 *
 *  coupling      :
 * ---------------------------------------------------------------------------
 */

void mattrans(std::vector<std::vector<double> > mat1,
              std::vector<std::vector<double> > &mat2,
              //          double mat1[3][3],
              //          double mat2[3][3],
              int mat1r, int mat1c) {
    int row, col;

    mat2.resize(mat1c);  // rows
    for (std::vector<std::vector<double> >::iterator it = mat2.begin();
         it != mat2.end(); ++it)
        it->resize(mat1r);

    for (row = 0; row < mat1r; row++) {
        for (col = 0; col < mat1c; col++) mat2[col][row] = mat1[row][col];
    }
}

}  // namespace astMath
