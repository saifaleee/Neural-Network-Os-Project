#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

string VecToString(vector<double>& vec) 
{
    string result = "";

    for (int i = 0; i < vec.size(); i++) {
        result += to_string(vec[i]);
        result += ",";
    }

    return result;
}

vector<double> StringToVec(const char* input) 
{
    vector<double> values;
    stringstream ss(input);
    string token;

    while (std::getline(ss, token, ',')) 
    {
        try {
            double value = std::stod(token);
            values.push_back(value);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument exception for value: " << token << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Out of range exception for value: " << token << std::endl;
        }
    }
    return values;
}


// int main(){

//     vector<double> test = {0.1,-2.1,4.2};
//     string  temp;
//     vector<double> ans;
//     temp = VecToString(test); cout << temp << endl;
//     ans = parseValues(temp.c_str());
    
//     printVector(ans);

// }
