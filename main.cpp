#include <iostream>
#include "matrix.h"

using namespace std;


int main() {
    /*
     * Синтаксис вывода в файл
     * ofstream file("filename");
     * file << matrix name;
     */
    Matrix<int> zeroMatrix = Matrix<int>::zeros(3, 3);
    /*
     * Синтаксис конструктора матриц
     * Matrix mtx(3, 3) -> ввод матрицы из консоли
     * Matrix mtx(3, 3, "filename.txt") -> ввод матрицы из файла
     */

    return 0;
};


