
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

struct IoPin
{
    string pin_name;
    string replace_pin_name;
    string pin_info;
    string side;
};
struct IoPinSide
{
    string side;
    vector<IoPin> io_pin_vector;
};

string TOP = "top";
string LEFT = "left";
string BOTTOM = "bottom";
string RIGHT = "right";

vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
void getIoPinInfo(string io_pin_file_name);
string getIoName(string str);

int main()
{
    string io_file_name = "io/6t49run44_9.io";
    getIoPinInfo(io_file_name);
}

void getIoPinInfo(string io_pin_file_name)
{
    ifstream io_pin_file(io_pin_file_name);
    string io_pin_content;
    int log = 0;

    if (io_pin_file)
    {
        while (getline(io_pin_file, io_pin_content))
        {
            if (io_pin_content.find(TOP) != string::npos)
            {
                vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") != string::npos)
                    {
                        vector<string> content_vector = splitByPattern(io_pin_content, " ");
                        string io_pin_content = content_vector[1];
                        string io_name = getIoName(io_pin_content);
                        cout << io_name << endl;
                   

                        //   cout << io_pin_content << endl;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            // if (io_pin_content.find(LEFT) != string::npos)
            // {
            //     vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

            //     while (getline(io_pin_file, io_pin_content))
            //     {
            //         if (io_pin_content.find("pin name") != string::npos)
            //         {
            //             cout << "LEFT : " << io_pin_content << endl;
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }
            // }
            // if (io_pin_content.find(RIGHT) != string::npos)
            // {
            //     vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

            //     while (getline(io_pin_file, io_pin_content))
            //     {
            //         if (io_pin_content.find("pin name") != string::npos)
            //         {
            //             cout << "LEFT : " << io_pin_content << endl;
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }
            // }
            // if (io_pin_content.find(BOTTOM) != string::npos)
            // {
            //     vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

            //     while (getline(io_pin_file, io_pin_content))
            //     {
            //         if (io_pin_content.find("pin name") != string::npos)
            //         {
            //             cout << "LEFT : " << io_pin_content << endl;
            //         }
            //         else
            //         {
            //             break;
            //         }
            //     }
            // }
        }
    }
    else
    {
        cout << "can't found file" << endl;
    }
    io_pin_file.close();
}

string getIoName(string str)
{
    int head = 0;
    int bottom = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '\"')
        {
            if (head == 0)
            {
                head = i;
            }
            else
            {
                bottom = i;
            }
        }
    }
    // cout << head << " " << bottom << endl;
    head = head + 1;
    int length = (bottom - head);
    // cout << head << " " << bottom << endl;
    string io_name = str.substr(head, length);
    // cout << io_name << endl;
    return io_name;
}

vector<string> splitByPattern(string content, string pattern)
{
    vector<string> words;
    size_t pos = 0;
    // ===== 替換換行符號 ======
    while (content.find("\t") != string::npos)
    {
        content = content.replace(content.find("\t"), 1, " ");
    }
    // ===== 替換換行符號 ======
    while ((pos = content.find(pattern)) != string::npos)
    {
        string word = content.substr(0, pos);
        if (trim(word).size() > 0)
        {
            words.push_back(trim(word));
        }
        content.erase(0, pos + pattern.length());
    }
    if (trim(content).size() > 0)
    {
        words.push_back(trim(content));
    }
    return words;
}

string &trim(string &str)
{
    if (str.empty())
    {
        return str;
    }
    int str_length = str.length();
    int start = 0;
    int space_end = str.find_first_not_of(" ");
    str.erase(start, space_end);
    int space_start = (str.find_last_not_of(" ") + 1);
    str_length = str.length();
    str.erase(space_start, str_length);

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