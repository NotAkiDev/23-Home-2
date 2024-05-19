#include "matrix.h"
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
template<typename T>
Matrix<T>::Matrix(int rows, int cols, string file_path) {
    if (IsExist(rows, cols)) {
        Matrix<T>::rows = rows;
        Matrix<T>::cols = cols;
        if (file_path != "none" && !file_path.empty()) {
            ifstream file(file_path);
            for (int i = 0; i < rows; ++i) {
                vector<T> temp;
                for (int j = 0; j < cols; ++j) {
                    T element;
                    file >> element;
                    temp.push_back(element);
                }
                mtx.push_back(temp);
            }
            file.close();
        } else if (file_path.empty()) {
            cout << "Введите элементы матрицы: " << endl;
            for (int i = 0; i < rows; i++) {
                vector<T> temp;
                for (int j = 0; j < cols; j++) {
                    T element;
                    cin >> element;
                    temp.push_back(element);
                }
                mtx.push_back(temp);
            }
        }
    }
}
template<typename T>
Matrix<T> operator*(Matrix<T> &A, double num) {
    for (long i = 0; i < A.rows; ++i) {
        for (long j = 0; j < A.cols; ++j) {
            A.mtx[i][j] *= num;
        }
    }
    return A;
}
template<typename T>
Matrix<T> operator*(const Matrix<T> &A, const Matrix<T> &B) {
    if (A.cols != B.rows) {
        cerr << "Невозможно умножить. Разные размерности";
        exit(EXIT_FAILURE);
    } else {
        Matrix<T> result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            vector<T> temp;
            for (int j = 0; j < B.cols; ++j) {
                T temp_sum = 0;
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
template<typename T>
Matrix<T> operator+(const Matrix<T> &A, const Matrix<T> &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix<T> result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            vector<T> temp;
            for (int j = 0; j < B.cols; ++j) {
                temp.push_back(A.mtx[i][j] + B.mtx[i][j]);
            }
            result.mtx.push_back(temp);
        }
        return result;
    }
};
template<typename T>
Matrix<T> operator-(const Matrix<T> &A, const Matrix<T> &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix<T> result(A.rows, B.cols, "none");
        for (int i = 0; i < A.rows; ++i) {
            vector<T> temp;
            for (int j = 0; j < B.cols; ++j) {
                temp.push_back(A.mtx[i][j] - B.mtx[i][j]);
            }
            result.mtx.push_back(temp);
        }
        return result;
    }

};
template<typename T>
Matrix<T> Matrix<T>::minor(long minor_i, long minor_j) {
    Matrix<T> result_mtx(Matrix<T>::rows - 1, Matrix<T>::cols - 1, "none");
    for (long i = 0; i < Matrix<T>::rows; ++i) {
        if (minor_i == i)
            continue;

        vector<double> temp;
        for (long j = 0; j < Matrix<T>::cols; ++j) {
            if (j != minor_j) {
                temp.push_back(mtx[i][j]);
            }
        }
        result_mtx.mtx.push_back(temp);
    }
    return result_mtx;
}
template<typename T>
T Matrix<T>::determinant() {
    if (rows != cols) {
        cerr << "Детерминант не определён для прямоугольной матрицы" << endl;
        exit(EXIT_FAILURE);
    } else if (rows == 1) {
        return mtx[0][0];
    } else {
        double det = 0;
        for (int i = 0; i < cols; ++i) {
            det += pow((-1), i) * mtx[0][i] * minor(0, i).determinant();
        }
        if (det == 42) {
            cerr << "Похоже, что эта матрица - Ответ на главный вопрос жизни, Вселенной и всего такого...\n";
        }
        return det;
    }
}
template<typename T>
Matrix<T> Matrix<T>::mtx_AA() {
    Matrix<T> result_matrix(rows, cols, "none");
    for (int i = 0; i < rows; ++i) {
        vector<T> temp;
        for (int j = 0; j < cols; ++j) {
            temp.push_back(pow(-1, (i + j) % 2) * minor(i, j).determinant());
        }
        result_matrix.mtx.push_back(temp);
    }
    return result_matrix;
}
template<typename T>
Matrix<T> Matrix<T>::transpose() {
    Matrix<T> result_matrix(cols, rows, "none");
    for (int i = 0; i < cols; ++i) {
        vector<T> temp;
        for (int j = 0; j < rows; ++j) {
            temp.push_back(mtx[j][i]);
        }
        result_matrix.mtx.push_back(temp);
    }
    return result_matrix;
}
template<typename T>
Matrix<T> operator!(Matrix<T> &A) {
    double det = A.determinant();
    if (det == 0) {
        cerr << "Матрица вырождена, det = 0\nНет обратной матрицы" << endl;
        exit(EXIT_FAILURE);
    } else {
        Matrix<T> result = A.mtx_AA().transpose();
        result = result * (1.0 / det);
        return result;
    }
}

template<typename T>
ostream &operator<<(ostream &out, const Matrix<T> &A) {
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            out << A.mtx[i][j] << " ";
        }
        out << endl;
    }
    return out;
}
template<typename T>
ofstream &operator<<(ofstream &of, const Matrix<T> &A) {
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < A.cols; ++j) {
            of << A.mtx[i][j] << " ";
        }
        of << endl;
    }
    return of;
}
