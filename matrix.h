#define MAX_SIZE 100
#define ELEM_TYPE int

// 用三元组表示的稀疏矩阵
// 
// 矩阵就是一个 TritupleMatrix 的数组 T：
// - 首元素 T 存矩阵的行数、列数、非零元素个数
// - 后面的元素从 T[1] 到 T[H.val] (左右都包含) 为矩阵中某元素。
//
// 注意：所有算法实现时务必保持元素「按行优先」排列
typedef struct TritupleMatrix
{
    int row, col;
    ELEM_TYPE val;
} TritupleMatrix;

// Create a TritupleMatrix from a 2-D array A with m rows and n cols.
TritupleMatrix*
NewTritupleMatrix(ELEM_TYPE A[][MAX_SIZE], int m, int n);

// get an element at (row, col) from matrix M.
// return the value.
ELEM_TYPE
getMatrixValueAt(TritupleMatrix* M, int row, int col);

// print a TritupleMatrix
void
printTritupleMatrix(TritupleMatrix*);

// matrix add: B = A^T
// returns B
TritupleMatrix*
matrixT(TritupleMatrix*);

// matrix add: C = A + B
// returns C
TritupleMatrix*
matrixAdd(TritupleMatrix*, TritupleMatrix*);

// TritupleMatrix*
// matrixAddUnsorted(TritupleMatrix* A, TritupleMatrix* B);

// matrix mul: C = A * B
// returns C
TritupleMatrix*
matrixMul(TritupleMatrix*, TritupleMatrix*);