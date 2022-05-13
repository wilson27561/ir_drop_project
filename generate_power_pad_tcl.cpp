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

const string DEF_FILE = "def_file/b19/6t49_powerstripe_design_floorplan_original_transfer.def";

struct PowerPad
{
    string pad_name;
    string net_name;
    string layer;
    float length = 0;
    float width = 0;
    string x_location;
    string y_location;
};

string &trim(string &str);
vector<string> splitByPattern(string content, string pattern);
void getPowerPadLocation(string def_file_name);
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad);
int main()
{
    

    getPowerPadLocation(DEF_FILE);

    return 0;
}

void getPowerPadLocation(string def_file_name)
{

    ifstream def_file(def_file_name);
    string def_content;
    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("PINS") != string::npos)
            {
                while (getline(def_file, def_content))
                {
                    if (def_content.find("VDD") != string::npos || def_content.find("VSS") != string::npos)
                    {
                        PowerPad power_pad;
                        setPowerPadInfo(&def_file, def_content, &power_pad);
                    }
                    if (def_content.find("END PINS") != string::npos)
                    {
                        break;
                    }
                }
            }
        }
    }
}

void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad)
{

    vector<string> def_content_array = splitByPattern(def_content, " ");
    (*power_pad).pad_name = def_content_array[1];
    (*power_pad).net_name = def_content_array[4];
    while (getline((*def_file), def_content))
    {
        if (def_content.find("LAYER") != string::npos)
        {
            vector<string> def_content_array = splitByPattern(def_content, " ");
            setPowerPadLengthtWidth(&def_content_array, &(*power_pad));
        }
        else if (def_content.find("PLACED") != string::npos)
        {
            vector<string> def_content_array = splitByPattern(def_content, " ");
            setPowerPadLocation(&def_content_array, &(*power_pad));
            break;
        }
        else
        {
            cout << "read pin error " << endl;
            break;
        }
    }
}

void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad)
{
    string x_location = (*def_content_array)[3];
    string y_location = (*def_content_array)[4];
    (*power_pad).x_location = x_location;
    (*power_pad).y_location = y_location;
    cout << (*power_pad).x_location << endl;
}

void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad)
{
    string power_pad_left = (*def_content_array)[4];
    string power_pad_right = (*def_content_array)[8];
    float power_pad_left_float = abs(stof((*def_content_array)[4]));
    float power_pad_right_float = abs(stof((*def_content_array)[8]));
    (*power_pad).width = power_pad_left_float + power_pad_right_float;
    string power_pad_up = (*def_content_array)[5];
    string power_pad_down = (*def_content_array)[9];
    float power_pad_up_float = abs(stof((*def_content_array)[5]));
    float power_pad_down_float = abs(stof((*def_content_array)[9]));
    (*power_pad).length = power_pad_up_float + power_pad_down_float;
}

void setPowerPad(ifstream def_file)
{
}

//根據pattern切字串
vector<string> splitByPattern(string content, string pattern)
{
    vector<string> words;
    size_t pos = 0;
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

//去掉首尾全部空格
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