#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <ctime>
#include <iostream>
//#include <stdlib.h>

const int N = 40;

namespace bnu = boost::numeric::ublas;

bnu::matrix<double> m(N, N);


//determinant finding
//------------------------------------------------------------------------
int determinant_sign(const bnu::permutation_matrix<std ::size_t>& pm)
{
    int pm_sign=1;
    std::size_t size = pm.size();
    for (std::size_t i = 0; i < size; ++i)
        if (i != pm(i))
            pm_sign *= -1.0; // swap_rows would swap a pair of rows here, so we change sign
    return pm_sign;
}

double determinant( bnu::matrix<double>& m ) {
    bnu::permutation_matrix<std::size_t> pm(m.size1());
    double det = 1.0;
    if( bnu::lu_factorize(m,pm) ) {
        det = 0.0;
    } else {
        for(int i = 0; i < m.size1(); i++)
            det *= m(i,i); // multiply by elements on diagonal
        det = det * determinant_sign( pm );
    }
    return det;
}
//------------------------------------------------------------------------

double mtrxMinor(int row, int col){
    int r = 0;
    int c = 0;
    bnu::matrix<double> newmtrx(N-1, N-1);
    for ( int i = 0; i < m.size1(); i++ ){
        c = 0;
        for ( int j = 0; j < m.size2(); j++ ){
            if ( i != row && j != col){
                newmtrx(r, c) = m(i, j);
                c++;
                if (c >= m.size2()-1){
                    r++;
                }
            }
        }
    }
//    std::cout << newmtrx;
    double det = determinant(newmtrx);
//    std::cout << " " << det << std::endl;
    return det;
}

int main () {
    //Fill matrix with random():
    std::srand(std::time(0));
    for ( int i = 0; i < m.size1(); i++ ){
        for ( int j = 0; j < m.size2(); j++ ){
            m(i, j) = random()%10;
            std::cout << m(i, j) << " ";
        }
        std::cout << std::endl;
    }

    //Find summ of minors and avg. value:
    double summ = 0;
    int count   = 0;

    //параллельный for:
    #pragma omp for
    for ( int i = 0; i < m.size1(); i++ ){
        for ( int j = 0; j < m.size2(); j++ ){
            double mnr = mtrxMinor(i,j);
            //критическая область - выполняется только 1 потоком:
            #pragma omp critical (door)
            summ += mnr;
            //атомарная операция инкримента:
            #pragma omp atomic
            count++;
        }
    }

    std::cout << " summ : " << summ << std::endl << " avg : " << summ/count << std::endl;
}



