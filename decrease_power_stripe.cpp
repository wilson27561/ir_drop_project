
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
struct PowerGround
{
    //左下右上
    string start_x_location;
    string end_x_location;
    string start_y_location;
    string end_y_location;
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
    vector<string> filler_vector;
    long double range_total_power;
};
struct TrackInfo
{
    string layer;
    string start;
    string pitch;
};

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *filler_placed_map, unordered_map<string, CellInfo> *cell_info_map);
vector<string> splitByPattern(string content, string pattern);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
string &trim(string &str);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
float getConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float);
bool isInStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void setIpPowerInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void getFollowPin(string DEF_TRANSFER_FILE_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
string floatToString(const float value);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
bool isInStripeRangeFiller(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *filler_placed_map);
void setFillerIStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *filler_placed_map);

int main()
{
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellPlacedInfo> filler_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    vector<FollowPin> follow_pin_vdd_vector;
    vector<FollowPin> follow_pin_vss_vector;

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
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    setStripeRange(&vdd_stripe_vector, &vss_stripe_vector, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &filler_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    setIpPowerInStripe(&vdd_stripe_vector, &cell_ip_map, &cell_placed_map);
    getFollowPin(DEF_TRANSFER_FILE, &follow_pin_vdd_vector, &follow_pin_vss_vector);
    setFillerIStripe(&vdd_stripe_vector, &filler_placed_map);

    



    float total_power = 0;
    for (int i = 0; i < vdd_stripe_vector.size(); i++)
    {
        total_power += vdd_stripe_vector[i].range_total_power;
        // vdd_stripe_vector[i].filler_vector.size();
        cout << "filler size : " << vdd_stripe_vector[i].filler_vector.size() << endl;
        cout << "range_total_power : " << vdd_stripe_vector[i].range_total_power * 1000 << endl;
    }
    cout << "total power : " << total_power << endl;
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
    // case 4 : out side
    else
    {
        // cout << " out side " << endl;
        return false;
    }
}

//能夠進行優化 盡量減少跑的次數
void setIpPowerInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {

        for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
        {
            string cell_id = iter->first;
            CellInstancePowerInfo cell_instance_power_info = iter->second;

            if (isInStripeRange(&((*stripe_vector)[i]), cell_id, &(*cell_placed_map), &(*cell_ip_map)))
            {
                (*stripe_vector)[i].ip_power_vector.push_back(cell_id);
            }
        }
    }
}

bool isInStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
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
                        // TODO 這裡可以下log
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
