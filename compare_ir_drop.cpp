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
struct IrDropPoint
{
    string layer;
    string x_location;
    string y_location;
    string ir_drop;
};

struct IrDropLayer
{
    unordered_map<string, vector<IrDropPoint>> x_point_map;
    unordered_map<string, vector<IrDropPoint>> y_point_map;
};

// function
void getIrDropPointMap(vector<string> *ir_drop_file_vector, unordered_map<string, IrDropLayer> *ir_drop_layer_map);
void setIrDropMapNew(unordered_map<string, IrDropLayer> *ir_drop_layer_map, IrDropPoint *ir_drop_point);
void setIrDropMapAlready(unordered_map<string, IrDropLayer> *ir_drop_layer_map, IrDropPoint *ir_drop_point);
bool sortStripeLocationY(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b);
// boolean
bool isOddLayer(string layer);

// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);

int main()
{
    string ir_drop_config_file = "config/ir_drop_config.txt";
    unordered_map<string, IrDropLayer> ir_drop_layer_map;
    vector<string> ir_drop_file_vector;
    ifstream config(ir_drop_config_file);
    string config_content;

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

    getIrDropPointMap(&ir_drop_file_vector, &ir_drop_layer_map);

    vector<IrDropPoint> ir_drop_point_vector = ir_drop_layer_map["M3"].x_point_map["164.772"];
    sort(ir_drop_point_vector.begin(), ir_drop_point_vector.end(), sortStripeLocationY);
    for (int i = 0; i < ir_drop_point_vector.size(); i++)
    {
        IrDropPoint ir_drop_point = ir_drop_point_vector[i];
        float ir_drop_float = stof(ir_drop_point.ir_drop);
        ir_drop_float = ir_drop_float * 0.001;
        // cout << (0.7 - ir_drop_float) << endl;
        cout << ir_drop_point.y_location  << endl;
    }

    return 0;
}

bool sortStripeLocationY(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b)
{
    return stof(ir_drop_point_a.y_location) < stof(ir_drop_point_b.y_location);
}

void getIrDropPointMap(vector<string> *ir_drop_file_vector, unordered_map<string, IrDropLayer> *ir_drop_layer_map)
{

    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {
        string ir_drop_file_name = (*ir_drop_file_vector)[i];
        vector<string> ir_file_content_array = splitByPattern(ir_drop_file_name, "/");
        string file_name_detail = ir_file_content_array[1];
        vector<string> file_content_array = splitByPattern(file_name_detail, "_");
        string layer = file_content_array[4];

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

                    if ((*ir_drop_layer_map).count(ir_drop_point.layer))
                    {
                        setIrDropMapAlready(&(*ir_drop_layer_map), &ir_drop_point);
                    }
                    else
                    {
                        setIrDropMapNew(&(*ir_drop_layer_map), &ir_drop_point);
                    }
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

void setIrDropMapNew(unordered_map<string, IrDropLayer> *ir_drop_layer_map, IrDropPoint *ir_drop_point)
{
    IrDropLayer ir_drop_layer;
    unordered_map<string, vector<IrDropPoint>> x_point_map;
    unordered_map<string, vector<IrDropPoint>> y_point_map;
    string layer = (*ir_drop_point).layer;
    string x_location = (*ir_drop_point).x_location;
    string y_location = (*ir_drop_point).y_location;
    vector<IrDropPoint> ir_drop_x_point_vector;
    vector<IrDropPoint> ir_drop_y_point_vector;
    ir_drop_x_point_vector.push_back((*ir_drop_point));
    ir_drop_y_point_vector.push_back((*ir_drop_point));
    x_point_map.insert(pair<string, vector<IrDropPoint>>(x_location, ir_drop_x_point_vector));
    y_point_map.insert(pair<string, vector<IrDropPoint>>(y_location, ir_drop_y_point_vector));
    ir_drop_layer.x_point_map = x_point_map;
    ir_drop_layer.y_point_map = y_point_map;

    (*ir_drop_layer_map).insert(pair<string, IrDropLayer>(layer, ir_drop_layer));
};

void setIrDropMapAlready(unordered_map<string, IrDropLayer> *ir_drop_layer_map, IrDropPoint *ir_drop_point)
{
    string layer = (*ir_drop_point).layer;
    string x_location = (*ir_drop_point).x_location;
    string y_location = (*ir_drop_point).y_location;

    (*ir_drop_layer_map)[layer].x_point_map[x_location].push_back((*ir_drop_point));
    (*ir_drop_layer_map)[layer].y_point_map[y_location].push_back((*ir_drop_point));
};

bool isOddLayer(string layer)
{
    if (layer == "M3" || layer == "M5" || layer == "M7" || layer == "M9")
    {
        return true;
    }
    else if (layer == "M4" || layer == "M6" || layer == "M8")
    {
        return false;
    }
    else
    {
        cout << "error is Odd Layer" << endl;
        return false;
    }
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