#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <Eigen>
#include "mult.h"

using namespace std;
using namespace Eigen;

#define VERIFY true
#define MIN_ORDER 4
#define MAX_ORDER 12

typedef unsigned long long int ull;

MatrixXd readMatrix(const char*, long int);
algFn algStrToFn(const std::string&);
std::string algFnToStr(algFn);
ull profile(algFn alg, const std::string& dir, const std::string& size);
void profile(algFn alg, const std::string& dir);

int main(int argc, char* argv[]) {
    if (argc == 3) {
        std::string matrix_dir(argv[1]);
        std::string alg_str(argv[2]);

        algFn alg = algStrToFn(alg_str);

        if (alg == nullptr) {
            std::cout << "Bad function argument." << std::endl;
            return -1;
        }
        profile(alg, matrix_dir);
    } else if (argc == 4) {
        std::string matrix_dir(argv[1]);
        std::string size(argv[3]);
        std::string alg_str(argv[2]);

        algFn alg = algStrToFn(alg_str);

        if (alg == nullptr) {
            std::cout << "Bad function argument." << std::endl;
            return -1;
        }
        std::cout << profile(alg, matrix_dir, size) << std::endl;
    } else {
        cout << "Wrong inputs." << endl;
        return -1;
    }

    return 0;
}

ull profile(algFn alg, const std::string& dir, const std::string& size) {
    std::string a_dir = dir + "/A" + size + ".csv";
    std::string b_dir = dir + "/B" + size + ".csv";
#ifdef VERIFY
    std::string c_dir = dir + "/C" + size + ".csv";
#endif

    MatrixXd A = readMatrix(a_dir.c_str(), std::stoi(size));
    MatrixXd B = readMatrix(b_dir.c_str(), std::stoi(size));
#ifdef VERIFY
    MatrixXd C = readMatrix(c_dir.c_str(), std::stoi(size));
#endif

    auto start = std::chrono::high_resolution_clock::now();
    MatrixXd res = alg(A, B);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
            (end - start).count();

#ifdef VERIFY
    if (res != C) {
        cout << size << endl;
        throw std::runtime_error("Verify failed");
    }
#endif

    return elapsed;
}

void profile(algFn alg, const std::string& dir) {
    cout << algFnToStr(alg) << endl;
    for (int order = MIN_ORDER; order <= MAX_ORDER; order++) {
        int size = (1 << order);
        ull time = profile(alg, dir, std::to_string(size));
        cout << size << ' ' << time << endl;
    }
}

algFn algStrToFn(const std::string& s) {
    if (s == "naive") {
        return naive;
    } else if (s == "block")
        return block;
    else if (s == "dac")
        return dac;
    else if (s == "strassen")
        return strassen;
    else
        return nullptr;
}

std::string algFnToStr(algFn af) {
    if (af == naive)
        return std::string("naive");
    else if (af == block)
        return std::string("block");
    else if (af == dac)
        return std::string("dac");
    else if (af == strassen)
        return std::string("strassen");
    else
        return std::string("unrecognized function");
}


// Adapted from https://stackoverflow.com/questions/20786220/
// eigen-library-initialize-matrix-with-data-from-file-or-existing-stdvector
MatrixXd readMatrix(const char *filename, long int width)
{
    int cols = 0, rows = 0;
    auto buff_ptr = std::make_unique<std::vector<double>>(width*width);

    // Read numbers from file into buffer.
    std::ifstream infile;
    infile.open(filename);
    while (! infile.eof())
    {
        std::string line;
        getline(infile, line);

        int temp_cols = 0;
        std::stringstream stream(line);
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
