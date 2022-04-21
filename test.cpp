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

float rounding(float num, int index);
string floatToString(const float value, unsigned int precision);

float ipPowerStringToFloat(string number);
  float rounding(float num, int index);

int main()
{
    // string number1 = "354.600006";
    // string number2 = "354.600006";

    // float number3 = 131.039993;
    float number4 = 0.00000000169623;
    float number3 = 0.00000000169624;
    string number = "1.51959e-05";

    float number5 = stof(number);

    cout << number5 << endl;

    if(number3>number4){
        cout << "is true" <<endl;
    }else{
         cout << "is false" <<endl;
    }

    
    //  if(number3 == number4){
    //     cout << "is true" << endl;
    // }


    // float num = 250.274923;  // 要被四捨五入的數字 
    // int decPointDigit = 3;  // 小數點後第幾位 
    // std::cout << floatToString(num, 6) << std::endl;


    //  float num = 250.27993;  // 要被四捨五入的數字 
    //  int decPointDigit = 3; // 小數點後第幾位 

    //     cout << "Rounded to one decimal after the first of several: ";

    //     num = rounding(num, decPointDigit);
    //     cout << "After rounding the result is: " << num << endl;
    //     cout << "===================================================" << endl;

    //     string str = to_string(num);
    //     cout << " str : " << str << endl;




    return 0;
}

float ipPowerStringToFloat(string number){
    int index_1 = number.size()-2;
    int index_2 = number.size()-1;
    string k;
    k.push_back(number[index_1]);
    k.push_back(number[index_2]);
    string j;
    for (int i = 0; i < number.size(); i++)
    {
        if(number[i] == 'e'){
            break;
        }else{
            j.push_back(number[i]);
        }
    }

    int k_int = stoi(k);
    float j_float = stof(j);
    float init_m = 0.1;
    float init_n = 1;
    
    for (int i = 0; i < k_int; i++)
    {
        init_n*=init_m;
    }
     j_float*init_n;


    return j_float*init_n;
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
	
    if(num < 0) // if this number is negative, then convert to positive number
    {
        isNegative = true;	
        num = -num;
    }
	
    if(index >= 0)
    {
        int multiplier;
        multiplier = pow(10, index);
        num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
    }
	
    if(isNegative) // if this number is negative, then convert to negative number
    {
        num = -num;
    }
	
    return num;
}