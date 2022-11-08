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
#include <iomanip>
struct IrDropPoint
{
    string layer;
    string x_location;
    string y_location;
    string ir_drop;
};

struct IrDropInfo
{
    string config_name;
    // key : x_y : ir_drop_point
    unordered_map<string, IrDropPoint> point_map;
};

struct IrDropComparePoint
{
    string x_location;
    string y_location;
    // key_config_name , ir_drop_value
    unordered_map<string, string> ir_drop_map;
};

void getIrDropPointMap(vector<string> *ir_drop_file_vector, map<string, IrDropInfo> *ir_drop_info_map, vector<string> *config_name_vector);
void getIrDropCompareMap(map<string, IrDropInfo> *ir_drop_info_map, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map, int config_size);
void printXLocationInfo(string compare_file_name, vector<string> *x_point_vector, vector<string> *config_name_vector, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map);
void printYLocationInfo(string compare_file_name, vector<string> *y_point_vector, vector<string> *config_name_vector, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map);
void getIrDropInfo(map<string, IrDropInfo> *ir_drop_info_map);
bool sortIrDropComparePointY(IrDropComparePoint ir_drop_compare_point_a, IrDropComparePoint ir_drop_compare_point_b);
bool sortIrDropComparePointX(IrDropComparePoint ir_drop_compare_point_a, IrDropComparePoint ir_drop_compare_point_b);
bool sortIrDropPointY(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b);
bool isSameDistance(float y_location_float);
// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);

int main()
{
    string ir_drop_config_file = "config/ir_drop_config.txt";
    // string compare_file_name = "ir_drop_compare.txt";
    vector<string> ir_drop_file_vector;
    ifstream config(ir_drop_config_file);
    string config_content;

    map<string, IrDropInfo> ir_drop_info_map;
    // key:x_y value : ir_drop_value
    unordered_map<string, IrDropComparePoint> ir_drop_compare_map;
    vector<string> config_name_vector;

    if (config)
    {
        while (getline(config, config_content))
        {
            vector<string> config_content_array = splitByPattern(config_content, " ");
            if (config_content_array[0] == "IR_REPORT_FILE")
            {
                ir_drop_file_vector.push_back(config_content_array[2]);
            }
        }
    }
    // vector<string> x_point_vector;
    // x_point_vector.push_back("164.772");
    // vector<string> y_point_vector;
    // y_point_vector.push_back("125.856");
    //記得與config file 同步
    // config_name_vector.push_back("power_rail_28_ir_M3_rpt");
    // config_name_vector.push_back("power_rail_23_ir_M3_rpt");
    // config_name_vector.push_back("power_rail_24_ir_M3_rpt");
    // config_name_vector.push_back("power_rail_25_ir_M3_rpt");
    // config_name_vector.push_back("power_rail_26_ir_M3_rpt");
    // config_name_vector.push_back("power_rail_29_ir_M3_rpt");
    // config_name_vector.push_back("power_rail_30_ir_M3_rpt");

    getIrDropPointMap(&ir_drop_file_vector, &ir_drop_info_map, &config_name_vector);
    getIrDropInfo(&ir_drop_info_map);
    // getIrDropCompareMap(&ir_drop_info_map, &ir_drop_compare_map, config_name_vector.size());
    // printXLocationInfo(compare_file_name, &x_point_vector, &config_name_vector, &ir_drop_compare_map);
    // printYLocationInfo(compare_file_name, &y_point_vector, &config_name_vector, &ir_drop_compare_map);

    // for (auto ir_drop_info_map_iter = ir_drop_info_map.begin(); ir_drop_info_map_iter != ir_drop_info_map.end(); ++ir_drop_info_map_iter)
    // {
    //     cout << ir_drop_info_map_iter->first << endl;
    //     cout << ir_drop_info_map_iter->second.point_map.size() << endl;
    // }

    // for (auto ir_drop_compare_map_iter = ir_drop_compare_map.begin(); ir_drop_compare_map_iter != ir_drop_compare_map.end(); ++ir_drop_compare_map_iter)
    // {
    //     // cout << ir_drop_compare_map_iter->first << endl;
    //     // cout << ir_drop_compare_map_iter->second.ir_drop_map.size() << endl;
    //     // if (ir_drop_compare_map_iter->second.ir_drop_map.size() <= 1)
    //     // {
    //     //     cout << ir_drop_compare_map_iter->first << endl;
    //     // }
    // }

    return 0;
}

void getIrDropInfo(map<string, IrDropInfo> *ir_drop_info_map)
{
    for (auto ir_drop_info_map_iter = (*ir_drop_info_map).begin(); ir_drop_info_map_iter != (*ir_drop_info_map).end(); ++ir_drop_info_map_iter)
    {
        string config_name = ir_drop_info_map_iter->first;
        IrDropInfo ir_drop_info = (*ir_drop_info_map)[config_name];
        unordered_map<string, IrDropPoint> point_map = ir_drop_info.point_map;
        vector<IrDropPoint> ir_drop_point_vector;
        for (auto point_map_iter = point_map.begin(); point_map_iter != point_map.end(); ++point_map_iter)
        {
            string x_y_location = point_map_iter->first;
            vector<string> x_y_location_vector = splitByPattern(x_y_location, "_");
            string x_location = x_y_location_vector[0];
            string y_location = x_y_location_vector[1];

            if (x_location == "164.772")
            {
                ir_drop_point_vector.push_back(point_map_iter->second);
            }
        }
        sort(ir_drop_point_vector.begin(), ir_drop_point_vector.end(), sortIrDropPointY);
        for (int i = 0; i < ir_drop_point_vector.size(); i++)
        {
            float y_location_float = stof(ir_drop_point_vector[i].y_location);
            // cout << "(" << ir_drop_point_vector[i].x_location << " " << ir_drop_point_vector[i].y_location << ")" << endl;
            if ((i != 0 || i != (ir_drop_point_vector.size() - 1)) && isSameDistance(y_location_float)&& y_location_float <=274.465 &&  y_location_float >= 170.783)
            {
                float ir_float = stof(ir_drop_point_vector[i].ir_drop);
                cout << ir_drop_point_vector[i].x_location << " " << ir_drop_point_vector[i].y_location << " " << ir_drop_point_vector[i].ir_drop << endl;
            }
        }
    }
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

bool sortIrDropPointY(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b)
{
    return stof(ir_drop_point_a.y_location) > stof(ir_drop_point_b.y_location);
}

bool sortIrDropComparePointY(IrDropComparePoint ir_drop_compare_point_a, IrDropComparePoint ir_drop_compare_point_b)
{
    return stof(ir_drop_compare_point_a.y_location) < stof(ir_drop_compare_point_b.y_location);
}

bool sortIrDropComparePointX(IrDropComparePoint ir_drop_compare_point_a, IrDropComparePoint ir_drop_compare_point_b)
{
    return stof(ir_drop_compare_point_a.x_location) < stof(ir_drop_compare_point_b.x_location);
}

//寫一個function  給 x (vector) 座標 就dump 出 這條線上所有座標的ir_drop_值

void printXLocationInfo(string compare_file_name, vector<string> *x_point_vector, vector<string> *config_name_vector, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map)
{
    // key : x  : y :vector<IrDropComparePoint>
    map<string, vector<IrDropComparePoint>> compare_point_map;
    for (int i = 0; i < (*x_point_vector).size(); i++)
    {
        string goal_x_location = (*x_point_vector)[i];
        for (auto ir_drop_compare_map_iter = (*ir_drop_compare_map).begin(); ir_drop_compare_map_iter != (*ir_drop_compare_map).end(); ++ir_drop_compare_map_iter)
        {
            string ir_drop_point = ir_drop_compare_map_iter->first;
            vector<string> point_location_array = splitByPattern(ir_drop_point, "_");
            string x_location = point_location_array[0];
            string y_location = point_location_array[1];
            IrDropComparePoint ir_drop_compare_point = ir_drop_compare_map_iter->second;

            if (goal_x_location == x_location)
            {
                if (compare_point_map.count(goal_x_location))
                {
                    compare_point_map[goal_x_location].push_back(ir_drop_compare_point);
                }
                else
                {
                    vector<IrDropComparePoint> compare_point_vector;
                    compare_point_vector.push_back(ir_drop_compare_point);
                    compare_point_map.insert(pair<string, vector<IrDropComparePoint>>(goal_x_location, compare_point_vector));
                }
            }
        }
    }
    ofstream myfile;
    myfile.open(compare_file_name);

    // for (int k = 0; k < (*config_name_vector).size(); k++)
    // {
    //     myfile << (*config_name_vector)[k] << "    ";
    // }
    // myfile << "" << endl;
    //印出所有的點
    for (int i = 0; i < (*x_point_vector).size(); i++)
    {
        string x_point = (*x_point_vector)[i];
        vector<IrDropComparePoint> ir_drop_compare_point_vector = compare_point_map[x_point];
        sort(ir_drop_compare_point_vector.begin(), ir_drop_compare_point_vector.end(), sortIrDropComparePointY);
        for (int j = 0; j < ir_drop_compare_point_vector.size(); j++)
        {
            IrDropComparePoint ir_drop_compare_point = ir_drop_compare_point_vector[j];
            myfile << "(" << ir_drop_compare_point.x_location << " " << ir_drop_compare_point.y_location << ")" << endl;
            for (int k = 1; k < (*config_name_vector).size(); k++)
            {
            }
        }

        // for (int k = 0; k < (*config_name_vector).size(); k++)
        // {
        //     string config_name = (*config_name_vector)[k];
        //     string ir_drop_value = ir_drop_compare_point.ir_drop_map[config_name];
        //     // if (config_name == "power_rail_30_ir_M3_rpt")
        //     // {
        //     float ir_drop_float = stof(ir_drop_value);
        //     ir_drop_float = ir_drop_float * 0.001;

        //     myfile << (0.7 - ir_drop_float) << endl;
        //     // }
        // }
    }
    myfile.close();
}

// 寫一個function  給 y 座標 就dump 出 這條線上所有座標的ir_drop_值
void printYLocationInfo(string compare_file_name, vector<string> *y_point_vector, vector<string> *config_name_vector, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map)
{
    map<string, vector<IrDropComparePoint>> compare_point_map;
    for (int i = 0; i < (*y_point_vector).size(); i++)
    {
        string goal_y_location = (*y_point_vector)[i];
        for (auto ir_drop_compare_map_iter = (*ir_drop_compare_map).begin(); ir_drop_compare_map_iter != (*ir_drop_compare_map).end(); ++ir_drop_compare_map_iter)
        {
            string ir_drop_point = ir_drop_compare_map_iter->first;
            vector<string> point_location_array = splitByPattern(ir_drop_point, "_");
            string y_location = point_location_array[1];
            IrDropComparePoint ir_drop_compare_point = ir_drop_compare_map_iter->second;
            //    cout << "y_location : " << y_location << endl;
            if (goal_y_location == y_location)
            {

                if (compare_point_map.count(goal_y_location))
                {
                    compare_point_map[goal_y_location].push_back(ir_drop_compare_point);
                }
                else
                {
                    vector<IrDropComparePoint> compare_point_vector;
                    compare_point_vector.push_back(ir_drop_compare_point);
                    compare_point_map.insert(pair<string, vector<IrDropComparePoint>>(goal_y_location, compare_point_vector));
                }
            }
        }
    }

    ofstream myfile;
    myfile.open(compare_file_name);

    // for (int k = 0; k < (*config_name_vector).size(); k++)
    // {
    //     myfile << (*config_name_vector)[k] << "    ";
    // }
    // myfile << "" << endl;
    //印出所有的點

    for (int i = 0; i < (*y_point_vector).size(); i++)
    {
        string y_point = (*y_point_vector)[i];
        // cout << "y_point" << y_point << endl;
        vector<IrDropComparePoint> ir_drop_compare_point_vector = compare_point_map[y_point];
        // cout << "ir_drop_compare_point_vector " << ir_drop_compare_point_vector.size() << endl;
        sort(ir_drop_compare_point_vector.begin(), ir_drop_compare_point_vector.end(), sortIrDropComparePointX);
        for (int j = 0; j < ir_drop_compare_point_vector.size(); j++)
        {
            IrDropComparePoint ir_drop_compare_point = ir_drop_compare_point_vector[j];
            // myfile << "(" << ir_drop_compare_point.x_location << " " << ir_drop_compare_point.y_location << ")";

            for (int k = 0; k < (*config_name_vector).size(); k++)
            {
                string config_name = (*config_name_vector)[k];
                string ir_drop_value = ir_drop_compare_point.ir_drop_map[config_name];

                float ir_drop_float = stof(ir_drop_value);
                // ir_drop_float = ir_drop_float * 0.001;

                myfile << ir_drop_float << " ";
            }
            myfile << "" << endl;
        }

        // for (int k = 0; k < (*config_name_vector).size(); k++)
        // {
        //     string config_name = (*config_name_vector)[k];
        //     string ir_drop_value = ir_drop_compare_point.ir_drop_map[config_name];
        //     // if (config_name == "power_rail_30_ir_M3_rpt")
        //     // {
        //     float ir_drop_float = stof(ir_drop_value);
        //     ir_drop_float = ir_drop_float * 0.001;

        //     myfile << (0.7 - ir_drop_float) << endl;
        //     // }
        // }
    }
    myfile.close();
};

// 找到重複的點 做出一格point map
void getIrDropCompareMap(map<string, IrDropInfo> *ir_drop_info_map, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map, int config_size)
{
    for (auto ir_drop_info_map_iter = (*ir_drop_info_map).begin(); ir_drop_info_map_iter != (*ir_drop_info_map).end(); ++ir_drop_info_map_iter)
    {
        string config_name = ir_drop_info_map_iter->first;
        cout << "config_name : " << config_name << endl;
        unordered_map<string, IrDropPoint> ir_drop_point_map = ir_drop_info_map_iter->second.point_map;

        for (auto ir_drop_point_map_iter = ir_drop_point_map.begin(); ir_drop_point_map_iter != ir_drop_point_map.end(); ++ir_drop_point_map_iter)
        {
            string point_key = ir_drop_point_map_iter->first;
            vector<string> point_key_vector = splitByPattern(point_key, "_");
            string x_point = point_key_vector[0];
            string y_point = point_key_vector[1];
            if ((*ir_drop_compare_map).count(point_key))
            {
                string ir_drop = (ir_drop_point_map)[point_key].ir_drop;
                (*ir_drop_compare_map)[point_key].ir_drop_map.insert(pair<string, string>(config_name, ir_drop));
            }
            else
            {
                string ir_drop = (ir_drop_point_map)[point_key].ir_drop;
                unordered_map<string, string> ir_drop_map;
                ir_drop_map.insert(pair<string, string>(config_name, ir_drop));
                IrDropComparePoint ir_drop_compare_point;
                ir_drop_compare_point.ir_drop_map = ir_drop_map;
                ir_drop_compare_point.x_location = x_point;
                ir_drop_compare_point.y_location = y_point;
                (*ir_drop_compare_map).insert(pair<string, IrDropComparePoint>(point_key, ir_drop_compare_point));
            }
        }
    }

    //刪除不重複的key
    vector<string> erase_key;
    for (auto ir_drop_compare_map_iter = (*ir_drop_compare_map).begin(); ir_drop_compare_map_iter != (*ir_drop_compare_map).end(); ++ir_drop_compare_map_iter)
    {
        string key = ir_drop_compare_map_iter->first;
        IrDropComparePoint ir_drop_compare_point = ir_drop_compare_map_iter->second;
        // cout << ir_drop_compare_point.ir_drop_map.size() << " " << config_size << endl;
        if (ir_drop_compare_point.ir_drop_map.size() != config_size)
        {
            erase_key.push_back(key);
        }
    }
    for (int i = 0; i < erase_key.size(); i++)
    {
        (*ir_drop_compare_map).erase(erase_key[i]);
    }
}

void getIrDropPointMap(vector<string> *ir_drop_file_vector, map<string, IrDropInfo> *ir_drop_info_map, vector<string> *config_name_vector)
{
    // put name in side ir_drop_info
    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {
        string ir_drop_file_name = (*ir_drop_file_vector)[i];
        vector<string> ir_file_content_array = splitByPattern(ir_drop_file_name, "/");
        string file_name_detail = ir_file_content_array[1];
        vector<string> file_content_array = splitByPattern(file_name_detail, "_");
        string layer = file_content_array[4];
        cout << file_name_detail << endl;
        IrDropInfo ir_drop_info;
        unordered_map<string, IrDropPoint> point_map;
        ir_drop_info.point_map = point_map;
        (*ir_drop_info_map).insert(pair<string, IrDropInfo>(file_name_detail, ir_drop_info));
    }

    // put info in side ir_drop_info
    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {
        string ir_drop_file_name = (*ir_drop_file_vector)[i];
        vector<string> ir_file_content_array = splitByPattern(ir_drop_file_name, "/");
        string file_name_detail = ir_file_content_array[1];
        vector<string> file_content_array = splitByPattern(file_name_detail, "_");
        string layer = file_content_array[4];
        cout << file_name_detail << endl;

        if (file_content_array.size() != 6)
        {
            cout << "ir_drop_report_error" << endl;
        }

        ifstream ir_file(ir_drop_file_name);
        string ir_content;
        int log = 0;
        if (ir_file)
        {
            while (getline(ir_file, ir_content))
            {
                if (ir_content.find("ir") != string::npos)
                {
                    continue;
                }
                else if (ir_content.find("Range") != string::npos && ir_content.find("Total") != string::npos)
                {
                    break;
                }
                else
                {

                    vector<string> ir_content_array = splitByPattern(ir_content, " ");
                    IrDropPoint ir_drop_point;
                    ir_drop_point.ir_drop = ir_content_array[0];
                    ir_drop_point.layer = ir_content_array[1];
                    ir_drop_point.x_location = ir_content_array[2];
                    ir_drop_point.y_location = ir_content_array[3];

                    string key = ir_drop_point.x_location + "_" + ir_drop_point.y_location;
                    (*ir_drop_info_map)[file_name_detail].point_map.insert(pair<string, IrDropPoint>(key, ir_drop_point));
                    log++;
                    if (log % 1000 == 0)
                    {
                        cout << "ir data : " << log << endl;
                    }
                }
            }
        }
        else
        {
            cout << " read ir file error " << endl;
        }
    }
}

bool sortStripeLocationY(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b)
{
    return stof(ir_drop_point_a.y_location) < stof(ir_drop_point_b.y_location);
}
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