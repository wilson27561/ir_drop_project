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

string check(float a)
{
    std::stringstream buf;
    buf.precision(3);
    buf.setf(std::ios::fixed);
    buf << a;
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
bool isSameDistance(float y_location_float)
{
    float goal_loaction = 274.464;
    int temp = (274.464 - y_location_float) * 1000;
    int section = temp % 1728;

    if (section == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{

    if (isSameDistance(268.138))
    {
        cout << "check in side" << endl;
    }
    else
    {
        cout << "no" << endl;
    }

    // string str = "     	OAI21xp33_ASAP7_6t_fix";
    // cout << "prevent : " << str.size() << endl;
    // while (str.find("\t") != string::npos)
    // {
    //     str = str.replace(str.find("\t"), 1, " ");
    // }
    //  cout << "after : " << str.size() << endl;

    // // str = str.replace(str.find("\t"), 1, "#");
    // cout << str << endl;
    // if (str.find("\t") != string::npos)
    // {
    //     cout << "check in side " << endl;
    // }

    // string num= "32.0100";
    //  float num_flo= stof(num);

    //  cout << num << endl;
    // std::ostringstream oss;
    // oss<<num;
    // std::cout<<oss.str()<<std::endl;

    // vector<float> float_vec;
    // float_vec.push_back(10.2);
    // float_vec.push_back(12.5);
    // float_vec.push_back(12.1);
    // sort(float_vec.begin(), float_vec.end());
    // for (int i = 0; i < float_vec.size(); i++)
    // {
    //     cout << float_vec[i] << endl;
    // }

    // float top_y_loc = 278.028;
    // float top_ir = 0.684086;
    // float down_ir = 0.629802;
    // float down_y_loc = 172.512;
    // float sheet = 0.9;
    // float width = 0.224;
    // float goal_width = 0;
    // float goal_ir = 0.03;

    // float temp_y = top_y_loc - down_y_loc;
    // float square = temp_y / width;
    // float resist = square * sheet;
    // float temp_v = top_ir - down_ir;

    // float current = temp_v/resist;
    // goal_width = (current * temp_y * sheet)/goal_ir;

    // cout << goal_width << endl;

    //     // cout << floatToString(0.0359802) << endl;
    //     check(0.0359802);
    //  cout <<  check(0.0359802) << endl;
    return 0;
}