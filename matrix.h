#ifndef matrix
#define matrix

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

template<typename T>
class Matrix {
private:
    int cols;
    int rows;
    vector<vector<T>> mtx;

    // Дружелюбные перегрузки для взаимодействия с приватными переменными

    // Изменяют исходную матрицу
    template<typename U>
    friend Matrix<U> operator*(const Matrix<U> &A, const Matrix<U> &B);

    template<typename U>
    friend Matrix<U> operator*(Matrix<U> &A, double num);

    // Не изменяют исходную, возвращают новую матрицу
    template<typename U>
    friend Matrix<U> operator+(const Matrix<U> &A, const Matrix<U> &B);

    template<typename U>
    friend Matrix<U> operator-(const Matrix<U> &A, const Matrix<U> &B);

    template<typename U>
    friend Matrix<U> operator!(Matrix<U> &A);

    // Перегрузки вывода в консоль и файл
    template<typename U>
    friend ostream& operator<<(ostream &out, const Matrix<U> &A);

    template<typename U>
    friend ofstream& operator<<(ofstream &of, const Matrix<U> &A);


public:
    // Для ввода из консоли и корректной работы функций, которые создают матрицы
    Matrix<T>(int rows, int cols, string file_path = ""); // Конструктор для ввода из файла


    // Функции поиска детерминанта.
    T determinant();

    Matrix<T> minor(long minor_i, long minor_j);

    // Алгебраическое дополнение и транспонирование для обратной матрицы
    Matrix<T> mtx_AA();
    Matrix<T> transpose();

    // Статические методы создания единичной и нулевой матрицы
    static Matrix<T> ones(int rows, int cols);
    static Matrix<T> zeros(int rows, int cols);
};
#endif
