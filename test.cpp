using namespace std;
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <math.h>

float rounding(float num, int index);
string floatToString(const float value, unsigned int precision);

float ipPowerStringToFloat(string number);
float rounding(float num, int index);
int getStrlength(string resource_str, int pos_int);
int main()
{

    // vector<int> num_vector;
    // num_vector.push_back(1);
    // num_vector.push_back(2);
    // num_vector.push_back(3);
    // num_vector.push_back(4);
    // num_vector.push_back(5);
    // num_vector.push_back(6);
    // num_vector.push_back(7);
    // num_vector.push_back(8);
    // num_vector.push_back(9);
    // cout << "check 1: " << num_vector.size() << endl;
    // for (int i = 0; i < num_vector.size(); i++)
    // {
    //     if (num_vector[i] == 4)
    //     {
    //         cout << num_vector[i] << endl;
    //         num_vector.erase(num_vector.begin() + i);
    //     }
    // }

    // cout << "check 2: " << num_vector.size() << endl;
    // for (int i = 0; i < num_vector.size(); i++)
    // {

    //     cout << num_vector[i] << endl;
    // }

    int x1 = 5;

    int x2 = 15;

    int x3 = 7;
    int x4 = 10;



    cout << ((float)x1)/x2 << endl;

  


    return 0;
}

//  subreplace(def_content,ori_position,new_position);
void subreplace(string &resource_str, string &sub_str, string &new_str)
{
    string::size_type pos = 0;
    while ((pos = resource_str.find(sub_str)) != string::npos) //替换所有指定子串
    {
        resource_str.replace(pos, sub_str.length(), new_str);
        cout << " ========== " << resource_str << "============" << endl;
    }
}

int getStrlength(string resource_str, int pos_int)
{
    int length = 0;
    while (true)
    {
        cout << resource_str[pos_int] << endl;
        if (resource_str[pos_int] == ' ')
        {

            break;
        }
        length++;
        pos_int++;
    }
    return length;
}
float ipPowerStringToFloat(string number)
{
    int index_1 = number.size() - 2;
    int index_2 = number.size() - 1;
    string k;
    k.push_back(number[index_1]);
    k.push_back(number[index_2]);
    string j;
    for (int i = 0; i < number.size(); i++)
    {
        if (number[i] == 'e')
        {
            break;
        }
        else
        {
            j.push_back(number[i]);
        }
    }

    int k_int = stoi(k);
    float j_float = stof(j);
    float init_m = 0.1;
    float init_n = 1;

    for (int i = 0; i < k_int; i++)
    {
        init_n *= init_m;
    }
    j_float *init_n;

    return j_float * init_n;
}

string floatToString(const float value, unsigned int precision)
{
    std::ostringstream out;
    if (precision > 0)
        out.precision(precision);

    out << value;
    return out.str();
}

float rounding(float num, int index)
{
    bool isNegative = false; // whether is negative number or not

    if (num < 0) // if this number is negative, then convert to positive number
    {
        isNegative = true;
        num = -num;
    }

    if (index >= 0)
    {
        int multiplier;
        multiplier = pow(10, index);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }

    if (isNegative) // if this number is negative, then convert to negative number
    {
        num = -num;
    }

    return num;
}