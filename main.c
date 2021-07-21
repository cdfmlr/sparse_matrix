#include "matrix.h"
#include <stdio.h>

int
main()
{
    int A[][MAX_SIZE] = { { 1, 0, 1 }, { 0, 0, 0 }, { 2, 0, 0 }, { 0, 0, 3 } };
    int B[][MAX_SIZE] = { { 0, 1, 1 }, { 1, 0, 0 }, { 0, 0, -3 }, { 1, 2, 3 } };

    TritupleMatrix* M = NewTritupleMatrix(A, 4, 3);
    TritupleMatrix* N = NewTritupleMatrix(B, 4, 3);

    puts("M:");
    printTritupleMatrix(M);

    puts("N:");
    printTritupleMatrix(N);

    puts("M^T:");
    TritupleMatrix* tM = matrixT(M);
    printTritupleMatrix(tM);

    puts("M+N:");
    TritupleMatrix* radd = matrixAdd(M, N);
    if (radd) {
        printTritupleMatrix(radd);
    }

    puts("M*N^T:");
    TritupleMatrix* rmul = matrixMul(M, matrixT(N));
    if (rmul) {
        printTritupleMatrix(rmul);
    }

    return 0;
}