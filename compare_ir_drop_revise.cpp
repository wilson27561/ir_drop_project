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
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct Row
{
    string start_y_location;
    string end_y_location;
};

void getIrDropPointMap(vector<string> *ir_drop_file_vector, map<string, IrDropInfo> *ir_drop_info_map);
void getIrDropCompareMap(map<string, IrDropInfo> *ir_drop_info_map, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map, int config_size);
void printXLocationInfo(string compare_file_name, vector<string> *x_point_vector, vector<string> *config_name_vector, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map);
void printYLocationInfo(string compare_file_name, vector<string> *y_point_vector, vector<string> *config_name_vector, unordered_map<string, IrDropComparePoint> *ir_drop_compare_map);
void getIrDropInfo(map<string, IrDropInfo> *ir_drop_info_map);
bool sortIrDropComparePointY(IrDropComparePoint ir_drop_compare_point_a, IrDropComparePoint ir_drop_compare_point_b);
bool sortIrDropComparePointX(IrDropComparePoint ir_drop_compare_point_a, IrDropComparePoint ir_drop_compare_point_b);
bool sortIrDropPointY(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b);
bool isSameDistance(float y_location_float);
void getRow(CoreSite *core_site, vector<Row> *row_vector);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void getLocation(string x_location, vector<IrDropPoint> *ir_drop_point_vector, vector<Row> *row_vector);
void getIrPoint(vector<IrDropPoint> *ir_drop_point_vector, vector<Row> *row_vector, map<string, IrDropInfo> *ir_drop_info_map, vector<string> *ir_drop_file_vector);
// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
bool floatIsEqual(float a, float b);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool sortYLocation(IrDropPoint ir_drop_point_a,IrDropPoint ir_drop_point_b);

int main()
{

    // vector<string> x_point_vector;
    // x_point_vector.push_back("164.772");
    // vector<string> y_point_vector;
    // y_point_vector.push_back("125.856");
    //記得與config file 同步
    vector<string> ir_drop_file_vector;
    vector<IrDropPoint> ir_drop_point_vector;
    map<string, IrDropInfo> ir_drop_info_map;
    vector<Row> row_vector;

    set<string> x_y_location_set;

    CoreSite core_site;
    string DEF_TRANSFER_FILE = "def_file/b19_core_60/6t49_b19_routing_run0_17_transfer.def";
    // ir_drop_file_vector.push_back("ir_report/power_rail_17_ir_M3_rpt");
    // ir_drop_file_vector.push_back("ir_report/power_rail_27_ir_M3_rpt");
    ir_drop_file_vector.push_back("ir_report/power_rail_28_ir_M3_rpt");
    string x_location = "164.772";

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    getRow(&core_site, &row_vector);
    getLocation(x_location, &ir_drop_point_vector, &row_vector);
    getIrDropPointMap(&ir_drop_file_vector, &ir_drop_info_map);
    // getIrDropInfo(&ir_drop_info_map);
    getIrPoint(&ir_drop_point_vector,&row_vector, &ir_drop_info_map,&ir_drop_file_vector);

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
void getLocation(string x_location, vector<IrDropPoint> *ir_drop_point_vector, vector<Row> *row_vector)
{
    set<string> x_y_location_set;
    for (int i = 0; i < (*row_vector).size(); i++)
    {
        string start_y_location = (*row_vector)[i].start_y_location;
        string end_y_location = (*row_vector)[i].end_y_location;
        string start_location = x_location + "_" + start_y_location;
        string end_location = x_location + "_" + end_y_location;
        x_y_location_set.insert(start_location);
        x_y_location_set.insert(end_location);
    }
    for (auto location : x_y_location_set)
    {
        vector<string> ir_location_array = splitByPattern(location, "_");
        IrDropPoint ir_drop_point;
        ir_drop_point.x_location = ir_location_array[0];
        ir_drop_point.y_location = ir_location_array[1];
        (*ir_drop_point_vector).push_back(ir_drop_point);
    }
}
bool sortYLocation(IrDropPoint ir_drop_point_a,IrDropPoint ir_drop_point_b){
    return stof(ir_drop_point_a.y_location) > stof(ir_drop_point_b.y_location);
}

void getIrPoint(vector<IrDropPoint> *ir_drop_point_vector, vector<Row> *row_vector, map<string, IrDropInfo> *ir_drop_info_map, vector<string> *ir_drop_file_vector)
{
    vector<IrDropPoint> sort_ir_drop_point_vector = (*ir_drop_point_vector);

    sort(sort_ir_drop_point_vector.begin(),sort_ir_drop_point_vector.end(),sortYLocation);
    

    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {
        vector<string> ir_file_content_array = splitByPattern((*ir_drop_file_vector)[i], "/");
        string file_name_detail = ir_file_content_array[1];

        unordered_map<string, IrDropPoint> point_map = (*ir_drop_info_map)[file_name_detail].point_map;

        for (int j = 0; j <sort_ir_drop_point_vector.size(); j++)
        {
            string x_location = sort_ir_drop_point_vector[j].x_location;
            string y_location = sort_ir_drop_point_vector[j].y_location;
            string ir_location = x_location + "_" + y_location;

            cout << x_location << " " << y_location << " " << point_map[ir_location].ir_drop << endl;
        }
    }
}

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site)
{
    ifstream DEF_TRANSFER_FILE(DEF_TRANSFER_FILE_name);
    string def_content;

    if (DEF_TRANSFER_FILE)
    {
        while (getline(DEF_TRANSFER_FILE, def_content))
        {

            if (def_content.find("FE_CORE_BOX_LL_X") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                (*core_site).left_x_location = def_content_array[3];
            }
            if (def_content.find("FE_CORE_BOX_UR_X") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                (*core_site).right_x_location = def_content_array[3];
            }
            if (def_content.find("FE_CORE_BOX_LL_Y") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                (*core_site).down_y_location = def_content_array[3];
            }
            if (def_content.find("FE_CORE_BOX_UR_Y") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                (*core_site).up_y_location = def_content_array[3];
            }
            if (def_content.find("END PROPERTYDEFINITIONS") != string::npos)
            {
                break;
            }
        }
    }
    else
    {
        cout << "read " << DEF_TRANSFER_FILE_name << " error " << endl;
    }
}
void getRow(CoreSite *core_site, vector<Row> *row_vector)
{
    string core_start_y_location = (*core_site).down_y_location;
    float core_start_y_location_float = stof((*core_site).down_y_location);
    string core_end_y_location = (*core_site).up_y_location;
    float core_end_y_location_float = stof((*core_site).up_y_location);
    string row = "0.864";
    float row_float = stof(row);
    float i = core_end_y_location_float;

    while (floatIsEqualOrMore(i, core_start_y_location_float))
    {
        if (floatIsEqual(i, core_end_y_location_float))
        {
            float row_start_y_location_float = i;
            float row_end_y_location_float = i - row_float;
            string row_start_y_location = floatToString(row_start_y_location_float);
            string row_end_y_location = floatToString(row_end_y_location_float);

            Row row;
            row.start_y_location = row_start_y_location;
            row.end_y_location = row_end_y_location;
            (*row_vector).push_back(row);

            i -= row_float;
        }
        else
        {
            float row_start_y_location_float = i;
            float row_end_y_location_float = i - (row_float * 2);
            string row_start_y_location = floatToString(row_start_y_location_float);
            string row_end_y_location = floatToString(row_end_y_location_float);

            Row row;
            row.start_y_location = row_start_y_location;
            row.end_y_location = row_end_y_location;
            (*row_vector).push_back(row);

            float row_distance = row_float * 2;
            i -= row_distance;
        }
    }

    // for (int i = 0; i < (*row_vector).size(); i++)
    // {
    //     cout << (*row_vector)[i].start_y_location << " " << (*row_vector)[i].end_y_location << endl;
    // }
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
            if ((i != 0 || i != (ir_drop_point_vector.size() - 1)) && isSameDistance(y_location_float) && y_location_float <= 274.465 && y_location_float >= 170.783)
            {
                float ir_float = stof(ir_drop_point_vector[i].ir_drop);
                // cout << ir_drop_point_vector[i].x_location << " " << ir_drop_point_vector[i].y_location << " " << ir_drop_point_vector[i].ir_drop << endl;
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

void getIrDropPointMap(vector<string> *ir_drop_file_vector, map<string, IrDropInfo> *ir_drop_info_map)
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

                    float x_location_float = stof(ir_content_array[2]);
                    float y_location_float = stof(ir_content_array[3]);

                    ir_drop_point.x_location = floatToString(x_location_float);
                    ir_drop_point.y_location =  floatToString(y_location_float);

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
bool floatIsEqual(float a, float b)
{
    string a_str = floatToString(a);
    string b_str = floatToString(b);
    if (a_str == b_str)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool floatIsEqualOrLess(float a, float b)
{
    if (floatIsEqual(a, b))
    {
        return true;
    }
    else if (a < b)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool floatIsEqualOrMore(float a, float b)
{
    if (floatIsEqual(a, b))
    {
        return true;
    }
    else if (a > b)
    {
        return true;
    }
    else
    {
        return false;
    }
}