#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <math.h>
using namespace std;
using namespace std::chrono;

/**
Brian Shin + Samuel Adams
COMP 3270 - Yilmaz, Levent
Programming Assignment: MSCS Problem
Compile and run with C++ (mingw-w64\\x86_64-8.1.0-posix-seh-rt_v6-rev0)
IDE used: Visual Studio Code
References used:
c++ for vector: https://www.cplusplus.com/reference/vector/vector/
reuse of code from COMP 2710 for reading/writing files
stack overflow for delimiting commas in an input file: https://stackoverflow.com/questions/40945378/reading-a-comma-separated-file-into-an-integer-array
stack overflow for measuring execution time: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
Certification statement: I certify that I wrote the code I am submitting. I did not copy whole or parts of it from another student or have another person write the code for me.
Any code I am reusing in my program is clearly marked as such with its source clearly identified in comments.
**/

// function declarations
int algorithm1(vector<int>& X);
int algorithm2(vector<int>& X);
int algorithm3(vector<int>& X, int L, int U);
int algorithm4(vector<int>& X);

// helper function declarations
bool check_file(string);
vector<int> read_file(string);
void write_file(string fileName, vector<vector<double>> numIn);
double tnComplexity(int algo, int n);

// input file
string file = "phw_input.txt";

// main method
int main() {
    // verify file, pass it as an array for use and outputs MSCS of array
    check_file(file);
    vector<int> readArray = read_file(file);
    cout << "algorithm 1: " << algorithm1(readArray) << "\n";
    cout << "algorithm 2: " << algorithm2(readArray) << "\n";
    cout << "algorithm 3: " << algorithm3(readArray, 0, readArray.size() - 1) << "\n";
    cout << "algorithm 4: " << algorithm4(readArray) << "\n";

    // creating input matrix
    vector<vector<int>> matrix;
    // created 19 rows of differing sizes
    for (int i = 0; i < 19; i++) {
        vector<int> row;
        // makes each row's size and pushing random integers into each available spot from 5-100
        for (int j = 0; j < (i + 2) * 5; j++) {
            row.push_back(rand() % 100 + 1);
        }
        // filling each row of 2d matrix with random integers
        matrix.push_back(row);
    }

    // testing runtime execution using system clock
    
    vector<vector<double>> timeMatrix;

    int N = 100;

    double t1 = 0, t2 = 0, t3 = 0, t4 = 0;

    for (int i = 0; i < 19; i++) {
        /** This is the result vector that is put into the first row of the timeMatrix.
        The first 4 indeces of the result vector are the t1, t2, t3, t4 
        the last four are the theoretical complexity * emperical.
        **/
        
        vector<double> result;
        for (int j = 0; j < 8; j++) {

            for (int k = 0; k < N; k++) {

                t1 = 0, t2 = 0, t3 = 0, t4 = 0;

                // TIME TAKEN IN ns (NANOSECONDS)
                // Algo 1 runtime
                auto start = high_resolution_clock::now();
                algorithm1(matrix.at(i));
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                t1 += duration.count();

                // Algo 2 runtime
                start = high_resolution_clock::now();
                algorithm2(matrix.at(i));
                stop = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(stop - start);
                t2 += duration.count();

                // Algo 3 runtime
                start = high_resolution_clock::now();
                algorithm3(matrix.at(i), 0, matrix.at(i).size() - 1);
                stop = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(stop - start);
                t3 += duration.count();
                
                // Algo 4 runtime
                start = high_resolution_clock::now();
                algorithm4(matrix.at(i));
                stop = high_resolution_clock::now();
                duration = duration_cast<nanoseconds>(stop - start);
                t4 += duration.count();
            }

            // finding averages
            t1 /= N;
            t2 /= N;
            t3 /= N;
            t4 /= N;
            
            //load result with t1,t2,t3,t4 
            result.clear();
            result.push_back(t1);
            result.push_back(t2);
            result.push_back(t3);
            result.push_back(t4);

            //then load it with the emperical * theoretical time complexities.
            //the emp * ceiling(T(n)) will be loaded into the last 4 indeces of the array
            int inputSize = (i + 2) * 5;
            for (int algoNum = 0; algoNum <= 3; algoNum++) {
                result.push_back(tnComplexity(algoNum + 1, inputSize));
            }
        }
        timeMatrix.push_back(result);
    }
    write_file("adams_phw_output.txt", timeMatrix);
    return 0;
}

// function/algorithm definitions
// each algorithm is a solution to MSCS

// alg 1
int algorithm1(vector<int>& X) {
    int maxSoFar = 0;
    int P = 0;
    int Q = X.size();
    for (int L = P; L < Q; L++) {
        for (int U = L; U < Q; U++) {
            int sum = 0;
            for (int I = L; I <= U; I++) {
                sum = sum + X[I];
                // sum now contains the sum of X[L...U]
            }
            maxSoFar = max(maxSoFar, sum);
        }
    }
    return maxSoFar;
}

// alg 2
int algorithm2(vector<int>& X) {
    int maxSoFar = 0;
    int P = 0;
    int Q = X.size();
    for (int L = P; L <= Q; L++) {
        int sum = 0;
        for (int U = L; U < Q; U++) {
            sum = sum + X[U];
            // sum now contains the sum of X[L...U]
            maxSoFar = max(maxSoFar, sum);
        }
    }
    return maxSoFar;
}

// alg 3
int algorithm3(vector<int>& X, int L, int U) {
    if (L > U) {
        return 0; /* zero element vector */
    }
    if (L == U) {
        return max(0, X[L]);
    }
    int M = (L + U) / 2;  /* A is X[L..M], B is X[M+1..U] */
    int sum = 0;
    int	maxToLeft = 0;

    for (int I = M; I >= L; I--) {
        sum = sum + X[I];
        maxToLeft = max(maxToLeft, sum);
    }
    /* Find max crossing to right */
    sum = 0;
    int maxToRight = 0;
    for (int I = M + 1; I <= U; I++) {
        sum = sum + X[I];
        maxToRight = max(maxToRight, sum);
    }
    int maxCrossing = maxToLeft + maxToRight;

    int maxInA = algorithm3(X, L, M);
    int maxInB = algorithm3(X, M + 1, U);

    int finalMax = max(max(maxCrossing, maxInA), maxInB);
    return finalMax;
}

// alg 4	
int algorithm4(vector<int>& X) {
    int maxSoFar = 0;
    int maxEndingHere = 0;

    int P = 0;
    int Q = X.size();
    for (int I = P; I < Q; I++) {
        maxEndingHere = max(0, maxEndingHere + X[I]);
        maxSoFar = max(maxSoFar, maxEndingHere);
    }
    return maxSoFar;
}


// helper functions

// From Software Construction Project 3
bool check_file(string file) {
    ifstream stream;

    stream.open(file.c_str());
    if (stream.fail()) {
        cout << "Input file opening failed." << endl;
    }
    stream.close();
    return true;
}

// From Software Construction Project 3
vector<int> read_file(string fileIn) {
    ifstream stream;
    vector<int> X;
    int index;

    stream.open(fileIn);
    string number_as_string;
    while (getline(stream, number_as_string, ',')) {
        X.push_back(stoi(number_as_string));
    }
    return X;
}

// From Software Construction Project 3
void write_file(string fileName, vector<vector<double>> numIn) {
    ofstream write;

    write.open(fileName, ios::app);
    write << "algorithm-1,algorithm-2,algorithm-3,algorithm-4,T1(n),T2(n),T3(n), T4(n)" << "\n";
    for (int i = 0; i < 19; i++) {
        // i suggest using this on visual studio code to see output file instantly.
        
        for (int j = 0; j < 8; j++) {
            write << numIn.at(i).at(j) << ", ";
        }
        write << endl;
    }
    write.close();
}

// Runs empircally calculated complexity on four algorithms from the table
    double tnComplexity(int algo, int n) {
        double TN = 0;
        switch (algo) {
            case 1: // algo 1 T(n)
                TN = (7/6*n*n*n) + (13/2*n*n) + (22/3*n) + (5);
                break;
            case 2: // algo 2 T(n)
                TN = (6*n*n) + (8*n) + (5);
                break;
            case 3: // algo 3 T(n)
                if (n == 0) {
                    TN = 4; // base case
                }
                TN = ((11*n) + ((12*n)+36)*(n-1));
                break;
            case 4: // algo 4 T(n)
                TN = (13*n) + 5;
                break;
            default:
                break;
        }
        TN /= 100;
        return ceil(TN);
    }
