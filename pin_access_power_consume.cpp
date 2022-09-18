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
const double esp = 1e-6;
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";

// struct PinAccessShape
// {
//     // lef information
//     string left_bottom_x_location;
//     string left_bottom_y_location;
//     string right_top_x_location;
//     string right_top_y_location;
//     string middle_x_location;
//     string middle_y_location;
// };

struct Rect
{
    string bottom_x_location;
    string bottom_y_location;
    string top_x_location;
    string top_y_location;
    // vector<PinAccess> pin_access_shape_vector;
};

struct PinShape
{
    string pin_name;
    // layer    rect
    map<string, vector<Rect>> rect_map;
};
struct PinAccessPoint
{
    string middle_x_location;
    string middle_y_location;
};
struct CellInfo
{
    string cell_name;
    string height;
    string width;
    // key:PinName value:PinShape
    map<string, PinShape> pin_shape_map;
    map<string, vector<PinAccessPoint>> odd_pin_access_point_map;
    map<string, vector<PinAccessPoint>> even_pin_access_point_map;
};
struct PinAccess
{
    // def information
    string left_bottom_x_location_def;
    string left_bottom_y_location_def;
    string right_top_x_location_def;
    string right_top_y_location_def;
    // string middle_x_location_def;
    // string middle_y_location_def;
    // bool isPinAccessPoint;
};

struct Pin
{
    string pin_name;
    map<string, vector<PinAccess>> layer_pin_access_map;
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
    map<string, Pin> pin_map;
    // key pin name value pin_access
    map<string, vector<PinAccessPoint>> pin_access_point_map;
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
struct Stripe
{
    string net_name;
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string move_range_x_left;
    string move_range_x_right;
    string width;
    string vdd_track_x_cost;
    string vss_track_x_cost;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};

struct TrackPoint
{
    string x_point;
    string y_point;
    vector<string> power_cell_id_vector;
    bool isRoutingTrack;
    float total_pin_access_power_consum_cost;
};
struct TrackInfo
{
    string layer;
    string start;
    string pitch;
};

struct TrackOfCell
{
    string start_x_location;
    string end_x_location;
    string start_y_location;
    string end_y_location;
    string layer;
    string pitch;
};

// main function
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string DEF_TRANSFER_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);

// sub function
void getSizeOfCell(string lef_content, CellInfo *cell_info);
void getPinShapeOfCell(ifstream *lef_file, string left_content, PinShape *pin_shape);
void getPortOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape);
void getLayerOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getPinAccessFromRect(Rect *rect);
void getPinAccessLocationFromDef(Rect *rect, CellPlacedInfo *cell_placed_info, string cell_width, string cell_height, PinAccess *pin_access);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site);
void setPinAccessPointFromTrack(TrackInfo *m2_track_info, TrackInfo *m3_track_info, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void setRoutingTrackPowerConsuming(string power_stripe_width, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void setRoutingTrackNumberOfPinAccess(string power_stripe_width, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info);
void getTrackOfCell(CellPlacedInfo *cell_placed_info, TrackOfCell *track_of_cell, TrackInfo *track_info);
void isOnTrackPin(map<string, vector<PinAccessPoint>> *pin_access_point_map, map<string, Pin> *pin_map, string track_point_x, string track_point_y);
void isOnTrackPinNew(map<string, PinShape> *pin_shape_map, map<string, vector<PinAccessPoint>> *pin_access_point_map, string track_point_x, string track_point_y);
void getTrackPoint(TrackOfCell *m2_track_of_cell, TrackOfCell *m3_track_of_cell, map<string, Pin> *pin_map, map<string, vector<PinAccessPoint>> *pin_access_point_map);
void getAddStripeCost(unordered_map<string, TrackPoint> *m3_track_point_map, vector<Stripe> *stripe_vector, TrackInfo *track_info);
void getLeftCellPinAccessPoint(unordered_map<string, CellInfo> *cell_info_map);
void setCellPinAccessPoint(CellInfo *cell_info);
void getStripeRangeCost(Stripe *stripe, unordered_map<string, TrackPoint> *m3_track_point_map, float track_pitch);
void generateAddStripeTcl(vector<Stripe> *stripe_vector, string move_stripe_name, string power_stripe_width);
void transferPinAccessLocationFromDef(CellPlacedInfo *cell_placed_info, PinAccessPoint *pin_access_point, PinAccessPoint *pin_access_point_def, string cell_width, string cell_height);
float getPinAccessPointOfPlaced(float power_stripe_width_float, TrackPoint *m3_track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info);
//??¤æ??
bool isVertical(Rect *rect);
bool isOddTrackCell(string x_location, TrackInfo *m3_track_info);
bool isPinAccessNameAndLayer(string layer_name, string pin_name);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
bool isInTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width);
// bool isCoverPinAccess(PinAccessPoint *pin_access_point, TrackPoint *track_point, float power_stripe_width);
bool isCoverPinAccess(float pin_access_point_x_def_float, float track_point_x_float, float power_stripe_width);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool floatIsEqual(float a, float b);
// Util
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
string floatToString(const float value);
int stringToInt(string num);
float convertInnovusPoint(int number);
int main()
{
    string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
    string DEF_TRANSFER_FILE = "def_file/gpu_nerualNetwork_ricsv_aes_afterRouting/6t49_aes_routing_run0_transfer.def";
    string IP_REPORT_FILE = "power_report/activity_0.2/print_ip_riscv.report";
    string ADD_STRIPE_TCL = "stripe_tcl/addStripe_6t49_riscv_routing_pin_access.tcl";
    string v1_width = "0.072";
    string m2_y_pitch = "0.072";
    string m2_y_start = "1.44";
    string m3_x_pitch = "0.072";
    string m3_x_start = "0.18";
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, TrackPoint> m3_track_point_map;
    unordered_map<string, TrackPoint> m2_track_point_map;
    TrackInfo m3_track_info;
    TrackInfo m2_track_info;
    m3_track_info.layer = "M3";
    m3_track_info.start = m3_x_start;
    m3_track_info.pitch = m3_x_pitch;
    m2_track_info.layer = "M2";
    m2_track_info.start = m2_y_start;
    m2_track_info.pitch = m2_y_pitch;

    double START, END;
    START = clock();

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    setRoutingTrackPoint(&m2_track_point_map, &m3_track_point_map, m3_x_start, m3_x_pitch, m2_y_start, m2_y_pitch, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getLeftCellPinAccessPoint(&cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);
    // setPinAccessPointFromTrack(&m2_track_info, &m3_track_info, &cell_placed_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    setStripeRange(&vdd_stripe_vector, &vss_stripe_vector, &core_site);

    setRoutingTrackPowerConsuming("0.224", &m3_track_point_map, &cell_placed_map);
    // setRoutingTrackNumberOfPinAccess("0.224", &m3_track_point_map, &cell_placed_map, &cell_ip_map, &cell_info_map, &m3_track_info);

    // getAddStripeCost(&m3_track_point_map, &vdd_stripe_vector, &m3_track_info);
    // generateAddStripeTcl(&vdd_stripe_vector,ADD_STRIPE_TCL, "0.224");
    cout << endl
         << "µ{¦¡°õ¦æ©Òªá¶O¡G" << (double)clock() / CLOCKS_PER_SEC << " S";
    cout << endl
         << "¶i¦æ¹Bºâ©Òªá¶Oªº®É¶¡¡G" << (END - START) / CLOCKS_PER_SEC << " S" << endl;

    // for (auto cellInfo : cell_info_map)
    // {
    //     cout << "cell name : " << cellInfo.first << endl;
    //     map<string, vector<PinAccessPoint>> pin_access_map = cellInfo.second.even_pin_access_point_map;
    //     for (auto pin_access : pin_access_map)
    //     {
    //         vector<PinAccessPoint> pin_access_vector = pin_access.second;
    //         for (int i = 0; i < pin_access_vector.size(); i++)
    //         {
    //             if (cellInfo.first == "A2O1A1Ixp33_ASAP7_6t_fix")
    //             {
    //                 cout << "(" << floatToString(stof(pin_access_vector[i].middle_x_location_def) + 561.96) << " " << floatToString(stof(pin_access_vector[i].middle_y_location_def) + 78.336) << ")" << endl;
    //             }
    //         }
    //     }
    // }

    // for (auto cell_place : cell_placed_map)
    // {
    //     cout << "cell id :" << cell_place.first << endl;

    //     cout << cell_place.second.left_x_location << " " << cell_place.second.down_y_location << " " << cell_place.second.right_x_location << " " << cell_place.second.up_y_location << endl;
    // }

    return 0;
}

void getLeftCellPinAccessPoint(unordered_map<string, CellInfo> *cell_info_map)
{
    cout << "========== getLeftCellPinAccessPoint start ==========" << endl;
    for (auto cell_info_iter = (*cell_info_map).begin(); cell_info_iter != (*cell_info_map).end(); ++cell_info_iter)
    {

        setCellPinAccessPoint(&(cell_info_iter->second));
    }
    cout << "========== getLeftCellPinAccessPoint end ==========" << endl;
}

void setCellPinAccessPoint(CellInfo *cell_info)
{
    float odd_start_m3_track_float = 0.036;
    float even_start_m3_track_float = 0.108;
    float end_m3_track_float = stof((*cell_info).width);
    float m3_pitch_track = 0.072;
    map<string, vector<PinAccessPoint>> odd_pin_access_point_map;
    map<string, vector<PinAccessPoint>> even_pin_access_point_map;
    (*cell_info).odd_pin_access_point_map = odd_pin_access_point_map;
    (*cell_info).even_pin_access_point_map = even_pin_access_point_map;

    float start_m2_track_float = 0.0;
    float end_m2_track_float = stof((*cell_info).height);
    float m2_pitch_track = 0.072;

    // odd
    for (float i = odd_start_m3_track_float; floatIsEqualOrLess(i, end_m3_track_float); i += m3_pitch_track)
    {
        for (float j = start_m2_track_float; floatIsEqualOrLess(j, end_m2_track_float); j += m2_pitch_track)
        {
            isOnTrackPinNew(&((*cell_info).pin_shape_map), &((*cell_info).odd_pin_access_point_map), floatToString(i), floatToString(j));
        }
    }

    for (float i = even_start_m3_track_float; floatIsEqualOrLess(i, end_m3_track_float); i += m3_pitch_track)
    {
        for (float j = start_m2_track_float; floatIsEqualOrLess(j, end_m2_track_float); j += m2_pitch_track)
        {
            isOnTrackPinNew(&((*cell_info).pin_shape_map), &((*cell_info).even_pin_access_point_map), floatToString(i), floatToString(j));
        }
    }
}

void isOnTrackPinNew(map<string, PinShape> *pin_shape_map, map<string, vector<PinAccessPoint>> *pin_access_point_map, string track_point_x, string track_point_y)
{
    float track_point_x_float_right = stof(track_point_x) + 0.036;
    float track_point_x_float_left = stof(track_point_x) - 0.036;
    float track_point_y_float_top = stof(track_point_y) + 0.036;
    float track_point_y_float_bottom = stof(track_point_y) - 0.036;

    bool is_ontrack_pin = false;
    string pin_name;
    for (auto pin_shape_iter = (*pin_shape_map).begin(); pin_shape_iter != (*pin_shape_map).end(); ++pin_shape_iter)
    {
        pin_name = pin_shape_iter->first;
        // map<string, vector<PinAccess>> layer_pin_access_map = pin_iter->second.layer_pin_access_map;
        for (auto rect_iter = pin_shape_iter->second.rect_map.begin(); rect_iter != pin_shape_iter->second.rect_map.end(); ++rect_iter)
        {
            string layer_name = rect_iter->first;
            if (layer_name == "M1")
            {
                // vector<PinAccess> pin_access_vector = layer_pin_access_iter->second;

                for (int i = 0; i < rect_iter->second.size(); i++)
                {

                    float rect_bottom_x_float = stof((rect_iter->second)[i].bottom_x_location);
                    float rect_bottom_y_float = stof((rect_iter->second)[i].bottom_y_location);
                    float rect_top_x_float = stof((rect_iter->second)[i].top_x_location);
                    float rect_top_y_float = stof((rect_iter->second)[i].top_y_location);
                    // on vertical shape
                    if (floatIsEqual(rect_bottom_x_float, track_point_x_float_left) && floatIsEqual(rect_top_x_float, track_point_x_float_right) && floatIsEqualOrMore(track_point_y_float_bottom, rect_bottom_y_float) && floatIsEqualOrLess(track_point_y_float_top, rect_top_y_float))
                    {
                        is_ontrack_pin = true;
                    }
                    // on horizontal shape
                    else if (floatIsEqual(rect_bottom_y_float, track_point_y_float_bottom) && floatIsEqual(rect_top_y_float, track_point_y_float_top) && floatIsEqualOrMore(track_point_x_float_left, rect_bottom_x_float) && floatIsEqualOrLess(track_point_x_float_right, rect_top_x_float))
                    {
                        is_ontrack_pin = true;
                    }
                    if (is_ontrack_pin)
                    {
                        break;
                    }
                }
            }
            if (is_ontrack_pin)
            {
                break;
            }
        }
        if (is_ontrack_pin)
        {
            break;
        }
    }

    if (is_ontrack_pin)
    {
        PinAccessPoint pin_access_point;
        pin_access_point.middle_x_location = track_point_x;
        pin_access_point.middle_y_location = track_point_y;
        if ((*pin_access_point_map).count(pin_name))
        {
            (*pin_access_point_map)[pin_name].push_back(pin_access_point);
        }
        else
        {
            vector<PinAccessPoint> pin_access_point_vector;
            pin_access_point_vector.push_back(pin_access_point);
            (*pin_access_point_map).insert(pair<string, vector<PinAccessPoint>>(pin_name, pin_access_point_vector));
        }
    }
}

void getAddStripeCost(unordered_map<string, TrackPoint> *m3_track_point_map, vector<Stripe> *stripe_vector, TrackInfo *track_info)
{
    cout << "========== getAddStripeCost start ==========" << endl;
    float track_start = stof((*track_info).start);
    float track_pitch = stof((*track_info).pitch) * 2;

    // step1 : è¨?ç®? moving range track
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        // cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << " " << endl;

        float move_range_x_left_float = stof((*stripe_vector)[i].move_range_x_left);
        float move_range_x_right_float = stof((*stripe_vector)[i].move_range_x_right);

        float move_range_left_x_float_temp = move_range_x_left_float - track_start;
        float temp_pitch_start = move_range_x_left_float / track_pitch;
        int temp_pitch_start_int = (int)temp_pitch_start;
        float start_x = (temp_pitch_start_int * track_pitch) + track_start;

        float move_range_x_right_float_temp = move_range_x_right_float - track_start;
        float temp_pitch_end = move_range_x_right_float_temp / track_pitch;
        int temp_pitch_end_int = (int)temp_pitch_end;

        temp_pitch_end_int -= 1;
        float end_x = (temp_pitch_end_int * track_pitch) + track_start;

        (*stripe_vector)[i].move_range_x_left = floatToString(start_x);
        (*stripe_vector)[i].move_range_x_right = floatToString(end_x);
    }
    // step2 :
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        getStripeRangeCost(&((*stripe_vector)[i]), &(*m3_track_point_map), track_pitch);
    }
    cout << "========== getAddStripeCost end ==========" << endl;
}

void getStripeRangeCost(Stripe *stripe, unordered_map<string, TrackPoint> *m3_track_point_map, float track_pitch)
{
    float move_range_left = stof((*stripe).move_range_x_left);
    float move_range_right = stof((*stripe).move_range_x_right);
    float vdd_stripe_cost = 100000000000000000000000.0;
    float vss_stripe_cost = 100000000000000000000000.0;
    string vdd_track = 0;
    string vss_track = 0;
    map<string, float> track_map;
    for (float i = move_range_left; i <= move_range_right; i += track_pitch)
    {
        string i_str = floatToString(i);
        if ((*m3_track_point_map).count(i_str))
        {
            if ((*m3_track_point_map)[i_str].total_pin_access_power_consum_cost < vdd_stripe_cost)
            {
                vdd_stripe_cost = (*m3_track_point_map)[i_str].total_pin_access_power_consum_cost;
                vdd_track = i_str;
            }
        }
        else
        {
            cout << " getStripeRangeCost has error " << endl;
        }
    }
    (*stripe).vdd_track_x_cost = vdd_track;

    // ====== TODO ¥ª¥k¨â±ø¤£¨Ï¥Î========
    float vdd_track_float = stof(vdd_track);
    float left_track_float = vdd_track_float - track_pitch;
    float right_track_float = vdd_track_float + track_pitch;
    string left_track = floatToString(left_track_float);
    string right_track = floatToString(right_track_float);
    track_map.insert(pair<string, float>(left_track, left_track_float));
    track_map.insert(pair<string, float>(right_track, right_track_float));
    // ======  ========

    for (float i = move_range_left; i <= move_range_right; i += track_pitch)
    {
        string i_str = floatToString(i);
        if ((*m3_track_point_map).count(i_str))
        {
            if ((*m3_track_point_map)[i_str].total_pin_access_power_consum_cost < vss_stripe_cost && !(track_map).count(i_str))
            {
                vss_stripe_cost = (*m3_track_point_map)[i_str].total_pin_access_power_consum_cost;
                vss_track = i_str;
            }
        }
        else
        {
            cout << " getStripeRangeCost has error " << endl;
        }
    }

    (*stripe).vdd_track_x_cost = vdd_track;
    (*stripe).vss_track_x_cost = vss_track;
}

void generateAddStripeTcl(vector<Stripe> *stripe_vector, string move_stripe_name, string power_stripe_width)
{
    ofstream myfile;
    myfile.open(move_stripe_name);
    float power_stripe_width_float = stof(power_stripe_width);

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        float vdd_track_cost = stof((*stripe_vector)[i].vdd_track_x_cost);
        float vdd_left_track_cost = vdd_track_cost - (power_stripe_width_float / 2);
        float vdd_right_track_cost = vdd_track_cost + (power_stripe_width_float / 2);

        float vss_track_x_cost = stof((*stripe_vector)[i].vss_track_x_cost);
        float vss_left_track_cost = vss_track_x_cost - (power_stripe_width_float / 2);
        float vss_right_track_cost = vss_track_x_cost + (power_stripe_width_float / 2);

        myfile << "addStripe -nets { VDDX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_left_track_cost << " " << (*stripe_vector)[i].start_y_location << " " << vdd_right_track_cost << " " << (*stripe_vector)[i].end_y_location << " }" << endl;
        myfile << "addStripe -nets { VSSX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_left_track_cost << " " << (*stripe_vector)[i].start_y_location << " " << vss_right_track_cost << " " << (*stripe_vector)[i].end_y_location << " }" << endl;
    }
    myfile.close();
};

void setPinAccessPointFromTrack(TrackInfo *m2_track_info, TrackInfo *m3_track_info, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    int log = 0;
    for (auto cell_placed_iter = (*cell_placed_map).begin(); cell_placed_iter != (*cell_placed_map).end(); ++cell_placed_iter)
    {
        map<string, Pin> pin_map = cell_placed_iter->second.pin_map;
        string cell_id = cell_placed_iter->first;
        map<string, vector<PinAccessPoint>> pin_access_point_map;
        TrackOfCell m2_track_of_cell;
        TrackOfCell m3_track_of_cell;
        // getTrackOfCell(&((*cell_placed_map)[cell_id]), &m2_track_of_cell, &(*m2_track_info));
        // getTrackOfCell(&((*cell_placed_map)[cell_id]), &m3_track_of_cell, &(*m3_track_info));
        // cout << "cell_id : " << cell_id << endl;

        getTrackPoint(&m2_track_of_cell, &m3_track_of_cell, &pin_map, &pin_access_point_map);

        // cout << "check size pin_access_point_vector : " << pin_access_point_vector.size() << endl;
        (*cell_placed_map)[cell_id].pin_access_point_map = pin_access_point_map;
        log++;
        if (log % 10000 == 0)
        {
            cout << "pinAccessPointFromTrack data : " << log << endl;
        }
    }
}

void getTrackPoint(TrackOfCell *m2_track_of_cell, TrackOfCell *m3_track_of_cell, map<string, Pin> *pin_map, map<string, vector<PinAccessPoint>> *pin_access_point_map)
{

    float start_m3_track_float = stof((*m3_track_of_cell).start_x_location);
    float end_m3_track_float = stof((*m3_track_of_cell).end_x_location);
    float m3_pitch_track = stof((*m3_track_of_cell).pitch);

    float start_m2_track_float = stof((*m2_track_of_cell).start_y_location);
    float end_m2_track_float = stof((*m2_track_of_cell).end_y_location);
    float m2_pitch_track = stof((*m2_track_of_cell).pitch);

    for (float i = start_m3_track_float; floatIsEqualOrLess(i, end_m3_track_float); i += m3_pitch_track)
    {
        for (float j = start_m2_track_float; floatIsEqualOrLess(j, end_m2_track_float); j += m2_pitch_track)
        {

            // TrackPoint track_point;
            // track_point.x_point = floatToString(i);
            // track_point.y_point = floatToString(j);
            // cout << "is onTrackPin start " << endl;
            isOnTrackPin(&(*pin_access_point_map), &(*pin_map), floatToString(i), floatToString(j));
            // cout << "is onTrackPin end " << endl;
        }
    }
}

void isOnTrackPin(map<string, vector<PinAccessPoint>> *pin_access_point_map, map<string, Pin> *pin_map, string track_point_x, string track_point_y)
{
    float track_point_x_float_right = stof(track_point_x) + 0.036;
    float track_point_x_float_left = stof(track_point_x) - 0.036;
    float track_point_y_float_top = stof(track_point_y) + 0.036;
    float track_point_y_float_bottom = stof(track_point_y) - 0.036;
    bool is_ontrack_pin = false;
    string pin_name;

    for (auto pin_iter = (*pin_map).begin(); pin_iter != (*pin_map).end(); ++pin_iter)
    {
        pin_name = pin_iter->first;
        // map<string, vector<PinAccess>> layer_pin_access_map = pin_iter->second.layer_pin_access_map;
        for (auto layer_pin_access_iter = pin_iter->second.layer_pin_access_map.begin(); layer_pin_access_iter != pin_iter->second.layer_pin_access_map.end(); ++layer_pin_access_iter)
        {
            string layer_name = layer_pin_access_iter->first;
            if (layer_name == "M1")
            {
                // vector<PinAccess> pin_access_vector = layer_pin_access_iter->second;

                for (int i = 0; i < layer_pin_access_iter->second.size(); i++)
                {
                    PinAccess pin_access = layer_pin_access_iter->second[i];
                    float left_bottom_x_location_def_float = stof(pin_access.left_bottom_x_location_def);
                    float left_bottom_y_location_def_float = stof(pin_access.left_bottom_y_location_def);
                    float right_top_x_location_def_float = stof(pin_access.right_top_x_location_def);
                    float right_top_y_location_def_float = stof(pin_access.right_top_y_location_def);
                    // on vertical shape
                    if (floatIsEqual(left_bottom_x_location_def_float, track_point_x_float_left) && floatIsEqual(right_top_x_location_def_float, track_point_x_float_right) && floatIsEqualOrMore(track_point_y_float_bottom, left_bottom_y_location_def_float) && floatIsEqualOrLess(track_point_y_float_top, right_top_y_location_def_float))
                    {
                        is_ontrack_pin = true;
                    }
                    // on horizontal shape
                    else if (floatIsEqual(left_bottom_y_location_def_float, track_point_y_float_bottom) && floatIsEqual(right_top_y_location_def_float, track_point_y_float_top) && floatIsEqualOrMore(track_point_x_float_left, left_bottom_x_location_def_float) && floatIsEqualOrLess(track_point_x_float_right, right_top_x_location_def_float))
                    {
                        is_ontrack_pin = true;
                    }
                    if (is_ontrack_pin)
                    {
                        break;
                    }
                }
            }
            if (is_ontrack_pin)
            {
                break;
            }
        }
        if (is_ontrack_pin)
        {
            break;
        }
    }

    if (is_ontrack_pin)
    {
        PinAccessPoint pin_access_point;
        pin_access_point.middle_x_location = track_point_x;
        pin_access_point.middle_y_location = track_point_y;
        if ((*pin_access_point_map).count(pin_name))
        {
            (*pin_access_point_map)[pin_name].push_back(pin_access_point);
        }
        else
        {
            vector<PinAccessPoint> pin_access_point_vector;
            pin_access_point_vector.push_back(pin_access_point);
            (*pin_access_point_map).insert(pair<string, vector<PinAccessPoint>>(pin_name, pin_access_point_vector));
        }
    }
}

void getTrackOfCell(CellPlacedInfo *cell_placed_info, TrackOfCell *track_of_cell, TrackInfo *track_info)
{
    float cell_placed_left_x_float = stof((*cell_placed_info).left_x_location);
    float cell_placed_down_y_float = stof((*cell_placed_info).down_y_location);

    float cell_placed_right_x_float = stof((*cell_placed_info).right_x_location);
    float cell_placed_up_y_float = stof((*cell_placed_info).up_y_location);

    // int track_start_x = ( stof((*track_of_cell).start_x_location) * 1000)/4;
    float track_start = stof((*track_info).start);
    float track_pitch = stof((*track_info).pitch);

    string layer = (*track_info).layer;

    if (layer == "M2")
    {
        // cout << (*cell_placed_info).left_x_location << " " << (*cell_placed_info).down_y_location << endl;
        float cell_placed_down_y_float_temp = cell_placed_down_y_float - track_start;
        // cout << cell_placed_down_y_float_temp << endl;
        int temp_pitch_start = cell_placed_down_y_float_temp / track_pitch;
        // cout << "temp_pitch_start : " << temp_pitch_start << endl;
        float start_y = (temp_pitch_start * track_pitch) + track_start;

        float cell_placed_down_up_float_temp = cell_placed_up_y_float - track_start;
        int temp_pitch_end = cell_placed_down_up_float_temp / track_pitch;
        // cout << "temp_pitch_end : " << temp_pitch_end << endl;
        temp_pitch_end += 1;
        float end_y = (temp_pitch_end * track_pitch) + track_start;
        // cout << start_y << " " << end_y << endl;

        (*track_of_cell).start_y_location = floatToString(start_y);
        (*track_of_cell).end_y_location = floatToString(end_y);
        (*track_of_cell).pitch = floatToString(track_pitch);
    }
    else if (layer == "M3")
    {
        // cout << (*cell_placed_info).left_x_location << " " << (*cell_placed_info).down_y_location << endl;
        // cout << cell_placed_left_x_int << " " << cell_placed_down_y_int << " " << cell_placed_right_x_int << " " << cell_placed_up_y_int << endl;

        float cell_placed_left_x_float_temp = cell_placed_left_x_float - track_start;
        // cout << cell_placed_left_x_float_temp << endl;
        int temp_pitch_start = cell_placed_left_x_float_temp / track_pitch;
        // cout << "temp_pitch_start : " << temp_pitch_start << endl;
        float start_x = (temp_pitch_start * track_pitch) + track_start;

        float cell_placed_right_x_float_temp = cell_placed_right_x_float - track_start;
        int temp_pitch_end = cell_placed_right_x_float_temp / track_pitch;
        // cout << "temp_pitch_end : " << temp_pitch_end << endl;
        temp_pitch_end += 1;
        float end_x = (temp_pitch_end * track_pitch) + track_start;
        // cout << start_x << " " << end_x << endl;
        (*track_of_cell).start_x_location = floatToString(start_x);
        (*track_of_cell).end_x_location = floatToString(end_x);
        (*track_of_cell).pitch = floatToString(track_pitch);
    }
    else
    {
        cout << " has error " << endl;
    }
}

//æ¯?æ¢?routint track ?????¨å??å°?pin access point --- 1
void setRoutingTrackNumberOfPinAccess(string power_stripe_width, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info)
{
    cout << "========== setRoutingTrackNumberOfPinAccess start ==========" << endl;
    float power_stripe_width_float = stof(power_stripe_width);
    for (auto m3_track_point_iter = (*m3_track_point_map).begin(); m3_track_point_iter != (*m3_track_point_map).end(); ++m3_track_point_iter)
    {
        TrackPoint track_point = m3_track_point_iter->second;
        if (track_point.isRoutingTrack)
        {
            float total_cell_pin_access_power_consume_cost = 0;
            for (int i = 0; i < track_point.power_cell_id_vector.size(); i++)
            {
                string cell_id = track_point.power_cell_id_vector[i];
                CellPlacedInfo cell_placed_info = (*cell_placed_map)[cell_id];

                // cost function
                // ä¸????routing track ??¨é?????cell ??? pin_access_cost
                float cell_pin_access_cost = getPinAccessPointOfPlaced(power_stripe_width_float, &(m3_track_point_iter->second), &cell_placed_info, &(*cell_ip_map), &(*cell_info_map), &(*m3_track_info));
                float power_consuming_cost = ((*cell_ip_map)[cell_id].instance_total_power * 1000000);
                power_consuming_cost = 1 / power_consuming_cost;

                float pin_access_power_consuming_cost = cell_pin_access_cost * power_consuming_cost;
                total_cell_pin_access_power_consume_cost += pin_access_power_consuming_cost;
            }

            m3_track_point_iter->second.total_pin_access_power_consum_cost = total_cell_pin_access_power_consume_cost;
        }
    }
    cout << "========== setRoutingTrackNumberOfPinAccess end ==========" << endl;
}

// new
float getPinAccessPointOfPlaced(float power_stripe_width_float, TrackPoint *m3_track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info)
{

    float m3_track_point_left = stof((*m3_track_point).x_point) - (power_stripe_width_float / 2);
    float m3_track_point_right = stof((*m3_track_point).x_point) + (power_stripe_width_float / 2);
    float m3_track_point_middle_float = stof((*m3_track_point).x_point);
    string cell_placed_left_x_location = (*cell_placed_info).left_x_location;
    string cell_placed_right_x_location = (*cell_placed_info).right_x_location;

    string cell_name = (*cell_placed_info).cell_name;
    string direction = (*cell_placed_info).direction;
    string cell_width = (*cell_info_map)[cell_name].width;
    string cell_height = (*cell_info_map)[cell_name].height;
    float total_pin_access_cost = 0;

    // cout << (*cell_placed_info).cell_id << "    " << (*cell_placed_info).left_x_location << " " << (*cell_placed_info).down_y_location << " " << (*cell_placed_info).right_x_location << " " << (*cell_placed_info).up_y_location << endl;
    // cout << " track point " << (*m3_track_point).x_point << endl;

    if (isOddTrackCell(cell_placed_left_x_location, &(*m3_track_info)))
    {

        for (auto odd_pin_access_point_iter = ((*cell_info_map)[cell_name].odd_pin_access_point_map).begin(); odd_pin_access_point_iter != ((*cell_info_map)[cell_name].odd_pin_access_point_map).end(); ++odd_pin_access_point_iter)
        {
            int pin_access_point = 0;
            for (int i = 0; i < (odd_pin_access_point_iter->second).size(); i++)
            {
                //???pin è½? def
                PinAccessPoint pin_access_point_def;
                transferPinAccessLocationFromDef(&(*cell_placed_info), &(odd_pin_access_point_iter->second)[i], &pin_access_point_def, cell_width, cell_height);
                float pin_access_point_x_def_float = stof(pin_access_point_def.middle_x_location);
                //ä¹?å¾???¨å?¤æ??
                if (isCoverPinAccess(pin_access_point_x_def_float, m3_track_point_middle_float, power_stripe_width_float))
                {
                    pin_access_point += 1;
                }
            }
            float pin_access_cost = (float)pin_access_point / (odd_pin_access_point_iter->second).size();
            // cout << "pin name : " << odd_pin_access_point_iter->first << endl;
            // cout << "pin_access_point : " << pin_access_point << " " << (odd_pin_access_point_iter->second).size() << endl;
            // cout << "pin cost : " << pin_access_cost << endl;
            total_pin_access_cost += pin_access_cost;
        }
        // cout << "total_pin_access_cost :" << total_pin_access_cost << endl;
    }
    else
    {

        for (auto even_pin_access_point_iter = ((*cell_info_map)[cell_name].even_pin_access_point_map).begin(); even_pin_access_point_iter != ((*cell_info_map)[cell_name].even_pin_access_point_map).end(); ++even_pin_access_point_iter)
        {

            int pin_access_point = 0;
            for (int i = 0; i < (even_pin_access_point_iter->second).size(); i++)
            {
                //???pin è½? def
                PinAccessPoint pin_access_point_def;
                transferPinAccessLocationFromDef(&(*cell_placed_info), &(even_pin_access_point_iter->second)[i], &pin_access_point_def, cell_width, cell_height);
                float pin_access_point_x_def_float = stof(pin_access_point_def.middle_x_location);
                if (isCoverPinAccess(pin_access_point_x_def_float, m3_track_point_middle_float, power_stripe_width_float))
                {
                    pin_access_point += 1;
                }
                //ä¹?å¾???¨å?¤æ??
            }
            float pin_access_cost = (float)pin_access_point / (even_pin_access_point_iter->second).size();
            total_pin_access_cost += pin_access_cost;
            // cout << "pin name : " << odd_pin_access_point_iter->first << endl;
            // cout << "pin_access_point : " << pin_access_point << " " << (odd_pin_access_point_iter->second).size() << endl;
            // cout << "pin cost : " << pin_access_cost << endl;
        }
    }
    return total_pin_access_cost;
}

bool isCoverPinAccess(float pin_access_point_x_def_float, float track_point_x_float, float power_stripe_width)
{

    float track_point_x_left = track_point_x_float - (power_stripe_width / 2);
    float track_point_x_right = track_point_x_float + (power_stripe_width / 2);

    float pin_access_x_left = pin_access_point_x_def_float - (0.072 / 2);
    float pin_access_x_right = pin_access_point_x_def_float + (0.072 / 2);

    if (track_point_x_left < pin_access_x_left && track_point_x_right < pin_access_x_left)
    {
        return false;
    }
    else if (track_point_x_left > pin_access_x_left && track_point_x_right > pin_access_x_right)
    {
        return false;
    }
    else
    {
        return true;
    }
};

void transferPinAccessLocationFromDef(CellPlacedInfo *cell_placed_info, PinAccessPoint *pin_access_point, PinAccessPoint *pin_access_point_def, string cell_width, string cell_height)
{
    float cell_place_left_x = stof((*cell_placed_info).left_x_location);
    float cell_place_down_y = stof((*cell_placed_info).down_y_location);

    float cell_width_float = stof(cell_width);
    float cell_height_float = stof(cell_height);

    float pin_access_x_point = stof((*pin_access_point).middle_x_location);
    float pin_access_y_point = stof((*pin_access_point).middle_y_location);

    if ((*cell_placed_info).direction == "N")
    {
        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + pin_access_x_point);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + pin_access_y_point);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "S")
    {

        float temp_middle_x_float = abs(cell_width_float - pin_access_x_point);
        float temp_middle_y_float = abs(cell_height_float - pin_access_y_point);

        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + temp_middle_x_float);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + temp_middle_y_float);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FN")
    {

        float temp_middle_x_float = abs(cell_width_float - pin_access_x_point);

        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + temp_middle_x_float);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + pin_access_y_point);

        //  cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FS")
    {

        float temp_middle_y_float = abs(cell_height_float - pin_access_y_point);

        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + pin_access_x_point);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + temp_middle_y_float);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else
    {
        cout << " getPinAccessLocationFromDef error " << endl;
    }
};
// pin shape è½???? def
void getPinAccessLocationFromDef(Rect *rect, CellPlacedInfo *cell_placed_info, string cell_width, string cell_height, PinAccess *pin_access)
{
    float cell_place_left_x = stof((*cell_placed_info).left_x_location);
    float cell_place_down_y = stof((*cell_placed_info).down_y_location);

    float cell_width_float = stof(cell_width);
    float cell_height_float = stof(cell_height);

    float bottom_x_location_float = stof((*rect).bottom_x_location);
    float bottom_y_location_float = stof((*rect).bottom_y_location);

    float top_x_location_float = stof((*rect).top_x_location);
    float top_y_location_float = stof((*rect).top_y_location);

    if ((*cell_placed_info).direction == "N")
    {

        (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + bottom_x_location_float);
        (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + bottom_y_location_float);
        (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + top_x_location_float);
        (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + top_y_location_float);

        // cout << "pin : (" << (*pin_access).left_bottom_x_location_def << " " << (*pin_access).left_bottom_y_location_def << " " << (*pin_access).right_top_x_location_def << " " << (*pin_access).right_top_y_location_def << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "S")
    {

        float temp_left_bottom_x_location_float = top_x_location_float;
        float temp_left_bottom_y_location_float = top_y_location_float;
        float temp_right_top_x_location_float = bottom_x_location_float;
        float temp_right_top_y_location_float = bottom_y_location_float;

        temp_left_bottom_x_location_float = abs(cell_width_float - temp_left_bottom_x_location_float);
        temp_left_bottom_y_location_float = abs(cell_height_float - temp_left_bottom_y_location_float);

        temp_right_top_x_location_float = abs(cell_width_float - temp_right_top_x_location_float);
        temp_right_top_y_location_float = abs(cell_height_float - temp_right_top_y_location_float);

        (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + temp_left_bottom_x_location_float);
        (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + temp_left_bottom_y_location_float);

        (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + temp_right_top_x_location_float);
        (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + temp_right_top_y_location_float);

        // cout << "pin : (" << (*pin_access).left_bottom_x_location_def << " " << (*pin_access).left_bottom_y_location_def << " " << (*pin_access).right_top_x_location_def << " " << (*pin_access).right_top_y_location_def << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FN")
    {
        float temp_left_bottom_x_location_float = top_x_location_float;
        float temp_left_bottom_y_location_float = bottom_y_location_float;

        float temp_right_top_x_location_float = bottom_x_location_float;
        float temp_right_top_y_location_float = top_y_location_float;

        temp_left_bottom_x_location_float = abs(cell_width_float - temp_left_bottom_x_location_float);
        temp_right_top_x_location_float = abs(cell_width_float - temp_right_top_x_location_float);

        (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + temp_left_bottom_x_location_float);
        (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + temp_left_bottom_y_location_float);

        (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + temp_right_top_x_location_float);
        (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + temp_right_top_y_location_float);

        // cout << "pin : (" << (*pin_access).left_bottom_x_location_def << " " << (*pin_access).left_bottom_y_location_def << " " << (*pin_access).right_top_x_location_def << " " << (*pin_access).right_top_y_location_def << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FS")
    {
        float temp_left_bottom_x_location_float = bottom_x_location_float;
        float temp_left_bottom_y_location_float = top_y_location_float;

        float temp_right_top_x_location_float = top_x_location_float;
        float temp_right_top_y_location_float = bottom_y_location_float;

        temp_left_bottom_y_location_float = abs(cell_height_float - temp_left_bottom_y_location_float);
        temp_right_top_y_location_float = abs(cell_height_float - temp_right_top_y_location_float);

        (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + temp_left_bottom_x_location_float);
        (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + temp_left_bottom_y_location_float);

        (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + temp_right_top_x_location_float);
        (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + temp_right_top_y_location_float);

        // cout << "pin : (" << (*pin_access).left_bottom_x_location_def << " " << (*pin_access).left_bottom_y_location_def << " " << (*pin_access).right_top_x_location_def << " " << (*pin_access).right_top_y_location_def << ")" << endl;
    }
    else
    {
        cout << " getPinAccessLocationFromDef error " << endl;
    }
};

bool isOddTrackCell(string x_location, TrackInfo *m3_track_info)
{
    float m3_x_float_temp = stof(x_location) - stof((*m3_track_info).start);
    float temp_pitch_start = m3_x_float_temp / (stof((*m3_track_info).pitch) * 2);
    int temp_pitch_start_int = (int)temp_pitch_start;
    temp_pitch_start_int += 1;

    float start_x = (temp_pitch_start_int * (stof((*m3_track_info).pitch) * 2)) + stof((*m3_track_info).start);
    float distance = start_x - stof(x_location);

    if (floatIsEqual(distance, 0.108))
    {
        return false;
    }
    else
    {
        return true;
    }

    return true;
}

//æ¯?æ¢?routing track ?????¨å??å°?pin access pint ??¨ä?????cell è£¡é?? --- 2
// float getPinAccessPointOfPlaced(float power_stripe_width, TrackPoint *m3_track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
// {
//     float cell_placed_pin_access_cost = 0;
//     // cout << "cell_id : " << (*cell_placed_info).cell_id << endl;
//     // cout << "track point : " << (*m3_track_point).x_point << " " << (*m3_track_point).y_point << endl;

//     for (auto pin_access_point_iter = (*cell_placed_info).pin_access_point_map.begin(); pin_access_point_iter != (*cell_placed_info).pin_access_point_map.end(); ++pin_access_point_iter)
//     {
//         string pin_name = pin_access_point_iter->first;
//         // cout << "pin name : " << pin_name << endl;
//         // if ((*m3_track_point).x_point == "325.188" && pin_name == "Y")
//         // {
//         //     cout << " check " << endl;
//         // }
//         int cover_pin_access_point = 0;
//         for (int i = 0; i < pin_access_point_iter->second.size(); i++)
//         {
//             PinAccessPoint pin_access_point = (pin_access_point_iter->second)[i];
//             // if ((*m3_track_point).x_point == "325.188" && pin_name == "C")
//             // {
//             // cout << "in side " << endl;
//             // cout << " (" << pin_access_point.middle_x_location_def << " " << pin_access_point.middle_y_location_def << ") " << endl;
//             if (isCoverPinAccess(&pin_access_point, &(*m3_track_point), power_stripe_width))
//             {
//                 // cout << "is cover pin_access :"
//                 //      << " (" << pin_access_point.middle_x_location_def << " " << pin_access_point.middle_y_location_def << ") " << endl;
//                 // cout << " (" << pin_access_point.middle_x_location_def << " " << pin_access_point.middle_y_location_def << ") " << endl;
//                 cover_pin_access_point += 1;
//             }
//             // }
//         }
//         if (cover_pin_access_point > 0)
//         {
//             int total_pin_access = pin_access_point_iter->second.size();
//             float pin_access = ((float)(cover_pin_access_point) / total_pin_access);
//             cell_placed_pin_access_cost += pin_access;
//         }
//     }

//     // power_consume * pin_access
//     float power_consume = (*cell_ip_map)[(*cell_placed_info).cell_id].instance_total_power;
//     power_consume = power_consume * 1000000;
//     power_consume = (1 / power_consume) * cell_placed_pin_access_cost;
//     // cout << "power_consume : " << power_consume << endl;
//     return power_consume;
// }

//¤@±ørouting track ³q¹L¦h¤Öcell
void setRoutingTrackPowerConsuming(string power_stripe_width, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    cout << "========== setRoutingTrackPowerConsuming start ==========" << endl;
    float power_stripe_width_float = stof(power_stripe_width);
    for (auto m3_track_point_iter = (*m3_track_point_map).begin(); m3_track_point_iter != (*m3_track_point_map).end(); ++m3_track_point_iter)
    {
        TrackPoint track_point = m3_track_point_iter->second;
        if (track_point.isRoutingTrack)
        {
            for (auto cell_placed_iter = (*cell_placed_map).begin(); cell_placed_iter != (*cell_placed_map).end(); ++cell_placed_iter)
            {
                CellPlacedInfo cell_placed_info = cell_placed_iter->second;
                if (isInTrackStripeRange(&track_point, &cell_placed_info, power_stripe_width_float))
                {
                    m3_track_point_iter->second.power_cell_id_vector.push_back(cell_placed_info.cell_id);
                }
            }
        }
    }
    cout << "========== setRoutingTrackPowerConsuming end ==========" << endl;
}

bool isInTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width)
{
    float track_point_left = stof((*track_point).x_point) - (power_stripe_width / 2);
    float track_point_right = stof((*track_point).x_point) + (power_stripe_width / 2);
    float cell_placed_left = stof((*cell_placed_info).left_x_location);
    float cell_placed_right = stof((*cell_placed_info).right_x_location);

    // power stripe in middle
    if (track_point_left >= cell_placed_left && track_point_right <= cell_placed_right)
    {
        return true;
    }
    // power stripe in left
    else if (track_point_left <= cell_placed_left && track_point_right > cell_placed_left && track_point_right <= cell_placed_right)
    {
        return true;
    }
    // power stripe in right
    else if (track_point_left >= cell_placed_left && track_point_left <= cell_placed_right && cell_placed_right >= cell_placed_right)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site)
{
    cout << "========== setRoutingTrackPoint start ==========" << endl;
    int log = 0;
    int start_core_site_y_location_int = (stof((*core_site).down_y_location) * 1000) / 4;
    int end_core_site_y_location_int = (stof((*core_site).up_y_location) * 1000) / 4;
    vector<TrackPoint> m2_track_point_vector;
    float m2_y_start_float = (stof(m2_y_start) * 1000) / 4;
    float m2_y_pitch_float = (stof(m2_y_pitch) * 1000) / 4;
    m2_y_start = floatToString(m2_y_start_float);
    m2_y_pitch = floatToString(m2_y_pitch_float);

    int m2_y_start_int = stringToInt(m2_y_start);
    int m2_y_pitch_int = stringToInt(m2_y_pitch);

    // cout << m2_y_start_int << endl;
    // cout << m2_y_pitch_int << endl;

    for (int i = m2_y_start_int; i < end_core_site_y_location_int; i += m2_y_pitch_int)
    {
        if (i > start_core_site_y_location_int && i <= end_core_site_y_location_int)
        {
            TrackPoint m2_track_point;
            double i_double = (double)i;
            m2_track_point.y_point = floatToString((i_double * 4) / 1000);
            string key_m2_track_point = m2_track_point.y_point;

            m2_track_point_vector.push_back(m2_track_point);
            (*m2_track_point_map).insert(pair<string, TrackPoint>(key_m2_track_point, m2_track_point));
        }
        if (i > end_core_site_y_location_int)
        {
            cout << "check break" << endl;
            break;
        }
    };

    // cout << "m2 track size : " << (*m2_track_point_map).size() << endl;
    // cout << "m2 track size : " << m2_track_point_vector.size() << endl;
    // cout << "m2 track point : " << m2_track_point_vector[507].y_point << endl;

    int start_core_site_x_location_int = (stof((*core_site).left_x_location) * 1000) / 4;
    int end_core_site_x_location_int = (stof((*core_site).right_x_location) * 1000) / 4;

    vector<TrackPoint> m3_track_point_vector;

    float m3_x_start_float = (stof(m3_x_start) * 1000) / 4;
    float m3_x_pitch_float = (stof(m3_x_pitch) * 1000) / 4;

    m3_x_start = floatToString(m3_x_start_float);
    m3_x_pitch = floatToString(m3_x_pitch_float);

    int m3_x_start_int = stringToInt(m3_x_start);
    int m3_x_pitch_int = stringToInt(m3_x_pitch);

    int isRoutingTrackCount = 1;
    for (int i = m3_x_start_int; i < end_core_site_x_location_int; i += m3_x_pitch_int)
    {
        if (i > start_core_site_x_location_int && i <= end_core_site_x_location_int)
        {
            TrackPoint m3_track_point;
            double i_double = (double)i;
            m3_track_point.x_point = floatToString((i_double * 4) / 1000);
            string key_m3_track_point = m3_track_point.x_point;
            vector<string> power_cell_id_vector;
            m3_track_point.power_cell_id_vector = power_cell_id_vector;

            //??¤æ?·æ?¯ä????¯routing Track
            if ((isRoutingTrackCount % 2) == 1)
            {
                m3_track_point.isRoutingTrack = true;
                // cout << "isRouting track point : " << m3_track_point.x_point << " " << m3_track_point.isRoutingTrack << endl;
            }
            else
            {
                // cout << "isRouting track point : " << m3_track_point.x_point << " " << m3_track_point.isRoutingTrack << endl;
                m3_track_point.isRoutingTrack = false;
            }

            m3_track_point_vector.push_back(m3_track_point);
            (*m3_track_point_map).insert(pair<string, TrackPoint>(key_m3_track_point, m3_track_point));
        }
        if (i > end_core_site_x_location_int)
        {
            cout << "check break" << endl;
            break;
        }
        isRoutingTrackCount += 1;
    };
    // cout << "m2 track size : " << (*m3_track_point_map).size() << endl;
    // cout << "m2 track size : " << m3_track_point_vector.size() << endl;
    // cout << "m3 track point : " << m3_track_point_vector[509].x_point << endl;
    cout << "========== setRoutingTrackPoint end ==========" << endl;
};

int stringToInt(string num)
{
    stringstream ss;
    int num_int;
    ss << num;
    ss >> num_int;

    ss.str("");
    ss.clear();
    return num_int;
}

void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
{
    cout << "========== setStripeRange start ==========" << endl;
    setRange(&(*vdd_stripe_vector), &(*core_site));
    setRange(&(*vss_stripe_vector), &(*core_site));
    cout << "========== setStripeRange end ==========" << endl;
}

void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
{
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            (*stripe_vector)[i].move_range_x_left = (*core_site).left_x_location;
            float middle_x_power_stripe = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_right = floatToString(middle_x_power_stripe);
        }
        else if (i == ((*stripe_vector).size() - 1))
        {

            (*stripe_vector)[i].move_range_x_right = (*core_site).right_x_location;
            float middle_x_power_stripe = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_left = floatToString(middle_x_power_stripe);
        }
        else
        {
            float middle_x_power_stripe_right = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
            float middle_x_power_stripe_left = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_right = floatToString(middle_x_power_stripe_right);
            (*stripe_vector)[i].move_range_x_left = floatToString(middle_x_power_stripe_left);
        }
    }
}

//??ªè????°Stripe å°? break
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
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
                            cout << "VDDX :" << def_content << " " << def_content_array.size() << endl;
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.net_name = NET_NAME_VDD;

                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                (*vdd_stripe_vector).push_back(stripe);
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
                            cout << "VSSX :" << def_content << " " << def_content_array.size() << endl;
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.net_name = NET_NAME_VSS;
                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                (*vss_stripe_vector).push_back(stripe);
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

bool isPowerStripe(Stripe *stripe, CoreSite *core_site)
{

    if ((*stripe).end_x_location == "*")
    {
        (*stripe).end_x_location = (*stripe).start_x_location;
    }
    if ((*stripe).end_y_location == "*")
    {
        (*stripe).end_y_location = (*stripe).start_y_location;
    }

    if ((*stripe).start_x_location == (*stripe).end_x_location)
    {
        float y_distance = abs(stof((*stripe).start_y_location) - stof((*stripe).end_y_location));
        float core_site_y_distance = abs(stof((*core_site).down_y_location) - stof((*core_site).up_y_location)) / 2;
        if (y_distance < core_site_y_distance)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if ((*stripe).start_y_location == (*stripe).end_y_location)
    {
        return false;
    }
    else
    {
        return false;
        cout << " isPowerStripe error  << " << endl;
    }
}

void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{
    cout << "========== getIpPowerReport start ==========" << endl;
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
    cout << "========== getIpPowerReport end ==========" << endl;
}

void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map)
{
    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    bool is_placed_end = false;
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
                        // TODO ???è£¡å?¯ä»¥ä¸?log
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        CellPlacedInfo cell_placed_info;
                        cell_placed_info.cell_id = def_content_array[1];
                        cell_placed_info.cell_name = def_content_array[2];
                        setPlacePosition(&def_content_array, &cell_placed_info);
                        getCellLocation(&cell_placed_info, &(*cell_info_map));
                        (*cell_placed_map).insert(pair<string, CellPlacedInfo>(cell_placed_info.cell_id, cell_placed_info));
                        log++;
                        if (log % 10000 == 0)
                        {
                            cout << "def data : " << log << endl;
                        }
                    }
                    if (def_content.find("END COMPONENTS") != string::npos)
                    {
                        cout << " end def data " << endl;
                        is_placed_end = true;
                        break;
                    }
                }
            }
            if (is_placed_end)
            {
                break;
            }
        }
    }
}

void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map)
{

    string cell_name = (*cell_placed_info).cell_name;

    if ((*cell_info_map).count(cell_name))
    {
        // cell ??? ??³ä??è³?è¨?
        string left_x_location = (*cell_placed_info).left_x_location;
        string down_y_location = (*cell_placed_info).down_y_location;
        float left_x_float = stof(left_x_location);
        float down_y_float = stof(down_y_location);
        string cell_width = (*cell_info_map)[cell_name].width;
        string cell_height = (*cell_info_map)[cell_name].height;
        float right_x_float = left_x_float + stof(cell_width);
        float up_y_float = down_y_float + stof(cell_height);
        (*cell_placed_info).right_x_location = floatToString(right_x_float);
        (*cell_placed_info).up_y_location = floatToString(up_y_float);
        // cout << cell_name << " " << left_x_float << " " << down_y_float << " " << right_x_float << " " << up_y_float << " " << (*cell_placed_info).direction << endl;

        // if ((*cell_placed_info).cell_id == "U145431")
        // {
        // cout << "cell_place_left_x : " << (*cell_placed_info).left_x_location << " cell_place_down_y : " << (*cell_placed_info).down_y_location << endl;
        // map<string, PinShape> pin_shape_map = (*cell_info_map)[cell_name].pin_shape_map;
        // map<string, Pin> pin_map;
        // // å°? pin access ??¾å?? cell placed imformation
        // for (auto pin_shape : pin_shape_map)
        // {
        //     Pin pin;
        //     map<string, vector<Rect>> rect_map = pin_shape.second.rect_map;
        //     // cout << pin_shape.first << endl;
        //     string pin_name = pin_shape.first;
        //     map<string, vector<PinAccess>> pin_access_layer_map;
        //     for (auto rect : rect_map)
        //     {
        //         string layer_name = rect.first;
        //         vector<Rect> rect_vector = rect.second;
        //         vector<PinAccess> pin_access_vector;
        //         for (int i = 0; i < rect_vector.size(); i++)
        //         {
        //             // vector<PinAccessShape> pin_access_shape_vector = rect_vector[i].pin_access_shape_vector;
        //             // if (pin_access_shape_vector.size() > 0)
        //             // {
        //             // for (int i = 0; i < pin_access_shape_vector.size(); i++)
        //             // {
        //             PinAccess pin_access;
        //             // cout << cell_name << " " << left_x_float << " " << down_y_float << " " << right_x_float << " " << up_y_float << " " << (*cell_placed_info).direction << endl;
        //             // cout << pin_access_shape_vector[i].left_bottom_x_location << " " << pin_access_shape_vector[i].left_bottom_y_location << " " << pin_access_shape_vector[i].right_top_x_location << " " << pin_access_shape_vector[i].right_top_y_location << endl;
        //             // cout << pin_name << endl;
        //             getPinAccessLocationFromDef(&(rect_vector[i]), &(*cell_placed_info), cell_width, cell_height, &pin_access);
        //             pin_access_vector.push_back(pin_access);
        //             // }
        //             // cout << "" << endl;
        //             // }
        //         }
        //         pin_access_layer_map.insert(pair<string, vector<PinAccess>>(layer_name, pin_access_vector));
        //     }
        //     pin.layer_pin_access_map = pin_access_layer_map;
        //     pin_map.insert(pair<string, Pin>(pin_name, pin));
        // }
        // (*cell_placed_info).pin_map = pin_map;
        // }
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
    cout << "========== getLefCellImformation start ==========" << endl;
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
                map<string, PinShape> pin_shape_map;
                while (getline(lef_file, lef_content))
                {
                    vector<string> lef_content_array = splitByPattern(lef_content, " ");
                    if (lef_content.find("SIZE") != string::npos && lef_content.find("BY") != string::npos)
                    {
                        getSizeOfCell(lef_content, &cell_info);
                    }
                    if (lef_content.find("PIN") != string::npos)
                    {
                        PinShape pin_shape;
                        map<string, vector<Rect>> rect_map;
                        pin_shape.rect_map = rect_map;
                        getPinShapeOfCell(&lef_file, lef_content, &pin_shape);
                        pin_shape_map.insert(pair<string, PinShape>(pin_shape.pin_name, pin_shape));
                        cell_info.pin_shape_map = pin_shape_map;
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
    cout << "========== getLefCellImformation end ==========" << endl;
}

void getSizeOfCell(string lef_content, CellInfo *cell_info)
{
    vector<string> lef_content_array = splitByPattern(lef_content, " ");
    (*cell_info).height = lef_content_array[3];
    (*cell_info).width = lef_content_array[1];
}

void getPinShapeOfCell(ifstream *lef_file, string lef_content, PinShape *pin_shape)
{
    vector<string> lef_content_array = splitByPattern(lef_content, " ");
    (*pin_shape).pin_name = lef_content_array[1];
    // cout << (*pin).pin_name << endl;
    while (getline(*lef_file, lef_content))
    {
        vector<string> lef_content_array = splitByPattern(lef_content, " ");
        if ((lef_content_array[0] == "PORT"))
        {
            getPortOfPinShape(&(*lef_file), lef_content, &(*pin_shape));
        }
        if ((lef_content_array[0] == "END") && (lef_content_array[1] == (*pin_shape).pin_name))
        {
            // cout << lef_content << endl;
            break;
        }
    }
}

void getPortOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape)
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
                (*pin_shape).rect_map.insert(pair<string, vector<Rect>>(layer_name, rect_vector));
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
            // cout << "====================" << endl;
            if (isPinAccessNameAndLayer(layer_name, pin_shape->pin_name))
            {
                // cout << lef_content << endl;
                // getPinAccessFromRect(&rect);
                rect_vector.push_back(rect);
            }
            // cout << "=====================" << endl;
            // rect_vector.push_back(rect);
        }
        if ((lef_content_array[0] == "END"))
        {
            if (rect_vector.size() > 0)
            {
                (*pin_shape).rect_map.insert(pair<string, vector<Rect>>(layer_name, rect_vector));
                rect_vector.clear();
            }
            // cout << lef_content << endl;
            break;
        }
    }
}
bool isPinAccessNameAndLayer(string layer_name, string pin_name)
{
    if ((layer_name == "M1" || layer_name == "M2") && pin_name != "VDD" && pin_name != "VSS")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool lessThanOrEqualTo(float a, float b)
{
    if (a < b)
    {
        return true;
    }
    else if (floatIsEqual(a, b))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// void getPinAccessFromRect(Rect *rect)
// {
//     float bottom_x_location_float = stof((*rect).bottom_x_location);
//     float bottom_y_location_float = stof((*rect).bottom_y_location);
//     float top_x_location_float = stof((*rect).top_x_location);
//     float top_y_location_float = stof((*rect).top_y_location);
//     vector<PinAccessShape> pin_access_shape_vector;
//     // step 1: ??¤æ?·æ?? ?????´é????¯æ°´å¹?
//     if (isVertical(&(*rect)))
//     {
//         for (float i = bottom_y_location_float; i < top_y_location_float; i += 0.072)
//         {
//             // cout << " i " << i;

//             float middle_x_location = (abs(bottom_x_location_float - top_x_location_float) / 2) + bottom_x_location_float;
//             float middle_y_location = i + ((0.072) / 2);

//             float left_bottom_x_location = bottom_x_location_float;
//             float left_bottom_y_location = i;
//             float right_top_x_location = top_x_location_float;
//             float right_top_y_location = i + 0.072;
//             PinAccessShape pin_access_shape;

//             pin_access_shape.middle_x_location = floatToString(middle_x_location);
//             pin_access_shape.middle_y_location = floatToString(middle_y_location);
//             pin_access_shape.left_bottom_x_location = floatToString(left_bottom_x_location);
//             pin_access_shape.left_bottom_y_location = floatToString(left_bottom_y_location);
//             pin_access_shape.right_top_x_location = floatToString(right_top_x_location);
//             pin_access_shape.right_top_y_location = floatToString(right_top_y_location);
//             if (right_top_y_location < top_y_location_float || floatIsEqual(right_top_y_location, top_y_location_float))
//             {
//                 pin_access_shape_vector.push_back(pin_access_shape);
//             }

//             // cout << " middle " << middle_x_location << " " << middle_y_location;
//             // cout <<" vertical " << " left_bottom_x_location : " << pin_access.left_bottom_x_location << " left_bottom_y_location : " << pin_access.left_bottom_y_location << " right_top_x_location : " << pin_access.right_top_x_location << " right_top_y_location : " << pin_access.right_top_y_location << endl;
//         }
//         (*rect).pin_access_shape_vector = pin_access_shape_vector;
//         // cout << "" << endl;
//     }
//     else
//     {
//         for (float i = bottom_x_location_float; i < top_x_location_float; i += 0.072)
//         {
//             // cout << " i " << i;

//             float middle_x_locaiton = i + ((0.072) / 2);
//             float middle_y_location = (abs(bottom_y_location_float - top_y_location_float) / 2) + bottom_y_location_float;
//             // cout << " middle " << middle_x_locaiton << " " << middle_y_location;

//             float left_bottom_x_location = i;
//             float left_bottom_y_location = bottom_y_location_float;
//             float right_top_x_location = i + 0.072;
//             float right_top_y_location = top_y_location_float;

//             PinAccessShape pin_access_shape;
//             pin_access_shape.middle_x_location = floatToString(middle_x_locaiton);
//             pin_access_shape.middle_y_location = floatToString(middle_y_location);
//             pin_access_shape.left_bottom_x_location = floatToString(left_bottom_x_location);
//             pin_access_shape.left_bottom_y_location = floatToString(left_bottom_y_location);
//             pin_access_shape.right_top_x_location = floatToString(right_top_x_location);
//             pin_access_shape.right_top_y_location = floatToString(right_top_y_location);
//             if (right_top_x_location < top_x_location_float || floatIsEqual(right_top_x_location, top_x_location_float))
//             {
//                 pin_access_shape_vector.push_back(pin_access_shape);
//             }
//             // cout << " horizontal " << " left_bottom_x_location : " << pin_access.left_bottom_x_location << " left_bottom_y_location : " << pin_access.left_bottom_y_location << " right_top_x_location : " << pin_access.right_top_x_location << " right_top_y_location : " << pin_access.right_top_y_location << endl;
//         }
//         (*rect).pin_access_shape_vector = pin_access_shape_vector;
//         // cout << "" << endl;
//     }
// }

float convertInnovusPoint(int number)
{
    return (number * 4) / 1000.0;
}

bool isVertical(Rect *rect)
{
    float bottom_x_location_float = stof((*rect).bottom_x_location);
    float bottom_y_location_float = stof((*rect).bottom_y_location);
    float top_x_location_float = stof((*rect).top_x_location);
    float top_y_location_float = stof((*rect).top_y_location);

    float width = abs(bottom_x_location_float - top_x_location_float);
    float height = abs(bottom_y_location_float - top_y_location_float);

    if (width > height)
    {
        return false;
    }
    else
    {
        return true;
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
//??¹æ??pattern???å­?ä¸?
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

//??»æ??é¦?å°¾å?¨é?¨ç©º???
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
string floatToString(const float value)
{
    std::stringstream buf;
    buf.precision(3);
    buf.setf(std::ios::fixed);
    buf << value;
    string str = buf.str();
    buf.clear();
    return str;
}

// for (auto cellInfo : cell_info_map)
// {
//     cout << "cell name : " << cellInfo.first << endl;
//     map<string, PinShape> pin_shape_map = cellInfo.second.pin_shape_map;
//     for (auto pin_shape : pin_shape_map)
//     {
//         cout << pin_shape.second.pin_name << " " << pin_shape.second.rect_map.size() << endl;
//         for (auto rect : pin_shape.second.rect_map)
//         {
//             cout << rect.first << endl;
//             for (int i = 0; i < rect.second.size(); i++)
//             {
//                 cout << rect.second[i].bottom_x_location << " " << rect.second[i].bottom_y_location << " " << rect.second[i].top_x_location << " " << rect.second[i].top_y_location << " "  << rect.second[i].pin_access_vector.size() << endl;
//             }
//         }
//     }
// }
// cout << " after " << endl;
// cout << "" << endl;
// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;
//     map<string, Pin> pin_map = cell_place.second.pin_map;
//     for (auto pin : pin_map)
//     {
//         cout << "pin name : " << pin.first << endl;
//         for (auto pin_layer : pin.second.layer_pin_access_map)
//         {
//             cout << "layer : " << pin_layer.first << " " << pin_layer.second.size();
//             for (int i = 0; i < pin_layer.second.size(); i++)
//             {
//                 cout << "(" << pin_layer.second[i].middle_x_location_def << " " << pin_layer.second[i].middle_y_location_def << ")";
//             }
//             std::cout << "" << std::endl;
//         };
//     }
// }
// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;
//     map<string, Pin> pin_map = cell_place.second.pin_map;
//     for (auto pin : pin_map)
//     {
//         cout << "pin name : " << pin.first << endl;
//         for (auto pin_layer : pin.second.layer_pin_access_map)
//         {
//             cout << "layer : " << pin_layer.first << " " << pin_layer.second.size();
//             for (int i = 0; i < pin_layer.second.size(); i++)
//             {
//                 // cout << "(" << pin_layer.second[i].middle_x_location_def << " " << pin_layer.second[i].middle_y_location_def << ")";
//             }
//             std::cout << "" << std::endl;
//         };
//     }
// }
// for (auto cell_place : cell_placed_map)
// {
//     cout << "pin id :" << cell_place.first << endl;
//     map<string, Pin> pin_map = cell_place.second.pin_map;
//     for (auto pin : pin_map)
//     {
//         cout << "pin name : " << pin.first << endl;
//         for (auto pin_layer : pin.second.layer_pin_access_map)
//         {
//             cout << "layer : " << pin_layer.first << " ";
//             for (int i = 0; i < pin_layer.second.size(); i++)
//             {
//                 cout << "(" << pin_layer.second[i].middle_x_location_def << " " << pin_layer.second[i].middle_y_location_def << ")";
//             }
//             std::cout << "" << std::endl;
//         };
//     }
// }

// for (auto cell_place : cell_placed_map["U148369"].pin_map)
// {
//     cout << cell_place.first << endl;

//     for (auto cell_layer : cell_place.second.layer_pin_access_map)
//     {
//         for (int i = 0; i < cell_layer.second.size(); i++)
//         {
//             cout << cell_place.first << "  (" << cell_layer.second[i].middle_x_location_def << " " << cell_layer.second[i].middle_y_location_def << ")" << endl;
//         }
//     }
// }
// for (auto m3_track_point : m3_track_point_map)
// {
//     if (m3_track_point.second.power_cell_id_vector.size() > 0)
//     {

//         cout << "track point :" << m3_track_point.second.x_point << " " << m3_track_point.second.power_cell_id_vector.size() << endl;
//         for (int i = 0; i < m3_track_point.second.power_cell_id_vector.size(); i++)
//         {
//             cout << m3_track_point.second.power_cell_id_vector[i] << endl;
//         }
//     }
// }

// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;
//     for (auto pin_access_map : cell_place.second.pin_access_point_map)
//     {
//         cout << "pin name : " << pin_access_map.first << endl;
//         for (auto pin_access_pin : pin_access_map.second)
//         {
//             cout << " (" << pin_access_pin.middle_x_location_def << " " << pin_access_pin.middle_y_location_def << ") " << endl;
//         }
//     }
// }

// for (auto cellInfo : cell_info_map)
// {
//     cout << "cell name : " << cellInfo.first << endl;
//     map<string, PinShape> pin_shape_map = cellInfo.second.pin_shape_map;
//     for (auto pin_shape : pin_shape_map)
//     {
//         cout << pin_shape.second.pin_name << " " << pin_shape.second.rect_map.size() << endl;
//         for (auto rect : pin_shape.second.rect_map)
//         {
//             cout << rect.first << endl;
//             for (int i = 0; i < rect.second.size(); i++)
//             {
//                 cout << rect.second[i].bottom_x_location << " " << rect.second[i].bottom_y_location << " " << rect.second[i].top_x_location << " " << rect.second[i].top_y_location << " "  << endl;
//             }
//         }
//     }
// }

// void getPinAccessFromRect(Rect *rect)
// {
//     int bottom_x_location_int = (stof((*rect).bottom_x_location) * 1000) / 4;
//     int bottom_y_location_int = (stof((*rect).bottom_y_location) * 1000) / 4;
//     int top_x_location_int = (stof((*rect).top_x_location) * 1000) / 4;
//     int top_y_location_int = (stof((*rect).top_y_location) * 1000) / 4;
//     vector<PinAccessShape> pin_access_shape_vector;
//     // step 1: ??¤æ?·æ?? ?????´é????¯æ°´å¹?
//     if (isVertical(&(*rect)))
//     {
//         for (int i = bottom_y_location_int; i < top_y_location_int; i += 18)
//         {
//             // cout << " i " << i;

//             int middle_x_location = (abs(bottom_x_location_int - top_x_location_int) / 2) + bottom_x_location_int;
//             int middle_y_location = i + (18 / 2);

//             int left_bottom_x_location = bottom_x_location_int;
//             int left_bottom_y_location = i;
//             int right_top_x_location = top_x_location_int;
//             int right_top_y_location = i + 18;
//             PinAccessShape pin_access_shape;

//             pin_access_shape.middle_x_location = floatToString(convertInnovusPoint(middle_x_location));
//             pin_access_shape.middle_y_location = floatToString(convertInnovusPoint(middle_y_location));
//             pin_access_shape.left_bottom_x_location = floatToString(convertInnovusPoint(left_bottom_x_location));
//             pin_access_shape.left_bottom_y_location = floatToString(convertInnovusPoint(left_bottom_y_location));
//             pin_access_shape.right_top_x_location = floatToString(convertInnovusPoint(right_top_x_location));
//             pin_access_shape.right_top_y_location = floatToString(convertInnovusPoint(right_top_y_location));
//             if (right_top_y_location <= top_y_location_int)
//             {
//                 pin_access_shape_vector.push_back(pin_access_shape);
//             }

//             // cout << " middle " << middle_x_location << " " << middle_y_location;
//             // cout <<" vertical " << " left_bottom_x_location : " << pin_access.left_bottom_x_location << " left_bottom_y_location : " << pin_access.left_bottom_y_location << " right_top_x_location : " << pin_access.right_top_x_location << " right_top_y_location : " << pin_access.right_top_y_location << endl;
//         }
//         (*rect).pin_access_shape_vector = pin_access_shape_vector;
//         // cout << "" << endl;
//     }
//     else
//     {
//         for (int i = bottom_x_location_int; i < top_x_location_int; i += 18)
//         {
//             // cout << " i " << i;

//             int middle_x_locaiton = i + (18 / 2);
//             int middle_y_location = (abs(bottom_y_location_int - top_y_location_int) / 2) + bottom_y_location_int;
//             // cout << " middle " << middle_x_locaiton << " " << middle_y_location;

//             int left_bottom_x_location = i;
//             int left_bottom_y_location = bottom_y_location_int;
//             int right_top_x_location = i +18;
//             int right_top_y_location = top_y_location_int;

//             PinAccessShape pin_access_shape;
//             pin_access_shape.middle_x_location = floatToString(convertInnovusPoint(middle_x_locaiton));
//             pin_access_shape.middle_y_location = floatToString(convertInnovusPoint(middle_y_location));
//             pin_access_shape.left_bottom_x_location = floatToString(convertInnovusPoint(left_bottom_x_location));
//             pin_access_shape.left_bottom_y_location = floatToString(convertInnovusPoint(left_bottom_y_location));
//             pin_access_shape.right_top_x_location = floatToString(convertInnovusPoint(right_top_x_location));
//             pin_access_shape.right_top_y_location = floatToString(convertInnovusPoint(right_top_y_location));
//             if (right_top_x_location <= top_x_location_int)
//             {
//                 pin_access_shape_vector.push_back(pin_access_shape);
//             }
//             // cout << " horizontal " << " left_bottom_x_location : " << pin_access.left_bottom_x_location << " left_bottom_y_location : " << pin_access.left_bottom_y_location << " right_top_x_location : " << pin_access.right_top_x_location << " right_top_y_location : " << pin_access.right_top_y_location << endl;
//         }
//         (*rect).pin_access_shape_vector = pin_access_shape_vector;
//         // cout << "" << endl;
//     }
// }

//è¨?ç®? ??¯ä»¥ä½¿ç?? ??°ç??vectorä¾????ä»???????vector
// void setPinAccessPointFromTrack(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
// {
//     for (auto cell_placed_iter = (*cell_placed_map).begin(); cell_placed_iter != (*cell_placed_map).end(); ++cell_placed_iter)
//     {
//         map<string, Pin> pin_map = cell_placed_iter->second.pin_map;
//         string cell_name = cell_placed_iter->first;
//         for (auto pin_iter = pin_map.begin(); pin_iter != pin_map.end(); ++pin_iter)
//         {
//             string pin_name = pin_iter->first;
//             map<string, vector<PinAccess>> layer_pin_access_map = pin_iter->second.layer_pin_access_map;

//             for (auto layer_pin_access_iter = layer_pin_access_map.begin(); layer_pin_access_iter != layer_pin_access_map.end(); ++layer_pin_access_iter)
//             {
//                 string layer_name = layer_pin_access_iter->first;
//                 vector<PinAccess> pin_access_is_on_track_vector;
//                 vector<PinAccess> pin_access_vector = layer_pin_access_iter->second;

//                 for (int i = 0; i < pin_access_vector.size(); i++)
//                 {
//                     string middle_x_location_def = (*cell_placed_map)[cell_name].pin_map[pin_name].layer_pin_access_map[layer_name][i].middle_x_location_def;
//                     string middle_y_location_def = (*cell_placed_map)[cell_name].pin_map[pin_name].layer_pin_access_map[layer_name][i].middle_y_location_def;

//                     if (((*m2_track_point_map).count(middle_y_location_def) && (*m3_track_point_map).count(middle_x_location_def)))
//                     {
//                         pin_access_is_on_track_vector.push_back((*cell_placed_map)[cell_name].pin_map[pin_name].layer_pin_access_map[layer_name][i]);
//                     }
//                 }
//                 (*cell_placed_map)[cell_name].pin_map[pin_name].layer_pin_access_map[layer_name] = pin_access_is_on_track_vector;
//             }

//         }
//     }
// }