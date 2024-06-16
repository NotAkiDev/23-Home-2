#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

bool IsExist(long rows, long cols) {
    if (rows > 0 && cols > 0)
        return true;
    cerr << "Неверные параметры матрицы" << endl;
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

template<typename T>
class Matrix {
private:
    int cols;
    int rows;
    vector<vector<T>> mtx;

    template<typename U>
    friend Matrix<U> operator*(const Matrix<U> &A, const Matrix<U> &B);

    template<typename U>
    friend Matrix<U> operator*(const Matrix<U> &A, double num);

    template<typename U>
    friend Matrix<U> operator+(const Matrix<U> &A, const Matrix<U> &B);

    template<typename U>
    friend Matrix<U> operator-(const Matrix<U> &A, const Matrix<U> &B);

    template<typename U>
    friend Matrix<U> operator!(Matrix<U> &A);

    template<typename U>
    friend ostream& operator<<(ostream &out, const Matrix<U> &A);

    template<typename U>
    friend ofstream& operator<<(ofstream &of, const Matrix<U> &A);

public:
    Matrix(int rows, int cols, string file_path = "");

    T determinant();

    Matrix<T> minor(long minor_i, long minor_j);

    Matrix<T> mtx_AA();
    Matrix<T> transpose();

    static Matrix<T> ones(int rows, int cols);
    static Matrix<T> zeros(int rows, int cols);
};

template<typename T>
Matrix<T>::Matrix(int rows, int cols, string file_path) {
    if (IsExist(rows, cols)) {
        this->rows = rows;
        this->cols = cols;
        if (!file_path.empty()) {
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
        } else {
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
Matrix<T> Matrix<T>::zeros(int rows, int cols) {
    Matrix<T> zeroMatrix(rows, cols);
    zeroMatrix.mtx.resize(rows, vector<T>(cols, 0));
    return zeroMatrix;
}

template<typename T>
Matrix<T> Matrix<T>::ones(int rows, int cols) {
    Matrix<T> oneMatrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        vector<T> row(cols, 0);
        if (i < cols) {
            row[i] = 1;
        }
        oneMatrix.mtx.push_back(row);
    }
    return oneMatrix;
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &A, double num) {
    Matrix<T> result(A.rows, A.cols);
    for (long i = 0; i < A.rows; ++i) {
        for (long j = 0; j < A.cols; ++j) {
            result.mtx[i][j] = A.mtx[i][j] * num;
        }
    }
    return result;
}

template<typename T>
Matrix<T> operator*(const Matrix<T> &A, const Matrix<T> &B) {
    if (A.cols != B.rows) {
        cerr << "Невозможно умножить. Разные размерности" << endl;
        exit(EXIT_FAILURE);
    }
    Matrix<T> result(A.rows, B.cols);
    for (int i = 0; i < A.rows; ++i) {
        for (int j = 0; j < B.cols; ++j) {
            T temp_sum = 0;
            for (int k = 0; k < A.cols; ++k) {
                temp_sum += A.mtx[i][k] * B.mtx[k][j];
            }
            result.mtx[i][j] = temp_sum;
        }
    }
    return result;
}

template<typename T>
Matrix<T> operator+(const Matrix<T> &A, const Matrix<T> &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix<T> result(A.rows, A.cols);
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < A.cols; ++j) {
                result.mtx[i][j] = A.mtx[i][j] + B.mtx[i][j];
            }
        }
        return result;
    }
    return Matrix<T>(0, 0); // это нужно для предупреждения, хотя на практике сюда не зайдет
}

template<typename T>
Matrix<T> operator-(const Matrix<T> &A, const Matrix<T> &B) {
    if (IsPossibleEdit(A.rows, B.rows, A.cols, B.cols)) {
        Matrix<T> result(A.rows, A.cols);
        for (int i = 0; i < A.rows; ++i) {
            for (int j = 0; j < A.cols; ++j) {
                result.mtx[i][j] = A.mtx[i][j] - B.mtx[i][j];
            }
        }
        return result;
    }
    return Matrix<T>(0, 0); // это нужно для предупреждения, хотя на практике сюда не зайдет
}

template<typename T>
Matrix<T> Matrix<T>::minor(long minor_i, long minor_j) {
    Matrix<T> result_mtx(this->rows - 1, this->cols - 1);
    for (long i = 0; i < this->rows; ++i) {
        if (minor_i == i)
            continue;
        vector<T> temp;
        for (long j = 0; j < this->cols; ++j) {
            if (j != minor_j) {
                temp.push_back(this->mtx[i][j]);
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
        T det = 0;
        for (int i = 0; i < cols; ++i) {
            det += pow(-1, i) * mtx[0][i] * minor(0, i).determinant();
        }
        if (det == 42) {
            cerr << "Похоже, что эта матрица - Ответ на главный вопрос жизни, Вселенной и всего такого...\n";
        }
        return det;
    }
}

template<typename T>
Matrix<T> Matrix<T>::mtx_AA() {
    Matrix<T> result_matrix(rows, cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result_matrix.mtx[i][j] = pow(-1, (i + j) % 2) * minor(i, j).determinant();
        }
    }
    return result_matrix;
}

template<typename T>
Matrix<T> Matrix<T>::transpose() {
    Matrix<T> result_matrix(cols, rows);
    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            result_matrix.mtx[i][j] = mtx[j][i];
        }
    }
    return result_matrix;
}

template<typename T>
Matrix<T> operator!(Matrix<T> &A) {
    T det = A.determinant();
    if (det == 0) {
        cerr << "Матрица вырождена, det = 0\nНет обратной матрицы" << endl;
        exit(EXIT_FAILURE);
    }
    Matrix<T> result = A.mtx_AA().transpose();
    result = result * (1.0 / det);
    return result;
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

int main() {
    Matrix<int> zeroMatrix = Matrix<int>::zeros(3, 3);
    cout << zeroMatrix;

    return 0;
}
