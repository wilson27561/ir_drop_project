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
float rounding(float num, int index);
string floatToString(const float value, unsigned int precision);

//x1 x2 是否有在 k1 k2 內
bool isInRange(int x1,int x2,int k1,int k2){
    
     if(x1< k1 && x2<=k1){

     }else if(x1 >k2 && x2>k2){
         
     }else{

     }
}

int main()
{
    string number1 = "354.600006";
    string number2 = "354.600006";

    float number3 = 131.039993;
    float number4 = 354.600006;

    if(number1 == number2){
        cout << "is true" << endl;
    }

     if(number3 == number4){
        cout << "is true" << endl;
    }


    float num = 250.272003;  // 要被四捨五入的數字 
    int decPointDigit = 3;  // 小數點後第幾位 
    std::cout << floatToString(number3, 6) << std::endl;








    return 0;
}


 string floatToString(const float value, unsigned int precision)                                                                                                                                                                                                          
  {
     std::ostringstream out;
     if (precision > 0)
         out.precision(precision);
  
      out << value;
     return out.str();
  }