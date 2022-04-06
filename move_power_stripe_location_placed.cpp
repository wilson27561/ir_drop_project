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

// const string DEF_FILE = "6t49_b19_routing_88_9_floorplan.def";
const string LEF_FILE = "characterization_6T_ALL_20200610area_4x.lef";
// const string DEF_TRANSFER_FILE = "6t49_b19_routing_88_9_floorplan_transfer.def";

struct Cell
{
    string cell_name;
    string cell_height;
    string cell_width;
};

vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
void getLefFileCellImformation(string lef_file, unordered_map<string, Cell> *cell_map);

int main()
{
    unordered_map<string, Cell> cell_map;
    getLefFileCellImformation(LEF_FILE, &cell_map);
    





    return 0;
}

void getLefFileCellImformation(string LEF_FILE, unordered_map<string, Cell> *cell_map)
{

    ifstream lef_file(LEF_FILE);
    string lef_content;

    if (lef_file)
    {
        while (getline(lef_file, lef_content))
        {
            if (lef_content.find("MACRO") != string::npos and lef_content.find("6t_fix") != string::npos)
            {
                vector<string> lef_content_array = splitByPattern(lef_content, " ");
                string cell_name = lef_content_array[1];
                while (getline(lef_file, lef_content))
                {
                    if (lef_content.find("SIZE") != string::npos and lef_content.find("BY") != string::npos)
                    {
                        vector<string> lef_content_array = splitByPattern(lef_content, " ");
                        string cell_width = lef_content_array[1];
                        string cell_height = lef_content_array[3];
                        Cell cell;
                        cell.cell_name = cell_name;
                        cell.cell_height = cell_height;
                        cell.cell_width = cell_width;
                        (*cell_map).insert(pair<string, Cell>(cell_name, cell));
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
    lef_file.close();
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
