
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
#include <regex>
struct IoPin
{
    string pin_name;
    string replace_pin_name;
    string pin_info;
    string side;
    string width;
    string offset;
};
struct IoPinSide
{
    string side;
    vector<IoPin> io_power_pin_vector;
    vector<IoPin> power_pin_vector;
    vector<IoPin> io_pin_vector;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct Track
{

    string layer;
    string x_location;
    string y_location;
};

string TOP = "top";
string LEFT = "left";
string BOTTOM = "bottom";
string RIGHT = "right";
float POWERPAD_WIDTH = 1.728;
string M3 = "M3";
string M4 = "M4";
string M4_Y_STEP = "0.216";
string M3_X_STEP = "0.144";
string M3_START_STEP = "0.18";
string M4_START_STEP = "0.18";
string ODD = "odd";
string EVEN = "even";

vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
void getIoPinInfo(string io_pin_file_name, vector<string> *io_pin_side_vector, unordered_map<string, IoPinSide> *io_pin_side_map);
string getIoName(string str);
void setIoPinInfo(unordered_map<string, IoPinSide> *io_pin_side_map, string side, string io_pin_content);
void setIoPinPosition(IoPinSide *io_pin_side);
void setIoPinPostionInfo(unordered_map<string, IoPinSide> *io_pin_side_map);
string subreplace(string &resource_str, string &sub_str, string &new_str);
void replaceContent(unordered_map<string, IoPinSide> *io_pin_side_map);
void printIoFile(unordered_map<string, IoPinSide> *io_pin_side_map, string io_pin_file_name, string io_pin_file_content);
string replaceString(string resource_string, string original_string, string replace_string);
void widenPowerPad(IoPinSide *io_pin_side);
void changeLayer(unordered_map<string, IoPinSide> *io_pin_side_map);
void widenEachPowerPad(unordered_map<string, IoPinSide> *io_pin_side_map);
void setOffsetOnTrack(unordered_map<string, IoPinSide> *io_pin_side_map);
void getCoreSite(CoreSite *core_site);
void getTrackInfo(unordered_map<string, vector<Track>> *track_map, CoreSite *core_site);
string getNextStepTrack(string layer, float offset);
string getBackStepTack(string layer, float offset);
void getTrackInfoVector(string layer, string start_step, string end_step, vector<Track> *track_vector);
void setIoTrackInfo(unordered_map<string, vector<Track>> *track_map, unordered_map<string, IoPinSide> *io_pin_side_map);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool floatIsEqual(float a, float b);
int tranferToTrackInt(float number);
float tranferToTrackFloat(int number);
void popOffSideTrack(unordered_map<string, IoPinSide> *io_pin_side_map);
void popNumberOffSideTrack(unordered_map<string, IoPinSide> *io_pin_side_map, string side, int start_offset, int number_of_track);
int main()
{

    unordered_map<string, IoPinSide> io_pin_side_map;
    unordered_map<string, vector<Track>> track_map;
    vector<string> io_pin_side_vector;
    CoreSite core_site;

    string io_file_name = "io/6t49run88_6.io";
    string io_file_reorder = "io/6t49run88_neuralNetwork_average_distribution.io";
    string io_file_reorder_change_oiwer_pad_side = "io/6t49run88_neuralNetwork_average_distribution_change_power_pad_each_power_pad.io";
    getIoPinInfo(io_file_name, &io_pin_side_vector, &io_pin_side_map);
    getCoreSite(&core_site);
    getTrackInfo(&track_map, &core_site);
    setIoPinPostionInfo(&io_pin_side_map);
    replaceContent(&io_pin_side_map);
    // setIoTrackInfo(&track_map, &io_pin_side_map);
    printIoFile(&io_pin_side_map, io_file_name, io_file_reorder);

    widenEachPowerPad(&io_pin_side_map);
    changeLayer(&io_pin_side_map);
    setOffsetOnTrack(&io_pin_side_map);

    popOffSideTrack(&io_pin_side_map);

    printIoFile(&io_pin_side_map, io_file_reorder, io_file_reorder_change_oiwer_pad_side);
    // for (int i = 0; i < io_pin_side_map[TOP].io_pin_vector.size(); i++)
    // {
    //     cout <<  io_pin_side_map[TOP].io_pin_vector[i].pin_name << endl;
    // }
}

void popNumberOffSideTrack(unordered_map<string, IoPinSide> *io_pin_side_map, string side, int start_offset, int number_of_track)
{

    if (side == LEFT || side == RIGHT)
    {
        for (int i = start_offset; i < (*io_pin_side_map)[side].io_power_pin_vector.size(); i++)
        {
            string offset = (*io_pin_side_map)[side].io_power_pin_vector[i].offset;

            float offset_float = stof(offset);
            float m4_y_step = stof(M4_Y_STEP);
            m4_y_step = m4_y_step * number_of_track;
            offset_float -= m4_y_step;

            string offset_str = floatToString(offset_float);

            string resource_str = (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info;
            // cout << "before : " << resource_str << endl;
            resource_str = replaceString(resource_str, offset, offset_str);
            // cout << "after : " << resource_str << endl;
            (*io_pin_side_map)[side].io_power_pin_vector[i].offset = offset_str;
            (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info = resource_str;
            // cout << "offset : " << offset << endl;
            // cout << "m4_y_step : " << m4_y_step << endl;
            // cout << "temp : " << temp << endl;
            // cout << "start_step : " << start_step << endl;
            // cout << "step : " << step << endl;
        }
    }
    else
    {

        for (int i = start_offset; i < (*io_pin_side_map)[side].io_power_pin_vector.size(); i++)
        {
            string offset = (*io_pin_side_map)[side].io_power_pin_vector[i].offset;
            float offset_float = stof(offset);
            float m3_x_step = stof(M3_X_STEP);
            m3_x_step = m3_x_step * number_of_track;
            offset_float -= m3_x_step;

            string offset_str = floatToString(offset_float);
            string resource_str = (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info;
            cout << "before : " << resource_str << endl;
            resource_str = replaceString(resource_str, offset, offset_str);
            cout << "after : " << resource_str << endl;
            (*io_pin_side_map)[side].io_power_pin_vector[i].offset = offset_str;
            (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info = resource_str;
        }
    }
}

void popOffSideTrack(unordered_map<string, IoPinSide> *io_pin_side_map)
{
    for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
    {
        string side = io_pin_side_map_it->first;
        if (side == LEFT || side == RIGHT)
        {

            for (int i = 0; i < ((*io_pin_side_map)[side].io_power_pin_vector.size() - 1); i++)
            {
                int next_index = i + 1;
                int index = i;
                string offset = (*io_pin_side_map)[side].io_power_pin_vector[index].offset;
                string next_offset = (*io_pin_side_map)[side].io_power_pin_vector[next_index].offset;
                float offset_float = stof(offset);
                float next_offset_float = stof(next_offset);
                float distance = next_offset_float - offset_float;

                if (floatIsEqual(1.296, distance))
                {
                     popNumberOffSideTrack(&(*io_pin_side_map), side, next_index, 2);
                }
                // if (floatIsEqual(1.08, distance))
                // {
                //       popNumberOffSideTrack(&(*io_pin_side_map), side, next_index, 1);
                // }
            }
        }
        else
        {
            // float start_step = stof((*io_pin_side_map)[side].io_power_pin_vector[0].offset);
            // for (int i = 0; i < (*io_pin_side_map)[side].io_power_pin_vector.size(); i++)
            // {
            // }
        }
    }
}

// void setIoTrackInfo(unordered_map<string, vector<Track>> *track_map, unordered_map<string, IoPinSide> *io_pin_side_map)
// {

//     for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
//     {
//         string side = io_pin_side_map_it->first;
//         IoPinSide io_pin_side = (*io_pin_side_map)[side];
//         vector<IoPin> io_power_pin_vector = io_pin_side.io_power_pin_vector;
//         cout << "size : " << io_power_pin_vector.size() << endl;

//         if (side == "top" || side == "bottom")
//         {
//             vector<Track> track_vector = (*track_map)[M3];
//             int track_index = 0;
//             for (int i = 0; i < io_power_pin_vector.size(); i++)
//             {
//                 string offset = track_vector[track_index].x_location;
//                 string before_offset = io_power_pin_vector[i].offset;
//                 string resource_str = io_power_pin_vector[i].pin_info;
//                 cout << "before : " << resource_str << endl;
//                 resource_str = replaceString(resource_str, before_offset, offset);
//                 cout << "after : " << resource_str << endl;
//                 io_power_pin_vector[i].offset = offset;
//                 io_power_pin_vector[i].pin_info = resource_str;
//                 track_index += 7;
//             }
//             (*io_pin_side_map)[side].io_power_pin_vector = io_power_pin_vector;
//         }
//         else
//         {
//             vector<Track> track_vector = (*track_map)[M4];
//             int track_index = 0;
//             for (int i = 0; i < io_power_pin_vector.size(); i++)
//             {
//                 string offset = track_vector[track_index].x_location;
//                 string before_offset = io_power_pin_vector[i].offset;
//                 string resource_str = io_power_pin_vector[i].pin_info;
//                 resource_str = replaceString(resource_str, before_offset, offset);
//                 io_power_pin_vector[i].offset = offset;
//                 io_power_pin_vector[i].pin_info = resource_str;
//                 track_index += 4;
//             }
//             (*io_pin_side_map)[side].io_power_pin_vector = io_power_pin_vector;
//         }
//     }
// }

void getTrackInfo(unordered_map<string, vector<Track>> *track_map, CoreSite *core_site)
{
    float core_left_x_location = stof((*core_site).left_x_location);
    float core_right_x_location = stof((*core_site).right_x_location);
    float core_up_y_location = stof((*core_site).up_y_location);
    float core_down_y_location = stof((*core_site).down_y_location);

    string start_left_track = getNextStepTrack(ODD, core_left_x_location);
    string end_right_track = getBackStepTack(ODD, core_right_x_location);
    string start_down_track = getNextStepTrack(EVEN, core_down_y_location);
    string end_up_track = getBackStepTack(EVEN, core_up_y_location);

    vector<Track> m3_track_vector;
    vector<Track> m4_track_vector;
    getTrackInfoVector(ODD, start_left_track, end_right_track, &m3_track_vector);
    getTrackInfoVector(EVEN, start_down_track, end_up_track, &m4_track_vector);
    (*track_map).insert(pair<string, vector<Track>>(M3, m3_track_vector));
    (*track_map).insert(pair<string, vector<Track>>(M4, m4_track_vector));

    cout << "m3_track size : " << m3_track_vector.size() << endl;
    cout << "m4_track size : " << m4_track_vector.size() << endl;
}
int tranferToTrackInt(float number)
{
    number = number * 1000;
    int number_int = (int)number / 4;
    return number_int;
}
float tranferToTrackFloat(int number)
{
    number = (number * 4);
    float number_float = (float)number / 1000;
    return number_float;
}
void getTrackInfoVector(string layer, string start_step, string end_step, vector<Track> *track_vector)
{
    float start_step_float = stof(start_step);
    float end_step_float = stof(end_step);
    float m3_step = stof(M3_X_STEP);
    float m4_step = stof(M4_Y_STEP);

    int start_step_int = tranferToTrackInt(start_step_float);
    int end_step_int = tranferToTrackInt(end_step_float);
    int m3_step_int = tranferToTrackInt(m3_step);
    int m4_step_int = tranferToTrackInt(m4_step);

    if (layer == ODD)
    {
        for (int i = start_step_int; i <= end_step_int; i += m3_step_int)
        {
            float x_location = tranferToTrackFloat(i);
            Track track;
            track.x_location = floatToString(x_location);
            track.layer = M3;
            (*track_vector).push_back(track);
        }
    }
    else
    {
        for (int i = start_step_int; i <= end_step_int; i += m4_step_int)
        {
            float x_location = tranferToTrackFloat(i);
            Track track;
            track.x_location = floatToString(x_location);
            track.layer = M4;
            (*track_vector).push_back(track);
        }
    }
}

string getNextStepTrack(string layer, float offset)
{
    if (layer == ODD)
    {
        float m3_x_step = stof(M3_X_STEP);
        float start_step = stof(M3_START_STEP);
        offset = offset - start_step;
        int temp = (int)(offset / m3_x_step);
        temp += 1;

        float step = (temp * m3_x_step) + start_step;
        string step_str = floatToString(step);
        return step_str;
    }
    else
    {
        float m4_y_step = stof(M4_Y_STEP);
        float start_step = stof(M4_START_STEP);
        offset = offset - start_step;
        int temp = (int)(offset / m4_y_step);
        temp += 1;

        float step = (temp * m4_y_step) + start_step;
        string step_str = floatToString(step);
        return step_str;
    }
}

string getBackStepTack(string layer, float offset)
{
    if (layer == ODD)
    {
        float m3_x_step = stof(M3_X_STEP);
        float start_step = stof(M3_START_STEP);
        offset = offset - start_step;
        int temp = (int)(offset / m3_x_step);

        float step = (temp * m3_x_step) + start_step;
        string step_str = floatToString(step);
        return step_str;
    }
    else
    {
        float m4_y_step = stof(M4_Y_STEP);
        float start_step = stof(M4_START_STEP);
        offset = offset - start_step;
        int temp = (int)(offset / m4_y_step);

        float step = (temp * m4_y_step) + start_step;
        string step_str = floatToString(step);
        return step_str;
    }
}

void getCoreSite(CoreSite *core_site)
{
    (*core_site).left_x_location = "15.1200";
    (*core_site).right_x_location = "484.4160";
    (*core_site).down_y_location = "15.1200";
    (*core_site).up_y_location = "390.0960";
}
void setOffsetOnTrack(unordered_map<string, IoPinSide> *io_pin_side_map)
{
    for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
    {
        string side = io_pin_side_map_it->first;
        if (side == LEFT || side == RIGHT)
        {

            for (int i = 0; i < (*io_pin_side_map)[side].io_power_pin_vector.size(); i++)
            {
                string offset = (*io_pin_side_map)[side].io_power_pin_vector[i].offset;
                float offset_float = stof(offset);
                float m4_y_step = stof(M4_Y_STEP);
                int temp = (int)(offset_float / m4_y_step);
                temp -= 1;
                float start_step = stof(M4_START_STEP);
                float step = (temp * m4_y_step) + start_step;
                string step_str = floatToString(step);

                string resource_str = (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info;
                // cout << "before : " << resource_str << endl;
                resource_str = replaceString(resource_str, offset, step_str);
                // cout << "after : " << resource_str << endl;
                (*io_pin_side_map)[side].io_power_pin_vector[i].offset = step_str;
                (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info = resource_str;
                // cout << "offset : " << offset << endl;
                // cout << "m4_y_step : " << m4_y_step << endl;
                // cout << "temp : " << temp << endl;
                // cout << "start_step : " << start_step << endl;
                // cout << "step : " << step << endl;
            }
        }
        else
        {
            float start_step = stof((*io_pin_side_map)[side].io_power_pin_vector[0].offset);
            for (int i = 0; i < (*io_pin_side_map)[side].io_power_pin_vector.size(); i++)
            {
                string offset = (*io_pin_side_map)[side].io_power_pin_vector[i].offset;
                float offset_float = stof(offset);
                float m3_x_step = stof(M3_X_STEP);
                float start_step = stof(M3_START_STEP);
                int temp = (int)(offset_float / m3_x_step);
                temp -= 1;
                float step = (temp * m3_x_step) + start_step;
                string step_str = floatToString(step);
                string resource_str = (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info;
                // cout << "before : " << resource_str << endl;
                resource_str = replaceString(resource_str, offset, step_str);
                // cout << "after : " << resource_str << endl;
                (*io_pin_side_map)[side].io_power_pin_vector[i].offset = step_str;
                (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info = resource_str;
            }
        }
    }
}
void changeLayer(unordered_map<string, IoPinSide> *io_pin_side_map)
{
    for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
    {
        string side = io_pin_side_map_it->first;
        if (side == LEFT || side == RIGHT)
        {
            string original_layer = "layer=4";
            string replaced_layer = "layer=5";
            for (int i = 0; i < io_pin_side_map_it->second.io_power_pin_vector.size(); i++)
            {
                string resource_str = io_pin_side_map_it->second.io_power_pin_vector[i].pin_info;
                resource_str = replaceString(resource_str, original_layer, replaced_layer);
                (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info = resource_str;
            }

            // resource_str = replaceString(resource_str, offset_str, replace_offset);
            // resource_str = replaceString(resource_str, width_str, replace_power_pad_width);
        }
        else
        {
            string original_layer = "layer=5";
            string replace_layer = "layer=4";
            for (int i = 0; i < io_pin_side_map_it->second.io_power_pin_vector.size(); i++)
            {
                string resource_str = io_pin_side_map_it->second.io_power_pin_vector[i].pin_info;
                resource_str = replaceString(resource_str, original_layer, replace_layer);
                (*io_pin_side_map)[side].io_power_pin_vector[i].pin_info = resource_str;
            }
        }
    }
}

void widenEachPowerPad(unordered_map<string, IoPinSide> *io_pin_side_map)
{
    for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
    {
        string side = io_pin_side_map_it->first;
        // if (side == LEFT || side == RIGHT)
        // {
        widenPowerPad(&(*io_pin_side_map)[side]);
        // }
    }
}

string getSpacing(vector<IoPin> *original_io_pin_vector, int index)
{
    int before_index = index - 1;

    float offset = stof((*original_io_pin_vector)[index].offset) - stof((*original_io_pin_vector)[before_index].offset);
    float total_width = (stof((*original_io_pin_vector)[index].width) / 2) + (stof((*original_io_pin_vector)[before_index].width) / 2);
    float spacing = offset - total_width;

    string spacing_str = floatToString(spacing);
    return spacing_str;
}

void widenPowerPad(IoPinSide *io_pin_side)
{
    vector<IoPin> original_io_pin_vector = (*io_pin_side).io_power_pin_vector;
    bool start_power_pad = false;
    for (int i = 0; i < (*io_pin_side).io_power_pin_vector.size(); i++)
    {

        string pin_name = (*io_pin_side).io_power_pin_vector[i].pin_name;
        if ((*io_pin_side).io_power_pin_vector[i].pin_info.find("VDD") != string::npos || (*io_pin_side).io_power_pin_vector[i].pin_info.find("VSS") != string::npos)
        {
            start_power_pad = true;
        }
        if (start_power_pad)
        {

            if ((*io_pin_side).io_power_pin_vector[i].pin_info.find("VDD") != string::npos || (*io_pin_side).io_power_pin_vector[i].pin_info.find("VSS") != string::npos)
            {
                int index = i;
                int before_index = i - 1;
                float befor_offset = stof((*io_pin_side).io_power_pin_vector[before_index].offset);
                float befor_width = stof((*io_pin_side).io_power_pin_vector[before_index].width);
                float befor_half_width = befor_width / 2;
                float power_pad_width = (POWERPAD_WIDTH / 2);
                string spacing = getSpacing(&original_io_pin_vector, index);
                float spacing_float = stof(spacing);

                float temp = befor_offset + befor_half_width;
                temp = temp + spacing_float;
                temp = temp + power_pad_width;

                string replace_offset = floatToString(temp);
                string replace_power_pad_width = floatToString(POWERPAD_WIDTH);
                string offset_str = (*io_pin_side).io_power_pin_vector[i].offset;
                string width_str = (*io_pin_side).io_power_pin_vector[i].width;
                // cout << "pin name : " << (*io_pin_side).io_power_pin_vector[i].pin_name << endl;
                // cout << "pin name : " << (*io_pin_side).io_power_pin_vector[i].replace_pin_name << endl;
                string resource_str = (*io_pin_side).io_power_pin_vector[i].pin_info;

                // cout << "before : " << resource_str << endl;

                resource_str = replaceString(resource_str, offset_str, replace_offset);
                resource_str = replaceString(resource_str, width_str, replace_power_pad_width);
                // cout << "after : " << resource_str << endl;
                (*io_pin_side).io_power_pin_vector[i].pin_info = resource_str;
                (*io_pin_side).io_power_pin_vector[i].offset = replace_offset;
                (*io_pin_side).io_power_pin_vector[i].width = replace_power_pad_width;

                // (*io_pin_side).io_power_pin_vector[i].offset = offset;
                // (*io_pin_side).io_power_pin_vector[i].width = power_pad_width;
                // cout << "before pin name : " << original_io_pin_vector[before_index].pin_name << endl;
                // cout << "pin name : " << pin_name << endl;
                // cout << "before offset : " << befor_offset << endl;
                // cout << "befor_half_width : " << befor_half_width << endl;
                // cout << "power_pad_width : " << power_pad_width << endl;
                // cout << "spacing : " << spacing << endl;
                // cout << "offset : " << replace_offset << endl;
                cout << "-----------" << endl;
            }
            else
            {
                // cout << "check in side" << endl;
                int index = i;
                int before_index = i - 1;
                float befor_offset = stof((*io_pin_side).io_power_pin_vector[before_index].offset);
                float befor_width = stof((*io_pin_side).io_power_pin_vector[before_index].width);
                float befor_half_width = befor_width / 2;
                float power_pad_width = (stof((*io_pin_side).io_power_pin_vector[index].width));
                string spacing = getSpacing(&original_io_pin_vector, index);
                float spacing_float = stof(spacing);

                float temp = befor_offset + befor_half_width;
                temp = temp + spacing_float;
                temp = temp + power_pad_width;

                string replace_offset = floatToString(temp);
                string replace_power_pad_width = floatToString(stof((*io_pin_side).io_power_pin_vector[index].width));
                string offset_str = (*io_pin_side).io_power_pin_vector[i].offset;
                string width_str = (*io_pin_side).io_power_pin_vector[i].width;
                // cout << "pin name : " << (*io_pin_side).io_power_pin_vector[i].pin_name << endl;
                // cout << "pin name : " << (*io_pin_side).io_power_pin_vector[i].replace_pin_name << endl;
                string resource_str = (*io_pin_side).io_power_pin_vector[i].pin_info;

                // cout << "before : " << resource_str << endl;

                resource_str = replaceString(resource_str, offset_str, replace_offset);
                resource_str = replaceString(resource_str, width_str, replace_power_pad_width);
                // cout << "after : " << resource_str << endl;
                (*io_pin_side).io_power_pin_vector[i].pin_info = resource_str;
                (*io_pin_side).io_power_pin_vector[i].offset = replace_offset;
                (*io_pin_side).io_power_pin_vector[i].width = replace_power_pad_width;
            }
        }
    }
};

void setIoPinPostionInfo(unordered_map<string, IoPinSide> *io_pin_side_map)
{
    for (auto io_pin_side_map_it = (*io_pin_side_map).begin(); io_pin_side_map_it != (*io_pin_side_map).end(); ++io_pin_side_map_it)
    {
        string side = io_pin_side_map_it->first;
        cout << "side : " << side << endl;
        setIoPinPosition(&(*io_pin_side_map)[side]);
    }
}

void printIoFile(unordered_map<string, IoPinSide> *io_pin_side_map, string io_pin_file_name, string io_pin_file_content)
{
    ifstream io_pin_file(io_pin_file_name);
    string io_pin_content;
    int log = 0;
    ofstream myfile;
    myfile.open(io_pin_file_content);

    if (io_pin_file)
    {
        while (getline(io_pin_file, io_pin_content))
        {
            myfile << io_pin_content << endl;
            if (io_pin_content.find(TOP) != string::npos)
            {

                for (int i = 0; i < (*io_pin_side_map)[TOP].io_power_pin_vector.size(); i++)
                {
                    myfile << (*io_pin_side_map)[TOP].io_power_pin_vector[i].pin_info << endl;
                }
                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") == string::npos)
                    {
                        myfile << io_pin_content << endl;
                        break;
                    }
                }
            }
            if (io_pin_content.find(LEFT) != string::npos)
            {
                for (int i = 0; i < (*io_pin_side_map)[LEFT].io_power_pin_vector.size(); i++)
                {
                    myfile << (*io_pin_side_map)[LEFT].io_power_pin_vector[i].pin_info << endl;
                }
                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") == string::npos)
                    {
                        myfile << io_pin_content << endl;
                        break;
                    }
                    // else
                    // {
                    //     myfile << io_pin_content << endl;
                    //     break;
                    // }
                }
            }
            if (io_pin_content.find(RIGHT) != string::npos)
            {
                for (int i = 0; i < (*io_pin_side_map)[RIGHT].io_power_pin_vector.size(); i++)
                {
                    myfile << (*io_pin_side_map)[RIGHT].io_power_pin_vector[i].pin_info << endl;
                }
                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") == string::npos)
                    {
                        myfile << io_pin_content << endl;
                        break;
                    }
                    // else
                    // {
                    //     myfile << io_pin_content << endl;
                    //     break;
                    // }
                }
            }
            if (io_pin_content.find(BOTTOM) != string::npos)
            {
                for (int i = 0; i < (*io_pin_side_map)[BOTTOM].io_power_pin_vector.size(); i++)
                {
                    myfile << (*io_pin_side_map)[BOTTOM].io_power_pin_vector[i].pin_info << endl;
                }
                while (getline(io_pin_file, io_pin_content))
                {
                    if (io_pin_content.find("pin name") == string::npos)
                    {
                        myfile << io_pin_content << endl;
                        break;
                    }
                    // else
                    // {
                    //     myfile << io_pin_content << endl;
                    //     break;
                    // }
                }
            }
        }
    }
    else
    {
        cout << "can't found file" << endl;
    }
    io_pin_file.close();
    myfile.close();
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
            // cout << "replace_pin_name : " << replace_pin_name << endl;
            // cout << "pin_name         : " << pin_name << endl;
            // cout << "pin_info         : " << pin_info << endl;
            pin_info = subreplace(pin_info, pin_name, replace_pin_name);
            // cout << "pin_info         : " << pin_info << endl;
            io_pin_side_map_it->second.io_power_pin_vector[i].pin_info = pin_info;
        }
    }
}

string replaceString(string resource_string, string original_string, string replace_string)
{

    string output = regex_replace(resource_string, regex(original_string), replace_string);

    return output;
}

string subreplace(string &resource_str, string &sub_str, string &new_str)
{
    // cout << "begin : " << resource_str << endl;
    int begin = resource_str.find('\"') + 1;
    resource_str.replace(begin, sub_str.length(), new_str);

    return resource_str;
}

void setIoPinPosition(IoPinSide *io_pin_side)
{
    cout << "power pin size   : " << (*io_pin_side).power_pin_vector.size() << endl;
    cout << "io    pin size   : " << (*io_pin_side).io_pin_vector.size() << endl;
    int number_of_power_pad = (*io_pin_side).power_pin_vector.size();
    number_of_power_pad = number_of_power_pad / 2;
    int divide_side = number_of_power_pad + 1;
    cout << "divide side       : " << divide_side << endl;

    int number_of_pin = (*io_pin_side).io_pin_vector.size();
    int number_of_side = number_of_pin / divide_side;
    cout << "number of side    : " << number_of_side << endl;
    cout << "======================" << endl;

    vector<IoPin> io_pin_vector = (*io_pin_side).io_pin_vector;
    vector<IoPin> power_pin_vector = (*io_pin_side).power_pin_vector;

    int number_of_side_count = 1;

    for (int i = 0; i < (*io_pin_side).io_power_pin_vector.size(); i++)
    {
        // cout << i << endl;
        // cout << "number_of_side_count : " << number_of_side_count << endl;
        if (number_of_side_count == (number_of_side + 1) && power_pin_vector.size() > 0)
        {
            // cout << "number_of_side_count first : " << number_of_side_count << endl;
            (*io_pin_side).io_power_pin_vector[i].replace_pin_name = power_pin_vector[0].pin_name;
            power_pin_vector.erase(power_pin_vector.begin());
            number_of_side_count += 1;
        }
        else if (number_of_side_count == (number_of_side + 2) && power_pin_vector.size() > 0)
        {
            // cout << "number_of_side_count second : " << number_of_side_count << endl;
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

// string getOffset(string io_pin_content){

// }

void setIoPinInfo(unordered_map<string, IoPinSide> *io_pin_side_map, string side, string io_pin_content)
{
    vector<string> content_vector = splitByPattern(io_pin_content, " ");

    vector<string> offset_content_vector = splitByPattern(content_vector[2], "=");
    string offset = offset_content_vector[1];
    vector<string> width_content_vector = splitByPattern(content_vector[4], "=");
    string width = width_content_vector[1];

    // cout << "content_vector : " << content_vector[2] << endl;

    string pin_name = getIoName(io_pin_content);

    if ((*io_pin_side_map).count(side))
    {
        IoPin io_pin;
        io_pin.pin_name = pin_name;
        io_pin.side = side;
        io_pin.pin_info = io_pin_content;
        io_pin.width = width;
        io_pin.offset = offset;
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
        io_pin.width = width;
        io_pin.offset = offset;
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
