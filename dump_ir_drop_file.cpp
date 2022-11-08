using namespace std;
#include <iostream>

#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
struct IrDropPoint
{
    string ir_drop;
    string layer;
    string x_location;
    string y_location;
};
void setIrDropReport(string ir_drop_report_file, vector<IrDropPoint> *ir_drop_point_map);
void generateIrDropFile(string ir_drop_location_file, vector<IrDropPoint> *ir_drop_point_vector);
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);

int main()
{

    vector<IrDropPoint> ir_drop_point_vector;
    string ir_drop_report_file = "ir_report/ir_drop_point_all_layer_report_28";
    string dump_ir_drop_file = "ir_report/ir_drop_point_all_layer_report_28_location";

    setIrDropReport(ir_drop_report_file, &ir_drop_point_vector);
    generateIrDropFile(dump_ir_drop_file, &ir_drop_point_vector);
}
void generateIrDropFile(string ir_drop_location_file, vector<IrDropPoint> *ir_drop_point_vector)
{
    ofstream myfile;
    myfile.open(ir_drop_location_file);

    for (int i = 0; i < 50; i++)
    {
        IrDropPoint ir_drop_point = (*ir_drop_point_vector)[i];
        myfile << "(" << ir_drop_point.x_location << " " << ir_drop_point.y_location << ")" << endl;
    }

    myfile.close();
}

void setIrDropReport(string ir_drop_report_file, vector<IrDropPoint> *ir_drop_point_vector)
{

    ifstream ir_file(ir_drop_report_file);
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
                ir_drop_point.ir_drop = ir_content_array[0];
                ir_drop_point.layer = ir_content_array[1];
                ir_drop_point.x_location = ir_content_array[2];
                ir_drop_point.y_location = ir_content_array[3];
                (*ir_drop_point_vector).push_back(ir_drop_point);

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