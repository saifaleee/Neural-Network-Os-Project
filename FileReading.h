#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


const int INPUT_LAYER_SIZE = 2; // total rows in the input layers
const int HIDDEN_LAYER_SIZE = 8;
const int INPUT_DATA_SIZE = 2; // total neurons and their values
const int OUTPUT_LAYER_SIZE = 8; // total rows
using namespace std;


vector<vector<double>> transposeMatrix(const vector<vector<double>>& matrix) {
    size_t rows = matrix.size();
    size_t cols = matrix[0].size();

    vector<vector<double>> transposed(cols, vector<double>(rows));

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }

    return transposed;
}


vector<vector<double>> ReadHiddenLayers(const string& heading, const string& filename) {
    ifstream file(filename);
    string line;
    bool found_heading = false;
    vector<std::vector<double>> weights; // 2d matrix


    while (std::getline(file, line)) { // read line by line
        if (line.find(heading) != string::npos) { // if we found the heading in file , set found heading to true (matching heading substring in line)
            found_heading = true;
        }
        else if (found_heading) {
            vector<double> row;
            stringstream ss(line);
            string value;
            double temp = 1;
            while (getline(ss, value, ',')) {
                // remove any whitespace from the beginning of the value
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                temp = stod(value); // convert to double
                // convert string to int, and keep doing this till 1 row is complete
                row.push_back(temp);
            }
            weights.push_back(row);


            if (heading == "Input layer" && weights.size() == INPUT_LAYER_SIZE)
                break; // break if input layer size limit reaches


            if (weights.size() == HIDDEN_LAYER_SIZE) {
                break; // break once hidden layer size reaches it limit
            }
        }
    }

    weights = transposeMatrix(weights);
    return weights; // return 2d vector
}


vector<double> ReadInputData(const string& heading, const string& filename) {
    ifstream file(filename);
    string line;
    bool found_heading = false;
    vector<double> weights; // 2d matrix


    while (std::getline(file, line)) { // read line by line
        if (line.find(heading) != string::npos) { // if we found the heading in file , set found heading to true (matching heading substring in line)
            found_heading = true;
        }
        else if (found_heading) {
            vector<double> row;
            stringstream ss(line);
            string value;
            double temp = 1;
            while (getline(ss, value, ',')) {
                // remove any whitespace from the beginning of the value
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                temp = stod(value); // convert to double
                // convert string to int, and keep doing this till 1 row is complete
                weights.push_back(temp);
            }
           
            if (weights.size() == INPUT_DATA_SIZE ) {
                break; // break once hidden layer size reaches it limit
            }
        }
    }

    return weights; // return 1d vector
}


vector<double> ReadOutputLayer(const string& heading, const string& filename) {
    ifstream file(filename);
    string line;
    bool found_heading = false;
    vector<double> weights; // 2d matrix


    while (std::getline(file, line)) { // read line by line
        if (line.find(heading) != string::npos) { // if we found the heading in file , set found heading to true (matching heading substring in line)
            found_heading = true;
        }
        else if (found_heading) {
            vector<double> row;
            stringstream ss(line);
            string value;
            double temp = 1;
            while (getline(ss, value, ',')) {
                // remove any whitespace from the beginning of the value
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                temp = stod(value); // convert to double
                // convert string to int, and keep doing this till 1 row is complete
                weights.push_back(temp);
            }


            if (weights.size() == OUTPUT_LAYER_SIZE) {
                break; // break once hidden layer size reaches it limit
            }
        }
    }


    return weights; // return 1d vector
}


void printMatrix(const vector<vector<double>>& matrix) { // simply print the 2d vector matrix
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (j!=matrix[i].size()-1)
            cout << matrix[i][j] << " ,";
            else
                cout << matrix[i][j];
        }
        cout << endl;
    }
}


void printVector(const vector<double>& vec) { // simply print the 2d vector matrix
    for (size_t i = 0; i < vec.size(); ++i) {
            if (i != vec.size() - 1)
                cout << vec[i] << " ,";
            else
                cout << vec[i];
       


    }
    cout << endl;
}

// int main() {


//     vector<vector<double>> weights; // 2d matrix
//     vector<vector<double>> inputlayer_weights;
//     vector<double> input_data;
//     vector<double> output_layer;
//     weights = ReadHiddenLayers("Hidden layer 1", "data.txt");
//     //printMatrix(weights);


//     inputlayer_weights = ReadHiddenLayers("Input layer", "data.txt");
//     //printMatrix(inputlayer_weights);


//     output_layer = ReadOutputLayer("Output layer", "data.txt");
//     input_data = ReadInputData("Input data", "data.txt");
//     printVector(output_layer);
//     return 0;
// }
