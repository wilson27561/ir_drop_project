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

struct PinAcess
{
    // string left_bottom_x_location;
    // string left_bottom_y_location;
    // string right_top_x_location;
    // string right_top_y_location;
    string middle_x_location;
    string middle_y_location;
};

struct Rect
{
    string bottom_x_location;
    string bottom_y_location;
    string top_x_location;
    string top_y_location;
    vector<PinAcess> pin_access_vector;
};
// struct PinShape
// {
//     string left_bottom_x_location;
//     string left_bottom_y_location;
//     string right_top_x_location;
//     string right_top_y_location;
// };

struct Pin
{
    string pin_name;
    map<string, vector<Rect>> rect_map;
    // map<string, vector<PinShape>> pin_shape_map;
};

struct CellInfo
{
    string cell_name;
    string height;
    string width;
    // key:PinName value:Pin
    map<string, Pin> pin_map;
};
struct CellPlacedInfo
{
    string cell_id;
    string cell_name;
    string left_x_location;
    string down_y_location;
    string right_x_location;
    string up_y_location;
    string direction;
};

void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void getSizeOfCell(string lef_content, CellInfo *cell_info);
void getPinOfCell(ifstream *lef_file, string left_content, Pin *pin);
void getPortOfPin(ifstream *lef_file, string lef_content, Pin *pin);
void getLayerOfPin(ifstream *lef_file, string lef_content, Pin *pin);
void getDefPlacedImformation(string DEF_TRANSFER_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);

int main()
{
    string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
    string DEF_TRANSFER_FILE = "def_file/gpu_nerualNetwork_ricsv_aes_afterRouting/6t49_aes_routing_run0_transfer.def";
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    string v1_width = "0.072";

    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);

    // for (auto cellInfo : cell_info_map)
    // {
    //     cout << "cell name : " << cellInfo.first << endl;
    //     map<string, Pin> pin_map = cellInfo.second.pin_map;
    //     for (auto pin : pin_map)
    //     {
    //         cout << pin.second.pin_name << " " << pin.second.rect_map.size() << endl;
    //         for (auto rect : pin.second.rect_map)
    //         {
    //             cout << rect.first << endl;
    //             for (int i = 0; i < rect.second.size(); i++)
    //             {
    //                 cout << rect.second[i].bottom_x_location << " " << rect.second[i].bottom_y_location << " " << rect.second[i].top_x_location << " " << rect.second[i].top_y_location << endl;
    //             }
    //         }
    //     }
    // }

    return 0;
}

void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map)
{
    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    int log = 0;
    if (def_transfer_file)
    {
        while (getline(def_transfer_file, def_content))
        {
            if (def_content.find("COMPONENT") != string::npos && ((def_content.find("COMPONENTPIN") != string::npos) == false))
            {
                while (getline(def_transfer_file, def_content))
                {
                    if (def_content.find("PLACED") != string::npos)
                    {
                        // TODO 這裡可以下log
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        CellPlacedInfo cell_placed_info;
                        cell_placed_info.cell_id = def_content_array[1];
                        cell_placed_info.cell_name = def_content_array[2];
                        setPlacePosition(&def_content_array, &cell_placed_info);
                        getCellLocation(&cell_placed_info, &(*cell_info_map));

                        (*cell_placed_map).insert(pair<string, CellPlacedInfo>(cell_placed_info.cell_id, cell_placed_info));
                        log++;
                        if (log % 1000 == 0)
                        {
                            cout << "def data : " << log << endl;
                        }
                    }
                    if (def_content.find("END COMPONENTS") != string::npos)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map)
{

    string cell_name = (*cell_placed_info).cell_name;

    if ((*cell_info_map).count(cell_name))
    {
        string left_x_location = (*cell_placed_info).left_x_location;
        string down_y_location = (*cell_placed_info).down_y_location;
        float left_x_float = stof(left_x_location);
        float down_y_float = stof(down_y_location);
        string cell_width = (*cell_info_map)[cell_name].width;
        string cell_height = (*cell_info_map)[cell_name].height;
        float right_x_float = left_x_float + stof(cell_width);
        float up_y_float = down_y_float + stof(cell_height);

        (*cell_placed_info).right_x_location = to_string(right_x_float);
        (*cell_placed_info).up_y_location = to_string(up_y_float);
    }
    else
    {
        cout << "there some error in cell_info" << endl;
    }
}
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info)
{
    int index = 0;
    for (int i = 0; i < (*def_content_array).size(); i++)
    {
        string content = (*def_content_array)[i];
        if (content == "(")
        {
            (*cell_placed_info).left_x_location = (*def_content_array)[i + 1];
            (*cell_placed_info).down_y_location = (*def_content_array)[i + 2];
            (*cell_placed_info).direction = (*def_content_array)[i + 4];
            break;
        }
    }
}
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map)
{
    ifstream lef_file(LEF_FILE);
    string lef_content;
    int log = 0;
    if (lef_file)
    {
        while (getline(lef_file, lef_content))
        {
            if (lef_content.find("MACRO") != string::npos && lef_content.find("6t_fix") != string::npos)
            {
                CellInfo cell_info;
                vector<string> lef_content_array = splitByPattern(lef_content, " ");
                string cell_name = lef_content_array[1];
                cell_info.cell_name = cell_name;
                map<string, Pin> pin_map;
                while (getline(lef_file, lef_content))
                {
                    vector<string> lef_content_array = splitByPattern(lef_content, " ");
                    if (lef_content.find("SIZE") != string::npos && lef_content.find("BY") != string::npos)
                    {
                        getSizeOfCell(lef_content, &cell_info);
                    }
                    if (lef_content.find("PIN") != string::npos)
                    {
                        Pin pin;
                        map<string, vector<Rect>> rect_map;
                        pin.rect_map = rect_map;
                        getPinOfCell(&lef_file, lef_content, &pin);
                        pin_map.insert(pair<string, Pin>(pin.pin_name, pin));
                        cell_info.pin_map = pin_map;
                    }
                    if ((lef_content_array[0] == "END") && (lef_content_array[1] == cell_info.cell_name))
                    {
                        (*cell_info_map).insert(pair<string, CellInfo>(cell_info.cell_name, cell_info));
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

void getSizeOfCell(string lef_content, CellInfo *cell_info)
{
    vector<string> lef_content_array = splitByPattern(lef_content, " ");
    (*cell_info).height = lef_content_array[3];
    (*cell_info).width = lef_content_array[1];
}

void getPinOfCell(ifstream *lef_file, string lef_content, Pin *pin)
{
    vector<string> lef_content_array = splitByPattern(lef_content, " ");
    (*pin).pin_name = lef_content_array[1];
    // cout << (*pin).pin_name << endl;
    while (getline(*lef_file, lef_content))
    {
        vector<string> lef_content_array = splitByPattern(lef_content, " ");
        if ((lef_content_array[0] == "PORT"))
        {
            getPortOfPin(&(*lef_file), lef_content, &(*pin));
        }
        if ((lef_content_array[0] == "END") && (lef_content_array[1] == (*pin).pin_name))
        {
            // cout << lef_content << endl;
            break;
        }
    }
}

void getPortOfPin(ifstream *lef_file, string lef_content, Pin *pin)
{
    // cout << lef_content << endl;
    vector<Rect> rect_vector;
    string layer_name;
    while (getline(*lef_file, lef_content))
    {
        vector<string> lef_content_array = splitByPattern(lef_content, " ");
        if ((lef_content_array[0] == "LAYER"))
        {
            if (rect_vector.size() > 0)
            {
                (*pin).rect_map.insert(pair<string, vector<Rect>>(layer_name, rect_vector));
                rect_vector.clear();
            }
            layer_name = lef_content_array[1];
        }
        if ((lef_content_array[0] == "RECT"))
        {
            Rect rect;
            vector<string> lef_content_array = splitByPattern(lef_content, " ");
            rect.bottom_x_location = lef_content_array[1];
            rect.bottom_y_location = lef_content_array[2];
            rect.top_x_location = lef_content_array[3];
            rect.top_y_location = lef_content_array[4];
            




            rect_vector.push_back(rect);
        }
        if ((lef_content_array[0] == "END"))
        {
            if (rect_vector.size() > 0)
            {
                (*pin).rect_map.insert(pair<string, vector<Rect>>(layer_name, rect_vector));
                rect_vector.clear();
            }
            // cout << lef_content << endl;
            break;
        }
    }
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

// log++;
// if (log % 100 == 0)
// {
//     cout << "left cell data : " << log << endl;
// }
// break;