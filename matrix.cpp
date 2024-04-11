#include "Matrix.h"
#include <cmath>
#include <iostream>

using namespace std;

bool IsExist(long rows, long cols) {
    if (rows > 0 || cols > 0)
        return true;
    cerr << "Неверные параметры матрицы";
    exit(EXIT_FAILURE);
}

void Matrix::AllocMem() {
    Matrix::mtx = new double *[Matrix::rows];
    for (int i = 0; i < Matrix::rows; ++i) {
        Matrix::mtx[i] = new double[Matrix::cols];
    }
}

bool IsPossibleEdit(int rows_a, int rows_b, int a_cols, int b_cols) {
    if (rows_a == rows_b && a_cols == b_cols)
        return true;
    else {
        cerr << "Невозможно выполнить операцию. Разные размерности\n";
        exit(EXIT_FAILURE);
    }
}

// Конструктор класса матриц.
Matrix::Matrix(int rows, int cols, string file_path) {
    if (IsExist(rows, cols)) {
        Matrix::rows = rows;
        Matrix::cols = cols;
        AllocMem();
        if (file_path != "none" && !file_path.empty()) {
            ifstream file(file_path);
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    double element;
                    file >> element;
                    mtx[i][j] = element;
                }
            }
            file.close();
        } else if (file_path.empty()) {
            cout << "Введите элементы матрицы: " << endl;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    double element;
                    cin >> element;
                    mtx[i][j] = element;
                }
            }
        }
    }
}

Matrix operator*(Matrix &A, double num) {
    for (long i = 0; i < A.rows; ++i) {
        for (long j = 0; j < A.cols; ++j) {
            A.mtx[i][j] *= num;
        }
    }
    return A;
}

Matrix operator*(const Matrix &A, const Matrix &B) {
    if (A.cols != B.rows) {
        cerr << "Невозможно умножить. Разные размерности";
        exit(EXIT_FAILURE);
    } else {
        Matrix result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < B.cols; ++j) {
                for (int k = 0; k < A.cols; ++k) {
                    result.mtx[i][j] += A.mtx[i][k] * B.mtx[k][j];
                }
            }
        }
        return result;
    }

};

Matrix operator+(const Matrix &A, const Matrix &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < B.cols; ++j) {
                result.mtx[i][j] = A.mtx[i][j] + B.mtx[i][j];
            }
        }
        return result;
    }
};

Matrix operator-(const Matrix &A, const Matrix &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < B.cols; ++j) {
                result.mtx[i][j] = A.mtx[i][j] - B.mtx[i][j];
            }
        }
        return result;
    }

};

bool operator==(const Matrix &A, const Matrix &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < B.cols; ++j) {
                if (A.mtx[i][j] != B.mtx[i][j])
                    return false;
            }
        }
        return true;
    }
};

bool operator==(const Matrix &A, const double num) {
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            if (i == j) {
                if (A.mtx[i][j] != num)
                    return false;
            } else {
                if (A.mtx[i][j] != 0)
                    return false;
            }
        }
    }
    return true;
};

bool operator!=(const Matrix &A, const double num) {
    return !(A == num);
}

bool operator!=(const Matrix &A, const Matrix &B) {
    return !(A == B);
};

void Matrix::ET_type1(long row1, long row2) {
    if (row1 < 0 || row1 > Matrix::rows || row2 < 0 || row2 > Matrix::rows) {
        cerr << "Некорректные параметры преобразования";
        exit(EXIT_FAILURE);
    }
    swap(Matrix::mtx[row1 - 1], Matrix::mtx[row2 - 1]);
}

void Matrix::ET_type2(long row, double num) {
    if (row < 0 || row > Matrix::rows) {
        cerr << "Некорректные параметры преобразования";
        exit(EXIT_FAILURE);
    }
    if (num == 0) {
        cerr << "Такое преобразование приведёт к потере данных в матрице\n";
    }
    for (int i = 0; i < Matrix::cols; ++i) {
        Matrix::mtx[row - 1][i] *= num;
    }
}

void Matrix::ET_type3(long row1, long row2, double num) {
    if (row1 < 0 || row1 > Matrix::rows || row2 < 0 || row2 > Matrix::rows) {
        cerr << "Некорректные параметры преобразования";
        exit(EXIT_FAILURE);
    }
    if (num == 0) {
        cerr << "Бессмысленное преобразование\n";
    }
    for (int i = 0; i < Matrix::cols; ++i) {
        Matrix::mtx[row1 - 1][i] += (Matrix::mtx[row2 - 1][i] * num);
    }
}

Matrix Matrix::minor(long minor_i, long minor_j) {
    Matrix result_mtx(Matrix::rows - 1, Matrix::cols - 1, "none");
    for (long i = 0, progress_i = 0; i < Matrix::rows; ++i) {
        if (minor_i == i)
            continue;

        for (long j = 0, progress_j = 0; j < Matrix::cols; ++j) {
            if (j != minor_j) {
                result_mtx.mtx[progress_i][progress_j] = mtx[i][j];
                ++progress_j;
            }

        }

        ++progress_i;
    }
    return result_mtx;
}

double Matrix::determinant() {
    if (Matrix::rows != Matrix::cols) {
        cerr << "Детерминант не определён для прямоугольной матрицы" << endl;
        exit(EXIT_FAILURE);
    } else if (rows == 1)
        return mtx[0][0];
    else { // Детерминант n-ого порядка, это сумма произведений элементов первой строки на их алгебраические дополнения
        double determinant = 0;
        for (long i = 0; i < Matrix::cols; ++i) {
            determinant += (pow((-1), i) * Matrix::mtx[0][i] * minor(0, i).determinant());
        }
        if (determinant == 42) {
            cerr << "Похоже, что эта матрица - Ответ на главный вопрос жизни, Вселенной и всего такого...\n";
        }
        return determinant;
    }
}

Matrix Matrix::mtx_AA() {
    Matrix result_matrix(Matrix::rows, Matrix::cols, "none");
    for (long i = 0; i < Matrix::rows; ++i) {
        for (long j = 0; j < Matrix::cols; ++j) {
            result_matrix.mtx[i][j] = pow(-1, (i + j) % 2) * Matrix::minor(i, j).determinant();
        }
    }
    return result_matrix;
}

Matrix Matrix::transpose() {
    Matrix result_matrix(Matrix::cols, Matrix::rows, "none");
    for (long i = 0; i < Matrix::cols; ++i) {
        for (long j = 0; j < Matrix::rows; ++j)
            result_matrix.mtx[i][j] = mtx[j][i];
    }
    return result_matrix;
}

Matrix operator!(Matrix &A) {
    double det = A.determinant();
    if (det == 0) {
        cerr << "Матрица вырождена, det = 0\nНет обратной матрицы" << endl;
        exit(EXIT_FAILURE);
    } else {
        Matrix result = (A.mtx_AA().transpose());
        result * (1.0 / A.determinant());
        return result;
    }
}

ostream &operator<<(ostream &out, const Matrix &A) {
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            out << A.mtx[i][j] << " ";
        }
        out << endl;
    }
    return out;
}

ofstream &operator<<(ofstream &of, const Matrix &A) {
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            of << A.mtx[i][j] << " ";
        }
        of << endl;
    }
    return of;
}
