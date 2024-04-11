#ifndef matrix
#define matrix
#include <fstream>

using namespace std;

class Matrix {
private:
    int cols;
    int rows;
    double **mtx;

    // Дружелюбные перегрузки для взаимодествия с приватными переменными

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

    // Бинарные операторы сравнения
    friend bool operator==(const Matrix &A, const Matrix &B);

    friend bool operator==(const Matrix &A, double num);

    friend bool operator!=(const Matrix &A, double num);

    friend bool operator!=(const Matrix &A, const Matrix &B);


public:
    // Для ввода из консоли и корректной работы функций, которые создают матрицы
    Matrix(int rows, int cols, string file_path = ""); // Конструктор для ввода из файла

    // Выделение памяти под матрицу
    void AllocMem();


    // Методы элементарных преобразований.
    // Не создают новую матрицу, а изменяют текущую
    void ET_type1(long row1, long row2);

    void ET_type2(long row, double num);

    void ET_type3(long row1, long row2, double num);

    // Функции поиска детерминанта.
    double determinant();

    Matrix minor(long minor_i, long minor_j);

    // Алгебраическое дополнение и транспонирование для обратной матрицы
    Matrix mtx_AA();

    Matrix transpose();
};


#endif
