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
        if (file_path != "none" && !file_path.empty()) {
            ifstream file(file_path);
            for (int i = 0; i < rows; ++i) {
                vector<double> temp;
                for (int j = 0; j < cols; ++j) {
                    double element;
                    file >> element;
                    temp.push_back(element);
                }
                mtx.push_back(temp);
            }
            file.close();
        } else if (file_path.empty()) {
            cout << "Введите элементы матрицы: " << endl;
            for (int i = 0; i < rows; i++) {
                vector<double> temp;
                for (int j = 0; j < cols; j++) {
                    double element;
                    cin >> element;
                    temp.push_back(element);
                }
                mtx.push_back(temp);
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
            vector<double> temp;
            for (int j = 0; j < B.cols; ++j) {
                double temp_sum = 0;
                for (int k = 0; k < A.cols; ++k) {
                    temp_sum += A.mtx[i][k] * B.mtx[k][j];
                }
                temp.push_back(temp_sum);
            }
            result.mtx.push_back(temp);
        }
        return result;
    }

};

Matrix operator+(const Matrix &A, const Matrix &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            vector<double> temp;
            for (int j = 0; j < B.cols; ++j) {
                temp.push_back(A.mtx[i][j] + B.mtx[i][j]);
            }
            result.mtx.push_back(temp);
        }
        return result;
    }
};

Matrix operator-(const Matrix &A, const Matrix &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            vector<double> temp;
            for (int j = 0; j < B.cols; ++j) {
                temp.push_back(A.mtx[i][j] - B.mtx[i][j]);
            }
            result.mtx.push_back(temp);
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
