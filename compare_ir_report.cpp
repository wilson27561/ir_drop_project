
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
#include <iomanip>

struct IrDropPoint
{
    string ir_drop;
    string layer;
    string x_location;
    string y_location;
    string chip_name;
    float voltage;
    int ir_drop_int;
};
// voltage 大 到 小
// start <= range < end
struct Range
{
    float voltage_start; // 大
    float voltage_end;   // 小
    bool isEndRange = false;
    vector<IrDropPoint> low_ir_drop_point_vector;
    vector<IrDropPoint> high_ir_drop_point_vector;
};

// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
void setIrDropReport(vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
int transferIrDropToInt(float ir_drop_float);
void setRange(string ir_drop_file_low, vector<Range> *range_vector);
float tranferIrDropToVoltage(string ir_drop);
void setIrPoint(Range *range, vector<IrDropPoint> *ir_drop_point_vector, string format);
void setIrDropInRange(vector<Range> *range_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
bool isInRange(Range *range, IrDropPoint *ir_drop_point);
void generateCompareFile(vector<Range> *range_vector, string log_file);
int main(int argc, char *argv[])
// int main()
{

    // string config_file = "config/ir_report_config.txt";
    string config_file = argv[1];
    string IR_DROP_FILE_HIGH = "";
    string IR_DROP_FILE_LOW = "";
    string LOG_FILE = "";
    string LOW = "LOW";
    string HIGH = "HIGH";

    vector<string> ir_drop_file_vector;
    vector<Range> range_vector;
    unordered_map<string, vector<IrDropPoint>> ir_drop_point_map;
    ifstream config(config_file);
    string config_content;
    if (config)
    {
        while (getline(config, config_content))
        {
            vector<string> config_content_array = splitByPattern(config_content, " ");
            // std::cout << config_content_array[0] << " " << config_content_array[2] << std::endl;
            if (config_content_array[0] == "IR_DROP_FILE_HIGH")
            {
                IR_DROP_FILE_HIGH = config_content_array[2];
                ir_drop_file_vector.push_back(config_content_array[2]);
            }
            if (config_content_array[0] == "IR_DROP_FILE_LOW")
            {
                IR_DROP_FILE_LOW = config_content_array[2];
                ir_drop_file_vector.push_back(config_content_array[2]);
            }
            if (config_content_array[0] == "LOG_FILE")
            {
                LOG_FILE = config_content_array[2];
            }
        }
    }
    else
    {
        cout << "read_config_error" << endl;
    }
    setIrDropReport(&ir_drop_file_vector, &ir_drop_point_map);
    setRange(IR_DROP_FILE_LOW, &range_vector);
    setIrDropInRange(&range_vector, &ir_drop_point_map);
    generateCompareFile(&range_vector, LOG_FILE);

    return 0;
}

void generateCompareFile(vector<Range> *range_vector, string log_file)
{
    int total_high_range = 0;
    int total_low_range = 0;

    for (int i = 0; i < (*range_vector).size(); i++)
    {
        total_high_range += (*range_vector)[i].high_ir_drop_point_vector.size();
        total_low_range += (*range_vector)[i].low_ir_drop_point_vector.size();
    }

    cout << "total_high_range : " << total_high_range << endl;
    cout << "total_low_range : " << total_low_range << endl;
    ofstream myfile;
    myfile.open(log_file);
    myfile << " low ir drop report " << endl;
    for (int i = 0; i < (*range_vector).size(); i++)
    {
        int ir_drop_point_size = (*range_vector)[i].low_ir_drop_point_vector.size();
        double ir_drop_point_size_double = (double)(ir_drop_point_size);
        double total_low_range_double = (double)total_low_range;
        double percent = ir_drop_point_size_double / total_low_range;
        myfile << (*range_vector)[i].voltage_start << setw(3) << "~" << setw(10) << (*range_vector)[i].voltage_end << setw(15) << (percent * 100) << "%" << endl;
    }
    myfile << " high ir drop report " << endl;
    for (int i = 0; i < (*range_vector).size(); i++)
    {
        int ir_drop_point_size = (*range_vector)[i].high_ir_drop_point_vector.size();
        double ir_drop_point_size_double = (double)(ir_drop_point_size);
        double total_high_range_double = (double)total_high_range;
        double percent = ir_drop_point_size_double / total_high_range;
        myfile << (*range_vector)[i].voltage_start << setw(3) << "~" << setw(10) << (*range_vector)[i].voltage_end << setw(15) << (percent * 100) << "%" << endl;
    }
    myfile.close();
}

void setIrDropInRange(vector<Range> *range_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
{
    for (auto ir_drop_point_map_it = (*ir_drop_point_map).begin(); ir_drop_point_map_it != (*ir_drop_point_map).end(); ++ir_drop_point_map_it)
    {
        string format = ir_drop_point_map_it->first;
        vector<IrDropPoint> ir_drop_vector = (*ir_drop_point_map)[format];

        for (int i = 0; i < (*range_vector).size(); i++)
        {
            setIrPoint(&(*range_vector)[i], &ir_drop_vector, format);
        }
    }
}

void setIrPoint(Range *range, vector<IrDropPoint> *ir_drop_point_vector, string format)
{
    for (int i = 0; i < (*ir_drop_point_vector).size(); i++)
    {
        IrDropPoint ir_drop_point = (*ir_drop_point_vector)[i];
        if (isInRange(&(*range), &ir_drop_point))
        {
            if (format == "low")
            {
                (*range).low_ir_drop_point_vector.push_back(ir_drop_point);
            }
            else
            {
                (*range).high_ir_drop_point_vector.push_back(ir_drop_point);
            }
        }
    }
}

bool isInRange(Range *range, IrDropPoint *ir_drop_point)
{
    float ir_voltage = (*ir_drop_point).voltage;
    int ir_volage_int = transferIrDropToInt(ir_voltage);
    float range_start = (*range).voltage_start;
    int range_start_int = transferIrDropToInt(range_start);
    float range_end = (*range).voltage_end;
    int range_end_int = transferIrDropToInt(range_end);

    if (ir_volage_int > range_end_int && ir_volage_int <= range_start_int)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setRange(string ir_drop_file_low, vector<Range> *range_vector)
{

    ifstream ir_file(ir_drop_file_low);
    string ir_content;
    int log = 0;
    if (ir_file)
    {
        while (getline(ir_file, ir_content))
        {
            if (ir_content.find("Range") != string::npos && ir_content.find("Total") != string::npos)
            {
                while (getline(ir_file, ir_content))
                {

                    if (ir_content.find(">") != string::npos)
                    {
                        vector<string> ir_drop_content_array = splitByPattern(ir_content, " ");
                        float voltage_start_float = tranferIrDropToVoltage(ir_drop_content_array[1]);
                        Range range;
                        range.isEndRange = true;
                        range.voltage_start = voltage_start_float;
                        range.voltage_end = 0;
                        (*range_vector).push_back(range);
                    }
                    else if (ir_content.find("~") != string::npos)
                    {
                        vector<string> ir_drop_content_array = splitByPattern(ir_content, " ");
                        float voltage_start_float = tranferIrDropToVoltage(ir_drop_content_array[0]);
                        float voltage_end_float = tranferIrDropToVoltage(ir_drop_content_array[2]);
                        Range range;
                        range.voltage_start = voltage_start_float;
                        range.voltage_end = voltage_end_float;
                        (*range_vector).push_back(range);
                    }
                }
            }
        }
    }
    else
    {
        cout << " read ir file error " << endl;
    }
}

float tranferIrDropToVoltage(string ir_drop)
{
    float ir_drop_float = stof(ir_drop);
    ir_drop_float = ir_drop_float * 0.001;
    float voltage = 0.7 - ir_drop_float;

    return voltage;
}

void setIrDropReport(vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
{
    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {

        string ir_drop_file_name = (*ir_drop_file_vector)[i];
        vector<string> ir_file_content_array = splitByPattern(ir_drop_file_name, "/");
        string file_name_detail = ir_file_content_array[2];
        vector<string> file_content_array = splitByPattern(file_name_detail, "_");
        string chip_name = file_content_array[2];
        string file_format = file_content_array[0];
        vector<IrDropPoint> ir_drop_point_vector;
        if (file_content_array.size() != 7)
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
                if (ir_content.find("ir") != string::npos && ir_content.find("layer") != string::npos)
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
                    ir_drop_point.chip_name = chip_name;
                    ir_drop_point.ir_drop = ir_content_array[0];
                    ir_drop_point.layer = ir_content_array[1];
                    ir_drop_point.x_location = ir_content_array[2];
                    ir_drop_point.y_location = ir_content_array[3];
                    ir_drop_point.voltage = tranferIrDropToVoltage(ir_content_array[0]);
                    // cout << ir_drop_point.ir_drop << "  " << ir_drop_point.ir_drop_int << endl;

                    ir_drop_point_vector.push_back(ir_drop_point);

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
        (*ir_drop_point_map).insert(pair<string, vector<IrDropPoint>>(file_format, ir_drop_point_vector));
    }
}

int transferIrDropToInt(float ir_drop_float)
{

    ir_drop_float = ir_drop_float * 1000000;
    int ir_drop_int = (int)ir_drop_float;
    return ir_drop_int;
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