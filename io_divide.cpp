
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
    vector<IoPin> io_power_pin_vector;
    vector<IoPin> power_pin_vector;
    vector<IoPin> io_pin_vector;
};

string TOP = "top";
string LEFT = "left";
string BOTTOM = "bottom";
string RIGHT = "right";

vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
void getIoPinInfo(string io_pin_file_name, vector<string> *io_pin_side_vector, unordered_map<string, IoPinSide> *io_pin_side_map);
string getIoName(string str);
void setIoPinInfo(unordered_map<string, IoPinSide> *io_pin_side_map, string side, string io_pin_content);
void setIoPinPosition(IoPinSide *io_pin_side);
void subreplace(string &resource_str, string &sub_str, string &new_str);
void replaceContent(unordered_map<string, IoPinSide> *io_pin_side_map);
int main()
{
    unordered_map<string, IoPinSide> io_pin_side_map;
    vector<string> io_pin_side_vector;

    string io_file_name = "io/6t49run44_9.io";
    getIoPinInfo(io_file_name, &io_pin_side_vector, &io_pin_side_map);
    setIoPinPosition(&io_pin_side_map[TOP]);
    replaceContent(&io_pin_side_map);

    // for (int i = 0; i < io_pin_side_map[TOP].io_pin_vector.size(); i++)
    // {
    //     cout <<  io_pin_side_map[TOP].io_pin_vector[i].pin_name << endl;
    // }
}
void replaceContent(unordered_map<string, IoPinSide> *io_pin_side_map)
{
    for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
    {
        string side = io_pin_side_map_it->first;
        cout << "side : " << side << endl;
        IoPinSide io_pin_side = io_pin_side_map_it->second;

        for (int i = 0; i < io_pin_side.io_power_pin_vector.size(); i++)
        {
            IoPin io_pin = io_pin_side.io_power_pin_vector[i];
            string replace_pin_name = io_pin.replace_pin_name;
            string pin_name = io_pin.pin_name;
            string pin_info = io_pin.pin_info;
            cout << "replace_pin_name : " << replace_pin_name << endl;
            cout << "pin_name         : " << pin_name << endl;
            cout << "pin_info         : " << pin_info << endl;
            subreplace(pin_info, pin_name, replace_pin_name);
            // cout << "pin_info         : " << pin_info << endl;
        }
    }
}

void subreplace(string &resource_str, string &sub_str, string &new_str)
{

    string def_str = resource_str;

    string::size_type pos = 0;
    string temp_string;

    // while ((pos = def_str.find(sub_str)) != string::npos) //替换所有指定子串
    // {

    //     resource_str.replace(pos, sub_str.length(), new_str);
    // }
    for (int i = 0; i < resource_str.size(); i++)
    {
        if (resource_str[i] != ' ')
        {
            temp_string.insert(temp_string.size(), 1, resource_str[i]);
        }
        if (resource_str[i] == ' ' && temp_string.size() != 0)
        {
            if (sub_str == temp_string)
            {
                resource_str.replace(i - (temp_string.size()), sub_str.length(), new_str);
            }
            //   cout << temp_string << " " << temp_string.size() << " " << i- (temp_string.size()) << endl;
            temp_string = "";
        }
        if (i == (resource_str.size() - 1) && temp_string.size() != 0)
        {
            if (sub_str == temp_string)
            {
                resource_str.replace(i - (temp_string.size()), sub_str.length(), new_str);
            }
            //   cout << temp_string << " " << temp_string.size() << " " <<  i- temp_string.size()<< endl;
            temp_string = "";
        }
    }
    cout << "resource_str         : " << resource_str << endl;
}

void setIoPinPosition(IoPinSide *io_pin_side)
{
    int number_of_power_pad = (*io_pin_side).power_pin_vector.size();
    number_of_power_pad = number_of_power_pad / 2;
    int divide_side = number_of_power_pad + 1;
    cout << divide_side << endl;

    int number_of_pin = (*io_pin_side).io_pin_vector.size();
    int number_of_side = number_of_pin / divide_side;
    cout << number_of_side << endl;

    vector<IoPin> io_pin_vector = (*io_pin_side).io_pin_vector;
    vector<IoPin> power_pin_vector = (*io_pin_side).power_pin_vector;

    int number_of_side_count = 1;
    cout << (*io_pin_side).io_power_pin_vector.size() << endl;

    for (int i = 0; i < (*io_pin_side).io_power_pin_vector.size(); i++)
    {
        cout << i << endl;
        // cout << "number_of_side_count : " << number_of_side_count << endl;
        if (number_of_side_count == (number_of_side + 1))
        {
            (*io_pin_side).io_power_pin_vector[i].replace_pin_name = power_pin_vector[0].pin_name;
            power_pin_vector.erase(power_pin_vector.begin());
            number_of_side_count += 1;
        }
        else if (number_of_side_count == (number_of_side + 2))
        {
            (*io_pin_side).io_power_pin_vector[i].replace_pin_name = power_pin_vector[0].pin_name;
            power_pin_vector.erase(power_pin_vector.begin());
            number_of_side_count = 1;
        }
        else
        {
            // cout << "pin name : " << io_pin_vector[0].pin_name << endl;
            (*io_pin_side).io_power_pin_vector[i].replace_pin_name = io_pin_vector[0].pin_name;
            io_pin_vector.erase(io_pin_vector.begin());
            number_of_side_count += 1;
        }
    }

    // cout << "check in side" << endl;
    // for (int i = 0; i < (*io_pin_side).io_power_pin_vector.size(); i++)
    // {
    //     cout << (*io_pin_side).io_power_pin_vector[i].replace_pin_name << endl;
    // }
}

void setIoPinInfo(unordered_map<string, IoPinSide> *io_pin_side_map, string side, string io_pin_content)
{
    vector<string> content_vector = splitByPattern(io_pin_content, " ");

    string pin_name = getIoName(io_pin_content);

    if ((*io_pin_side_map).count(side))
    {
        IoPin io_pin;
        io_pin.pin_name = pin_name;
        io_pin.side = side;
        io_pin.pin_info = io_pin_content;
        (*io_pin_side_map)[side].io_power_pin_vector.push_back(io_pin);
        if (pin_name.find("VDD") != string::npos || pin_name.find("VSS") != string::npos)
        {
            (*io_pin_side_map)[side].power_pin_vector.push_back(io_pin);
        }
        else
        {
            (*io_pin_side_map)[side].io_pin_vector.push_back(io_pin);
        }
    }
    else
    {
        IoPinSide io_pin_side;
        vector<IoPin> io_power_pin_vector;
        io_pin_side.side = side;
        IoPin io_pin;
        io_pin.pin_name = pin_name;
        io_pin.side = side;
        io_pin.pin_info = io_pin_content;
        io_power_pin_vector.push_back(io_pin);
        io_pin_side.io_power_pin_vector = io_power_pin_vector;

        if (pin_name.find("VDD") != string::npos || pin_name.find("VSS") != string::npos)
        {
            vector<IoPin> power_pin_vector;
            power_pin_vector.push_back(io_pin);
            io_pin_side.power_pin_vector = power_pin_vector;
        }
        else
        {
            vector<IoPin> io_pin_vector;
            io_pin_vector.push_back(io_pin);
            io_pin_side.io_pin_vector = io_pin_vector;
        }

        (*io_pin_side_map).insert(pair<string, IoPinSide>(side, io_pin_side));
    }
}

void getIoPinInfo(string io_pin_file_name, vector<string> *io_pin_side_vector, unordered_map<string, IoPinSide> *io_pin_side_map)
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
                (*io_pin_side_vector).push_back(TOP);
                vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") != string::npos)
                    {
                        setIoPinInfo(&(*io_pin_side_map), TOP, io_pin_content);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (io_pin_content.find(LEFT) != string::npos)
            {
                vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") != string::npos)
                    {
                        setIoPinInfo(&(*io_pin_side_map), LEFT, io_pin_content);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (io_pin_content.find(RIGHT) != string::npos)
            {
                vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") != string::npos)
                    {
                        setIoPinInfo(&(*io_pin_side_map), RIGHT, io_pin_content);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (io_pin_content.find(BOTTOM) != string::npos)
            {
                vector<string> io_pin_content_array = splitByPattern(io_pin_content, " ");

                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") != string::npos)
                    {
                        setIoPinInfo(&(*io_pin_side_map), BOTTOM, io_pin_content);
                    }
                    else
                    {
                        break;
                    }
                }
            }
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