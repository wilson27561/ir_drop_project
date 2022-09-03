
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

// const string DEF_TRANSFER_FILE = "DEF_TRANSFER_FILE/b19/6t49_b19_routing_44_9_81_transfer.def";
// // 6t49_b19_routing_88_9_68_transfer.def
// const string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
// const string IP_REPORT_FILE = "power_report/activity_0.6/print_ip.report";
// // print_ip_8pad.report
// const string ADD_STRIPE_ESTIMATE = "stripe_tcl/addStripe_b19_4_pad.tcl";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string MIDDLE = "middle";
// const float BOUNDARY = 60;
const string UP = "UP";
const string DOWN = "DOWN";
const string M3 = "M3";
const string X = "X";
const float RANGER_POWER_RATIO = 0.3;
const float M3_SHEET_RESISTANCE_PAD = 0.8119971;
const float M3_SHEET_RESISTANCE_STRIPE = 0.9;
const float M2_SHEET_RESISTANCE = 0.811738;
const float LISD_SHEET_RESISTANCE = 1.0416;
const float M3_PAD_WIDTH = 1.728;
const float M2_WIDTH = 1.8;
const float VDD_PAD = 0.7;
const float IR_DROP = 0.07;
// const float POWER_STRIPE_HEIGHT = 313.92;
const float POWER_STRIPE_WIDTH = 0.224;
const float POWER_RAIL_HEIGHT = 385.344;
const float POWER_RAIL_WIDTH = 0.1;
// const int POWER_RAIL_NUMBER = 178;
// const float SPACING = 0.072;
// float START_Y_LOCATION_STRIPE = 6.48;
// float END_Y_LOCATION_STRIPE = 320.4;
float START_TRACK = 0;
const string POWERSTRIPEVALUE = "power_stripe_resistance";
const string POWERRAILVALUE = "power_rail_resistance";

struct Via
{
    string via_name;
    string x_location;
    string y_location;
};

struct FollowPin
{
    string layer;
    string y_location;
    vector<Via> via_vector;
    float total_via_resistance;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct PowerStripe
{
    //左下右上
    string start_x_location;
    string end_x_location;
    string start_y_location;
    string end_y_location;
};
struct PowerGround
{
    //左下右上
    string start_x_location;
    string end_x_location;
    string start_y_location;
    string end_y_location;
};
struct RoutingTrack
{
    float start_x_location;
    float end_x_location;
    float start_y_location;
    float end_y_location;
};
struct CurrentRange
{
    float left_x_location;
    float right_x_location;
    float range_total_power;
    vector<string> ip_power_vector;
    float pad_to_range;
    vector<PowerStripe> power_stripe_vector;
    vector<PowerGround> power_ground_vector;
    vector<RoutingTrack> routing_track_vector;
};
struct CellInfo
{
    string cell_name;
    string cell_height;
    string cell_width;
};
struct CellPlacedInfo
{
    string cell_id;
    string cell_name;

    string left_x_location;
    string down_y_location;
    string right_x_location;
    string up_y_location;
};
struct CellInstancePowerInfo
{
    string cell_id;
    string cell_name;
    float instance_total_power;
    float internal_power;
    float leakage_power;
    float switching_power;
};
struct Track
{
    string direction;
    string step;
    string layer;
    string start_step;
    vector<float> location_vector;
};
struct PowerPad
{
    string pad_name;
    string net_name;
    string layer;
    float length = 0;
    float width = 0;
    string x_location;
    string y_location;
    string side;
    float supply_range_start;
    float supply_range_end;
    float supply_location;
    string middle_postition;
};
struct DieArea
{
    string lower_left_x_location;
    string lower_left_y_location;
    string top_right_x_location;
    string top_right_y_location;
};
struct ShapeRing
{
    string layer;
    float width;
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string side;
};
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void getDefPlacedImformation(string DEF_TRANSFER_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
vector<string> splitByPattern(string content, string pattern);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
string &trim(string &str);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
float getConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float);
bool isInStripeRange(CurrentRange *current_range, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void setIpPowerInStripe(vector<CurrentRange> *current_range_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void setCurrentRange(CoreSite *core_site, vector<CurrentRange> *current_range_vector, float boundary);
void setTrackInfo(vector<string> def_content_array, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite);
void setTrackLocation(Track *track, CoreSite *coreSite);
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadInfo(ifstream *DEF_TRANSFER_FILE, string def_content, PowerPad *power_pad);
void getDieArea(string DEF_TRANSFER_FILE_name, DieArea *die_area);
void getPowerPadLocation(string DEF_TRANSFER_FILE_name, vector<PowerPad> *vdd_power_pad, vector<PowerPad> *vss_power_pad);
void setPowerPadDirection(map<string, vector<PowerPad>> *direction_power_pad);
void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
void sortPowerPad(map<string, vector<PowerPad>> *direction_power_pad);
void power_pad_loction_sort(vector<PowerPad> *power_pad_vec, string direction);
void setSide(PowerPad *power_pad, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
int setSinglePad(map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, set<string> *side_set, CoreSite *core_site);
void setCurrentRangePowerStripe(map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector, vector<CurrentRange> *current_range_vector, vector<FollowPin> *follow_pin_vdd_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, CoreSite *core_site);
float countPadToRing(map<string, ShapeRing> *shape_map, PowerPad *power_pad, string side);
void coutPadToRing(map<string, ShapeRing> *shape_map, PowerPad *power_pad);
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line);
void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector);
void getShapeRing(string DEF_TRANSFER_FILE_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector);
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area);
void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map);
void setRoutingTrackInCurrentRange(unordered_map<string, unordered_map<string, Track>> *layer_track_map, vector<CurrentRange> *current_range_vector);
void getFollowPin(string DEF_TRANSFER_FILE_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector);
void setPowerPadSupplyRange(CoreSite *core_site, map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector);
void sortRangePadVector(vector<PowerPad> *vdd_range_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, CoreSite *core_site);
float powerStripePowerRailToSupplyDistance(string transfer_supply_location_side, float x_location, float y_location, vector<PowerPad> *vdd_range_pad_vector, CoreSite *core_site);
float getSingleFolllowPinResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring);
float getFollowPinToCoreSiteLeft(map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site);
int getNumberOfPowerStripe(CurrentRange *current_range, float power_stripe_resist, float power_rail_resist, int number_of_power_rail);
string floatToString(const float value);
void generateAddStripeTcl(vector<CurrentRange> *current_range_vector, string add_stripe_estimate_name);
int setForthPadResist(map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, set<string> *side_set, CoreSite *core_site);
float getForthPowerStripeResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring_up_down, float power_stripe_height);
float getForthFollowPinResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring_left_right);
void generatePowerStripeTcl(CurrentRange *currenge, int number_of_stripe, float start_y_location_stripe, float end_y_location_stripe, float start_y_location_ground, float end_y_location_ground);
void getSwitchingPowerReport(string sw_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getTrack(string def_file_name, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite);
int main()
{
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    vector<CurrentRange> current_range_vector;
    unordered_map<string, unordered_map<string, Track>> layer_track_map;
    map<string, vector<PowerPad>> direction_power_pad;
    DieArea die_area;
    vector<PowerPad> vdd_power_pad_vector;
    vector<PowerPad> vss_power_pad_vector;
    map<string, ShapeRing> vdd_shape_map;
    map<string, ShapeRing> vss_shape_map;
    vector<ShapeRing> vdd_shape_ring_vector;
    vector<ShapeRing> vss_shape_ring_vector;
    vector<FollowPin> follow_pin_vdd_vector;
    vector<FollowPin> follow_pin_vss_vector;
    vector<PowerPad> vdd_range_pad_vector;

    string DEF_TRANSFER_FILE = "";
    string LEF_FILE = "";
    string IP_REPORT_FILE = "";
    string ADD_STRIPE_ESTIMATE = "";
    string config_file = "config.txt";

    ifstream config(config_file);
    string config_content;
    if (config)
    {
        while (getline(config, config_content))
        {
            vector<string> config_content_array = splitByPattern(config_content, " ");
            std::cout << config_content_array[0] << " " << config_content_array[2] << std::endl;
            if (config_content_array[0] == "DEF_TRANSFER_FILE")
            {
                DEF_TRANSFER_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "LEF_FILE")
            {
                LEF_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "IP_REPORT_FILE")
            {
                IP_REPORT_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "ADD_STRIPE_ESTIMATE")
            {
                ADD_STRIPE_ESTIMATE = config_content_array[2];
            }
        }
    }

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    float boundary = abs(stof(core_site.left_x_location) - stof(core_site.right_x_location)) / 6;
    cout << "boundary :" << boundary << endl;

    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);

    setCurrentRange(&core_site, &current_range_vector, boundary);

    setIpPowerInStripe(&current_range_vector, &cell_ip_map, &cell_placed_map);
    getTrack(DEF_TRANSFER_FILE, &layer_track_map, &core_site);

    setPowerPadDirection(&direction_power_pad);
    getPowerPadLocation(DEF_TRANSFER_FILE, &vdd_power_pad_vector, &vss_power_pad_vector);
    getDieArea(DEF_TRANSFER_FILE, &die_area);

    setPowerPadSide(&vdd_power_pad_vector, &vss_power_pad_vector, &direction_power_pad, &die_area);
    sortPowerPad(&direction_power_pad);

    getShapeRing(DEF_TRANSFER_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);
    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);

    unordered_map<string, Track> track_map = layer_track_map[M3];
    Track track = track_map["Y"];
    START_TRACK = stof(track.start_step);

    getFollowPin(DEF_TRANSFER_FILE, &follow_pin_vdd_vector, &follow_pin_vss_vector);
    setPowerPadSupplyRange(&core_site, &direction_power_pad, &vdd_range_pad_vector);
    setRoutingTrackInCurrentRange(&layer_track_map, &current_range_vector);

    setCurrentRangePowerStripe(&direction_power_pad, &vdd_range_pad_vector, &current_range_vector, &follow_pin_vdd_vector, &layer_track_map, &vdd_shape_map, &vss_shape_map, &core_site);
    generateAddStripeTcl(&current_range_vector, ADD_STRIPE_ESTIMATE);

    float total_ip_power = 0;
    for (int i = 0; i < current_range_vector.size(); i++)
    {

        total_ip_power += current_range_vector[i].range_total_power;

        // cout << "range start : " << current_range_vector[i].left_x_location << "  " << current_range_vector[i].right_x_location << endl;
        // cout << "distance : " << abs(current_range_vector[i].left_x_location - current_range_vector[i].right_x_location) << endl;
        // cout << "routing track size : " << current_range_vector[i].routing_track_vector.size() << endl;
        // for (int j = 0; j < current_range_vector[i].power_stripe_vector.size(); j++)
        // {
        //     cout << current_range_vector[i].power_stripe_vector[j].start_x_location << " " << current_range_vector[i].power_stripe_vector[j].start_y_location << endl;
        // }
    }

    cout << " power consuming of each range : " << total_ip_power << endl;
}

void getSwitchingPowerReport(string sw_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    ifstream sw_file(sw_report);
    string sw_content;
    int log = 0;
    float power = 0;
    if (sw_file)
    {
        while (getline(sw_file, sw_content))
        {
            if (sw_content.find("Cell") != string::npos && sw_content.find("Instance") != string::npos)
            {
                continue;
            }
            else if (sw_content.find("Range") != string::npos && sw_content.find("Total") != string::npos)
            {
                break;
            }
            else
            {
                vector<string> sw_content_array = splitByPattern(sw_content, " ");
                float switching_power = stof(sw_content_array[0]);
                string cell_name = sw_content_array[1];
                string cell_id = sw_content_array[2];

                if ((*cell_ip_map).count(cell_id))
                {
                    (*cell_ip_map)[cell_id].switching_power = switching_power;
                }
                else
                {
                    cout << " getSwitchingPowerReport exception  " << cell_id << endl;
                };

                log++;
                if (log % 1000 == 0)
                {
                    cout << "switching data : " << log << endl;
                }
            }
        }
    }
    cout << "total : " << power << endl;
}

void generateAddStripeTcl(vector<CurrentRange> *current_range_vector, string add_stripe_estimate_name)
{
    ofstream myfile;
    myfile.open(add_stripe_estimate_name);
    for (int i = 0; i < (*current_range_vector).size(); i++)
    {
        for (int j = 0; j < (*current_range_vector)[i].power_stripe_vector.size(); j++)
        {
            PowerStripe power_stripe = (*current_range_vector)[i].power_stripe_vector[j];
            PowerGround power_ground = (*current_range_vector)[i].power_ground_vector[j];
            myfile << "addStripe -nets { VDDX } -layer " << M3 << " -direction vertical -width " << POWER_STRIPE_WIDTH << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << power_stripe.start_x_location << " " << power_stripe.start_y_location << " " << power_stripe.end_x_location << " " << power_stripe.end_y_location << " }" << endl;
            myfile << "addStripe -nets { VSSX } -layer " << M3 << " -direction vertical -width " << POWER_STRIPE_WIDTH << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << power_ground.start_x_location << " " << power_ground.start_y_location << " " << power_ground.end_x_location << " " << power_ground.end_y_location << " }" << endl;
        }
    }
    myfile.close();
};

float tranferSupplyLocaion(string side, float x_location, float y_location, CoreSite *core_site)
{
    float supply_location = 0;

    float odd_section = stof((*core_site).up_y_location);
    float even_section = stof((*core_site).right_x_location);

    if (side == UP)
    {
        supply_location = x_location + odd_section;
    }
    else if (side == DOWN)
    {
        float range = abs(stof((*core_site).down_y_location) - y_location);
        supply_location = odd_section + even_section + odd_section + range;
    }
    else if (side == LEFT)
    {
        supply_location = y_location;
    }
    else if (side == RIGHT)
    {
        float range = abs(stof((*core_site).up_y_location) - y_location);
        supply_location = odd_section + even_section + range;
    }
    else
    {
        cout << "tranferSupplyLocaion error " << endl;
    }
    return supply_location;
}

float getRangeDistance(vector<PowerPad> *vdd_range_pad_vector, float supply_location, float total_range, CoreSite *core_site)
{
    float middle = 0;
    PowerPad power_pad;
    for (int i = 0; i < (*vdd_range_pad_vector).size(); i++)
    {
        power_pad = (*vdd_range_pad_vector)[i];
        float supply_range_start = power_pad.supply_range_start;
        float supply_range_end = power_pad.supply_range_end;
        string middle_position = power_pad.middle_postition;

        if (i == 0)
        {
            if (middle_position == LEFT)
            {
                if (supply_range_start <= supply_location && supply_range_end >= supply_location)
                {
                    middle = abs(power_pad.supply_location - supply_location);

                    break;
                }
                else
                {
                    // cout << "PowerPad location error" << endl;
                }
            }
            else if (middle_position == DOWN)
            {
                if (supply_range_start <= supply_location && supply_location <= total_range)
                {
                    middle = abs(power_pad.supply_location - stof((*core_site).left_x_location)) + abs(total_range - supply_location);
                    break;
                }
                else if (stof((*core_site).left_x_location) <= supply_location && supply_location <= supply_range_end)
                {
                    middle = abs(power_pad.supply_location - supply_location);
                    break;
                }
                else
                {
                }
            }
            else
            {
                cout << " powerStripePowerRailToSupplyDistance error " << endl;
            }
        }
        else if (i == ((*vdd_range_pad_vector).size() - 1))
        {
            if (middle_position == LEFT)
            {
                if ((supply_range_start <= supply_location && total_range >= supply_location))
                {
                    middle = abs(power_pad.supply_location - supply_location);
                    break;
                }
                else if (stof((*core_site).left_x_location) <= supply_location && supply_range_end >= supply_location)
                {
                    middle = abs(power_pad.supply_location - total_range) + abs(stof((*core_site).left_x_location) - supply_location);
                    break;
                }
                else
                {
                    // cout << "PowerPad location error" << endl;
                }
            }
            else if (middle_position == DOWN)
            {
                if (supply_range_start <= supply_location && supply_range_end >= supply_location)
                {
                    middle = abs(power_pad.supply_location - supply_location);
                    break;
                }
                else
                {
                    // cout << "PowerPad location error" << endl;
                }
            }
            else
            {
                cout << " powerStripePowerRailToSupplyDistance error " << endl;
            }
        }
        else
        {
            if (supply_range_start <= supply_location && supply_range_end >= supply_location)
            {
                middle = abs(power_pad.supply_location - supply_location);

                break;
            }
            else
            {
                // cout << "PowerPad location error" << endl;
            }
        }
    }
    // cout << "power pad side : " << power_pad.side << " power pad location : " << power_pad.x_location << " " << power_pad.y_location << " "
    //      << " power pad supply location : " << power_pad.supply_location << endl;
    return middle;
}

void setPowerPadSupplyRange(CoreSite *core_site, map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector)
{
    float odd_section = stof((*core_site).up_y_location);
    float even_section = stof((*core_site).right_x_location);

    sortRangePadVector(&(*vdd_range_pad_vector), &(*direction_power_pad), &(*core_site));

    for (int i = 0; i < (*vdd_range_pad_vector).size(); i++)
    {

        if (i == 0)
        {
            float supply_end_location = ((*vdd_range_pad_vector)[i].supply_location + (*vdd_range_pad_vector)[i + 1].supply_location) / 2;
            (*vdd_range_pad_vector)[i].supply_range_end = supply_end_location;
        }
        else if (i == ((*vdd_range_pad_vector).size() - 1))
        {
            float supply_start_location = ((*vdd_range_pad_vector)[i].supply_location + (*vdd_range_pad_vector)[i - 1].supply_location) / 2;

            //全長
            float range_section = odd_section + even_section + odd_section + even_section;

            // 尾邊界距離最後一個pad的長度
            float end_range_last = abs((*vdd_range_pad_vector)[i].supply_location - range_section);
            // 頭邊界距離第一個pad的長度
            float end_range_first = abs((*vdd_range_pad_vector)[0].supply_location - stof((*core_site).left_x_location));
            // 兩個相加 / 2
            float end_range = (end_range_first + end_range_last) / 2;
            // 從尾邊界開始算的長度
            float supply_end_location = ((*vdd_range_pad_vector)[i].supply_location) + end_range;

            // 兩個分界線在尾邊界之後
            if (supply_end_location > range_section)
            {
                // 如果頭尾分段在 頭的位置 以頭位置為主

                (*vdd_range_pad_vector)[i].supply_range_start = supply_start_location;
                (*vdd_range_pad_vector)[i].supply_range_end = supply_end_location - range_section;
                float first_supply_power_pad_start_location = (supply_end_location - range_section) + stof((*core_site).left_x_location);
                (*vdd_range_pad_vector)[0].supply_range_start = first_supply_power_pad_start_location;
                (*vdd_range_pad_vector)[0].middle_postition = LEFT;
                (*vdd_range_pad_vector)[i].middle_postition = LEFT;
            }
            // 兩個分界線在尾邊界之前
            else
            {
                // 如果頭尾分段在 尾的位置 以尾位置為主

                (*vdd_range_pad_vector)[i].supply_range_start = supply_start_location;
                (*vdd_range_pad_vector)[i].supply_range_end = supply_end_location;
                (*vdd_range_pad_vector)[0].supply_range_start = supply_end_location;
                (*vdd_range_pad_vector)[0].middle_postition = DOWN;
                (*vdd_range_pad_vector)[i].middle_postition = DOWN;
            }
        }
        else
        {
            float supply_end_location = ((*vdd_range_pad_vector)[i].supply_location + (*vdd_range_pad_vector)[i + 1].supply_location) / 2;
            float supply_start_location = ((*vdd_range_pad_vector)[i].supply_location + (*vdd_range_pad_vector)[i - 1].supply_location) / 2;
            (*vdd_range_pad_vector)[i].supply_range_start = supply_start_location;
            (*vdd_range_pad_vector)[i].supply_range_end = supply_end_location;
        }
    };

    for (int i = 0; i < (*vdd_range_pad_vector).size(); i++)
    {
        cout << "power pad side : " << (*vdd_range_pad_vector)[i].side << " location : " << (*vdd_range_pad_vector)[i].x_location << " " << (*vdd_range_pad_vector)[i].y_location << " supply loction : " << (*vdd_range_pad_vector)[i].supply_location << " "
             << "range start to end : "
             << " " << (*vdd_range_pad_vector)[i].supply_range_start << " " << (*vdd_range_pad_vector)[i].supply_range_end << endl;
    }
    cout << "odd_section    : " << odd_section << endl;
    cout << "even_section   : " << even_section << endl;
    cout << "total section  : " << odd_section + even_section + odd_section + even_section << endl;
}
void sortRangePadVector(vector<PowerPad> *vdd_range_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, CoreSite *core_site)
{
    float odd_section = stof((*core_site).up_y_location);
    float even_section = stof((*core_site).right_x_location);

    (*vdd_range_pad_vector).insert((*vdd_range_pad_vector).end(), (*direction_power_pad)[LEFT].begin(), (*direction_power_pad)[LEFT].end());
    (*vdd_range_pad_vector).insert((*vdd_range_pad_vector).end(), (*direction_power_pad)[UP].begin(), (*direction_power_pad)[UP].end());
    vector<PowerPad> right_power_pad_vector = (*direction_power_pad)[RIGHT];
    vector<PowerPad> down_power_pad_vector = (*direction_power_pad)[DOWN];
    reverse(right_power_pad_vector.begin(), right_power_pad_vector.end());
    reverse(down_power_pad_vector.begin(), down_power_pad_vector.end());
    (*vdd_range_pad_vector).insert((*vdd_range_pad_vector).end(), right_power_pad_vector.begin(), right_power_pad_vector.end());
    (*vdd_range_pad_vector).insert((*vdd_range_pad_vector).end(), down_power_pad_vector.begin(), down_power_pad_vector.end());

    // for (int i = 0; i < (*vdd_range_pad_vector).size(); i++)
    // {
    //     cout << (*vdd_range_pad_vector)[i].side << " " << (*vdd_range_pad_vector)[i].x_location << " " << (*vdd_range_pad_vector)[i].y_location << endl;
    // }

    for (int i = 0; i < (*vdd_range_pad_vector).size(); i++)
    {
        cout << (*vdd_range_pad_vector)[i].side << " " << (*vdd_range_pad_vector)[i].x_location << " " << (*vdd_range_pad_vector)[i].y_location << endl;
        string side = (*vdd_range_pad_vector)[i].side;

        if (side == LEFT)
        {
            (*vdd_range_pad_vector)[i].supply_location = stof((*vdd_range_pad_vector)[i].y_location);
        }
        else if (side == UP)
        {
            float range = abs(stof((*vdd_range_pad_vector)[i].x_location) - stof((*core_site).left_x_location));
            (*vdd_range_pad_vector)[i].supply_location = range + odd_section;
        }
        else if (side == RIGHT)
        {
            float range = abs(stof((*core_site).up_y_location) - stof((*vdd_range_pad_vector)[i].y_location));
            (*vdd_range_pad_vector)[i].supply_location = range + odd_section + even_section;
        }
        else if (side == DOWN)
        {
            float range = abs(stof((*core_site).right_x_location) - stof((*vdd_range_pad_vector)[i].x_location));
            (*vdd_range_pad_vector)[i].supply_location = range + odd_section + even_section + odd_section;
        }
        else
        {
            cout << " check error " << endl;
        }
    }
}

// TRACKS Y 0.144 DO 0.56725 STEP 0.144 LAYER M3 ;
// TRACKS X 0.18 DO 0.704 STEP 0.144 LAYER M3 ;
// DESIGN FE_CORE_BOX_LL_X REAL 10.0800 ;
// DESIGN FE_CORE_BOX_UR_X REAL 395.5680 ;
// DESIGN FE_CORE_BOX_LL_Y REAL 10.0800 ;
// DESIGN FE_CORE_BOX_UR_Y REAL 316.8000 ;

float getStartTrackLocation(Track *track, float start_x_location)
{
    float start_step = stof((*track).start_step);
    float step = stof((*track).step);

    float temp_location = start_x_location - start_step;
    float track_x_location = 0;

    if (fmod(temp_location, step) > 0)
    {
        int num_of_step = temp_location / step;
        num_of_step = num_of_step + 1;

        track_x_location = num_of_step * step;
    }
    else
    {
        int num_of_step = temp_location / step;
        track_x_location = num_of_step * step;
    }
    return (track_x_location + start_step);
}
void setRoutingTrack(CurrentRange *current_range, float start_x_location, float step)
{
    float right_x_locaiton = (*current_range).right_x_location;
    for (float i = start_x_location; i < right_x_locaiton; i += step)
    {
        RoutingTrack routing_track;
        routing_track.start_x_location = i;

        (*current_range).routing_track_vector.push_back(routing_track);
    }
}

void setRoutingTrackInCurrentRange(unordered_map<string, unordered_map<string, Track>> *layer_track_map, vector<CurrentRange> *current_range_vector)
{
    // TRACKS Y 0.144 DO 0.56725 STEP 0.144 LAYER M3 ;
    // power stripe track
    // TRACKS X 0.18 DO 0.704 STEP 0.144 LAYER M3 ;
    // step 1 : 計算出最近一條routing track
    // step 2 : 這個range 有多少routing track
    // (*layer_track_map)["M3"]["X"].start_step;

    // DESIGN FE_CORE_BOX_LL_X REAL 10.0800 ;
    // DESIGN FE_CORE_BOX_UR_X REAL 395.5680 ;
    // DESIGN FE_CORE_BOX_LL_Y REAL 10.0800 ;
    // DESIGN FE_CORE_BOX_UR_Y REAL 316.8000 ;
    Track track = (*layer_track_map)[M3][X];
    for (int i = 0; i < (*current_range_vector).size(); i++)
    {
        CurrentRange current_range = (*current_range_vector)[i];
        float start_x_location = getStartTrackLocation(&track, current_range.left_x_location);
        setRoutingTrack(&(*current_range_vector)[i], start_x_location, stof(track.step));
    }
}

void setCurrentRangePowerStripe(map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector, vector<CurrentRange> *current_range_vector, vector<FollowPin> *follow_pin_vdd_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, CoreSite *core_site)
{
    vector<string> side_vec;
    set<string> side_set;
    int number_of_power_stripe = 0;

    for (auto direct_pad_iter = (*direction_power_pad).begin(); direct_pad_iter != (*direction_power_pad).end(); ++direct_pad_iter)
    {
        string side = direct_pad_iter->first;

        vector<PowerPad> pad_vec = direct_pad_iter->second;
        if (pad_vec.size() > 0)
        {
            side_vec.push_back(side);
            side_set.insert(side);
            cout << "side : " << side << endl;
        }
    }

    // cout << side_vec.size() << endl;

    if (side_vec.size() == 1)
    {
        setSinglePad(&(*direction_power_pad), &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*current_range_vector), &(*layer_track_map), &(*vdd_shape_map), &(*vdd_shape_map), &side_set, &(*core_site));
    }
    else if (side_vec.size() == 2)
    {
    }
    else if (side_vec.size() == 3)
    {
    }
    else if (side_vec.size() == 4)
    {
        setForthPadResist(&(*direction_power_pad), &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*current_range_vector), &(*layer_track_map), &(*vdd_shape_map), &(*vss_shape_map), &side_set, &(*core_site));
    }
    else
    {
        cout << "foul side : " << side_vec.size() << endl;
    }
}
//平均分布四個邊
int setForthPadResist(map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, set<string> *side_set, CoreSite *core_site)
{
    float pad_to_ring_up_down = countPadToRing(&(*vdd_shape_map), &(*direction_power_pad)[UP][0], UP);
    float pad_to_ring_left_right = countPadToRing(&(*vdd_shape_map), &(*direction_power_pad)[LEFT][0], LEFT);
    int power_stripes = 0;
    float total_resist = 0;

    float power_stripe_y_location_start = stof((*vdd_shape_map)[DOWN].start_y_location) - ((*vdd_shape_map)[DOWN].width / 2);
    float power_stripe_y_location_end = stof((*vdd_shape_map)[UP].start_y_location) + ((*vdd_shape_map)[DOWN].width / 2);
    cout << "power_stripe_y_location_start : " << power_stripe_y_location_start << endl;
    cout << "power_stripe_y_location_end : " << power_stripe_y_location_end << endl;

    float power_ground_y_location_start = stof((*vss_shape_map)[DOWN].start_y_location) - ((*vdd_shape_map)[DOWN].width / 2);
    float power_ground_y_location_end = stof((*vss_shape_map)[UP].start_y_location) + ((*vdd_shape_map)[DOWN].width / 2);

    cout << "power_ground_y_location_start : " << power_ground_y_location_start << endl;
    cout << "power_ground_y_location_end : " << power_ground_y_location_end << endl;

    float power_stripe_height = abs(power_stripe_y_location_start - power_stripe_y_location_end);

    for (int i = 0; i < (*current_range_vector).size(); i++)
    {
        cout << "range : " << i << endl;
        float power_stripe_resist = getForthPowerStripeResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring_up_down, power_stripe_height);
        cout << "power_stripe_resist : " << power_stripe_resist << endl;
        float power_rail_resist = getForthFollowPinResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring_left_right);
        cout << "power_rail_resist : " << power_rail_resist << endl;
        cout << "routing track size : " << (*current_range_vector)[i].routing_track_vector.size() << endl;

        int number_of_stripe = getNumberOfPowerStripe(&(*current_range_vector)[i], power_stripe_resist, power_rail_resist, (*follow_pin_vdd_vector).size());
        cout << "number_of_stripe : " << number_of_stripe << endl;
        if (number_of_stripe > 0)
        {
            generatePowerStripeTcl(&(*current_range_vector)[i], number_of_stripe, power_stripe_y_location_start, power_stripe_y_location_end, power_ground_y_location_start, power_ground_y_location_end);
        }
        power_stripes += number_of_stripe;
        total_resist += power_stripe_resist;
        total_resist += power_rail_resist;
    }

    cout << "number of power stripes : " << power_stripes << endl;
    cout << "total_resist     : " << total_resist << endl;
}

float getForthFollowPinResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring_left_right)
{
    // ========= Pad TO Ring start =========
    float pad_to_ring_resist = (pad_to_ring_left_right / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    // ========= Pad TO Ring end =========

    // TODO 之後跑沒問題之後這段要改掉 放到下面的迴圈內
    //================= FollowPin To Range 距離 start =================
    float total_range_follow_pin_distance = 0;
    // TODO 這段會有重複計算的問題
    //  float followpin_coreSite_distance = getFollowPinToCoreSiteLeft(&(*vdd_shape_map), &(*core_site));
    //  range_follow_pin_distance += followpin_coreSite_distance;

    float range_follow_pin_distance = abs((*current_range).left_x_location - (*current_range).right_x_location);
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        total_range_follow_pin_distance += range_follow_pin_distance;
    }
    total_range_follow_pin_distance = total_range_follow_pin_distance / 2;

    float range_follw_pin_resist = (total_range_follow_pin_distance / POWER_RAIL_WIDTH) * LISD_SHEET_RESISTANCE;

    //================= FollowPin To Range 距離 end =================

    //================= 在此range裡所有的FollowPin start =================
    float power_rail_distance = 0;
    float power_rail_distance_left = 0;
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        // 目前follow pin 只有存 y location
        float power_distance_temp = powerStripePowerRailToSupplyDistance(LEFT, 0, stof((*follow_pin_vdd_vector)[i].y_location), &(*vdd_range_pad_vector), &(*core_site));
        power_rail_distance_left += power_distance_temp;
    }

    float power_rail_distance_right = 0;
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        // 目前follow pin 只有存 y location
        float power_distance_temp = powerStripePowerRailToSupplyDistance(RIGHT, 0, stof((*follow_pin_vdd_vector)[i].y_location), &(*vdd_range_pad_vector), &(*core_site));
        power_rail_distance_right += power_distance_temp;
    }

    if (power_rail_distance_left <= power_rail_distance_right)
    {
        power_rail_distance = power_rail_distance_left;
    }
    else
    {
        power_rail_distance = power_rail_distance_right;
    }
    float power_shape_ring_resist = (power_rail_distance / POWER_RAIL_WIDTH) * M2_SHEET_RESISTANCE;
    //================= 在此range裡所有的FollowPin end =================

    return (pad_to_ring_resist + range_follw_pin_resist + power_shape_ring_resist);
}

float getForthPowerStripeResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring_up_down, float power_stripe_height)
{

    // ========= Pad To Ring start =========
    float pad_to_ring_resist_up_down = (pad_to_ring_up_down / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;

    // ========= Pad To Ring end =========

    // ========= power stripe to shape Ring start =========
    float stripe_to_shape_ring = 0;
    float range_middle_location = ((*current_range).right_x_location + (*current_range).left_x_location) / 2;
    // cout << "range_middle_location : " << range_middle_location << endl;
    float power_stripe_up_distance_temp = powerStripePowerRailToSupplyDistance(UP, range_middle_location, 0, &(*vdd_range_pad_vector), &(*core_site));
    float power_stripe_down_distance_temp = powerStripePowerRailToSupplyDistance(DOWN, range_middle_location, 0, &(*vdd_range_pad_vector), &(*core_site));
    if (power_stripe_up_distance_temp <= power_stripe_down_distance_temp)
    {
        stripe_to_shape_ring = power_stripe_up_distance_temp;
    }
    else
    {
        stripe_to_shape_ring = power_stripe_down_distance_temp;
    }
    float stripe_to_shape_ring_resist = (stripe_to_shape_ring / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    // ========= power stripe to shape Ring end =========
    float power_stripe_resist = (power_stripe_height / POWER_STRIPE_WIDTH) * M3_SHEET_RESISTANCE_STRIPE;
    power_stripe_resist = power_stripe_resist / 2;

    return (pad_to_ring_resist_up_down + stripe_to_shape_ring_resist + power_stripe_resist);
}

float getSiglePowerPadToPowerRailDistanceUpAndDown(float power_pad_middle, CoreSite *core_site)
{
    float left_distance = abs(power_pad_middle - stof((*core_site).left_x_location));
    float right_distance = abs(power_pad_middle - stof((*core_site).right_x_location));
    if (left_distance > right_distance)
    {
        return right_distance;
    }
    else
    {
        return left_distance;
    }
};

int getSinglePadCurrentRangeNumberOfPowerStripe(CurrentRange *currentRange, float power_pad_middle, float pad_to_ring, CoreSite *core_site, string side, float power_stripe_height)
{
    if (side == UP || side == DOWN)
    {
        // ====== power stripe distance ========
        float current_range_middle = ((*currentRange).left_x_location + (*currentRange).right_x_location) / 2;
        float current_range_to_pad_distance = abs(power_pad_middle - current_range_middle);
        float other_side_distance = abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));
        // ====== power stripe distance ========

        // ====== power stripe resistance ========
        float pad_to_ring_resist = (pad_to_ring / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
        float current_range_to_resist = (current_range_to_pad_distance / M2_WIDTH) * M2_SHEET_RESISTANCE;
        float power_stripe_resist = (power_stripe_height / POWER_STRIPE_WIDTH) * M3_SHEET_RESISTANCE_STRIPE;
        float power_stripe_total_resist = pad_to_ring_resist + current_range_to_resist + power_stripe_resist + pad_to_ring_resist;
        // ====== power stripe resistance ========

        // ====== power rail distance ========
        float power_rail_side_distance = abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));
        float power_rail_range_distance = (stof((*core_site).left_x_location) - (*currentRange).right_x_location);
        float power_pad_to_power_rail_distance = getSiglePowerPadToPowerRailDistanceUpAndDown(power_pad_middle, &(*core_site));
        //  ====== power rail distance ========

        // ====== power rail resistance ========
        float power_rail_side_resist = (power_rail_side_distance / M2_WIDTH) * M2_SHEET_RESISTANCE;
        float power_rail_range_resist = (power_rail_range_distance / POWER_RAIL_WIDTH) * LISD_SHEET_RESISTANCE;
        float power_pad_to_power_rail_resist = (power_pad_to_power_rail_distance / M2_WIDTH) * M2_SHEET_RESISTANCE;

        float power_rail_total_resist = power_rail_side_resist + power_rail_range_resist + power_pad_to_power_rail_resist + pad_to_ring_resist;
        // ====== power rail resistance ========
    }
    else if (side == LEFT || side == RIGHT)
    {
        //    float width_distance =  abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));
        //    float pad_to_stripe_resistance = getPadToStripeResistance(&(*currentRange), power_pad_middle, pad_to_ring, false, width_distance);
        //    float pad_to_rail_resistance = getPadToRailResistance(&(*currentRange), power_pad_middle, pad_to_ring, true, 0);
    }
}

// Single side
float getSingleFolllowPinResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring)
{

    // ========= Pad TO Ring start =========
    float pad_to_ring_resist = (pad_to_ring / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    // ========= Pad TO Ring end =========

    // TODO 之後跑沒問題之後這段要改掉 放到下面的迴圈內
    //================= FollowPin To Range 距離 start =================
    float total_range_follow_pin_distance = 0;
    //這段會有重複計算的問題
    // float followpin_coreSite_distance = getFollowPinToCoreSiteLeft(&(*vdd_shape_map), &(*core_site));
    // range_follow_pin_distance += followpin_coreSite_distance;
    float range_follow_pin_distance = abs((*current_range).left_x_location - (*current_range).right_x_location);
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        total_range_follow_pin_distance += range_follow_pin_distance;
    }

    float range_follw_pin_resist = (total_range_follow_pin_distance / POWER_RAIL_WIDTH) * LISD_SHEET_RESISTANCE;
    //================= FollowPin To Range 距離 end =================

    //================= 在此range裡所有的FollowPin start =================
    float power_rail_distance = 0;
    float power_rail_distance_left = 0;
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        // 目前follow pin 只有存 y location
        float power_distance_temp = powerStripePowerRailToSupplyDistance(LEFT, 0, stof((*follow_pin_vdd_vector)[i].y_location), &(*vdd_range_pad_vector), &(*core_site));
        power_rail_distance_left += power_distance_temp;
    }

    float power_rail_distance_right = 0;
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        // 目前follow pin 只有存 y location
        float power_distance_temp = powerStripePowerRailToSupplyDistance(RIGHT, 0, stof((*follow_pin_vdd_vector)[i].y_location), &(*vdd_range_pad_vector), &(*core_site));
        power_rail_distance_right += power_distance_temp;
    }
    if (power_rail_distance_left <= power_rail_distance_right)
    {
        power_rail_distance = power_rail_distance_left;
    }
    else
    {
        power_rail_distance = power_rail_distance_right;
    }
    float power_shape_ring_resist = (power_rail_distance / POWER_RAIL_WIDTH) * M2_SHEET_RESISTANCE;
    //================= 在此range裡所有的FollowPin end =================

    return (pad_to_ring_resist + range_follw_pin_resist + power_shape_ring_resist);
}

//取LEFT follow Pin to CoreSite
float getFollowPinToCoreSiteLeft(map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site)
{
    float distance = abs(stof((*vdd_shape_map)[LEFT].start_x_location) - stof((*core_site).left_x_location));

    return distance;
}
float getSinglePowerStripeResist(CurrentRange *current_range, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, map<string, ShapeRing> *vdd_shape_map, CoreSite *core_site, float pad_to_ring, float power_stripe_height)
{
    // ========= Pad To Ring start =========
    float pad_to_ring_resist = (pad_to_ring / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    // ========= Pad To Ring end =========

    // ========= power stripe to shape Ring start =========
    float stripe_to_shape_ring = 0;
    float range_middle_location = ((*current_range).right_x_location + (*current_range).left_x_location) / 2;
    float power_stripe_up_distance_temp = powerStripePowerRailToSupplyDistance(UP, range_middle_location, 0, &(*vdd_range_pad_vector), &(*core_site));
    float power_stripe_down_distance_temp = powerStripePowerRailToSupplyDistance(DOWN, range_middle_location, 0, &(*vdd_range_pad_vector), &(*core_site));
    if (power_stripe_up_distance_temp <= power_stripe_down_distance_temp)
    {
        stripe_to_shape_ring = power_stripe_up_distance_temp;
    }
    else
    {
        stripe_to_shape_ring = power_stripe_down_distance_temp;
    }
    float stripe_to_shape_ring_resist = (stripe_to_shape_ring / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    // ========= power stripe to shape Ring end =========

    float power_stripe_resist = (power_stripe_height / POWER_STRIPE_WIDTH) * M3_SHEET_RESISTANCE_STRIPE;

    return (pad_to_ring_resist + stripe_to_shape_ring_resist + power_stripe_resist);
}

int getNumberOfPowerStripe(CurrentRange *current_range, float power_stripe_resist, float power_rail_resist, int number_of_power_rail)
{

    float total_power = ((*current_range).range_total_power) * 0.001 *0.3;

    float temp_total_current = total_power / VDD_PAD;

    // float temp_power_rail_current = (IR_DROP / power_rail_resist);
    // temp_total_current = (temp_total_current - temp_power_rail_current);

    int number_of_stripe = (temp_total_current * power_stripe_resist) / IR_DROP;
    return number_of_stripe;
}

void generatePowerStripeTcl(CurrentRange *currenge, int number_of_stripe, float start_y_location_vdd_stripe, float end_y_location_vdd_stripe, float start_y_location_vss_stripe, float end_y_location_vss_stripe)
{

    int routing_track_size = (*currenge).routing_track_vector.size();

    int interval = routing_track_size / number_of_stripe;
    interval += 1;
    PowerStripe power_stripe;
    PowerGround power_ground;
    float power_stripe_width = POWER_STRIPE_WIDTH / 2;
    for (int i = 0; i < routing_track_size; i += interval)
    {
        float routing_track_location = (*currenge).routing_track_vector[i].start_x_location;
        // TODO 之後要改成動態
        float routing_track_powerGround_location = (*currenge).routing_track_vector[i].start_x_location + START_TRACK;

        float start_x_location = routing_track_location - power_stripe_width;
        float end_x_location = routing_track_location + power_stripe_width;
        power_stripe.start_x_location = floatToString(start_x_location);
        power_stripe.start_y_location = floatToString(start_y_location_vdd_stripe);
        power_stripe.end_x_location = floatToString(end_x_location);
        power_stripe.end_y_location = floatToString(end_y_location_vdd_stripe);

        float power_middle_x_loaction = (stof(power_stripe.start_x_location) + stof(power_stripe.end_x_location)) / 2;
        float ground_middle_x_loaction = power_middle_x_loaction + (START_TRACK * 3);

        float start_x_ground_location = ground_middle_x_loaction - (POWER_STRIPE_WIDTH / 2);
        float end_x_ground_location = ground_middle_x_loaction + (POWER_STRIPE_WIDTH / 2);
        power_ground.start_x_location = floatToString(start_x_ground_location);
        power_ground.start_y_location = floatToString(start_y_location_vss_stripe);
        power_ground.end_x_location = floatToString(end_x_ground_location);
        power_ground.end_y_location = floatToString(end_y_location_vss_stripe);

        (*currenge).power_stripe_vector.push_back(power_stripe);
        (*currenge).power_ground_vector.push_back(power_ground);
    }
}

int setSinglePad(map<string, vector<PowerPad>> *direction_power_pad, vector<PowerPad> *vdd_range_pad_vector, vector<FollowPin> *follow_pin_vdd_vector, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, set<string> *side_set, CoreSite *core_site)
{
    float power_stripe_y_location_start = stof((*vdd_shape_map)[DOWN].start_y_location) - ((*vdd_shape_map)[DOWN].width / 2);
    float power_stripe_y_location_end = stof((*vdd_shape_map)[UP].start_y_location) + ((*vdd_shape_map)[DOWN].width / 2);
    cout << "power_stripe_y_location_start : " << power_stripe_y_location_start << endl;
    cout << "power_stripe_y_location_end : " << power_stripe_y_location_end << endl;

    float power_ground_y_location_start = stof((*vss_shape_map)[DOWN].start_y_location) - ((*vdd_shape_map)[DOWN].width / 2);
    float power_ground_y_location_end = stof((*vss_shape_map)[UP].start_y_location) + ((*vdd_shape_map)[DOWN].width / 2);

    cout << "power_ground_y_location_start : " << power_ground_y_location_start << endl;
    cout << "power_ground_y_location_end : " << power_ground_y_location_end << endl;

    float power_stripe_height = abs(power_stripe_y_location_start - power_stripe_y_location_end);

    //為了計算不同的pad to ring
    if ((*side_set).count(UP))
    {
        float pad_to_ring = countPadToRing(&(*vdd_shape_map), &(*direction_power_pad)[UP][0], UP);
        int power_stripes = 0;
        float total_resist = 0;
        //計算每個current range 需要幾條power_stripe

        for (int i = 0; i < (*current_range_vector).size(); i++)
        {
            cout << "range : " << i << endl;
            float power_stripe_resist = getSinglePowerStripeResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring, power_stripe_height);
            cout << "power_stripe_resist : " << power_stripe_resist << endl;
            float power_rail_resist = getSingleFolllowPinResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring);
            cout << "power_rail_resist : " << power_rail_resist / 178 << endl;
            int number_of_stripe = getNumberOfPowerStripe(&(*current_range_vector)[i], power_stripe_resist, power_rail_resist, (*follow_pin_vdd_vector).size());
            cout << "number_of_stripe : " << number_of_stripe << endl;
            cout << "routing track size : " << (*current_range_vector)[i].routing_track_vector.size() << endl;
            if (number_of_stripe > 0)
            {
                generatePowerStripeTcl(&(*current_range_vector)[i], number_of_stripe, power_stripe_y_location_start, power_stripe_y_location_end, power_ground_y_location_start, power_ground_y_location_end);
            }
            power_stripes += number_of_stripe;
            total_resist += power_stripe_resist;
            total_resist += power_rail_resist;
        }

        cout << "total stripe         : " << power_stripes << endl;
        cout << "toatl resist         : " << total_resist << endl;
    }
    else if ((*side_set).count(DOWN))
    {
        ofstream myfile;
        myfile.open("power_file/power stripe");
        float pad_to_ring = countPadToRing(&(*vdd_shape_map), &(*direction_power_pad)[DOWN][0], DOWN);
        cout << "pad_to_ring : " << pad_to_ring << endl;
        // myfile << "boundary        : " << boundary << endl;
        cout << "" << endl;
        float power_stripes = 0;
        //計算每個current range 需要幾條power_stripe
        for (int i = 0; i < (*current_range_vector).size(); i++)
        {
            float power_stripe_resist = getSinglePowerStripeResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring, power_stripe_height);
            float power_rail_resist = getSingleFolllowPinResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring);
            float number_of_stripe = getNumberOfPowerStripe(&(*current_range_vector)[i], power_stripe_resist, power_rail_resist, (*follow_pin_vdd_vector).size());
            power_stripes += number_of_stripe;
            myfile << "range            : " << i + 1 << endl;
            myfile << "number_of_stripe : " << number_of_stripe << endl;
            myfile << "track            : " << (*current_range_vector)[i].routing_track_vector.size() << endl;
            cout << "" << endl;
        }
        myfile << "total stripe         : " << power_stripes << endl;
        myfile.close();
    }
    else if ((*side_set).count(RIGHT))
    {

        ofstream myfile;
        myfile.open("power_file/power stripe");
        float pad_to_ring = countPadToRing(&(*vdd_shape_map), &(*direction_power_pad)[RIGHT][0], RIGHT);
        cout << "pad_to_ring : " << pad_to_ring << endl;
        // myfile << "boundary        : " << boundary << endl;
        cout << "" << endl;
        float power_stripes = 0;
        //計算每個current range 需要幾條power_stripe
        for (int i = 0; i < (*current_range_vector).size(); i++)
        {
            float power_stripe_resist = getSinglePowerStripeResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring, power_stripe_height);
            float power_rail_resist = getSingleFolllowPinResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring);
            float number_of_stripe = getNumberOfPowerStripe(&(*current_range_vector)[i], power_stripe_resist, power_rail_resist, (*follow_pin_vdd_vector).size());
            power_stripes += number_of_stripe;
            myfile << "range            : " << i + 1 << endl;
            myfile << "number_of_stripe : " << number_of_stripe << endl;
            myfile << "track            : " << (*current_range_vector)[i].routing_track_vector.size() << endl;
            cout << "" << endl;
            // addStripe -nets { VSSX } -layer M3 -direction vertical -width 1.728 -number_of_sets 1 -area { 97.38 0.72 99.108 6.408 }
        }
        myfile << "total stripe         : " << power_stripes << endl;
        myfile.close();
    }
    else if ((*side_set).count(LEFT))
    {
        ofstream myfile;
        myfile.open("power_file/power stripe");
        float pad_to_ring = countPadToRing(&(*vdd_shape_map), &(*direction_power_pad)[LEFT][0], LEFT);
        cout << "pad_to_ring : " << pad_to_ring << endl;
        // myfile << "boundary        : " << boundary << endl;
        cout << "" << endl;
        float power_stripes = 0;
        //計算每個current range 需要幾條power_stripe
        for (int i = 0; i < (*current_range_vector).size(); i++)
        {
            float power_stripe_resist = getSinglePowerStripeResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring, power_stripe_height);
            float power_rail_resist = getSingleFolllowPinResist(&(*current_range_vector)[i], &(*vdd_range_pad_vector), &(*follow_pin_vdd_vector), &(*vdd_shape_map), &(*core_site), pad_to_ring);
            float number_of_stripe = getNumberOfPowerStripe(&(*current_range_vector)[i], power_stripe_resist, power_rail_resist, (*follow_pin_vdd_vector).size());
            power_stripes += number_of_stripe;
            myfile << "range            : " << i + 1 << endl;
            myfile << "number_of_stripe : " << number_of_stripe << endl;
            myfile << "track            : " << (*current_range_vector)[i].routing_track_vector.size() << endl;
            cout << "" << endl;
        }
        myfile << "total stripe         : " << power_stripes << endl;
        myfile.close();
    }
    else
    {
        cout << " setSinglePad error " << endl;
    }
    return 0;
}
//計算 power rail or power stripe　到 power pad 最近的距離
float powerStripePowerRailToSupplyDistance(string transfer_supply_location_side, float x_location, float y_location, vector<PowerPad> *vdd_range_pad_vector, CoreSite *core_site)
{
    // side 是給 tranferSupplyLocaion 去做轉換而使用
    // cout << "tranfer :" << transfer_supply_location_side << " " << x_location << " " << y_location << endl;
    float supply_location = tranferSupplyLocaion(transfer_supply_location_side, x_location, y_location, &(*core_site));

    float odd_section = stof((*core_site).up_y_location);
    float even_section = stof((*core_site).right_x_location);
    float total_range = odd_section + even_section + odd_section + even_section;
    float middle = 0;
    if ((*vdd_range_pad_vector).size() > 1)
    {
        middle = getRangeDistance(&(*vdd_range_pad_vector), supply_location, total_range, &(*core_site));
    }
    else
    {
        middle = abs(supply_location - (*vdd_range_pad_vector)[0].supply_location);
    }
    // cout << middle << endl;

    // cout << "tranfer :" << transfer_supply_location_side << " " << x_location << " " << y_location << " ----> " << supply_location << endl;
    // cout << "power pad to power stripe : " << middle << endl;
    // powerStripePowerRailToSupplyDistance error
    // TODO check
    if (middle == 0)
    {
        // cout << " exception error " << endl;
    }
    return middle;
}

int caculateSinglePowerStripe(map<string, float> *power_value_map, float power_stripe_height, float power_rail_number)
{
    float m3_square = power_stripe_height / POWER_STRIPE_WIDTH;
    float m1_square = POWER_RAIL_HEIGHT / POWER_RAIL_WIDTH;
    float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
    float m1_resistance = LISD_SHEET_RESISTANCE * m1_square;

    m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
    m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

    float total_power = 5.14506685 * 0.001;
    float temp_total_current = total_power / VDD_PAD;

    float m3_current = IR_DROP / (m3_resistance);
    float m1_current = IR_DROP / (m1_resistance);

    float temp = temp_total_current - (power_rail_number * m1_current);
    int m3_number = temp / m3_current;

    // float temp_current = m3_current + (POWER_RAIL_NUMBER * m1_current);
    // temp_total_current = temp_total_current - temp_current;
    // int m3_number = temp_total_current / m3_current;
    return m3_number;
}
float countPadToRing(map<string, ShapeRing> *shape_map, PowerPad *power_pad, string side)
{

    if (side == UP)
    {
        float power_pad_length = stof((*power_pad).y_location) - ((*power_pad).length / 2);
        float pad_to_ring_distance = abs(stof((*shape_map)[UP].start_y_location) - power_pad_length);

        return pad_to_ring_distance;
    }
    else if (side == DOWN)
    {
        float power_pad_length = stof((*power_pad).y_location) + ((*power_pad).length / 2);
        float pad_to_ring_distance = abs(stof((*shape_map)[DOWN].start_y_location) - power_pad_length);

        return pad_to_ring_distance;
    }
    else if (side == LEFT)
    {
        float power_pad_length = stof((*power_pad).x_location) + ((*power_pad).length / 2);
        float pad_to_ring_distance = abs(stof((*shape_map)[LEFT].start_x_location) - power_pad_length);

        return pad_to_ring_distance;
    }
    else if (side == RIGHT)
    {
        float power_pad_length = stof((*power_pad).x_location) - ((*power_pad).length / 2);
        float pad_to_ring_distance = abs(stof((*shape_map)[RIGHT].start_x_location) - power_pad_length);

        return pad_to_ring_distance;
    }
    else
    {
        cout << " countPadToRing error " << endl;
        return 0;
    }
}

void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area)
{
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        setSide(&(*vdd_power_pad_vector)[i], &(*direction_power_pad), &(*die_area));
    }
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        setSide(&(*vss_power_pad_vector)[i], &(*direction_power_pad), &(*die_area));
    }
}
void sortPowerPad(map<string, vector<PowerPad>> *direction_power_pad)
{

    for (auto &item : (*direction_power_pad))
    {
        string direction = item.first;
        power_pad_loction_sort(&((*direction_power_pad)[direction]), direction);
    }

    // for (auto &item : (*direction_power_pad))
    // {
    //     string direction = item.first;
    //     if (direction == UP || direction == DOWN)
    //     {
    //         for (int i = 0; i < item.second.size(); i++)
    //         {
    //             cout << direction << " " << item.second[i].x_location << " ";
    //             cout << "" << endl;
    //         }
    //     }
    //     else
    //     {
    //         for (int i = 0; i < item.second.size(); i++)
    //         {
    //             cout << direction << " " << item.second[i].y_location << " ";
    //             cout << "" << endl;
    //         }
    //     }
    // }
}
//暫且紀錄M1
void getFollowPin(string def_transfer_file_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector)
{
    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    if (def_transfer_file)
    {
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
                    while (getline(def_transfer_file, def_content))
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
    def_transfer_file.close();
}
void power_pad_loction_sort(vector<PowerPad> *power_pad_vec, string direction)
{
    for (int i = 1; i < (*power_pad_vec).size(); i++)
    {
        if (direction == UP || direction == DOWN)
        {
            for (int i = 1; i < (*power_pad_vec).size(); i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (stof((*power_pad_vec)[j].x_location) > stof((*power_pad_vec)[i].x_location))
                    {
                        (*power_pad_vec).insert((*power_pad_vec).begin() + j, (*power_pad_vec)[i]);
                        (*power_pad_vec).erase((*power_pad_vec).begin() + i + 1);
                        break;
                    }
                }
            }
        }
        else
        {
            for (int i = 1; i < (*power_pad_vec).size(); i++)
            {
                for (int j = 0; j < i; j++)
                {
                    if (stof((*power_pad_vec)[j].y_location) > stof((*power_pad_vec)[i].y_location))
                    {
                        (*power_pad_vec).insert((*power_pad_vec).begin() + j, (*power_pad_vec)[i]);
                        (*power_pad_vec).erase((*power_pad_vec).begin() + i + 1);
                        break;
                    }
                }
            }
        }
    }
}
void setSide(PowerPad *power_pad, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area)
{
    if (stof(power_pad->x_location) == stof(die_area->lower_left_x_location))
    {
        power_pad->side = LEFT;
        (*direction_power_pad)[LEFT].push_back((*power_pad));
    }
    else if (stof((power_pad->y_location)) == stof(die_area->lower_left_y_location))
    {
        power_pad->side = DOWN;
        (*direction_power_pad)[DOWN].push_back((*power_pad));
    }
    else if (stof(power_pad->x_location) == stof(die_area->top_right_x_location))
    {
        power_pad->side = RIGHT;
        (*direction_power_pad)[RIGHT].push_back((*power_pad));
    }
    else if (stof(power_pad->y_location) == stof(die_area->top_right_y_location))
    {
        power_pad->side = UP;
        (*direction_power_pad)[UP].push_back((*power_pad));
    }
}

void getDieArea(string def_transfer_file_name, DieArea *die_area)
{
    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    if (def_transfer_file)
    {
        while (getline(def_transfer_file, def_content))
        {
            if (def_content.find("DIEAREA") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                die_area->lower_left_x_location = def_content_array[2];
                die_area->lower_left_y_location = def_content_array[3];
                die_area->top_right_x_location = def_content_array[6];
                die_area->top_right_y_location = def_content_array[7];
            }
        }
    }
}
void setPowerPadDirection(map<string, vector<PowerPad>> *direction_power_pad)
{
    vector<PowerPad> up_power_pad_vector;
    vector<PowerPad> down_power_pad_vector;
    vector<PowerPad> left_power_pad_vector;
    vector<PowerPad> right_power_pad_vector;
    (*direction_power_pad).insert(pair<string, vector<PowerPad>>(UP, up_power_pad_vector));
    (*direction_power_pad).insert(pair<string, vector<PowerPad>>(DOWN, down_power_pad_vector));
    (*direction_power_pad).insert(pair<string, vector<PowerPad>>(LEFT, left_power_pad_vector));
    (*direction_power_pad).insert(pair<string, vector<PowerPad>>(RIGHT, right_power_pad_vector));
}
void getPowerPadLocation(string def_transfer_file_name, vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector)
{

    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    if (def_transfer_file)
    {
        while (getline(def_transfer_file, def_content))
        {
            if (def_content.find("PINS") != string::npos)
            {
                while (getline(def_transfer_file, def_content))
                {
                    if (def_content.find("VDD") != string::npos || def_content.find("VSS") != string::npos)
                    {
                        PowerPad power_pad;
                        setPowerPadInfo(&def_transfer_file, def_content, &power_pad);
                        if (def_content.find("VDD") != string::npos)
                        {
                            (*vdd_power_pad_vector).push_back(power_pad);
                        }
                        else
                        {
                            (*vss_power_pad_vector).push_back(power_pad);
                        }
                    }
                    if (def_content.find("END PINS") != string::npos)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void setPowerPadInfo(ifstream *def_transfer_file, string def_content, PowerPad *power_pad)
{

    vector<string> def_content_array = splitByPattern(def_content, " ");
    (*power_pad).pad_name = def_content_array[1];
    (*power_pad).net_name = def_content_array[4];
    while (getline((*def_transfer_file), def_content))
    {
        if (def_content.find("LAYER") != string::npos)
        {
            vector<string> def_content_array = splitByPattern(def_content, " ");
            setPowerPadLengthtWidth(&def_content_array, &(*power_pad));
        }
        else if (def_content.find("PLACED") != string::npos)
        {
            vector<string> def_content_array = splitByPattern(def_content, " ");
            setPowerPadLocation(&def_content_array, &(*power_pad));
            break;
        }
        else
        {
            cout << "read pin error " << endl;
            break;
        }
    }
}
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad)
{
    string x_location = (*def_content_array)[3];
    string y_location = (*def_content_array)[4];
    (*power_pad).x_location = x_location;
    (*power_pad).y_location = y_location;
}
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad)
{
    string power_pad_left = (*def_content_array)[4];
    string power_pad_right = (*def_content_array)[8];
    float power_pad_left_float = abs(stof((*def_content_array)[4]));
    float power_pad_right_float = abs(stof((*def_content_array)[8]));
    (*power_pad).width = power_pad_left_float + power_pad_right_float;
    string power_pad_up = (*def_content_array)[5];
    string power_pad_down = (*def_content_array)[9];
    float power_pad_up_float = abs(stof((*def_content_array)[5]));
    float power_pad_down_float = abs(stof((*def_content_array)[9]));
    (*power_pad).length = power_pad_up_float + power_pad_down_float;
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

void setCurrentRange(CoreSite *core_site, vector<CurrentRange> *current_range_vector, float boundary)
{
    float start = stof((*core_site).left_x_location);
    float end = stof((*core_site).right_x_location);

    int boundary_int = (int)boundary;
    boundary = (float)boundary_int;
    cout << "check boundary :" << boundary << endl;

    for (float i = start; i < end; i += boundary)
    {
        CurrentRange current_range;
        current_range.left_x_location = i;
        // TODO 這裡要改過 =========

        if (i + boundary >= end)
        {
            current_range.right_x_location = end;
            current_range.range_total_power = 0;
            (*current_range_vector).push_back(current_range);
        }
        else
        {
            current_range.right_x_location = i + boundary;
            current_range.range_total_power = 0;
            (*current_range_vector).push_back(current_range);
        }
    }
    cout << "core site : " << start << "  " << end << endl;
    for (int i = 0; i < (*current_range_vector).size(); i++)
    {
        cout << (*current_range_vector)[i].left_x_location << " " << (*current_range_vector)[i].right_x_location << endl;
    }
}

//能夠進行優化 盡量減少跑的次數
void setIpPowerInStripe(vector<CurrentRange> *current_range_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{

    for (int i = 0; i < (*current_range_vector).size(); i++)
    {

        for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
        {
            string cell_id = iter->first;
            CellInstancePowerInfo cell_instance_power_info = iter->second;
            if (isInStripeRange(&((*current_range_vector)[i]), cell_id, &(*cell_placed_map), &(*cell_ip_map)))
            {
                (*current_range_vector)[i].ip_power_vector.push_back(cell_id);
            }
        }
    }
}

bool isInStripeRange(CurrentRange *current_range, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{
    // string move_range_x_left = (*vdd_stripe).move_range_x_left;
    // string move_range_x_right = (*vdd_stripe).move_range_x_right;
    string left_x_location = (*cell_placed_map)[cell_id].left_x_location;
    string right_x_location = (*cell_placed_map)[cell_id].right_x_location;

    float move_range_x_left_float = (*current_range).left_x_location;
    float move_range_x_right_float = (*current_range).right_x_location;

    float left_x_location_float = stof(left_x_location);
    float right_x_location_float = stof(right_x_location);

    // cout << "move_range_x_left_float : " << move_range_x_left_float << endl;
    // cout << "move_range_x_right_float : " << move_range_x_right_float << endl;
    // cout << "left_x_location_float : " << left_x_location_float << endl;
    // cout << "right_x_location_float : " << right_x_location_float << endl;

    // case 1 : middle of left stripe
    if ((left_x_location_float < move_range_x_left_float) && (right_x_location_float > move_range_x_left_float) && (left_x_location_float < move_range_x_right_float) && (right_x_location_float < move_range_x_right_float))
    {
        float ratio = getConsumeRatio(LEFT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*current_range).range_total_power += power;
        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        float power = (*cell_ip_map)[cell_id].instance_total_power;

        (*current_range).range_total_power += power;

        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        float ratio = getConsumeRatio(RIGHT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*current_range).range_total_power += power;
        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
        float ratio = getConsumeRatio(MIDDLE, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*current_range).range_total_power += power;
        // cout << " over stripe moving location " << endl;
        return true;
    }
    // case 4 : out side
    else
    {
        // cout << " out side " << endl;
        return false;
    }
}

float getConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float)
{
    float ratio;
    if (boudary == LEFT)
    {
        float width = right_x_location_float - left_x_location_float;
        float space = right_x_location_float - move_range_x_left_float;
        ratio = space / width;
    }
    else if (boudary == RIGHT)
    {
        float width = right_x_location_float - left_x_location_float;
        float space = move_range_x_right_float - left_x_location_float;
        ratio = space / width;
    }
    else
    {
        float width = right_x_location_float - left_x_location_float;
        float space = move_range_x_right_float - move_range_x_left_float;
        ratio = space / width;
    }
    return ratio;
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
        string cell_width = (*cell_info_map)[cell_name].cell_width;
        string cell_height = (*cell_info_map)[cell_name].cell_height;
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
            break;
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
                vector<string> lef_content_array = splitByPattern(lef_content, " ");
                string cell_name = lef_content_array[1];
                while (getline(lef_file, lef_content))
                {
                    if (lef_content.find("SIZE") != string::npos && lef_content.find("BY") != string::npos)
                    {
                        vector<string> lef_content_array = splitByPattern(lef_content, " ");
                        string cell_width = lef_content_array[1];
                        string cell_height = lef_content_array[3];
                        CellInfo cell;
                        cell.cell_name = cell_name;
                        cell.cell_height = cell_height;
                        cell.cell_width = cell_width;
                        (*cell_info_map).insert(pair<string, CellInfo>(cell_name, cell));
                        log++;
                        if (log % 100 == 0)
                        {
                            cout << "left cell data : " << log << endl;
                        }
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
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    ifstream ip_file(ip_report);
    string ip_content;
    int log = 0;
    float power = 0;
    if (ip_file)
    {
        while (getline(ip_file, ip_content))
        {
            if (ip_content.find("Cell") != string::npos && ip_content.find("Instance") != string::npos)
            {
                continue;
            }
            else if (ip_content.find("Range") != string::npos && ip_content.find("Total") != string::npos)
            {
                break;
            }
            else
            {
                vector<string> ip_content_array = splitByPattern(ip_content, " ");
                CellInstancePowerInfo cell_instance_power_info;
                cell_instance_power_info.instance_total_power = stof(ip_content_array[0]);
                power += cell_instance_power_info.instance_total_power;
                cell_instance_power_info.cell_name = ip_content_array[1];
                cell_instance_power_info.cell_id = ip_content_array[2];
                (*cell_ip_map).insert(pair<string, CellInstancePowerInfo>(cell_instance_power_info.cell_id, cell_instance_power_info));
                log++;
                if (log % 1000 == 0)
                {
                    cout << "ip data : " << log << endl;
                }
            }
        }
    }
    // cout << "total : " << power << endl;
    cout << " count power consuming of total ip report : " << power << endl;
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

void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map)
{

    for (int i = 0; i < (*vdd_shape_ring_vector).size(); i++)
    {
        if ((*vdd_shape_map).count((*vdd_shape_ring_vector)[i].side) != true)
        {
            (*vdd_shape_map).insert(pair<string, ShapeRing>((*vdd_shape_ring_vector)[i].side, (*vdd_shape_ring_vector)[i]));
        }
        else
        {
            cout << "power ring side has error " << endl;
        }
    }
    for (int i = 0; i < (*vss_shape_ring_vector).size(); i++)
    {
        if ((*vss_shape_map).count((*vss_shape_ring_vector)[i].side) != true)
        {
            (*vss_shape_map).insert(pair<string, ShapeRing>((*vss_shape_ring_vector)[i].side, (*vss_shape_ring_vector)[i]));
        }
        else
        {
            cout << "power ring side has error" << endl;
        }
    }
}

void getShapeRing(string DEF_TRANSFER_FILE_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector)
{
    ifstream DEF_TRANSFER_FILE(DEF_TRANSFER_FILE_name);
    string def_content;
    if (DEF_TRANSFER_FILE)
    {
        while (getline(DEF_TRANSFER_FILE, def_content))
        {
            if (def_content.find("( * VDD )") != string::npos)
            {
                while (getline(DEF_TRANSFER_FILE, def_content))
                {
                    if (def_content.find("SHAPE RING") != string::npos)
                    {

                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        ShapeRing shape_ring;
                        setShapeRingLocation(&shape_ring, &def_content_array, &(*vdd_shape_ring_vector));
                    }
                    if (def_content.find("SHAPE STRIPE") != string::npos || def_content.find("+ USE POWER") != string::npos)
                    {
                        break;
                    }
                }
            }
            else if (def_content.find("( * VSS )") != string::npos)
            {
                while (getline(DEF_TRANSFER_FILE, def_content))
                {
                    if (def_content.find("SHAPE RING") != string::npos)
                    {
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        ShapeRing shape_ring;
                        setShapeRingLocation(&shape_ring, &def_content_array, &(*vss_shape_ring_vector));
                    }
                    if (def_content.find("SHAPE STRIPE") != string::npos || def_content.find("+ USE GROUND") != string::npos)
                    {
                        break;
                    }
                }
            }
        }
    }
}
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area)
{
    float middle_x_line = stof(die_area->top_right_x_location) / 2;
    float middle_y_line = stof(die_area->top_right_y_location) / 2;
    setRingSide(&(*vdd_shape_ring_vector), middle_x_line, middle_y_line);
    setRingSide(&(*vss_shape_ring_vector), middle_x_line, middle_y_line);
}
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line)
{

    for (int i = 0; i < (*shape_ring_vector).size(); i++)
    {
        // UP OR DOWN
        if ((*shape_ring_vector)[i].start_y_location == (*shape_ring_vector)[i].end_y_location)
        {

            if (stof((*shape_ring_vector)[i].start_y_location) > middle_y_line)
            {
                (*shape_ring_vector)[i].side = UP;
            }
            else
            {
                (*shape_ring_vector)[i].side = DOWN;
            }
        }
        else if ((*shape_ring_vector)[i].start_x_location == (*shape_ring_vector)[i].end_x_location)
        {
            if (stof((*shape_ring_vector)[i].start_x_location) > middle_x_line)
            {
                (*shape_ring_vector)[i].side = RIGHT;
            }
            else
            {
                (*shape_ring_vector)[i].side = LEFT;
            }
        }
    }
}

void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector)
{
    if ((*def_content_array).size() == 15)
    {
        (*shape_ring).layer = (*def_content_array)[2];
        (*shape_ring).width = stof((*def_content_array)[3]);
        (*shape_ring).start_x_location = (*def_content_array)[8];
        (*shape_ring).start_y_location = (*def_content_array)[9];
        // y
        if ((*def_content_array)[13] == "*")
        {
            (*shape_ring).end_y_location = (*def_content_array)[9];
            (*shape_ring).end_x_location = (*def_content_array)[12];
        }
        else if ((*def_content_array)[12] == "*")
        {
            (*shape_ring).end_x_location = (*def_content_array)[8];
            (*shape_ring).end_y_location = (*def_content_array)[13];
        }
        else
        {
            (*shape_ring).end_x_location = (*def_content_array)[12];
            (*shape_ring).end_y_location = (*def_content_array)[13];
        }
        (*shape_ring_vector).push_back((*shape_ring));
    }
    else if ((*def_content_array).size() == 14)
    {

        (*shape_ring).layer = (*def_content_array)[1];
        (*shape_ring).width = stof((*def_content_array)[2]);
        (*shape_ring).start_x_location = (*def_content_array)[7];
        (*shape_ring).start_y_location = (*def_content_array)[8];
        if ((*def_content_array)[12] == "*")
        {
            (*shape_ring).end_x_location = (*def_content_array)[11];
            (*shape_ring).end_y_location = (*shape_ring).start_y_location;
        }
        else if ((*def_content_array)[11] == "*")
        {
            (*shape_ring).end_x_location = (*shape_ring).start_x_location;
            (*shape_ring).end_y_location = (*def_content_array)[12];
        }
        else
        {
            (*shape_ring).end_x_location = (*def_content_array)[11];
            (*shape_ring).end_y_location = (*def_content_array)[12];
        }
        (*shape_ring_vector).push_back((*shape_ring));
    }
}
string floatToString(const float value)
{
    std::ostringstream out;
    out.precision(6);

    out << value;
    return out.str();
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

// ================= 調整side start =========================
// direction_power_pad.erase(direction_power_pad.find(UP));
// direction_power_pad.erase(direction_power_pad.find(DOWN));
// direction_power_pad.erase(direction_power_pad.find(RIGHT));
// direction_power_pad.erase(direction_power_pad.find(LEFT));
// ================= 調整side end =========================
// direction_power_pad[UP].pop_back();
// direction_power_pad[DOWN].pop_back();
// direction_power_pad[RIGHT].pop_back();
// direction_power_pad[LEFT].pop_back();
// for (auto shape : vdd_shape_map)
// {
//     cout << "side : " << shape.second.side << " " << shape.second.width << "==========" << endl;
//     cout << shape.second.start_x_location << " " << shape.second.start_y_location << endl;
//     cout << shape.second.end_x_location << " " << shape.second.end_y_location << endl;
//     cout << "============================================" << endl;
// }
