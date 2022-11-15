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

struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
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
    string middle_x_location;
    string middle_y_location;
};
struct CellInstancePowerInfo
{
    string cell_id;
    string cell_name;
    double instance_total_power;
};
struct Stripe
{
    string net_name;
    string layer;
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string move_range_x_left;
    string move_range_x_right;
    string move_range_y_up;
    string move_range_y_down;
    string width;
    string length;
    vector<string> ip_power_vector;
    vector<string> filler_vector;
    float range_total_power;
    float power_consuming_disatnce_cost;
    long double delta_ir_drop;
    float ir_drop_cost;
    float distance_cost;
    float distance_ir_drop_cost;
    string estimate_width;
};
struct TrackInfo
{
    string layer;
    string start;
    string pitch;
    string start_x_track_location;
    string end_x_track_location;
    string start_y_track_location;
    string end_y_track_location;
    string maximum_distacne;
};

struct Row
{
    string start_y_location;
    string end_y_location;
};

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

const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string MIDDLE = "middle";
const string UP = "UP";
const string DOWN = "DOWN";

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void generateTrackInfoMap(unordered_map<string, TrackInfo> *track_info_map);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void transferStripeToMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector);
void setStripeRange(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site);
void setOddLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setEvenLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void setStripeLength(Stripe *stripe);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *filler_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void reviseCellPlacedPosition(CellPlacedInfo *cell_placed_info);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getPowerOfStripe(unordered_map<string, vector<Stripe>> *vdd_stripe_map, vector<Row> *row_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getRow(CoreSite *core_site, vector<Row> *row_vector);
void setIpPowerInStripe(string layer, unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
// bool 判斷
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
bool sortOddStripeLocationVector(Stripe stripe_a, Stripe stripe_b);
bool isOddLayer(string layer);
bool sortEvenStripeLocationVector(Stripe stripe_a, Stripe stripe_b);
bool isInOddStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
bool isInEvenStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
float getOddConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float);
float getEvenConsumeRatio(string boudary, float down_y_location_float, float up_y_location_float, float move_range_y_down_float, float move_range_y_up_float);
// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
bool floatIsEqual(float a, float b);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);

int main(int argc, char *argv[])
// int main()
{
    cout << "-------------------------- decrease_power_stripe.cpp start --------------------------" << endl;
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellPlacedInfo> filler_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    unordered_map<string, vector<Stripe>> vdd_stripe_map;
    unordered_map<string, vector<Stripe>> vss_stripe_map;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    vector<Row> row_vector;
    // unordered_map<string, vector<Stripe>> resize_vdd_stripe_map;
    unordered_map<string, int> decreae_number_of_power_stripe_map;
    vector<string> ir_drop_file_vector;
    unordered_map<string, TrackInfo> track_info_map;

    // string config_file = argv[1];
    string config_file = "config/config_b19_28.txt";
    // string config_file = "config/config_b19_9.txt";
    string DEF_TRANSFER_FILE = "";
    string LEF_FILE = "";
    string IP_REPORT_FILE = "";
    string DECREASE_STRIPE_TCL = "";
    string DECREASE_WIRE_STRIPE_TCL = "";
    string IR_REPORT_FILE = "";
    string LOG_FILE = "";
    string ESTIMATE_WIDTH_STRIPE_TCL = "";

    ifstream config(config_file);
    string config_content;
    if (config)
    {
        while (getline(config, config_content))
        {
            vector<string> config_content_array = splitByPattern(config_content, " ");
            // std::cout << config_content_array[0] << " " << config_content_array[2] << std::endl;
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
            if (config_content_array[0] == "DECREASE_WIRE_STRIPE_TCL")
            {
                DECREASE_WIRE_STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "DECREASE_STRIPE_TCL")
            {
                DECREASE_STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "LOG_FILE")
            {
                LOG_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "ESTIMATE_WIDTH_STRIPE_TCL")
            {
                ESTIMATE_WIDTH_STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "IR_REPORT_FILE")
            {
                // IR_REPORT_FILE = config_content_array[2];
                ir_drop_file_vector.push_back(config_content_array[2]);
            }
        }
    }

    double START, END;
    START = clock();

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    generateTrackInfoMap(&track_info_map);
    getRow(&core_site, &row_vector);

    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    transferStripeToMap(&vdd_stripe_map, &vss_stripe_map, &vdd_stripe_vector, &vss_stripe_vector);

    setStripeRange(&vdd_stripe_map, &vss_stripe_map, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &filler_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);

    setIpPowerInStripe("M3", &vdd_stripe_map, &cell_ip_map, &cell_placed_map);
    getPowerOfStripe(&vdd_stripe_map, &row_vector, &cell_placed_map, &cell_ip_map);

    cout << endl
         << "Program excute time : " << (double)clock() / CLOCKS_PER_SEC << " S" << endl;

    return 0;
}
//每一個row跑了多少ir_drop

void getPowerOfStripe(unordered_map<string, vector<Stripe>> *vdd_stripe_map, vector<Row> *row_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{
    string layer = "M3";
    string stripe_x_location = "164.772";

    vector<Stripe> stripe_vector = (*vdd_stripe_map)[layer];

    for (int i = 0; i < stripe_vector.size(); i++)
    {
        if (stripe_vector[i].start_x_location == stripe_x_location)
        {

            for (int j = 0; j < (*row_vector).size(); j++)
            {
                float start_y_location_float = stof((*row_vector)[j].start_y_location);
                float end_y_location_float = stof((*row_vector)[j].end_y_location);
                double ip_power_double = 0;

                for (int k = 0; k < stripe_vector[i].ip_power_vector.size(); k++)
                {
                    string cell_id = stripe_vector[i].ip_power_vector[k];
                    float down_y_location_float = stof((*cell_placed_map)[cell_id].down_y_location);
                    float up_y_location_float = stof((*cell_placed_map)[cell_id].up_y_location);

                    if (floatIsEqualOrMore(down_y_location_float, end_y_location_float) && floatIsEqualOrLess(up_y_location_float, start_y_location_float))
                    {
                        double instance_power = (*cell_ip_map)[cell_id].instance_total_power;
                        ip_power_double += instance_power;
                    }
                }
                cout << " start y location : " << (*row_vector)[j].start_y_location << " end y location : " << (*row_vector)[j].end_y_location << " power : " << ip_power_double << endl;
            }
        }
    }
}

void setIpPowerInStripe(string layer, unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    cout << "========== start setIpPowerInStripe ==========" << endl;

    if (isOddLayer(layer))
    {
        cout << "========== start setIpPowerInStripe isOddLayer ==========" << endl;
        for (auto cell_ip_it = (*cell_ip_map).begin(); cell_ip_it != (*cell_ip_map).end(); ++cell_ip_it)
        {
            string cell_id = cell_ip_it->first;
            CellInstancePowerInfo cell_instance_power_info = cell_ip_it->second;
            for (int i = 0; i < (*stripe_map)[layer].size(); i++)
            {
                if (isInOddStripeRange(&(*stripe_map)[layer][i], cell_id, &(*cell_placed_map), &(*cell_ip_map)))
                {
                    (*stripe_map)[layer][i].ip_power_vector.push_back(cell_id);
                }
            }
        }
        cout << "========== end setIpPowerInStripe isOddLayer ==========" << endl;
    }
    else
    {
        cout << "========== start setIpPowerInStripe isEvenLayer ==========" << endl;
        for (auto cell_ip_it = (*cell_ip_map).begin(); cell_ip_it != (*cell_ip_map).end(); ++cell_ip_it)
        {
            string cell_id = cell_ip_it->first;
            CellInstancePowerInfo cell_instance_power_info = cell_ip_it->second;
            for (int i = 0; i < (*stripe_map)[layer].size(); i++)
            {
                if (isInEvenStripeRange(&(*stripe_map)[layer][i], cell_id, &(*cell_placed_map), &(*cell_ip_map)))
                {
                    (*stripe_map)[layer][i].ip_power_vector.push_back(cell_id);
                }
            }
        }
        cout << "========== end setIpPowerInStripe isEvenLayer ==========" << endl;
    }
    cout << "========== end setIpPowerInStripe ==========" << endl;

    for (auto stripe_map_it = (*stripe_map).begin(); stripe_map_it != (*stripe_map).end(); ++stripe_map_it)
    {
        string layer = stripe_map_it->first;
        int total_cell_ip = 0;
        for (int i = 0; i < stripe_map_it->second.size(); i++)
        {
            total_cell_ip += stripe_map_it->second[i].ip_power_vector.size();
        };
        cout << "layer : " << layer << endl;
        cout << "ip size : " << total_cell_ip << endl;
    }
}
bool isInEvenStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    // if (cell_id == "mult")
    // {
    //     cout << "check " << endl;
    // }

    string up_y_location = (*cell_placed_map)[cell_id].up_y_location;
    string down_y_location = (*cell_placed_map)[cell_id].down_y_location;

    float move_range_y_down_float = stof((*stripe_vector).move_range_y_down);
    float move_range_y_up_float = stof((*stripe_vector).move_range_y_up);

    float up_y_location_float = stof(up_y_location);
    float down_y_location_float = stof(down_y_location);

    // case 1 : middle of down stripe
    if ((up_y_location_float > move_range_y_down_float) && (down_y_location_float < move_range_y_down_float) && (up_y_location_float < move_range_y_up_float))
    {

        float ratio = getEvenConsumeRatio(DOWN, down_y_location_float, up_y_location_float, move_range_y_down_float, move_range_y_up_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*stripe_vector).range_total_power += power;

        return true;

    } // case 2 : middle of up stripe
    else if ((up_y_location_float > move_range_y_up_float) && (down_y_location_float < move_range_y_up_float) && (down_y_location_float > move_range_y_down_float))
    {
        float ratio = getEvenConsumeRatio(UP, down_y_location_float, up_y_location_float, move_range_y_down_float, move_range_y_up_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*stripe_vector).range_total_power += power;

        return true;
    } // case 3 : over all of stripe range
    else if ((up_y_location_float > move_range_y_up_float) && (down_y_location_float < move_range_y_down_float))
    {
        float ratio = getEvenConsumeRatio(MIDDLE, down_y_location_float, up_y_location_float, move_range_y_down_float, move_range_y_up_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*stripe_vector).range_total_power += power;
        return true;
    } // case 4 : in power srtripe range
    else if (floatIsEqualOrLess(up_y_location_float, move_range_y_up_float) && floatIsEqualOrMore(up_y_location_float, move_range_y_down_float) && floatIsEqualOrLess(down_y_location_float, move_range_y_up_float) && floatIsEqualOrMore(down_y_location_float, move_range_y_down_float))
    {
        float power = (*cell_ip_map)[cell_id].instance_total_power * 1.0;
        // cout << "--------------------------------" << power << endl;
        // cout << "power                                    : " << power << endl;
        // cout << "(*stripe_vector).range_total_power       : " << (*stripe_vector).range_total_power << endl;
        (*stripe_vector).range_total_power += power;
        // cout << "after (*stripe_vector).range_total_power : " << (*stripe_vector).range_total_power << endl;

        return true;
    }
    else
    {
        return false;
    }
}
float getEvenConsumeRatio(string boudary, float down_y_location_float, float up_y_location_float, float move_range_y_down_float, float move_range_y_up_float)
{
    float ratio;
    if (boudary == UP)
    {
        float height = up_y_location_float - down_y_location_float;
        float space = move_range_y_up_float - down_y_location_float;
        ratio = space / height;
    }
    else if (boudary == DOWN)
    {
        float height = up_y_location_float - down_y_location_float;
        float space = up_y_location_float - move_range_y_down_float;
        ratio = space / height;
    }
    else
    {
        float height = up_y_location_float - down_y_location_float;
        float space = move_range_y_up_float - move_range_y_down_float;
        ratio = space / height;
    }
    string ratio_str = floatToString(ratio);
    ratio = stof(ratio_str);
    return ratio;
}
bool isInOddStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    string left_x_location = (*cell_placed_map)[cell_id].left_x_location;
    string right_x_location = (*cell_placed_map)[cell_id].right_x_location;

    float move_range_x_left_float = stof((*stripe_vector).move_range_x_left);
    float move_range_x_right_float = stof((*stripe_vector).move_range_x_right);

    float left_x_location_float = stof(left_x_location);
    float right_x_location_float = stof(right_x_location);

    // case 1 : middle of left stripe
    if ((left_x_location_float < move_range_x_left_float) && (right_x_location_float > move_range_x_left_float) && (left_x_location_float < move_range_x_right_float) && (right_x_location_float < move_range_x_right_float))
    {
        float ratio = getOddConsumeRatio(LEFT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;

        (*stripe_vector).range_total_power += power;

        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        float power = (*cell_ip_map)[cell_id].instance_total_power;
        (*stripe_vector).range_total_power += power;

        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        float ratio = getOddConsumeRatio(RIGHT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;
        (*stripe_vector).range_total_power += power;

        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
        float ratio = getOddConsumeRatio(MIDDLE, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);

        float power = (*cell_ip_map)[cell_id].instance_total_power * ratio;

        (*stripe_vector).range_total_power += power;

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
float getOddConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float)
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
    string ratio_str = floatToString(ratio);
    ratio = stof(ratio_str);

    return ratio;
}

// 275.328 0.864

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

    for (int i = 0; i < (*row_vector).size(); i++)
    {
        cout << (*row_vector)[i].start_y_location << " " << (*row_vector)[i].end_y_location << endl;
    }

    // for (float i = core_end_y_location_float; floatIsEqualOrMore(i, core_start_y_location_float); i -= row_float)
    // {

    //     float row_start_y_location_float = i;
    //     float row_end_y_location_float = i - row_float;
    //     string row_start_y_location = floatToString(row_start_y_location_float);
    //     string row_end_y_location = floatToString(row_end_y_location_float);

    //     Row row;
    //     row.start_y_location = row_start_y_location;
    //     row.end_y_location = row_end_y_location;
    //     (*row_vector).push_back(row);
    // }
}

bool sortStripeLocationY(Row row_a, Row row_b)
{
    return stof(row_a.start_y_location) < stof(row_b.start_y_location);
}

void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    ifstream ip_file(ip_report);
    string ip_content;
    int log = 0;
    double power = 0;
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
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *filler_placed_map, unordered_map<string, CellInfo> *cell_info_map)
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

                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        CellPlacedInfo cell_placed_info;
                        cell_placed_info.cell_id = def_content_array[1];
                        cell_placed_info.cell_name = def_content_array[2];
                        setPlacePosition(&def_content_array, &cell_placed_info);
                        getCellLocation(&cell_placed_info, &(*cell_info_map));

                        if (cell_placed_info.cell_id.find("FILLER") != string::npos)
                        {
                            (*filler_placed_map).insert(pair<string, CellPlacedInfo>(cell_placed_info.cell_id, cell_placed_info));
                        }
                        else
                        {
                            (*cell_placed_map).insert(pair<string, CellPlacedInfo>(cell_placed_info.cell_id, cell_placed_info));
                        }
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
    // if ((*cell_placed_info).cell_id == "U40157")
    // {
    //     cout << "break" << endl;
    // }

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

        float middle_x_location = (left_x_float + right_x_float) / 2;
        float middle_y_location = (down_y_float + up_y_float) / 2;

        (*cell_placed_info).right_x_location = floatToString(right_x_float);
        (*cell_placed_info).up_y_location = floatToString(up_y_float);

        (*cell_placed_info).middle_x_location = floatToString(middle_x_location);
        (*cell_placed_info).middle_y_location = floatToString(middle_y_location);
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
            reviseCellPlacedPosition(&(*cell_placed_info));
            break;
        }
    }
}
void reviseCellPlacedPosition(CellPlacedInfo *cell_placed_info)
{
    float left_x_location_float = stof((*cell_placed_info).left_x_location);
    float down_y_location_float = stof((*cell_placed_info).down_y_location);

    if (left_x_location_float > 4000)
    {
        // cout << "check in side : " << left_x_location_float << endl;
        left_x_location_float = left_x_location_float / 4000;
        (*cell_placed_info).left_x_location = floatToString(left_x_location_float);
    }
    if (down_y_location_float > 4000)
    {
        // cout << "check in side : " << down_y_location_float << endl;
        down_y_location_float = down_y_location_float / 4000;
        (*cell_placed_info).down_y_location = floatToString(down_y_location_float);
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
void setStripeRange(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site)
{
    cout << "========== setStripeRange start ==========" << endl;
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        if (isOddLayer(layer))
        {
            setOddLayerRange(&((*vdd_stripe_map)[layer]), &(*core_site));
        }
        else
        {
            setEvenLayerRange(&((*vdd_stripe_map)[layer]), &(*core_site));
        };
    }
    cout << "========== setStripeRange end ==========" << endl;
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
void setEvenLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
{
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            (*stripe_vector)[i].move_range_y_down = (*core_site).down_y_location;
            float middle_y_power_stripe = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i + 1].start_y_location)) / 2;
            (*stripe_vector)[i].move_range_y_up = floatToString(middle_y_power_stripe);
        }
        else if (i == ((*stripe_vector).size() - 1))
        {
            (*stripe_vector)[i].move_range_y_up = (*core_site).up_y_location;
            float middle_y_power_stripe = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i - 1].start_y_location)) / 2;
            (*stripe_vector)[i].move_range_y_down = floatToString(middle_y_power_stripe);
        }
        else
        {
            float middle_y_power_stripe_up = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i + 1].start_y_location)) / 2;
            float middle_y_power_stripe_down = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i - 1].start_y_location)) / 2;
            (*stripe_vector)[i].move_range_y_up = floatToString(middle_y_power_stripe_up);
            (*stripe_vector)[i].move_range_y_down = floatToString(middle_y_power_stripe_down);
        }
    }
}
void setOddLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
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
        // cout << "stripe x_location : " << (*stripe_vector)[i].start_x_location << " " << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << endl;
    }
}

void transferStripeToMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector)
{
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        if ((*vdd_stripe_map).count((*vdd_stripe_vector)[i].layer))
        {
            (*vdd_stripe_map)[(*vdd_stripe_vector)[i].layer].push_back((*vdd_stripe_vector)[i]);
        }
        else
        {
            vector<Stripe> layer_vdd_stripe_vector;
            layer_vdd_stripe_vector.push_back((*vdd_stripe_vector)[i]);
            string layer = (*vdd_stripe_vector)[i].layer;
            (*vdd_stripe_map).insert(pair<string, vector<Stripe>>(layer, layer_vdd_stripe_vector));
        }
    }
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        vector<Stripe> stripe_vector = (*vdd_stripe_map)[layer];
        if (isOddLayer(layer))
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            (*vdd_stripe_map)[layer] = stripe_vector;
        }
        else
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            (*vdd_stripe_map)[layer] = stripe_vector;
        }
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        if ((*vss_stripe_map).count((*vss_stripe_vector)[i].layer))
        {
            (*vss_stripe_map)[(*vss_stripe_vector)[i].layer].push_back((*vss_stripe_vector)[i]);
        }
        else
        {
            vector<Stripe> layer_vss_stripe_vector;
            layer_vss_stripe_vector.push_back((*vss_stripe_vector)[i]);
            string layer = (*vss_stripe_vector)[i].layer;
            (*vss_stripe_map).insert(pair<string, vector<Stripe>>(layer, layer_vss_stripe_vector));
        }
    }
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        vector<Stripe> stripe_vector = (*vss_stripe_map)[layer];
        if (isOddLayer(layer))
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            (*vss_stripe_map)[layer] = stripe_vector;
        }
        else
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            (*vss_stripe_map)[layer] = stripe_vector;
        }
    }
}
bool sortOddStripeLocationVector(Stripe stripe_a, Stripe stripe_b)
{
    return stof(stripe_a.start_x_location) < stof(stripe_b.start_x_location);
}
bool sortEvenStripeLocationVector(Stripe stripe_a, Stripe stripe_b)
{
    return stof(stripe_a.start_y_location) < stof(stripe_b.start_y_location);
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
                            stripe.end_x_location = def_content_array[11];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.layer = def_content_array[1];
                            stripe.net_name = NET_NAME_VDD;
                            vector<string> ip_power_vector;
                            stripe.ip_power_vector = ip_power_vector;

                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                setStripeLength(&stripe);
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
                            stripe.end_x_location = def_content_array[11];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.layer = def_content_array[1];
                            stripe.net_name = NET_NAME_VSS;
                            vector<string> ip_power_vector;
                            stripe.ip_power_vector = ip_power_vector;
                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                setStripeLength(&stripe);
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
        (*stripe).length = floatToString(length);
    }
}

void generateTrackInfoMap(unordered_map<string, TrackInfo> *track_info_map)
{
    // TRACKS Y 0.18 DO 0.33025 STEP 0.216 LAYER M4 ;
    // TRACKS X 0.18 DO 0.611 STEP 0.144 LAYER M3;
    TrackInfo m3_track_info;
    TrackInfo m4_track_info;

    string m3_x_pitch = "0.144";
    string m3_x_start = "0.18";
    string m4_x_pitch = "0.216";
    string m4_x_start = "0.18";
    m3_track_info.layer = "M3";
    m3_track_info.start = m3_x_start;
    m3_track_info.pitch = m3_x_pitch;
    m4_track_info.layer = "M4";
    m4_track_info.start = m4_x_start;
    m4_track_info.pitch = m4_x_pitch;
    (*track_info_map).insert(pair<string, TrackInfo>(m3_track_info.layer, m3_track_info));
    (*track_info_map).insert(pair<string, TrackInfo>(m4_track_info.layer, m4_track_info));
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
    else if (((*stripe).start_y_location == (*stripe).end_y_location))
    {
        float x_distance = abs(stof((*stripe).start_x_location) - stof((*stripe).end_x_location));
        float core_site_x_distance = abs(stof((*core_site).left_x_location) - stof((*core_site).right_x_location)) / 2;
        if (x_distance < core_site_x_distance)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
        cout << " isPowerStripe error  << " << endl;
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