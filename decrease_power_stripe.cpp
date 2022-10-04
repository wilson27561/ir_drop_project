
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

const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string MIDDLE = "middle";
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string M3 = "M3";
const string M4 = "M4";
const string WIRE_STRIPE = "4";

struct FollowPin
{
    string layer;
    string y_location;
};
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
};
struct CellInstancePowerInfo
{
    string cell_id;
    string cell_name;
    double instance_total_power;
};
struct IrDropPoint
{
    string ir_drop;
    string layer;
    string x_location;
    string y_location;
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
    string height;
    vector<string> ip_power_vector;
    vector<string> filler_vector;
    long double delta_ir_drop;
    float range_total_power;
    float power_consuming_cost;
};

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *filler_placed_map, unordered_map<string, CellInfo> *cell_info_map);
vector<string> splitByPattern(string content, string pattern);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
string &trim(string &str);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
float getConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float);
bool isInStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
// void setIpPowerInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void setIpPowerInStripe(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void getFollowPin(string DEF_TRANSFER_FILE_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
string floatToString(const float value);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setEvenLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setOddLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setStripeRange(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site);
bool isInStripeRangeFiller(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *filler_placed_map);
void setFillerIStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *filler_placed_map);
void caculatePowerStripeCost(vector<Stripe> *stripe_vector);
bool sortStripeVector(Stripe stripe_a, Stripe stripe_b);
void transferStripeToMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector);
void resizePowerStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector);
void setIrDropReport(string ir_drop_report_file, vector<IrDropPoint> *ir_drop_point_vector);
bool isInPowerStripeWidth(Stripe *stripe, string power_stripe_width, IrDropPoint *ir_drop_point);
void setIrInPowerStripe(vector<Stripe> *resize_vdd_stripe_vector, vector<IrDropPoint> *ir_drop_point_vector);
int wirePowerStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector);
bool sortDeltaIrDrop(Stripe stripe_a, Stripe stripe_b);
bool isOddLayer(string layer);
void generateDecreaseStripeTcl(vector<Stripe> *resize_power_stripe, vector<Stripe> *vss_stripe_vector, string stripe_tcl);
void generateWireStripeTcl(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, string stripe_tcl, int number_of_power_stripe_wire);
int main()
{
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellPlacedInfo> filler_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    unordered_map<string, vector<Stripe>> vdd_stripe_map;
    unordered_map<string, vector<Stripe>> vss_stripe_map;
    vector<IrDropPoint> ir_drop_point_vector;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    vector<Stripe> resize_vdd_stripe_vector;

    vector<FollowPin> follow_pin_vdd_vector;
    vector<FollowPin> follow_pin_vss_vector;

    string DEF_TRANSFER_FILE = "";
    string LEF_FILE = "";
    string IP_REPORT_FILE = "";
    string DECREASE_STRIPE_TCL = "";
    string DECREASE_WIRE_STRIPE_TCL = "";
    string IR_REPORT_FILE = "";
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
            if (config_content_array[0] == "DECREASE_WIRE_STRIPE_TCL")
            {
                DECREASE_WIRE_STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "DECREASE_STRIPE_TCL")
            {
                DECREASE_STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "IR_REPORT_FILE")
            {
                IR_REPORT_FILE = config_content_array[2];
            }
        }
    }

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    setStripeRange(&vdd_stripe_map, &vss_stripe_map, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &filler_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    setIpPowerInStripe(&vdd_stripe_map, &cell_ip_map, &cell_placed_map);
    // getFollowPin(DEF_TRANSFER_FILE, &follow_pin_vdd_vector, &follow_pin_vss_vector);
    // setFillerIStripe(&vdd_stripe_vector, &filler_placed_map);
    // caculatePowerStripeCost(&vdd_stripe_vector);
    // resizePowerStripe(&vdd_stripe_vector, &resize_vdd_stripe_vector);
    // // generateDecreaseStripeTcl(&resize_vdd_stripe_vector, &vss_stripe_vector, DECREASE_STRIPE_TCL);
    // setIrDropReport(IR_REPORT_FILE, &ir_drop_point_vector);
    // setIrInPowerStripe(&resize_vdd_stripe_vector, &ir_drop_point_vector);
    // int number_of_power_stripe_wire = wirePowerStripe(&vdd_stripe_vector, &resize_vdd_stripe_vector);

    // generateWireStripeTcl(&resize_vdd_stripe_vector, &vss_stripe_vector, DECREASE_WIRE_STRIPE_TCL, number_of_power_stripe_wire);

    // float total_power = 0;
    // for (int i = 0; i < resize_vdd_stripe_vector.size(); i++)
    // {
    //     cout << " ir delta : " <<resize_vdd_stripe_vector[i].delta_ir_drop << endl;
    // }
}
void setIpPowerInStripe(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    for (auto stripe_map_it = (*stripe_map).begin(); stripe_map_it != (*stripe_map).end(); ++stripe_map_it)
    {
        string layer = stripe_map_it->first;
        if (isOddLayer(layer))
        {
            for (auto cell_ip_it = (*cell_ip_map).begin(); cell_ip_it != (*cell_ip_map).end(); ++cell_ip_it)
            {
                string cell_id = cell_ip_it->first;
                CellInstancePowerInfo cell_instance_power_info = cell_ip_it->second;
               


            }
        }
        else
        {
        }
    }

    // for (int i = 0; i < (*stripe_vector).size(); i++)
    // {
    //     for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
    //     {
    //         string cell_id = iter->first;
    //         CellInstancePowerInfo cell_instance_power_info = iter->second;

    //         if (isInStripeRange(&((*stripe_vector)[i]), cell_id, &(*cell_placed_map), &(*cell_ip_map)))
    //         {
    //             (*stripe_vector)[i].ip_power_vector.push_back(cell_id);
    //         }
    //     }
    // }
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
    string ratio_str = floatToString(ratio);
    ratio = stof(ratio_str);
    return ratio;
}
bool isInEvenStripeRange(){

    return false;
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
        float ratio = getConsumeRatio(LEFT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        long double power = (*cell_ip_map)[cell_id].instance_total_power * ratio;

        (*stripe_vector).range_total_power += power;

        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        long double power = (*cell_ip_map)[cell_id].instance_total_power;

        (*stripe_vector).range_total_power += power;

        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        float ratio = getConsumeRatio(RIGHT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        long double power = (*cell_ip_map)[cell_id].instance_total_power * ratio;

        (*stripe_vector).range_total_power += power;

        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
        float ratio = getConsumeRatio(MIDDLE, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);

        long double power = (*cell_ip_map)[cell_id].instance_total_power * ratio;

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

// void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
// {
//     cout << "========== setStripeRange start ==========" << endl;
//     setRange(&(*vdd_stripe_vector), &(*core_site));
//     setRange(&(*vss_stripe_vector), &(*core_site));

//     cout << "========== setStripeRange end ==========" << endl;
// }

// void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
// {

//     for (int i = 0; i < (*stripe_vector).size(); i++)
//     {

//         if (i == 0)
//         {
//             (*stripe_vector)[i].move_range_x_left = (*core_site).left_x_location;
//             float middle_x_power_stripe = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
//             (*stripe_vector)[i].move_range_x_right = floatToString(middle_x_power_stripe);
//         }
//         else if (i == ((*stripe_vector).size() - 1))
//         {

//             (*stripe_vector)[i].move_range_x_right = (*core_site).right_x_location;
//             float middle_x_power_stripe = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
//             (*stripe_vector)[i].move_range_x_left = floatToString(middle_x_power_stripe);
//         }
//         else
//         {
//             float middle_x_power_stripe_right = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
//             float middle_x_power_stripe_left = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
//             (*stripe_vector)[i].move_range_x_right = floatToString(middle_x_power_stripe_right);
//             (*stripe_vector)[i].move_range_x_left = floatToString(middle_x_power_stripe_left);
//         }
//     }
// }
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
}

int wirePowerStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector)
{
    // get power stripe width
    string power_stripe_width = (*resize_vdd_stripe_vector)[0].width;

    //總共有幾條 power stripe 可以被加寬
    int decreas_size = (*vdd_stripe_vector).size() - (*resize_vdd_stripe_vector).size();
    float wide_wire_float = stof(WIRE_STRIPE);
    float power_stripe_width_float = stof(power_stripe_width);
    float width = power_stripe_width_float * wide_wire_float;

    float temp_wire = width - power_stripe_width_float;
    float total_width = decreas_size * power_stripe_width_float;

    float number_of_power_stripe = (total_width / temp_wire);
    string number_of_power_stripe_str = floatToString(number_of_power_stripe);
    int number_of_power_stripe_int = stoi(number_of_power_stripe_str);
    sort((*resize_vdd_stripe_vector).begin(), (*resize_vdd_stripe_vector).end(), sortDeltaIrDrop);

    return number_of_power_stripe_int;
}

bool sortIrDropPoint(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b)
{
    return (stof(ir_drop_point_a.ir_drop) > stof(ir_drop_point_b.ir_drop));
}
bool sortDeltaIrDrop(Stripe stripe_a, Stripe stripe_b)
{
    return (stripe_a.delta_ir_drop > stripe_b.delta_ir_drop);
}

void setIrInPowerStripe(vector<Stripe> *resize_vdd_stripe_vector, vector<IrDropPoint> *ir_drop_point_vector)
{
    for (int i = 0; i < (*resize_vdd_stripe_vector).size(); i++)
    {
        vector<IrDropPoint> sort_ir_drop_point_vector;
        for (int j = 0; j < (*ir_drop_point_vector).size(); j++)
        {
            if (isInPowerStripeWidth(&(*resize_vdd_stripe_vector)[i], (*resize_vdd_stripe_vector)[i].width, &(*ir_drop_point_vector)[j]))
            {
                sort_ir_drop_point_vector.push_back((*ir_drop_point_vector)[j]);
            }
        }
        sort(sort_ir_drop_point_vector.begin(), sort_ir_drop_point_vector.end(), sortIrDropPoint);
        int last_index = sort_ir_drop_point_vector.size() - 1;
        float delta_ir_drop = stof(sort_ir_drop_point_vector[0].ir_drop) - stof(sort_ir_drop_point_vector[last_index].ir_drop);
        (*resize_vdd_stripe_vector)[i].delta_ir_drop = delta_ir_drop;
    }
}

bool isInPowerStripeWidth(Stripe *stripe, string power_stripe_width, IrDropPoint *ir_drop_point)
{
    string start_x_location = (*stripe).start_x_location;
    float power_stripe_width_float = stof(power_stripe_width);
    float x_left_float = stof(start_x_location) - power_stripe_width_float;
    float x_right_float = stof(start_x_location) + power_stripe_width_float;

    float ir_drop_point_x_location = stof((*ir_drop_point).x_location);

    if (ir_drop_point_x_location >= x_left_float && ir_drop_point_x_location <= x_right_float)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void resizePowerStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector)
{
    vector<Stripe> sort_cost_vector;
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        sort_cost_vector.push_back((*vdd_stripe_vector)[i]);
    }
    sort(sort_cost_vector.begin(), sort_cost_vector.end(), sortStripeVector);

    int decrease_power_stripe = (*vdd_stripe_vector).size() * 0.2;

    for (int i = 0; i < decrease_power_stripe; i++)
    {
        sort_cost_vector.pop_back();
    };

    for (int i = 0; i < sort_cost_vector.size(); i++)
    {
        (*resize_vdd_stripe_vector).push_back(sort_cost_vector[i]);
    }
}

void generateDecreaseStripeTcl(vector<Stripe> *resize_power_stripe, vector<Stripe> *vss_stripe_vector, string stripe_tcl)
{

    ofstream myfile;
    myfile.open(stripe_tcl);
    for (int i = 0; i < (*resize_power_stripe).size(); i++)
    {

        float power_stripe_width_float = stof((*resize_power_stripe)[i].width);
        string power_stripe_width = floatToString(power_stripe_width_float);
        float vdd_start_x_location_float = stof((*resize_power_stripe)[i].start_x_location) - (power_stripe_width_float / 2);
        float vdd_end_x_location_float = stof((*resize_power_stripe)[i].start_x_location) + (power_stripe_width_float / 2);
        string vdd_start_x_location = floatToString(vdd_start_x_location_float);
        string vdd_end_x_location = floatToString(vdd_end_x_location_float);
        myfile << "addStripe -nets { VDDX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_start_x_location << " " << (*resize_power_stripe)[i].start_y_location << " " << vdd_end_x_location << " " << (*resize_power_stripe)[i].end_y_location << " }" << endl;
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        float power_stripe_width_float = stof((*vss_stripe_vector)[i].width);
        string power_stripe_width = floatToString(power_stripe_width_float);
        float vss_start_x_location_float = stof((*vss_stripe_vector)[i].start_x_location) - (power_stripe_width_float / 2);
        float vss_end_x_location_float = stof((*vss_stripe_vector)[i].end_x_location) + (power_stripe_width_float / 2);
        string vss_start_x_location = floatToString(vss_start_x_location_float);
        string vss_end_x_location = floatToString(vss_end_x_location_float);

        myfile << "addStripe -nets { VSSX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_start_x_location << " " << (*vss_stripe_vector)[i].start_y_location << " " << vss_end_x_location << " " << (*vss_stripe_vector)[i].end_y_location << " }" << endl;
    }
    myfile.close();
}

void generateWireStripeTcl(vector<Stripe> *resize_power_stripe, vector<Stripe> *vss_stripe_vector, string stripe_tcl, int number_of_power_stripe_wire)
{
    // get power stripe width
    string power_stripe_width = (*resize_power_stripe)[0].width;

    float wire_multiples = stof(WIRE_STRIPE);
    float power_stripe_width_float = stof(power_stripe_width);
    float wire_power_stripe_width_float = power_stripe_width_float * wire_multiples;
    string wire_power_stripe_width_str = floatToString(wire_power_stripe_width_float);
    wire_power_stripe_width_float = stof(wire_power_stripe_width_str);

    ofstream myfile;
    myfile.open(stripe_tcl);

    for (int i = 0; i < (*resize_power_stripe).size(); i++)
    {
        if (number_of_power_stripe_wire != 0)
        {
            float vdd_start_x_location_float = stof((*resize_power_stripe)[i].start_x_location) - (wire_power_stripe_width_float / 2);
            float vdd_end_x_location_float = stof((*resize_power_stripe)[i].start_x_location) + (wire_power_stripe_width_float / 2);
            string vdd_start_x_location = floatToString(vdd_start_x_location_float);
            string vdd_end_x_location = floatToString(vdd_end_x_location_float);
            myfile << "addStripe -nets { VDDX } -layer "
                   << "M3"
                   << " -direction vertical -width " << wire_power_stripe_width_float << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_start_x_location << " " << (*resize_power_stripe)[i].start_y_location << " " << vdd_end_x_location << " " << (*resize_power_stripe)[i].end_y_location << " }" << endl;

            number_of_power_stripe_wire -= 1;
        }
        else
        {
            float vdd_start_x_location_float = stof((*resize_power_stripe)[i].start_x_location) - (power_stripe_width_float / 2);
            float vdd_end_x_location_float = stof((*resize_power_stripe)[i].start_x_location) + (power_stripe_width_float / 2);
            string vdd_start_x_location = floatToString(vdd_start_x_location_float);
            string vdd_end_x_location = floatToString(vdd_end_x_location_float);
            myfile << "addStripe -nets { VDDX } -layer "
                   << "M3"
                   << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_start_x_location << " " << (*resize_power_stripe)[i].start_y_location << " " << vdd_end_x_location << " " << (*resize_power_stripe)[i].end_y_location << " }" << endl;
        }
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        float vss_start_x_location_float = stof((*vss_stripe_vector)[i].start_x_location) - (power_stripe_width_float / 2);
        float vss_end_x_location_float = stof((*vss_stripe_vector)[i].end_x_location) + (power_stripe_width_float / 2);
        string vss_start_x_location = floatToString(vss_start_x_location_float);
        string vss_end_x_location = floatToString(vss_end_x_location_float);

        myfile << "addStripe -nets { VSSX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_start_x_location << " " << (*vss_stripe_vector)[i].start_y_location << " " << vss_end_x_location << " " << (*vss_stripe_vector)[i].end_y_location << " }" << endl;
    }

    myfile.close();
};

bool sortStripeVector(Stripe stripe_a, Stripe stripe_b)
{
    return stripe_a.power_consuming_cost > stripe_b.power_consuming_cost;
}

void caculatePowerStripeCost(vector<Stripe> *stripe_vector)
{

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {

        float power_consume_cost = (*stripe_vector)[i].range_total_power * 10.0;

        (*stripe_vector)[i].power_consuming_cost = power_consume_cost;
        // sort_cost_vector.push_back((*stripe_vector)[i]);

        cout << "range_total_power : " << power_consume_cost << endl;
    }
}

void setFillerIStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *filler_placed_map)
{
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        for (auto iter = (*filler_placed_map).begin(); iter != (*filler_placed_map).end(); ++iter)
        {
            string cell_id = iter->first;

            if (isInStripeRangeFiller(&((*stripe_vector)[i]), cell_id, &(*filler_placed_map)))
            {
                (*stripe_vector)[i].filler_vector.push_back(cell_id);
            }
        }
    }
}
bool isInStripeRangeFiller(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *filler_placed_map)
{

    string left_x_location = (*filler_placed_map)[cell_id].left_x_location;
    string right_x_location = (*filler_placed_map)[cell_id].right_x_location;

    float move_range_x_left_float = stof((*stripe_vector).move_range_x_left);
    float move_range_x_right_float = stof((*stripe_vector).move_range_x_right);

    float left_x_location_float = stof(left_x_location);
    float right_x_location_float = stof(right_x_location);

    // case 1 : middle of left stripe
    if ((left_x_location_float < move_range_x_left_float) && (right_x_location_float > move_range_x_left_float) && (left_x_location_float < move_range_x_right_float) && (right_x_location_float < move_range_x_right_float))
    {
        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
        // cout << " over stripe moving location " << endl;
        return true;
    }
    // case 5 : out side
    else
    {
        // cout << " out side " << endl;
        return false;
    }
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
                            stripe.layer = def_content_array[1];
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
void setIrDropReport(string ir_drop_report_file, vector<IrDropPoint> *ir_drop_point_vector)
{
    ifstream ir_file(ir_drop_report_file);
    string ir_content;
    int log = 0;
    double power = 0;
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
}

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

//�ھ�pattern���r��
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

//�h�����������Ů�
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
