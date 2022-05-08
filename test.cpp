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

    //  string str = "    NEW M1 288 + SHAPE FOLLOWPIN ( 40320 1153152 ) ( 1581696 * )";
    //  string str = "SHAPE FOLLOWPIN";
    //  string temp_string;
    //  int length = getStrlength(str, 6);
    //  cout << length << endl;
    //   number_vector.erase(number_vector.begin()) ;
    //   for
    // for (int i = 0; i < str.size(); i++)
    // {
    //   if(str[i]!=' '){
    //      temp_string.insert(temp_string.size(), 1, str[i]);
    //   }else if(str[i] == ' ' && temp_string.size() !=0){
    //       cout << temp_string << " " << temp_string.size() << " " << i- (temp_string.size()) << endl;
    //       temp_string ="";
    //   }
    //   if(i == (str.size()-1) && temp_string.size() !=0){
    //       cout << temp_string << " " << temp_string.size() << " " <<  i- temp_string.size()<< endl;
    //       temp_string ="";
    //   }
    // }

    double temp_num =  10.26 / 0.18 ;
  

    cout <<  temp_num << endl;


    vector<int> number_vector;
    number_vector.push_back(5);
    number_vector.push_back(3);
    number_vector.push_back(1);
    number_vector.push_back(2);
    number_vector.push_back(6);
    number_vector.push_back(4);

    //  number_vector.insert(number_vector.begin()+2,10);
    //  number_vector.erase(number_vector.begin() + 2);
    // for (int i = 0; i < number_vector.size(); i++)
    // {
    //     cout << number_vector[i] << endl;
    // }
   

    // for (int i = 1; i < number_vector.size(); i++)
    // {
    //     for (int j = 0; j < i; j++)
    //     {
    //         if (number_vector[j] > number_vector[i])
    //         {
    //             number_vector.insert(number_vector.begin()+j,number_vector[i]);
    //             number_vector.erase(number_vector.begin() + i+1);
    //             break;
    //         }
    //     }
    // }

    //  for (int i = 1; i < number_vector.size(); i++)
    // {
    //     for (int j = 0; j < i; j++)
    //     {
    //         if (number_vector[j] > number_vector[i])
    //         {
               
    //         }
    //     }
    // }

    // for (int i = 0; i < number_vector.size(); i++)
    // {
    //     cout << number_vector[i] << endl;
    // }




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