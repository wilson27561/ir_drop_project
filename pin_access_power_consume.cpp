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

struct Rect
{
    string bottom_x_location;
    string bottom_y_location;
    string top_x_location;
    string top_y_location;
};

struct PinShape
{
    string pin_name;
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
struct TrackPoint
{
    string x_point;
    string y_point;
    vector<string> power_cell_id_vector;
    bool isRoutingTrack;
    float total_pin_access_power_consum_cost;
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
    vector<string> ip_power_vector;
    vector<TrackPoint> track_point_vector;
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

struct CellRange
{
    vector<string> cell_id_vector;
    string start_x_location;
    string end_x_location;

    /* data */
};

// main function
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string DEF_TRANSFER_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);

// sub function
int getCoverTrack(float power_stripe_width);
void getSizeOfCell(string lef_content, CellInfo *cell_info);
void getPinShapeOfCell(ifstream *lef_file, string left_content, PinShape *pin_shape);
void getPortOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site, TrackInfo *track_info);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site);
void setRoutingTrackPowerConsuming(string power_stripe_width, vector<Stripe> *stripe_vector, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *m3_track_info);
void setRoutingTrackNumberOfPinAccess(string power_stripe_width, vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info);
void isOnTrackPin(map<string, PinShape> *pin_shape_map, map<string, vector<PinAccessPoint>> *pin_access_point_map, string track_point_x, string track_point_y);
void getAddStripeCost(unordered_map<string, TrackPoint> *m3_track_point_map, vector<Stripe> *stripe_vector, TrackInfo *track_info);
void getLeftCellPinAccessPoint(unordered_map<string, CellInfo> *cell_info_map);
void setCellPinAccessPoint(CellInfo *cell_info);
void getStripeRangeCost(Stripe *stripe, unordered_map<string, TrackPoint> *m3_track_point_map, float track_pitch);
void generateAddStripeTcl(vector<Stripe> *stripe_vector, string move_stripe_name, string power_stripe_width);
void transferPinAccessLocationFromDef(CellPlacedInfo *cell_placed_info, PinAccessPoint *pin_access_point, PinAccessPoint *pin_access_point_def, string cell_width, string cell_height);
float getPinAccessPointOfPlaced(float power_stripe_width_float, TrackPoint *m3_track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info);
void setCellStripeRange(vector<Stripe> *vdd_stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void generateLogFile(vector<Stripe> *stripe_vector, string log_file_name);
void setCellRange(string power_stripe_width, CoreSite *core_site, vector<CellRange> *cell_range_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);

bool isInStripeRange(Stripe *vdd_stripe, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map);

bool isVertical(Rect *rect);
bool isOddTrackCell(string x_location, TrackInfo *m3_track_info);
bool isPinAccessNameAndLayer(string layer_name, string pin_name);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
bool isInTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width);
bool isCoverPinAccess(float pin_access_point_x_def_float, float track_point_x_float, float power_stripe_width);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool floatIsEqual(float a, float b);
bool isInCellRange(CellRange *cell_range, CellPlacedInfo *cell_place_info, string power_stripe_width);
// Util
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
string floatToString(const float value);
int stringToInt(string num);
float convertInnovusPoint(int number);
int main()
{

    string v1_width = "0.072";
    string m2_y_pitch = "0.072";
    string m2_y_start = "1.44";
    string m3_x_pitch = "0.072";
    string m3_x_start = "0.18";
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    vector<CellRange> cell_range_vector;
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

    string LEF_FILE = "";
    string IP_REPORT_FILE = "";
    string ADD_STRIPE_TCL = "";
    string DEF_TRANSFER_FILE = "";
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
            if (config_content_array[0] == "ADD_STRIPE_TCL")
            {
                ADD_STRIPE_TCL = config_content_array[2];
            }
        }
    }

    double START, END;
    START = clock();

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    setStripeRange(&vdd_stripe_vector, &vss_stripe_vector, &core_site, &m3_track_info);

    // setRoutingTrackPoint(&m2_track_point_map, &m3_track_point_map, m3_x_start, m3_x_pitch, m2_y_start, m2_y_pitch, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getLeftCellPinAccessPoint(&cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);

    setCellStripeRange(&vdd_stripe_vector, &cell_ip_map, &cell_placed_map);
    setRoutingTrackPowerConsuming("0.224", &vdd_stripe_vector, &m3_track_point_map, &cell_placed_map, &m3_track_info);

    setRoutingTrackNumberOfPinAccess("0.224", &vdd_stripe_vector, &cell_placed_map, &cell_ip_map, &cell_info_map, &m3_track_info);
    getAddStripeCost(&m3_track_point_map, &vdd_stripe_vector, &m3_track_info);
    generateAddStripeTcl(&vdd_stripe_vector, ADD_STRIPE_TCL, "0.224");
    // generateLogFile(&vdd_stripe_vector, "pin_access_log_file.txt");

    cout << endl
         << "程式執行所花費：" << (double)clock() / CLOCKS_PER_SEC << " S";

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

    // int total_track = 0;
    // int total_track_cell_id = 0;
    // for (int i = 0; i < vdd_stripe_vector.size(); i++)
    // {
    //     for (int j = 0; j < vdd_stripe_vector[i].track_point_vector.size(); j++)
    //     {
    //         TrackPoint track_point = vdd_stripe_vector[i].track_point_vector[j];
    //         cout << "track poin cost : " << track_point.x_point << "  " << track_point.total_pin_access_power_consum_cost << endl;
    //     }
    // }

    // cout << "track size : " << total_track << endl;
    // cout << "total_track_cell_id : " << total_track_cell_id << endl;

    // for (auto cell_place : cell_placed_map)
    // {
    //     cout << "cell id :" << cell_place.first << endl;

    //     cout << cell_place.second.left_x_location << " " << cell_place.second.down_y_location << " " << cell_place.second.right_x_location << " " << cell_place.second.up_y_location << endl;
    // }

    return 0;
}

void generateLogFile(vector<Stripe> *stripe_vector, string log_file_name)
{
    cout << "========== generateLogFile start ==========" << endl;
    ofstream myfile;
    myfile.open(log_file_name);
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        myfile << "vdd track cost : " << (*stripe_vector)[i].vdd_track_x_cost << endl;
        myfile << "vss track cost : " << (*stripe_vector)[i].vss_track_x_cost << endl;
    }

    myfile.close();
    cout << "========== generateLogFile close ==========" << endl;
};

//一條routing track 通過多少cell 只剩這裡有問題
void setRoutingTrackPowerConsuming(string power_stripe_width, vector<Stripe> *stripe_vector, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *m3_track_info)
{
    cout << "========== setRoutingTrackPowerConsuming start ==========" << endl;

    float power_stripe_width_float = stof(power_stripe_width);
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        int start_move_range_x_left = (stof((*stripe_vector)[i].move_range_x_left) * 1000) / 4;
        int start_move_range_x_right = (stof((*stripe_vector)[i].move_range_x_right) * 1000) / 4;

        float m3_x_start_float = (stof((*m3_track_info).start) * 1000) / 4;
        float m3_x_pitch_float = (stof((*m3_track_info).pitch) * 1000) / 4;

        int m3_x_start_int = (int)m3_x_start_float;
        m3_x_pitch_float = m3_x_pitch_float * 2;
        int m3_x_pitch_int = (int)m3_x_pitch_float;

        cout << " ----- moving range start -----" << endl;
        cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << endl;
        vector<TrackPoint> track_point_vector;
        for (int j = start_move_range_x_left; j <= start_move_range_x_right; j += m3_x_pitch_float)
        {
            float x_track_point = (j * 4) / 1000.0;

            vector<string> power_cell_id_vector;
            TrackPoint track_point;
            track_point.x_point = floatToString(x_track_point);
            track_point.power_cell_id_vector = power_cell_id_vector;
            vector<string> ip_power_vector = (*stripe_vector)[i].ip_power_vector;
            for (int k = 0; k < ip_power_vector.size(); k++)
            {
                string cell_id = ip_power_vector[k];
                if (isInTrackStripeRange(&track_point, &(*cell_placed_map)[cell_id], power_stripe_width_float))
                {
                    track_point.power_cell_id_vector.push_back(cell_id);
                }
            }
            track_point_vector.push_back(track_point);
        }
        cout << " ----- moving range end -----" << endl;
        (*stripe_vector)[i].track_point_vector = track_point_vector;
    }

    cout << "========== setRoutingTrackPowerConsuming end ==========" << endl;
}

// 將 cell 放入 stripe range 當中
void setCellStripeRange(vector<Stripe> *vdd_stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    cout << "========== setCellStripeRange start ==========" << endl;
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
        {
            string cell_id = iter->first;
            if (isInStripeRange((&(*vdd_stripe_vector)[i]), cell_id, &(*cell_placed_map)))
            {
                (*vdd_stripe_vector)[i].ip_power_vector.push_back(cell_id);
            }
        }
    }
    cout << "========== setCellStripeRange end ==========" << endl;
}
bool isInStripeRange(Stripe *vdd_stripe, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    string move_range_x_left = (*vdd_stripe).move_range_x_left;
    string move_range_x_right = (*vdd_stripe).move_range_x_right;
    string left_x_location = (*cell_placed_map)[cell_id].left_x_location;
    string right_x_location = (*cell_placed_map)[cell_id].right_x_location;
    float move_range_x_left_float = stof(move_range_x_left);
    float move_range_x_right_float = stof(move_range_x_right);
    float left_x_location_float = stof(left_x_location);
    float right_x_location_float = stof(right_x_location);

    // case 1 : middle of left stripe
    if ((left_x_location_float < move_range_x_left_float) && (right_x_location_float > move_range_x_left_float) && (left_x_location_float < move_range_x_right_float) && (right_x_location_float < move_range_x_right_float))
    {
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        return true;
    } // case 4 : out side
    else
    {
        return false;
    }
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
            isOnTrackPin(&((*cell_info).pin_shape_map), &((*cell_info).odd_pin_access_point_map), floatToString(i), floatToString(j));
        }
    }

    for (float i = even_start_m3_track_float; floatIsEqualOrLess(i, end_m3_track_float); i += m3_pitch_track)
    {
        for (float j = start_m2_track_float; floatIsEqualOrLess(j, end_m2_track_float); j += m2_pitch_track)
        {
            isOnTrackPin(&((*cell_info).pin_shape_map), &((*cell_info).even_pin_access_point_map), floatToString(i), floatToString(j));
        }
    }
}

void isOnTrackPin(map<string, PinShape> *pin_shape_map, map<string, vector<PinAccessPoint>> *pin_access_point_map, string track_point_x, string track_point_y)
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
    float track_pitch = stof((*track_info).pitch) * 2;

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        getStripeRangeCost(&((*stripe_vector)[i]), &(*m3_track_point_map), track_pitch);
    }
    cout << "========== getAddStripeCost end ==========" << endl;
}

void getStripeRangeCost(Stripe *stripe, unordered_map<string, TrackPoint> *m3_track_point_map, float track_pitch)
{

    float vdd_stripe_cost = 100000.0;
    float vss_stripe_cost = 100000.0;
    string vdd_track = "0";
    string vss_track = "0";
    map<string, float> track_map;

    for (int i = 0; i < (*stripe).track_point_vector.size(); i++)
    {
        float total_pin_access_power_consum_cost = (*stripe).track_point_vector[i].total_pin_access_power_consum_cost;
        if (total_pin_access_power_consum_cost < vdd_stripe_cost)
        {
            vdd_stripe_cost = total_pin_access_power_consum_cost;
            vdd_track = (*stripe).track_point_vector[i].x_point;
        }
    }

    // ====== TODO 左右兩條不使用 ========
    float vdd_track_float = stof(vdd_track);
    float left_track_float = vdd_track_float - track_pitch;
    float right_track_float = vdd_track_float + track_pitch;
    string left_track = floatToString(left_track_float);
    string right_track = floatToString(right_track_float);
    track_map.insert(pair<string, float>(left_track, left_track_float));
    track_map.insert(pair<string, float>(right_track, right_track_float));
    // ====== TODO 左右兩條不使用 ========

    for (int i = 0; i < (*stripe).track_point_vector.size(); i++)
    {
        float total_pin_access_power_consum_cost = (*stripe).track_point_vector[i].total_pin_access_power_consum_cost;
        string x_track = (*stripe).track_point_vector[i].x_point;
        if (total_pin_access_power_consum_cost < vss_stripe_cost && (!(track_map).count(x_track)))
        {
            vss_stripe_cost = total_pin_access_power_consum_cost;
            vss_track = (*stripe).track_point_vector[i].x_point;
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
        cout << "========== range " << i << "  start ==========" << endl;
        float vdd_track_cost = stof((*stripe_vector)[i].vdd_track_x_cost);
        float vdd_left_track_cost = vdd_track_cost - (power_stripe_width_float / 2);
        float vdd_right_track_cost = vdd_track_cost + (power_stripe_width_float / 2);
        cout << "vdd_track_cost : " << vdd_track_cost << endl;
        cout << "vdd_left_track_cost : " << vdd_left_track_cost << endl;
        cout << "vdd_right_track_cost : " << vdd_right_track_cost << endl;

        string vdd_left_track_cost_str = floatToString(vdd_left_track_cost);
        string vdd_right_track_cost_str = floatToString(vdd_right_track_cost);

        float vss_track_x_cost = stof((*stripe_vector)[i].vss_track_x_cost);
        float vss_left_track_cost = vss_track_x_cost - (power_stripe_width_float / 2);
        float vss_right_track_cost = vss_track_x_cost + (power_stripe_width_float / 2);

        cout << "vss_track_cost : " << vdd_track_cost << endl;
        cout << "vss_left_track_cost : " << vdd_left_track_cost << endl;
        cout << "vss_right_track_cost : " << vdd_right_track_cost << endl;

        string vss_left_track_cost_str = floatToString(vss_left_track_cost);
        string vss_right_track_cost_str = floatToString(vss_right_track_cost);
        cout << "========== range " << i << "  end ==========" << endl;
        myfile << "addStripe -nets { VDDX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_left_track_cost_str << " " << (*stripe_vector)[i].start_y_location << " " << vdd_right_track_cost_str << " " << (*stripe_vector)[i].end_y_location << " }" << endl;
        myfile << "addStripe -nets { VSSX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_left_track_cost_str << " " << (*stripe_vector)[i].start_y_location << " " << vss_right_track_cost_str << " " << (*stripe_vector)[i].end_y_location << " }" << endl;
    }
    myfile.close();
};

void setRoutingTrackNumberOfPinAccess(string power_stripe_width, vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info)
{
    cout << "========== setRoutingTrackNumberOfPinAccess start ==========" << endl;
    float power_stripe_width_float = stof(power_stripe_width);

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        cout << " ======== stripe range " << i << " ===========" << endl;
        for (int j = 0; j < (*stripe_vector)[i].track_point_vector.size(); j++)
        {
            TrackPoint track_point = (*stripe_vector)[i].track_point_vector[j];
            float total_cell_pin_access_power_consume_cost = 0;

            for (int k = 0; k < track_point.power_cell_id_vector.size(); k++)
            {
                string cell_id = track_point.power_cell_id_vector[k];
                CellPlacedInfo cell_placed_info = (*cell_placed_map)[cell_id];

                // cost function
                float cell_pin_access_cost = getPinAccessPointOfPlaced(power_stripe_width_float, &track_point, &cell_placed_info, &(*cell_ip_map), &(*cell_info_map), &(*m3_track_info));

                float power_consuming_cost = ((*cell_ip_map)[cell_id].instance_total_power * 10000);
                power_consuming_cost = 1 / power_consuming_cost;

                float pin_access_power_consuming_cost = cell_pin_access_cost * power_consuming_cost;
                total_cell_pin_access_power_consume_cost += pin_access_power_consuming_cost;
            }
            (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = total_cell_pin_access_power_consume_cost;
        }
    }

    cout << "========== setRoutingTrackNumberOfPinAccess end ==========" << endl;
}

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

    if (isOddTrackCell(cell_placed_left_x_location, &(*m3_track_info)))
    {

        for (auto odd_pin_access_point_iter = ((*cell_info_map)[cell_name].odd_pin_access_point_map).begin(); odd_pin_access_point_iter != ((*cell_info_map)[cell_name].odd_pin_access_point_map).end(); ++odd_pin_access_point_iter)
        {
            int pin_access_point = 0;
            for (int i = 0; i < (odd_pin_access_point_iter->second).size(); i++)
            {
                PinAccessPoint pin_access_point_def;
                transferPinAccessLocationFromDef(&(*cell_placed_info), &(odd_pin_access_point_iter->second)[i], &pin_access_point_def, cell_width, cell_height);

                float pin_access_point_x_def_float = stof(pin_access_point_def.middle_x_location);
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
    }
    else
    {
        for (auto even_pin_access_point_iter = ((*cell_info_map)[cell_name].even_pin_access_point_map).begin(); even_pin_access_point_iter != ((*cell_info_map)[cell_name].even_pin_access_point_map).end(); ++even_pin_access_point_iter)
        {
            int pin_access_point = 0;
            for (int i = 0; i < (even_pin_access_point_iter->second).size(); i++)
            {
                PinAccessPoint pin_access_point_def;
                transferPinAccessLocationFromDef(&(*cell_placed_info), &(even_pin_access_point_iter->second)[i], &pin_access_point_def, cell_width, cell_height);
                float pin_access_point_x_def_float = stof(pin_access_point_def.middle_x_location);
                if (isCoverPinAccess(pin_access_point_x_def_float, m3_track_point_middle_float, power_stripe_width_float))
                {
                    pin_access_point += 1;
                }
            }
            float pin_access_cost = (float)pin_access_point / (even_pin_access_point_iter->second).size();
            total_pin_access_cost += pin_access_cost;
            // cout << "pin name : " << odd_pin_access_point_iter->first << endl;
            // cout << "pin_access_point : " << pin_access_point << " " << (odd_pin_access_point_iter->second).size() << endl;
            // cout << "pin cost : " << pin_access_cost << endl;
        }
    }
    if (total_pin_access_cost == 0)
    {
        total_pin_access_cost = 1;
    }

    return total_pin_access_cost;
}

bool isCoverPinAccess(float pin_access_point_x_def_float, float track_point_x_float, float power_stripe_width)
{
    // cover_track + 0.036
    int cover_track = getCoverTrack(power_stripe_width);
    float cover_distance = (cover_track * 0.144) + 0.036;
    float track_point_x_left = track_point_x_float - cover_distance;
    float track_point_x_right = track_point_x_float + cover_distance;

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

int getCoverTrack(float power_stripe_width)
{
    float half_power_stripe_width = power_stripe_width / 2;
    int temp_pitch = (int)(half_power_stripe_width / 0.144);
    int track = 0;
    float temp_width = temp_pitch * 0.144;
    temp_width = half_power_stripe_width - temp_width;
    if (floatIsEqualOrLess(temp_width, 0.036))
    {
        track = temp_pitch;
    }
    else
    {
        track = temp_pitch + 1;
    }
    return track;
}
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

bool isInTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width)
{

    float track_point_left = stof((*track_point).x_point) - (power_stripe_width / 2);
    float track_point_right = stof((*track_point).x_point) + (power_stripe_width / 2);
    float cell_placed_left = stof((*cell_placed_info).left_x_location);
    float cell_placed_right = stof((*cell_placed_info).right_x_location);
    // cout << " track_point " << track_point_left << " " << track_point_right << endl;
    // cout << " cell_placed " << cell_placed_left << " " << cell_placed_right << endl;

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
    cout << "========== isInTrackStripeRange end ==========" << endl;
}

void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site)
{
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

void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site, TrackInfo *track_info)
{
    cout << "========== setStripeRange start ==========" << endl;
    setRange(&(*vdd_stripe_vector), &(*core_site));
    setRange(&(*vss_stripe_vector), &(*core_site));
    float track_start = stof((*track_info).start);
    float track_pitch = stof((*track_info).pitch) * 2;

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        // cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << " " << endl;

        float move_range_x_left_float = stof((*vdd_stripe_vector)[i].move_range_x_left);
        float move_range_x_right_float = stof((*vdd_stripe_vector)[i].move_range_x_right);

        float move_range_left_x_float_temp = move_range_x_left_float - track_start;
        float temp_pitch_start = move_range_x_left_float / track_pitch;
        int temp_pitch_start_int = (int)temp_pitch_start;
        float start_x = (temp_pitch_start_int * track_pitch) + track_start;

        float move_range_x_right_float_temp = move_range_x_right_float - track_start;
        float temp_pitch_end = move_range_x_right_float_temp / track_pitch;
        int temp_pitch_end_int = (int)temp_pitch_end;

        temp_pitch_end_int -= 1;
        float end_x = (temp_pitch_end_int * track_pitch) + track_start;

        (*vdd_stripe_vector)[i].move_range_x_left = floatToString(start_x);
        (*vdd_stripe_vector)[i].move_range_x_right = floatToString(end_x);
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        // cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << " " << endl;

        float move_range_x_left_float = stof((*vss_stripe_vector)[i].move_range_x_left);
        float move_range_x_right_float = stof((*vss_stripe_vector)[i].move_range_x_right);

        float move_range_left_x_float_temp = move_range_x_left_float - track_start;
        float temp_pitch_start = move_range_x_left_float / track_pitch;
        int temp_pitch_start_int = (int)temp_pitch_start;
        float start_x = (temp_pitch_start_int * track_pitch) + track_start;

        float move_range_x_right_float_temp = move_range_x_right_float - track_start;
        float temp_pitch_end = move_range_x_right_float_temp / track_pitch;
        int temp_pitch_end_int = (int)temp_pitch_end;

        temp_pitch_end_int -= 1;
        float end_x = (temp_pitch_end_int * track_pitch) + track_start;

        (*vss_stripe_vector)[i].move_range_x_left = floatToString(start_x);
        (*vss_stripe_vector)[i].move_range_x_right = floatToString(end_x);
    }

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
    std::ostringstream oss;

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
