#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Eigen>
#include "mult.h"

using namespace std;
using namespace Eigen;

MatrixXd readMatrix(const char*, long int);
bool verify(algFn, const std::string&, const std::string&);
algFn algStrToFn(const std::string&);


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Must have first argument matrix directory, "
                     "second argument size, third argument algorithm ("
                     "naive, block)." << std::endl;
        return -1;
    }
    std::string matrix_dir(argv[1]);
    std::string size(argv[2]);
    std::string alg_str(argv[3]);

    algFn alg = algStrToFn(alg_str);

    if (alg == nullptr) {
        std::cout << "Bad function argument." << std::endl;
        return -1;
    }

    cout << verify(alg, matrix_dir, size) << endl;

    return 0;
}

bool verify(algFn alg, const std::string& dir, const std::string& size) {
    std::string a_dir = dir + "/A" + size + ".csv";
    std::string b_dir = dir + "/B" + size + ".csv";
    std::string c_dir = dir + "/C" + size + ".csv";

    MatrixXd A = readMatrix(a_dir.c_str(), std::stoi(size));
    MatrixXd B = readMatrix(b_dir.c_str(), std::stoi(size));
    MatrixXd C = readMatrix(c_dir.c_str(), std::stoi(size));

    MatrixXd res = alg(A, B);

    return res == C;
}

algFn algStrToFn(const std::string& s) {
    if (s == "naive") {
        return naive;
    } else if (s == "block")
        return block;
    else
        return nullptr;
}


// Adapted from https://stackoverflow.com/questions/20786220/
// eigen-library-initialize-matrix-with-data-from-file-or-existing-stdvector
MatrixXd readMatrix(const char *filename, long int width)
{
    int cols = 0, rows = 0;
    auto buff_ptr = std::make_unique<std::vector<double>>(width);

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
            stream >> (*buff_ptr)[cols*rows+temp_cols++];

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
            result(i,j) = (*buff_ptr)[ cols*i+j ];

    return result;
};
