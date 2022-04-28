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

struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};

struct Track
{
    string direction;
    string step;
    string layer;
    string start_step;
    vector<float> location_vector;
};
struct FollowPin
{
    string layer;
    string y_location;
};

string &trim(string &str);
vector<string> splitByPattern(string content, string pattern);
void getCoreSite(string def_file_name, CoreSite *core_site);
void getTrack(string def_file_name, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite);
void setTrackInfo(vector<string> def_content_array, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite);
void setTrackLocation(Track *track, CoreSite *coreSite);
void getFollowPin(string def_file_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector);
int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number);
int main()
{
    // key : layer  key direction
    // unordered_map<string, unordered_map<string, Track>> layer_track_map;
    // CoreSite core_site;
    vector<FollowPin> follow_pin_vdd_vector;
    vector<FollowPin> follow_pin_vss_vector;

    // getCoreSite(DEF_FILE, &core_site);
    // getTrack(DEF_FILE, &layer_track_map, &core_site);
    getFollowPin(DEF_FILE, &follow_pin_vdd_vector, &follow_pin_vss_vector);
    cout << "follow_pin vdd size : " << follow_pin_vdd_vector.size() << endl;
    cout << "follow_pin vss size : " << follow_pin_vss_vector.size() << endl;

    // cout << core_site.left_x_location << " " << core_site.right_x_location << endl;

    // for (unordered_map<string, unordered_map<string, Track>>::iterator layer_track_map_it = layer_track_map.begin(); layer_track_map_it != layer_track_map.end(); ++layer_track_map_it)
    // {
    //     cout << layer_track_map_it->first << endl;

    //     for (unordered_map<string, Track>::iterator track_map_it = layer_track_map_it->second.begin(); track_map_it != layer_track_map_it->second.end(); ++track_map_it)
    //     {

    //         cout << track_map_it->first << endl;
    //         cout << track_map_it->second.location_vector.size() << endl;
    //     }
    // }
    const float TOTAL_POWER = 4.4743;
    const float M3_SHEET_RESISTANCE = 3.1445326;
    const float M1_SHEET_RESISTANCE = 3.1445326;
    const float VDD_PAD = 0.7;
    const float IR_DROP = 0.07;
    // const float POWER_STRIPE_HEIGHT = 313.92;
    const float POWER_STRIPE_HEIGHT = 156.96;
    const float POWER_STRIPE_WIDTH = 0.224;
    // const float POWER_RAIL_HEIGHT = 385.344;
    const float POWER_RAIL_HEIGHT = 192.672;
    const float POWER_RAIL_WIDTH = 0.072;
    const int POWER_RAIL_NUMBER = 178;
    float num = caculate_power_stripe(TOTAL_POWER, IR_DROP, M3_SHEET_RESISTANCE, M1_SHEET_RESISTANCE, VDD_PAD, POWER_STRIPE_HEIGHT, POWER_STRIPE_WIDTH, POWER_RAIL_HEIGHT, POWER_RAIL_WIDTH, POWER_RAIL_NUMBER);
    return 0;
}

int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number)
{
    float m3_square = power_stripe_height / power_stripe_width;
    float m1_square = power_rail_height / power_rail_width;
    float m3_resistance = m3_sheet_resistance * m3_square;
    float m1_resistance = m1_sheet_resistance * m1_square;
 
    
    total_power = total_power * 0.001;
   
    float m3_current = ir_drop / m3_resistance;
    float m1_current = ir_drop / m1_resistance;
    
    float temp_power = (total_power / vdd_pad);
   
    float temp_current =  ((power_rail_number * m1_current));
    temp_power = temp_power - temp_current;
    int m3_number = temp_power / m3_current;

    cout << m3_number << endl;

    return 0;
}

int caculate_power_stripe(float total_power, float resistance, float vdd_pad, float target_voltage, float height, float width)
{
    total_power = total_power * 0.001;
    float square = height / width;
    float temp = total_power * resistance * square;
    float temp_power = vdd_pad * target_voltage;
    float number_stripe_float = temp / temp_power;
    int number_stripe = (int)number_stripe_float;
    return number_stripe;
}

//暫且紀錄M1
void getFollowPin(string def_file_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector)
{
    ifstream def_file(def_file_name);
    string def_content;
    if (def_file)
    {
        if (def_file)
        {
            while (getline(def_file, def_content))
            {
                if (def_content.find("( * VDD )") != string::npos)
                {
                    // VDDX stripe
                    while (getline(def_file, def_content))
                    {
                        if (def_content.find("( * VSS )") != string::npos)
                        {
                            break;
                        }
                        if (def_content.find("SHAPE FOLLOWPIN") != string::npos && def_content.find("M1") != string::npos)
                        {
                            vector<string> def_content_array = splitByPattern(def_content, " ");
                            FollowPin follow_pin;
                            follow_pin.layer = def_content_array[1];
                            follow_pin.y_location = def_content_array[8];
                            (*follow_pin_vdd_vector).push_back(follow_pin);

                            cout << "VDDX : " << def_content << endl;
                        }
                    }
                    // VSSX stripe
                    while (getline(def_file, def_content))
                    {
                        if (def_content.find("SHAPE FOLLOWPIN") != string::npos && def_content.find("M1") != string::npos)
                        {
                            vector<string> def_content_array = splitByPattern(def_content, " ");
                            FollowPin follow_pin;
                            follow_pin.layer = def_content_array[1];
                            follow_pin.y_location = def_content_array[8];
                            (*follow_pin_vss_vector).push_back(follow_pin);
                            cout << "VSSX : " << def_content << endl;
                        }
                    }
                }
            }
        }
        else
        {
            cout << "can't found file" << endl;
        }
    }
    def_file.close();
}

void getTrack(string def_file_name, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite)
{
    ifstream def_file(def_file_name);
    string def_content;
    bool track_content_end = false;

    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("TRACKS") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                setTrackInfo(def_content_array, &(*layer_track_map), &(*corsite));
                while (getline(def_file, def_content))
                {
                    if (def_content.find("TRACKS") != string::npos == false)
                    {
                        track_content_end = true;
                        break;
                    }
                    vector<string> def_content_array = splitByPattern(def_content, " ");
                    setTrackInfo(def_content_array, &(*layer_track_map), &(*corsite));
                }
                if (track_content_end)
                {
                    break;
                }
            }
        }
    }
}

void setTrackInfo(vector<string> def_content_array, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite)
{
    Track track;
    track.direction = def_content_array[1];
    track.layer = def_content_array[8];
    track.step = def_content_array[6];
    track.start_step = def_content_array[2];
    setTrackLocation(&track, &(*corsite));

    if ((*layer_track_map).count(track.layer))
    {
        (*layer_track_map)[track.layer].insert(pair<string, Track>(track.direction, track));
    }
    else
    {
        unordered_map<string, Track> track_map;
        track_map.insert(pair<string, Track>(track.direction, track));
        (*layer_track_map).insert(pair<string, unordered_map<string, Track>>(track.layer, track_map));
    }
}

void setTrackLocation(Track *track, CoreSite *coreSite)
{
    float index = stof((*track).start_step);
    float boundary_left = stof(coreSite->left_x_location);
    float boundary_right = stof(coreSite->right_x_location);
    float step = stof(track->step);
    vector<float> loacation_vector;

    while (index < boundary_right)
    {
        if (index > boundary_left && index < boundary_right)
        {
            loacation_vector.push_back(index);
        }
        index += step;
    }
    track->location_vector = loacation_vector;
};

void getCoreSite(string def_file_name, CoreSite *core_site)
{
    ifstream def_file(def_file_name);
    string def_content;

    if (def_file)
    {
        while (getline(def_file, def_content))
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
        cout << "read " << def_file_name << " error " << endl;
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
