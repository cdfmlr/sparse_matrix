#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

TritupleMatrix*
NewTritupleMatrix(ELEM_TYPE A[][MAX_SIZE], int m, int n)
{
    // XXX: 如果不多这一次遍历，如何申请合适的空间？？
    int non_zero = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (A[i][j] != 0) {
                non_zero++;
            }
        }
    }
    // printf("non_zero: %d\n", non_zero);

    TritupleMatrix* t = malloc(sizeof(TritupleMatrix) * (non_zero + 1));

    t->row = m;
    t->col = n;

    TritupleMatrix* p = t + 1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            int a = A[i][j];
            if (a != 0) {
                p->row = i;
                p->col = j;
                p->val = a;

                p++;
                t->val++;
            }
        }
    }
    return t;
}

// get an element at (row, col) from matrix M.
// return a TritupleMatrix*, or NULL if not found (element value is zero).
TritupleMatrix*
getMatrixElement(TritupleMatrix* M, int row, int col)
{
    TritupleMatrix* e;
    for (int i = 1; i <= M->val; i++) {
        e = M + i;
        if ((e->row == row) && (e->col == col)) {
            return e;
        }
    }
    return NULL;
}

ELEM_TYPE
getMatrixValueAt(TritupleMatrix* M, int row, int col)
{
    TritupleMatrix* t = getMatrixElement(M, row, col);
    return (t ? t->val : 0);
}

void
printTritupleMatrix(TritupleMatrix* M)
{
    printf("Matrix: shape=(%d, %d), non_zero=%d:\n", M->row, M->col, M->val);
    for (int i = 1; i <= M->val; i++) {
        TritupleMatrix* t = M + i;
        printf("\t(%d, %d) \t %d\n", t->row, t->col, t->val);
    }
}

TritupleMatrix*
matrixT(TritupleMatrix* A)
{
    TritupleMatrix* B = malloc(sizeof(TritupleMatrix) * (A->val + 1));

    B->col = A->row;
    B->row = A->col;
    B->val = A->val;

    // 下面这种算法虽简单，但不能保证行优先排列：
    //
    // for (int i = 1; i <= A->val; i++) {
    //     B[i].row = A[i].col;
    //     B[i].col = A[i].row;
    //     B[i].val = A[i].val;
    // }

    TritupleMatrix *p = B + 1, a;
    for (int col = 0; col < A->col; col++) { // 逐 列 -> 行
        for (int i = 1; i <= A->val; i++) {
            a = A[i];
            if (a.col == col) {
                p->row = a.col;
                p->col = a.row;
                p->val = a.val;

                p++;
            }
        }
    }

    return B;
}

// 对未按行优先的任意顺序 TritupleMatrix 做加法。
// 取每个元素都扫一遍表，效率极低
TritupleMatrix*
matrixAddUnsorted(TritupleMatrix* A, TritupleMatrix* B)
{
    if ((A->row != B->row) || (A->col != B->col)) {
        printf("Matrix Add failed: shape (%d, %d) and (%d, %d)\n",
               A->row,
               A->col,
               B->row,
               B->col);
        return NULL;
    }

    TritupleMatrix* C = malloc(sizeof(TritupleMatrix) * (A->val + B->val + 1));
    C->row = A->row;
    C->col = A->col;

    TritupleMatrix* p = C + 1;

    ELEM_TYPE a, b, c;
    for (int i = 0; i < A->row; i++) {
        for (int j = 0; j < A->col; j++) {
            a = getMatrixValueAt(A, i, j);
            b = getMatrixValueAt(B, i, j);
            c = a + b;

            if (c) {
                p->row = i;
                p->col = j;
                p->val = c;

                p++;
                C->val++;
            }
        }
    }
    return C;
}

// write _e (TritupleMatrix*) with _row, _col, _val and _e++
// _E is the head of TritupleMatrix to append
// _e is the tail of TritupleMatrix to append, new data will be write into _e.
#define __appendPTritupleMatrix(_E, _e, _row, _col, _val)                      \
    {                                                                          \
        /* append row, col, val to _e (TritupleMatrix*) */                     \
        _e->row = _row;                                                        \
        _e->col = _col;                                                        \
        _e->val = _val;                                                        \
        _e++;                                                                  \
        _E->val++;                                                             \
    }

// 对「按行优先」排序的 TritupleMatrix 做快速加法。
TritupleMatrix*
matrixAdd(TritupleMatrix* A, TritupleMatrix* B)
{
    if ((A->row != B->row) || (A->col != B->col)) {
        printf("Matrix Add failed: shape (%d, %d) and (%d, %d)\n",
               A->row,
               A->col,
               B->row,
               B->col);
        return NULL;
    }

    TritupleMatrix* C = malloc(sizeof(TritupleMatrix) * (A->val + B->val + 1));
    C->row = A->row;
    C->col = A->col;

    /* 对矩阵位置 a 属于 A，b 属于 B，可能的情况：
    1. A 有 B 无：a.val
    2. A 有 B 有：a.val + b.val
    3. A 无 B 有：b.val
    4. A 无 B 无：0
    */
    int i = 1, j = 1;

    TritupleMatrix* a = A + i;
    TritupleMatrix* b = B + j;
    TritupleMatrix* c = C + 1;

    while ((i <= A->val) && (j <= B->val)) {
        a = A + i;
        b = B + j;

        if (a->row == b->row) {
            if (a->col < b->col) {
                __appendPTritupleMatrix(C, c, a->row, a->col, a->val);

                i++;
            } else if (a->col > b->col) {
                __appendPTritupleMatrix(C, c, b->row, b->col, b->val);

                j++;
            } else { // a->col == b->col
                int s = a->val + b->val;
                if (s) {
                    __appendPTritupleMatrix(
                      C, c, a->row, a->col, a->val + b->val);
                }

                i++;
                j++;
            }
        } else if (a->row < b->row) {
            __appendPTritupleMatrix(C, c, a->row, a->col, a->val);

            i++;
        } else { // a->row > b->row
            __appendPTritupleMatrix(C, c, b->row, b->col, b->val);

            j++;
        }
    }

    // 以下两种末尾处理只会执行一种：

    for (; i <= A->val; i++) {
        a = A + i;
        __appendPTritupleMatrix(C, c, a->row, a->col, a->val);
    }

    for (; j <= B->val; j++) {
        b = B + j;
        __appendPTritupleMatrix(C, c, b->row, b->col, b->val);
    }
    return C;
}

TritupleMatrix*
matrixMulUnsorted(TritupleMatrix* A, TritupleMatrix* B)
{
    if (A->col != B->row) {
        printf("Matrix Mul failed: shape (%d, %d) and (%d, %d)\n",
               A->row,
               A->col,
               B->row,
               B->col);
        return NULL;
    }

    TritupleMatrix* C = malloc(sizeof(TritupleMatrix) * (A->row * B->col + 1));
    C->row = A->row;
    C->col = A->col;

    int i = 1, j = 1;

    TritupleMatrix* p = C + 1;

    ELEM_TYPE a, b, c;
    for (int i = 0; i < A->row; i++) {
        for (int j = 0; j < B->col; j++) {

            p->row = i;
            p->col = j;

            for (int k = 0; k < A->col; k++) {
                a = getMatrixValueAt(A, i, k);
                b = getMatrixValueAt(B, k, j);
                
                c = a * b;
                p->val += c;
            }

            if (p->val) { // 如果是 0，下次还写这里，覆盖掉就行了
                p++;
                C->val++;
            }
        }
    }

    return C;
}

// 这个mafj啊，不想写了
inline TritupleMatrix*
matrixMul(TritupleMatrix* A, TritupleMatrix* B)
{
    return matrixMulUnsorted(A, B);
}
