using namespace std;
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <time.h>

int getdir(string dir, vector<string> &files);

// string floatToString(const float value)
// {
//     std::ostringstream out;
//     out.precision(6);
//     out << value;
//     return out.str();
// }

string check(float a){
    std::stringstream buf;
    buf.precision(3);
    buf.setf(std::ios::fixed);
    buf<<a;
    string str = buf.str();
    buf.clear();
    return str;
}
string floatToString(const float value)
{
    std::stringstream buf;
    std::ostringstream oss;

    buf.precision(3);
    buf.setf(std::ios::fixed);
    buf << value;
    string str = buf.str();
    buf.clear();

    return str;
}


int main(){

string num= "32.0100"; 
 float num_flo= stof(num);

 cout << num << endl;
// std::ostringstream oss; 
// oss<<num; 
// std::cout<<oss.str()<<std::endl; 


    
//     // cout << floatToString(0.0359802) << endl;
//     check(0.0359802); 
//  cout <<  check(0.0359802) << endl;
    return 0;
}