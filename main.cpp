#include <iostream>
#include <fstream>
#include <string>
#include <Eigen>
#include "mult.h"

using namespace std;
using namespace Eigen;

#define MAXBUFSIZE  ((int) 1e6)

MatrixXd readMatrix(const char*);
bool verify(algFn);


int main() {
    cout << verify(naive) << endl;

    return 0;
}

bool verify(algFn alg) {
    MatrixXd A = readMatrix("/Users/alex/Documents/Code/C++/matrixmultiply/matrix/A16.csv");
    MatrixXd B = readMatrix("/Users/alex/Documents/Code/C++/matrixmultiply/matrix/B16.csv");
    MatrixXd C = readMatrix("/Users/alex/Documents/Code/C++/matrixmultiply/matrix/C16.csv");

    MatrixXd res = naive(A, B);

    return res == C;
}


// From https://stackoverflow.com/questions/20786220/
// eigen-library-initialize-matrix-with-data-from-file-or-existing-stdvector
MatrixXd readMatrix(const char *filename)
{
    int cols = 0, rows = 0;
    double buff[MAXBUFSIZE];

    // Read numbers from file into buffer.
    ifstream infile;
    infile.open(filename);
    while (! infile.eof())
    {
        string line;
        getline(infile, line);

        int temp_cols = 0;
        stringstream stream(line);
        while(! stream.eof())
            stream >> buff[cols*rows+temp_cols++];

        if (temp_cols == 0)
            continue;

        if (cols == 0)
            cols = temp_cols;

        rows++;
    }

    infile.close();

    rows--;

    // Populate matrix with numbers.
    MatrixXd result(rows,cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(i,j) = buff[ cols*i+j ];

    return result;
};
