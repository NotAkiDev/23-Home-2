#ifndef matrix
#define matrix

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Matrix {
private:
    int cols;
    int rows;
    vector<vector<double>> mtx;

    // Дружелюбные перегрузки для взаимодействия с приватными переменными

    // Изменяют исходную матрицу
    friend Matrix operator*(const Matrix &A, const Matrix &B);

    friend Matrix operator*(Matrix &A, double num);

    // Не изменяют исходную, возвращают новую матрицу
    friend Matrix operator+(const Matrix &A, const Matrix &B);

    friend Matrix operator-(const Matrix &A, const Matrix &B);

    friend Matrix operator!(Matrix &A);

    // Перегрузки вывода в консоль и файл
    friend ostream& operator<<(ostream &out, const Matrix &A);

    friend ofstream& operator<<(ofstream &of, const Matrix &A);


public:
    // Для ввода из консоли и корректной работы функций, которые создают матрицы
    Matrix(int rows, int cols, string file_path = ""); // Конструктор для ввода из файла


    // Функции поиска детерминанта.
    double determinant();
    Matrix minor(long minor_i, long minor_j);

    // Алгебраическое дополнение и транспонирование для обратной матрицы
    Matrix mtx_AA();
    Matrix transpose();
};
#endif
