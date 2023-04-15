
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

struct Stripe
{
    string net_name;
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string direction;
    string width;
    string layer;
    string length;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct TrackInfo
{
    string layer;
    string start_x_track_location;
    string end_x_track_location;
    string start_y_track_location;
    string end_y_track_location;
    // core_site內
    float total_track_length;
    float occupy_resource_percent;
};
struct Track
{

    string layer;
    string x_location;
    string y_location;
};

vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
string floatToString(const float value);
int stringToInt(string num);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
void getStripeLocation(string def_transfer_file_name, map<string, vector<Stripe>> *vdd_stripe_map, map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site);
void setPowerStripeDirection(Stripe *stripe);
bool isOddLayer(string layer);
void setStripeLength(Stripe *stripe);
int tranferToTrackInt(float number);
float tranferToTrackFloat(int number);
string getNextStepTrack(string layer, float offset);
string getBackStepTack(string layer, float offset);
void getTrackInfo(CoreSite *core_site, map<string, TrackInfo> *track_info_map);
void getTrackInfoVector(string layer, string start_step, string end_step, vector<Track> *track_vector);
void getMetalResource(map<string, vector<Stripe>> *vdd_stripe_map, map<string, TrackInfo> *track_info_map, CoreSite *core_site);
float getStripeResource(string layer, float odd_core_site_length, float even_core_site_length, Stripe *stripe);
int getTrackPitch(string width);
void generateLogFile(string def_transfer_file, map<string, TrackInfo> *track_info_map);
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string VERTICAL = "Vertical";
const string HORIZONTAL = "Horizontal";
string M3 = "M3";
string M4 = "M4";
string M4_Y_STEP = "0.216";
string M3_X_STEP = "0.144";
string M3_START_STEP = "0.18";
string M4_START_STEP = "0.18";
string ODD = "odd";
string EVEN = "even";
// int main()
int main(int argc, char *argv[])
{

    // string config_file = "config/config_caculate_metal_resource.txt";
    // ifstream config(config_file);
    string config_content;
    string DEF_TRANSFER_FILE = "";
    map<string, vector<Stripe>> vdd_stripe_map;
    map<string, vector<Stripe>> vss_stripe_map;
    map<string, TrackInfo> track_info_map;
    map<string, string> filen_name_map;
    vector<string> def_transfer_file_vector;
    CoreSite core_site;
    // if (config)
    // {
    //     while (getline(config, config_content))
    //     {
    //         vector<string> config_content_array = splitByPattern(config_content, " ");
    //         // std::cout << config_content_array[0] << " " << config_content_array[2] << std::endl;
    //         if (config_content_array[0] == "DEF_FILE")
    //         {
    //             DEF_TRANSFER_FILE = config_content_array[2];
    //             filen_name_map.insert(pair<string, string>(DEF_TRANSFER_FILE, DEF_TRANSFER_FILE));
    //         }
    //         if (config_content_array[0] == "DEF_TRANSFER_FILE")
    //         {
    //             DEF_TRANSFER_FILE = config_content_array[2];
    //             def_transfer_file_vector.push_back(DEF_TRANSFER_FILE);
    //         }
    //     }
    // }
    DEF_TRANSFER_FILE = argv[2];

    // cout <<  "DEF_TRANSFER_FILE : " <<DEF_TRANSFER_FILE << endl;
    // DEF_TRANSFER_FILE = "def_file/metal_resource_def/6t49_aes_routing_run0_5track_pitch_clockTree_tranfer.def";
    // cout << "=================" << DEF_TRANSFER_FILE << endl;
    // for (int i = 0; i < def_transfer_file_vector.size(); i++)
    // {
    //     vector<string> file_name_transfer = splitByPattern(def_transfer_file_vector[i], "/");
    //     vector<string> file_name_transfer_detail_vector = splitByPattern(file_name_transfer[2], ".");
    //     string def_transfer_file = file_name_transfer_detail_vector[0];
    //     for (auto filen_name_map_it = filen_name_map.begin(); filen_name_map_it != filen_name_map.end(); ++filen_name_map_it)
    //     {
    //         string file_name = filen_name_map_it->first;
    //         vector<string> file_name_vector = splitByPattern(file_name, "/");
    //         vector<string> file_name_detail_vector = splitByPattern(file_name_vector[2], ".");
    //         string def_file = file_name_detail_vector[0];
    //         if (def_transfer_file.find(def_file) != string::npos)
    //         {
    //             filen_name_map[file_name] = def_transfer_file_vector[i];
    //             break;
    //         }
    //     }
    // }

    // for (auto filen_name_map_it = filen_name_map.begin(); filen_name_map_it != filen_name_map.end(); ++filen_name_map_it)
    // {

    //     cout << filen_name_map_it->first << "   " << filen_name_map_it->second << endl;
    // }

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_map, &vss_stripe_map, &core_site);
    getTrackInfo(&core_site, &track_info_map);
    getMetalResource(&vdd_stripe_map, &track_info_map, &core_site);
    generateLogFile(DEF_TRANSFER_FILE, &track_info_map);
    for (auto stripe : vdd_stripe_map)
    {
        cout << stripe.first << endl;
        cout << stripe.second.size() << endl;
    }

    return 0;
}

void generateLogFile(string def_transfer_file, map<string, TrackInfo> *track_info_map)
{
    cout << "========== generateLogFile start ==========" << endl;
    cout << def_transfer_file << endl;
    vector<string> file_name = splitByPattern(def_transfer_file, "/");
    string log_name = "log_file/log_" + file_name[2];
    ofstream myfile;
    myfile.open(log_name);

    for (auto track_info_map_it = (*track_info_map).begin(); track_info_map_it != (*track_info_map).end(); ++track_info_map_it)
    {
        string layer = track_info_map_it->first;
        float occupy_resource_percent = (*track_info_map)[layer].occupy_resource_percent;
        cout << layer << " " << occupy_resource_percent << endl;
        myfile << layer << " " << occupy_resource_percent << "%" << endl;
    }
    myfile.close();
    cout << "========== generateLogFile end ==========" << endl;
}

void getMetalResource(map<string, vector<Stripe>> *vdd_stripe_map, map<string, TrackInfo> *track_info_map, CoreSite *core_site)
{
    cout << "========== getMetalResource start ==========" << endl;
    float core_left_x_location = stof((*core_site).left_x_location);
    float core_right_x_location = stof((*core_site).right_x_location);
    float core_up_y_location = stof((*core_site).up_y_location);
    float core_down_y_location = stof((*core_site).down_y_location);

    float odd_core_site_length = abs((core_left_x_location) - (core_right_x_location));
    float even_core_site_length = abs((core_up_y_location) - (core_down_y_location));

    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        float total_stripe_resource = 0;

        for (int i = 0; i < (*vdd_stripe_map)[layer].size(); i++)
        {
            Stripe stripe = (*vdd_stripe_map)[layer][i];
            float stripe_length = stof(stripe.length);
            float metal_resource = getStripeResource(layer, odd_core_site_length, even_core_site_length, &stripe);
            total_stripe_resource += metal_resource;
        }
        float total_track_metal_resource = (*track_info_map)[layer].total_track_length;
        float resource_percent = total_stripe_resource / total_track_metal_resource;
        (*track_info_map)[layer].occupy_resource_percent = resource_percent * 100;

        cout << "layer : " << layer << " " << (*track_info_map)[layer].occupy_resource_percent << "%" << endl;
    }
    cout << "========== getMetalResource end ==========" << endl;
}

float getStripeResource(string layer, float odd_core_site_length, float even_core_site_length, Stripe *stripe)
{
    float stripe_length = stof((*stripe).length);

    if (isOddLayer(layer))
    {
        if (stripe_length > odd_core_site_length)
        {
            int track_pitch = getTrackPitch((*stripe).width);
            float metal_resource = odd_core_site_length * track_pitch;
            return metal_resource;
        }
        else
        {
            int track_pitch = getTrackPitch((*stripe).width);
            float metal_resource = stripe_length * track_pitch;
            return metal_resource;
        }
    }
    else
    {
        if (stripe_length > even_core_site_length)
        {
            int track_pitch = getTrackPitch((*stripe).width);
            float metal_resource = odd_core_site_length * track_pitch;
            return metal_resource;
        }
        else
        {
            int track_pitch = getTrackPitch((*stripe).width);
            float metal_resource = stripe_length * track_pitch;
            return metal_resource;
        }
    };
}

int getTrackPitch(string width)
{
    if (width == "0.36")
    {
        return 3;
    }
    else if (width == "0.648")
    {
        return 5;
    }
    else if (width == "0.936")
    {
        return 7;
    }
    else if (width == "0.224")
    {
        return 3;
    }
    else
    {
        cout << "track pitch error" << endl;
        return 0;
    }
}

// 計算track 在 core site中的數量
void getTrackInfo(CoreSite *core_site, map<string, TrackInfo> *track_info_map)
{
    float core_left_x_location = stof((*core_site).left_x_location);
    float core_right_x_location = stof((*core_site).right_x_location);
    float core_up_y_location = stof((*core_site).up_y_location);
    float core_down_y_location = stof((*core_site).down_y_location);

    string start_left_track = getNextStepTrack(ODD, core_left_x_location);
    string end_right_track = getBackStepTack(ODD, core_right_x_location);
    string start_down_track = getNextStepTrack(EVEN, core_down_y_location);
    string end_up_track = getBackStepTack(EVEN, core_up_y_location);
    // cout << "start_left_track : " << start_left_track << endl;
    // cout << "end_right_track : " << end_right_track << endl;
    // cout << "start_down_track : " << start_down_track << endl;
    // cout << "end_up_track : " << end_up_track << endl;
    float odd_core_site_length = abs((core_left_x_location) - (core_right_x_location));
    float even_core_site_length = abs((core_up_y_location) - (core_down_y_location));
    // cout << "odd_core_site_length : " << odd_core_site_length << endl;
    // cout << "even_core_site_length : " << even_core_site_length << endl;

    vector<Track> m3_track_vector;
    vector<Track> m4_track_vector;
    getTrackInfoVector(ODD, start_left_track, end_right_track, &m3_track_vector);
    getTrackInfoVector(EVEN, start_down_track, end_up_track, &m4_track_vector);

    float m3_track_size_float = (float)m3_track_vector.size();
    float m4_track_size_float = (float)m4_track_vector.size();
    // cout << "m3_track_vector : " << m3_track_size_float << endl;
    // cout << "m4_track_vector : " << m4_track_size_float << endl;

    TrackInfo m3_track_info;
    TrackInfo m4_track_info;
    float m3_track_resource = odd_core_site_length * m3_track_size_float;
    float m4_track_resource = even_core_site_length * m4_track_size_float;
    // cout << "m3_track resource : " << m3_track_resource << endl;
    // cout << "m4_track resource : " << m4_track_resource << endl;
    m3_track_info.total_track_length = m3_track_resource;
    m4_track_info.total_track_length = m4_track_resource;
    (*track_info_map).insert(pair<string, TrackInfo>(M3, m3_track_info));
    (*track_info_map).insert(pair<string, TrackInfo>(M4, m4_track_info));
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

void getStripeLocation(string def_transfer_file_name, map<string, vector<Stripe>> *vdd_stripe_map, map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site)
{
    cout << "========== getStripeLocation start ==========" << endl;
    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;

    if (def_transfer_file)
    {
        while (getline(def_transfer_file, def_content))
        {
            if (def_content.find("( * VDD )") != string::npos)
            {
                // VDDX stripe
                while (getline(def_transfer_file, def_content))
                {
                    if (def_content.find("( * VSS )") != string::npos)
                    {
                        break;
                    }
                    if (def_content.find("NEW") != string::npos)
                    {
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        if (def_content_array[4] == "SHAPE" && def_content_array[5] == "STRIPE" && def_content_array.size() == 14)
                        {
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[11];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.layer = def_content_array[1];
                            stripe.net_name = NET_NAME_VDD;
                            setPowerStripeDirection(&stripe);
                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                cout << "VDDX :" << def_content << " " << def_content_array.size() << endl;
                                setStripeLength(&stripe);
                                string layer = stripe.layer;
                                if ((*vdd_stripe_map).count(stripe.layer) != 0)
                                {
                                    (*vdd_stripe_map)[layer].push_back(stripe);
                                }
                                else
                                {
                                    vector<Stripe> stripe_vector;
                                    (*vdd_stripe_map).insert(pair<string, vector<Stripe>>(layer, stripe_vector));
                                    (*vdd_stripe_map)[layer].push_back(stripe);
                                };
                                // (*vdd_stripe_vector).push_back(stripe);
                            }
                        }
                    }
                }
                // VSSX stripe
                while (getline(def_transfer_file, def_content))
                {
                    if (def_content.find("END SPECIALNETS") != string::npos)
                    {
                        break;
                    }
                    if (def_content.find("NEW") != string::npos)
                    {
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        if (def_content_array[4] == "SHAPE" && def_content_array[5] == "STRIPE" && def_content_array.size() == 14)
                        {
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[11];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.layer = def_content_array[1];
                            stripe.net_name = NET_NAME_VSS;
                            setPowerStripeDirection(&stripe);
                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                cout << "VSSX :" << def_content << " " << def_content_array.size() << endl;
                                setStripeLength(&stripe);
                                string layer = stripe.layer;
                                if ((*vss_stripe_map).count(stripe.layer) != 0)
                                {
                                    (*vss_stripe_map)[layer].push_back(stripe);
                                }
                                else
                                {
                                    vector<Stripe> stripe_vector;
                                    (*vss_stripe_map).insert(pair<string, vector<Stripe>>(layer, stripe_vector));
                                    (*vss_stripe_map)[layer].push_back(stripe);
                                };
                                // (*vss_stripe_vector).push_back(stripe);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        cout << "can't found file" << endl;
    }
    def_transfer_file.close();
    cout << "========== getStripeLocation end ==========" << endl;
}
void setStripeLength(Stripe *stripe)
{
    if (isOddLayer((*stripe).layer))
    {
        float length = abs(stof((*stripe).start_y_location) - stof((*stripe).end_y_location));
        (*stripe).length = floatToString(length);
    }
    else
    {
        float length = abs(stof((*stripe).start_x_location) - stof((*stripe).end_x_location));
        // cout << "length : " << (*stripe).start_x_location << " " << (*stripe).end_x_location << endl;
        (*stripe).length = floatToString(length);
    }
}

bool isPowerStripe(Stripe *stripe, CoreSite *core_site)
{
    if ((*stripe).width == "1.728")
    {
        return false;
    }
    else
    {
        return true;
    }
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

void setPowerStripeDirection(Stripe *stripe)
{
    // step 1 : 將 * 填值
    if ((*stripe).end_x_location == "*")
    {
        (*stripe).end_x_location = (*stripe).start_x_location;
        (*stripe).direction = VERTICAL;
    }
    if ((*stripe).end_y_location == "*")
    {
        (*stripe).end_y_location = (*stripe).start_y_location;
        (*stripe).direction = HORIZONTAL;
    }
}

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site)
{
    cout << "========== getCoreSite start ==========" << endl;
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
    cout << "========== getCoreSite end ==========" << endl;
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
