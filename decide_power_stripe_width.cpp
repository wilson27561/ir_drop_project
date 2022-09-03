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

const float TOTAL_POWER = 4.4743;
const float M3_SHEET_RESISTANCE = 3.1445326;
const float M1_SHEET_RESISTANCE = 3.1445326;
const float V1_SHEET_RESISTANCE = 8.7348129;
const float V2_SHEET_RESISTANCE = 8.7348129;
const float VDD_PAD = 0.693;
const float IR_DROP = 0.07;
// const float POWER_STRIPE_HEIGHT = 313.92;
const float POWER_STRIPE_HEIGHT = 156.96;
const float POWER_STRIPE_WIDTH = 0.224;
// const float POWER_RAIL_HEIGHT = 385.344;
const float POWER_RAIL_HEIGHT = 192.672;
const float POWER_RAIL_WIDTH = 0.072;
const int POWER_RAIL_NUMBER = 178;
const float SPACING = 0.072;
const string DEF_FILE = "def_file/b19/6t32_run0_b19_floor_plan_check_transfer.def";
const string DEF_FILE_ORI = "def_file/b19/6t49_b19_routing_88_9_39_transfer.def";
const string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
const string IP_REPORT_FILE = "ip_report/print_ip_39.report";
const string ADD_SRIPE_FILE = "stripe_tcl/add_Stripe_tcl_39.tcl";
const string ADD_STRIPE_ORI_FILE = "";
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string LEFT = "left";
const string RIGHT = "right";
const string MIDDLE = "middle";
const string M3_M2_VIA = "M3_M2_3";
const string M2_M1_VIA = "M2_M1_2";
const float RESIZE_IR_RANGE = 0.8;

struct Via
{
    string via_name;
    string x_location;
    string y_location;
};

struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct ResistLine
{
    string x_location;
    string total_resistance;
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
    map<string, ResistLine> resist_line_map;
    ResistLine total_resist_line;
    string total_resist = "0";
    vector<string> ip_power_vector;
    string left_x_ip_power;
    string right_x_ip_power;
    float range_total_power = 0;
    float ir_drop = 0;
    vector<Via> via_vector;
    float total_via_resistance;
};
struct Range
{
    Stripe star_stripe;
    Stripe end_stripe;
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
    float instance_power;
};
struct FollowPin
{
    string layer;
    string y_location;
    vector<Via> via_vector;
    float total_via_resistance;
};

void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getCoreSite(string def_file_name, CoreSite *core_site);

vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
float getInterval(CoreSite *core_site, int power_stripe_number);
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, vector<Range> *vdd_range_vector, vector<Range> *vss_range_vector);
void getStripeMovingRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getDefPlacedImformation(string DEF_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
bool isInStripeRange(Stripe *vdd_stripe, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void setIpPowerInStripe(vector<Stripe> *vdd_stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
float getConsumeRatio(string boudary, float left_x_location_float, float right_x_location_float, float move_range_x_left_float, float move_range_x_right_float);
void resizeVddStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector, float height, float width, float vdd_pad, float sheet_resistance);
// void resizeVddStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector, float height, float width, float vdd_pad, float sheet_resistance, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number, float v1_resistance, float v2_resistance, map<string, FollowPin> *follow_pin_vdd_map, map<string, FollowPin> *follow_pin_vss_map, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map);
void getPowerStripeVia(string def_file_name, map<string, vector<Via>> *m3_m2_via_vdd_map, map<string, vector<Via>> *m2_m1_via_vdd_map, map<string, vector<Via>> *m3_m2_via_vss_map, map<string, vector<Via>> *m2_m1_via_vss_map);
void transferStripeVectorToMap(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map);
void getFollowPin(string def_file_name, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector);
void transferFollowPinVectoToMap(map<string, FollowPin> *follow_pin_vdd_map, map<string, FollowPin> *follow_pin_vss_map, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector);
void getPowerStripeVia(string def_file_name, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map, map<string, FollowPin> *follow_pin_vdd_map, map<string, FollowPin> *follow_pin_vss_map);
float getFollowPinViaResistance(map<string, FollowPin> *follow_pin_map, float via_resistance_float);
float getStripeViaResistance(map<string, Stripe> *stripe_map, float via_resistance_float);
void generateAddStripeTcl(vector<Stripe> *vdd_stripe_vector);
string floatToString(const float value);
int main()
{
    CoreSite core_site;
    //留vector 做 range
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    map<string, Stripe> vdd_stripe_map;
    map<string, Stripe> vss_stripe_map;
    vector<FollowPin> follow_pin_vdd_vector;
    vector<FollowPin> follow_pin_vss_vector;
    map<string, FollowPin> follow_pin_vdd_map;
    map<string, FollowPin> follow_pin_vss_map;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    vector<Stripe> resize_vdd_stripe_vector;
    vector<Range> vdd_range_vector;
    vector<Range> vss_range_vector;

    // -------- start 計算需要的PDN stripe pdn --------
    getStripeLocation(DEF_FILE_ORI, &vdd_stripe_vector, &vss_stripe_vector, &vdd_range_vector, &vss_range_vector);
    transferStripeVectorToMap(&vdd_stripe_vector, &vss_stripe_vector, &vdd_stripe_map, &vss_stripe_map);
    getFollowPin(DEF_FILE_ORI, &follow_pin_vdd_vector, &follow_pin_vss_vector);
    transferFollowPinVectoToMap(&follow_pin_vdd_map, &follow_pin_vss_map, &follow_pin_vdd_vector, &follow_pin_vss_vector);
    getPowerStripeVia(DEF_FILE_ORI, &vdd_stripe_map, &vss_stripe_map, &follow_pin_vdd_map, &follow_pin_vss_map);

    float power_stripes_number = caculate_power_stripe(TOTAL_POWER, IR_DROP, M3_SHEET_RESISTANCE, M1_SHEET_RESISTANCE, VDD_PAD, POWER_STRIPE_HEIGHT, POWER_STRIPE_WIDTH, POWER_RAIL_HEIGHT, POWER_RAIL_WIDTH, POWER_RAIL_NUMBER, V1_SHEET_RESISTANCE, V2_SHEET_RESISTANCE, &follow_pin_vdd_map, &follow_pin_vss_map, &vdd_stripe_map, &vss_stripe_map);
    getCoreSite(DEF_FILE, &core_site);
    float interval = getInterval(&core_site, power_stripes_number);
    cout << "before power stripe number : " << power_stripes_number << endl;
    cout << "before interval : " << interval << endl;

    //將多餘的stripe 拿掉
    getStripeMovingRange(&vdd_stripe_vector, &vss_stripe_vector, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_FILE_ORI, &cell_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    setIpPowerInStripe(&vdd_stripe_vector, &cell_ip_map, &cell_placed_map);

    resizeVddStripe(&vdd_stripe_vector, &resize_vdd_stripe_vector, POWER_STRIPE_HEIGHT, POWER_STRIPE_HEIGHT, VDD_PAD, M3_SHEET_RESISTANCE);
    // resizeVddStripe(&vdd_stripe_vector, &resize_vdd_stripe_vector, POWER_STRIPE_HEIGHT, POWER_STRIPE_HEIGHT, VDD_PAD, M3_SHEET_RESISTANCE, &cell_ip_map, &cell_placed_map);
    generateAddStripeTcl(&vdd_stripe_vector);
    // float resize_interval = getInterval(&core_site, vdd_stripe_vector.size());
    // cout << "resize_interval : " << resize_interval << endl;

    return 0;
};

void generateAddStripeTcl(vector<Stripe> *vdd_stripe_vector)
{
    ofstream myfile;
    myfile.open(ADD_SRIPE_FILE);

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        string start_x_location = (*vdd_stripe_vector)[i].start_x_location;
        string start_y_location = (*vdd_stripe_vector)[i].start_y_location;
        float half_width = POWER_STRIPE_WIDTH / 2;
        float start_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location) - half_width;

        float end_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location) + half_width;

        end_x_location_float = SPACING + POWER_STRIPE_WIDTH + end_x_location_float;

        string end_x_location = to_string(end_x_location_float);
        string end_y_location = (*vdd_stripe_vector)[i].end_y_location;
        start_x_location = floatToString(start_x_location_float);
        end_x_location = floatToString(end_x_location_float);

        float start_x_location_float_vss = end_x_location_float + SPACING;
        float start_y_location_float_vss = 4.608;
        float end_x_location_float_vss = end_x_location_float + SPACING + POWER_STRIPE_WIDTH;
        float end_y_location_float_vss = 322.272;

        string add_stripe_vdd_tcl = "addStripe -nets { VDDX } -layer M3 -direction vertical -width " + to_string(POWER_STRIPE_WIDTH) + " -number_of_sets 1  -area { " + start_x_location + " " + start_y_location + " " + end_x_location + " " + end_y_location + " }";
        string add_stripe_vss_tcl = "addStripe -nets { VSSX } -layer M3 -direction vertical -width " + to_string(POWER_STRIPE_WIDTH) + " -number_of_sets 1  -area { " + to_string(start_x_location_float_vss) + " " + to_string(start_y_location_float_vss) + " " + to_string(end_x_location_float_vss) + " " + to_string(end_y_location_float_vss) + " }";
        myfile << add_stripe_vdd_tcl << endl;
        myfile << add_stripe_vss_tcl << endl;
    }
    myfile.close();
}

int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number, float v1_resistance, float v2_resistance, map<string, FollowPin> *follow_pin_vdd_map, map<string, FollowPin> *follow_pin_vss_map, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map)
{
    float m3_square = power_stripe_height / power_stripe_width;
    float m1_square = power_rail_height / power_rail_width;
    float m3_resistance = m3_sheet_resistance * m3_square;
    float m1_resistance = m1_sheet_resistance * m1_square;
    float vdd_v2_via_resistacne = getStripeViaResistance(&(*vdd_stripe_map), v2_resistance);
    float vdd_v1_via_resistacne = getFollowPinViaResistance(&(*follow_pin_vdd_map), v1_resistance);
    cout << "vdd_v1_via_resistacne : " << vdd_v1_via_resistacne << endl;
    cout << "vdd_v2_via_resistacne :" << vdd_v2_via_resistacne << endl;

    // m3_resistance += (vdd_v2_via_resistacne / 2);
    // m1_resistance += (vdd_v1_via_resistacne / 2);

    //    float vss_v2_via_resistacne = getStripeViaResistance(&(*vss_stripe_map), v2_resistance);
    //    float vss_v1_via_resistacne = getFollowPinViaResistance(&(*follow_pin_vss_map), v1_resistance);

    total_power = total_power * 0.001;

    float m3_current = ir_drop / m3_resistance;
    float m1_current = ir_drop / m1_resistance;

    float temp_power = (total_power / vdd_pad);

    float temp_current = ((power_rail_number * 2 * m1_current));
    temp_power = temp_power - temp_current;
    int m3_number = temp_power / m3_current;
    m3_number = m3_number / 2;
    return m3_number;
}
float getFollowPinViaResistance(map<string, FollowPin> *follow_pin_map, float via_resistance_float)
{
    float via_resistance = 0;
    for (auto iter = (*follow_pin_map).begin(); iter != (*follow_pin_map).end(); ++iter)
    {
        vector<Via> via_vector = iter->second.via_vector;
        float total_via_resistance = iter->second.total_via_resistance;
        for (int i = 0; i < via_vector.size(); i++)
        {
            float via_resistance_in_parallel = 1 / via_resistance_float;
            total_via_resistance += via_resistance_in_parallel;
        }
        (*follow_pin_map)[iter->first].total_via_resistance = total_via_resistance;
        via_resistance = total_via_resistance;
    }
    return via_resistance;
}
//取得一條的powerstripe
float getStripeViaResistance(map<string, Stripe> *stripe_map, float via_resistance_float)
{
    float via_resistance = 0;
    for (auto iter = (*stripe_map).begin(); iter != (*stripe_map).end(); ++iter)
    {
        vector<Via> via_vector = iter->second.via_vector;
        float total_via_resistance = iter->second.total_via_resistance;

        for (int i = 0; i < via_vector.size(); i++)
        {
            float via_resistance_in_parallel = 1 / via_resistance_float;

            total_via_resistance += via_resistance_in_parallel;
        }
        (*stripe_map)[iter->first].total_via_resistance = total_via_resistance;
        via_resistance = total_via_resistance;
    }
    return via_resistance;
}

void getPowerStripeVia(string def_file_name, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map, map<string, FollowPin> *follow_pin_vdd_map, map<string, FollowPin> *follow_pin_vss_map)
{
    ifstream def_file(def_file_name);
    string def_content;
    int log = 0;

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
                        if (def_content.find("SHAPE STRIPE") != string::npos && def_content.find(M3_M2_VIA) != string::npos)
                        {
                            vector<string> def_content_array = splitByPattern(def_content, " ");
                            Via via;
                            via.x_location = def_content_array[7];
                            via.y_location = def_content_array[8];
                            via.via_name = def_content_array[10];
                            if ((*vdd_stripe_map).count(via.x_location))
                            {
                                (*vdd_stripe_map)[via.x_location].via_vector.push_back(via);
                            }
                            else
                            {
                                cout << "M3_M2 VDD via map error " << endl;
                            };
                            log++;
                            // cout << "VDDX : " << def_content << endl;
                        }
                        if (def_content.find("SHAPE STRIPE") != string::npos && def_content.find(M2_M1_VIA) != string::npos)
                        {
                            vector<string> def_content_array = splitByPattern(def_content, " ");
                            Via via;
                            via.x_location = def_content_array[7];
                            via.y_location = def_content_array[8];
                            via.via_name = def_content_array[10];

                            if ((*follow_pin_vdd_map).count(via.y_location))
                            {
                                (*follow_pin_vdd_map)[via.y_location].via_vector.push_back(via);
                            }
                            else
                            {
                                cout << def_content << endl;
                                cout << "M2_M1 VDD via map error " << endl;
                            }
                            log++;
                        }
                        if (log % 1000 == 0 && log > 0)
                        {
                            cout << "vdd via data : " << log << endl;
                        }
                    }
                    // VSSX stripe
                    while (getline(def_file, def_content))
                    {
                        if (def_content.find("END SPECIALNETS") != string::npos)
                        {
                            break;
                        }
                        if (def_content.find("SHAPE STRIPE") != string::npos && def_content.find(M3_M2_VIA) != string::npos)
                        {
                            vector<string> def_content_array = splitByPattern(def_content, " ");
                            Via via;
                            via.x_location = def_content_array[7];
                            via.y_location = def_content_array[8];
                            via.via_name = def_content_array[10];

                            if ((*vss_stripe_map).count(via.x_location))
                            {
                                (*vss_stripe_map)[via.x_location].via_vector.push_back(via);
                            }
                            else
                            {
                                cout << "M3_M2 VSS via map error " << endl;
                            }
                            log++;
                            // cout << "VSSX : " << def_content << endl;
                        }
                        if (def_content.find("SHAPE STRIPE") != string::npos && def_content.find(M2_M1_VIA) != string::npos)
                        {
                            vector<string> def_content_array = splitByPattern(def_content, " ");
                            Via via;
                            via.x_location = def_content_array[7];
                            via.y_location = def_content_array[8];
                            via.via_name = def_content_array[10];
                            if ((*follow_pin_vss_map).count(via.y_location))
                            {
                                (*follow_pin_vss_map)[via.y_location].via_vector.push_back(via);
                            }
                            else
                            {
                                cout << "M2_M1 VSS via map error " << endl;
                            }

                            log++;
                            // cout << "VSSX : " << def_content << endl;
                        }
                        if (log % 1000 == 0 && log > 0)
                        {
                            cout << "vss via data : " << log << endl;
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
};

void transferFollowPinVectoToMap(map<string, FollowPin> *follow_pin_vdd_map, map<string, FollowPin> *follow_pin_vss_map, vector<FollowPin> *follow_pin_vdd_vector, vector<FollowPin> *follow_pin_vss_vector)
{
    for (int i = 0; i < (*follow_pin_vdd_vector).size(); i++)
    {
        (*follow_pin_vdd_map).insert(pair<string, FollowPin>((*follow_pin_vdd_vector)[i].y_location, (*follow_pin_vdd_vector)[i]));
    }
    for (int i = 0; i < (*follow_pin_vss_vector).size(); i++)
    {
        (*follow_pin_vss_map).insert(pair<string, FollowPin>((*follow_pin_vss_vector)[i].y_location, (*follow_pin_vss_vector)[i]));
    }
}

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
                        if (def_content.find("END SPECIALNETS") != string::npos)
                        {
                            break;
                        }
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

void transferStripeVectorToMap(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map)
{
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {

        (*vdd_stripe_map).insert(pair<string, Stripe>((*vdd_stripe_vector)[i].start_x_location, (*vdd_stripe_vector)[i]));
    }
    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        (*vss_stripe_map).insert(pair<string, Stripe>((*vss_stripe_vector)[i].start_x_location, (*vss_stripe_vector)[i]));
    }
}

string floatToString(const float value)
{
    std::ostringstream out;
    out.precision(6);

    out << value;
    return out.str();
}

// case 2 : 所有平均加起來 將平均以下的拿掉
void resizeVddStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector, float height, float width, float vdd_pad, float sheet_resistance)
{
    ofstream myfile;
    myfile.open("ir_report_0693_39_53.txt");
    float square = height / width;
    float resistance = sheet_resistance * square;
    float total_ir_drop = 0;

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
        float current = total_power / vdd_pad;
        float ir_drop = current * resistance;
        (*vdd_stripe_vector)[i].ir_drop = ir_drop;
      
        myfile << "x_location : " <<  (*vdd_stripe_vector)[i].start_x_location << "  " << ir_drop << endl;
        total_ir_drop += ir_drop;
    }
    float average_ir_drop = total_ir_drop / (*vdd_stripe_vector).size();

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        if (average_ir_drop <= (*vdd_stripe_vector)[i].ir_drop)
        {
            resize_vdd_stripe_vector->push_back((*vdd_stripe_vector)[i]);
        }
    }
    myfile.close();
}

// case 1 : 取三條平均以下 就拿掉 只保留原來的
// void resizeVddStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector, float height, float width, float vdd_pad, float sheet_resistance)
// {
//     cout << "vector size : " << (*vdd_stripe_vector).size() << endl;
//     for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
//     {
//         cout << "index :" << i << endl;
//         float square = height / width;
//         float resistance = sheet_resistance * square;
//         if (i == 0)
//         {
//             float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
//             float current = total_power / vdd_pad;
//             float ir_drop = current * resistance;

//             float total_power_next = (*vdd_stripe_vector)[i + 1].range_total_power * 0.001;
//             float current_next = total_power_next / vdd_pad;
//             float ir_drop_next = current_next * resistance;
//             cout << "ir_drop_next : " << ir_drop_next << endl;
//             cout << "ir_drop_next * RESIZE_IR_RANGE : " << ir_drop_next * RESIZE_IR_RANGE << endl;
//             cout << "ir_drop : " << ir_drop << endl;
//             // RESIZE_IR_RANGE
//             if ((ir_drop_next * RESIZE_IR_RANGE > ir_drop) == false)
//             {
//                 (*resize_vdd_stripe_vector).push_back((*vdd_stripe_vector)[i]);
//             }
//         }
//         else if (i == ((*vdd_stripe_vector).size() - 1))
//         {
//             float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
//             float current = total_power / vdd_pad;
//             float ir_drop = current * resistance;

//             float total_power_prevent = (*vdd_stripe_vector)[i - 1].range_total_power * 0.001;
//             float current_prevent = total_power_prevent / vdd_pad;
//             float ir_drop_prevent = current_prevent * resistance;

//             if ((ir_drop_prevent * RESIZE_IR_RANGE > ir_drop) == false)
//             {
//                 (*resize_vdd_stripe_vector).push_back((*vdd_stripe_vector)[i]);
//             }
//         }
//         else
//         {
//             float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
//             float current = total_power / vdd_pad;
//             float ir_drop = current * resistance;

//             float total_power_prevent = (*vdd_stripe_vector)[i - 1].range_total_power * 0.001;
//             float current_prevent = total_power_prevent / vdd_pad;
//             float ir_drop_prevent = current_prevent * resistance;

//             float total_power_next = (*vdd_stripe_vector)[i + 1].range_total_power * 0.001;
//             float current_next = total_power_next / vdd_pad;
//             float ir_drop_next = current_next * resistance;

//             float averange_ir_drop = (ir_drop_next+ir_drop_prevent+ir_drop)/3;

//              if ((averange_ir_drop * RESIZE_IR_RANGE > ir_drop) == false)
//             {
//                 (*resize_vdd_stripe_vector).push_back((*vdd_stripe_vector)[i]);
//             }
//         }
//     }
// }

// case 3 比旁邊小 0.2
// void resizeVddStripe(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *resize_vdd_stripe_vector, float height, float width, float vdd_pad, float sheet_resistance, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
// {
//     cout << "before size : " << (*vdd_stripe_vector).size() << endl;
//     int j = 0;
//     for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
//     {
//         ofstream myfile;
//         string power_file = "power_file/range_power_" + to_string(i) + "_" + to_string(j);
//         myfile.open(power_file);

//         float square = height / width;
//         float resistance = sheet_resistance * square;
//         cout << "power stripe index : " << i << endl;
//         if (i == 0)
//         {
//             float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
//             float current = total_power / vdd_pad;
//             float ir_drop = current * resistance;

//             float total_power_next = (*vdd_stripe_vector)[i + 1].range_total_power * 0.001;
//             float current_next = total_power_next / vdd_pad;
//             float ir_drop_next = current_next * resistance;
//             // RESIZE_IR_RANGE
//             cout << "ir_drop :      " << ir_drop << endl;
//             cout << "ir_drop_next : " << ir_drop_next << endl;
//             cout << "ir_drop_next RESIZE_IR_RANGE : " << ir_drop_next * RESIZE_IR_RANGE << endl;
//             cout << "total_power : " << total_power << endl;
//             cout << "total_power_next : " << total_power_next << endl;
//             if (ir_drop_next * RESIZE_IR_RANGE > ir_drop)
//             {
//                 cout << "erase this power stripe" << endl;
//                 std::cout << "" << std::endl;
//                 (*vdd_stripe_vector).erase((*vdd_stripe_vector).begin() + i);
//                 setIpPowerInStripe(&(*vdd_stripe_vector), &(*cell_ip_map), &(*cell_placed_map));
//                 for (int m = 0; m < (*vdd_stripe_vector).size(); m++)
//                 {
//                     myfile << "index : " << m << " " << to_string((*vdd_stripe_vector)[m].range_total_power) << endl;
//                 }

//                 i = i - 1;
//             }else{
//                   for (int m = 0; m < (*vdd_stripe_vector).size(); m++)
//                 {
//                     myfile << "index : " << m << " " << to_string((*vdd_stripe_vector)[m].range_total_power) << endl;
//                 }
//             }
//         }
//         else if (i == ((*vdd_stripe_vector).size() - 1))
//         {
//             float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
//             float current = total_power / vdd_pad;
//             float ir_drop = current * resistance;

//             float total_power_prevent = (*vdd_stripe_vector)[i - 1].range_total_power * 0.001;
//             float current_prevent = total_power_prevent / vdd_pad;
//             float ir_drop_prevent = current_prevent * resistance;

//             cout << "ir_drop :      " << ir_drop << endl;
//             cout << "ir_drop_prevent : " << ir_drop_prevent << endl;
//             cout << "ir_drop_prevent RESIZE_IR_RANGE : " << ir_drop_prevent * RESIZE_IR_RANGE << endl;
//             cout << "total_power : " << total_power << endl;
//             cout << "total_power_prevent : " << total_power_prevent << endl;
//             if (ir_drop_prevent * RESIZE_IR_RANGE > ir_drop)
//             {
//                 cout << "erase this power stripe" << endl;
//                 std::cout << "" << std::endl;
//                 (*vdd_stripe_vector).erase((*vdd_stripe_vector).begin() + i);
//                 setIpPowerInStripe(&(*vdd_stripe_vector), &(*cell_ip_map), &(*cell_placed_map));
//                 for (int m = 0; m < (*vdd_stripe_vector).size(); m++)
//                 {
//                     myfile << "index : " << m << " " << to_string((*vdd_stripe_vector)[m].range_total_power) << endl;
//                 }
//                 i = i - 1;
//             }else{
//                   for (int m = 0; m < (*vdd_stripe_vector).size(); m++)
//                 {
//                     myfile << "index : " << m << " " << to_string((*vdd_stripe_vector)[m].range_total_power) << endl;
//                 }
//             }
//         }
//         else
//         {
//             float total_power = (*vdd_stripe_vector)[i].range_total_power * 0.001;
//             float current = total_power / vdd_pad;
//             float ir_drop = current * resistance;

//             float total_power_prevent = (*vdd_stripe_vector)[i - 1].range_total_power * 0.001;
//             float current_prevent = total_power_prevent / vdd_pad;
//             float ir_drop_prevent = current_prevent * resistance;

//             float total_power_next = (*vdd_stripe_vector)[i + 1].range_total_power * 0.001;
//             float current_next = total_power_next / vdd_pad;
//             float ir_drop_next = current_next * resistance;
//             // 3 條線平均
//             // float averange_ir_drop = (ir_drop_next + ir_drop_prevent + ir_drop) / 3;

//             // if ((averange_ir_drop * RESIZE_IR_RANGE > ir_drop) == false)
//             // {
//             //     (*vdd_stripe_vector).erase((*vdd_stripe_vector).begin() + i);
//             //     setIpPowerInStripe(&(*vdd_stripe_vector), &(*cell_ip_map), &(*cell_placed_map));
//             //     i = i - 1;
//             // }
//             cout << "ir_drop :      " << ir_drop << endl;
//             cout << "ir_drop_prevent : " << ir_drop_prevent << endl;
//             cout << "ir_drop_prevent RESIZE_IR_RANGE : " << ir_drop_prevent * RESIZE_IR_RANGE << endl;
//             cout << "ir_drop_next : " << ir_drop_next << endl;
//             cout << "ir_drop_next RESIZE_IR_RANGE : " << ir_drop_next * RESIZE_IR_RANGE << endl;
//             cout << "total_power : " << total_power << endl;
//             cout << "total_power_next : " << total_power_next << endl;
//             cout << "total_power_prevent : " << total_power_prevent << endl;
//             if ((ir_drop_next * RESIZE_IR_RANGE) > ir_drop || (ir_drop_prevent * RESIZE_IR_RANGE) > ir_drop)
//             {
//                 cout << "erase this power stripe" << endl;
//                 std::cout << "" << std::endl;
//                 (*vdd_stripe_vector).erase((*vdd_stripe_vector).begin() + i);
//                 setIpPowerInStripe(&(*vdd_stripe_vector), &(*cell_ip_map), &(*cell_placed_map));
//                 for (int m = 0; m < (*vdd_stripe_vector).size(); m++)
//                 {
//                     myfile << "index : " << m << " " << to_string((*vdd_stripe_vector)[m].range_total_power) << endl;
//                 }
//                 i = i - 1;
//             }else{
//                   for (int m = 0; m < (*vdd_stripe_vector).size(); m++)
//                 {
//                     myfile << "index : " << m << " " << to_string((*vdd_stripe_vector)[m].range_total_power) << endl;
//                 }
//             }
//             cout << "" << endl;
//         }
//         j++;
//         myfile.close();
//     }
//     cout << "after size : " << (*vdd_stripe_vector).size() << endl;
// }

//能夠進行優化 盡量減少跑的次數
void setIpPowerInStripe(vector<Stripe> *vdd_stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
        {
            string cell_id = iter->first;
            CellInstancePowerInfo cell_instance_power_info = iter->second;
            if (isInStripeRange((&(*vdd_stripe_vector)[i]), cell_id, &(*cell_placed_map), &(*cell_ip_map)))
            {
                (*vdd_stripe_vector)[i].ip_power_vector.push_back(cell_id);
            }
        }
    }
}

bool isInStripeRange(Stripe *vdd_stripe, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
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
        float ratio = getConsumeRatio(LEFT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_power * ratio;
        (*vdd_stripe).range_total_power += power;
        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        float power = (*cell_ip_map)[cell_id].instance_power;
        (*vdd_stripe).range_total_power += power;
        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        float ratio = getConsumeRatio(RIGHT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_power * ratio;
        (*vdd_stripe).range_total_power += power;
        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
        float ratio = getConsumeRatio(MIDDLE, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_power * ratio;
        (*vdd_stripe).range_total_power += power;
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

float getInterval(CoreSite *core_site, int power_stripe_number)
{

    float width_float = stof((*core_site).right_x_location) - stof((*core_site).left_x_location);

    float number_float = float(power_stripe_number);

    float interval = width_float / (number_float + 1);

    return interval;
}

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

//只讀到Stripe 就 break
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, vector<Range> *vdd_range_vector, vector<Range> *vss_range_vector)
{

    ifstream def_file(def_file_name);
    string def_content;

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
                            stripe.net_name = NET_NAME_VDD;
                            vector<Via> via_vector;
                            stripe.via_vector = via_vector;

                            (*vdd_stripe_vector).push_back(stripe);
                            // int index = (*vdd_stripe_vector).size();
                            // if ((*vdd_stripe_vector).size() >= 2)
                            // {
                            //     Range stripe_range;
                            //     stripe_range.end_stripe = (*vdd_stripe_vector)[index - 1];
                            //     stripe_range.star_stripe = (*vdd_stripe_vector)[index - 2];
                            //     (*vdd_range_vector).push_back(stripe_range);
                            // }
                        }
                    }
                }
                // VSSX stripe
                while (getline(def_file, def_content))
                {
                    if (def_content.find("NEW") != string::npos)
                    {
                        if (def_content.find("END SPECIALNETS") != string::npos)
                        {
                            break;
                        }
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        if (def_content_array[4] == "SHAPE" && def_content_array[5] == "STRIPE" && def_content_array.size() == 14)
                        {
                            cout << "VSSX :" << def_content << " " << def_content_array.size() << endl;
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location = def_content_array[12];
                            stripe.net_name = NET_NAME_VSS;
                            vector<Via> via_vector;
                            stripe.via_vector = via_vector;
                            (*vss_stripe_vector).push_back(stripe);

                            // int index = (*vss_stripe_vector).size();
                            // if ((*vss_stripe_vector).size() >= 2)
                            // {
                            //     Range stripe_range;
                            //     stripe_range.end_stripe = (*vss_stripe_vector)[index - 1];
                            //     stripe_range.star_stripe = (*vss_stripe_vector)[index - 2];
                            //     (*vss_range_vector).push_back(stripe_range);
                            // }
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

    def_file.close();
}
void getStripeMovingRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
{

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            double distance = (((stof((*vdd_stripe_vector)[i + 1].start_x_location) - stof((*vdd_stripe_vector)[i].end_x_location)) / 2) + stof((*vdd_stripe_vector)[i].start_x_location));
            (*vdd_stripe_vector)[i].move_range_x_left = (*core_site).left_x_location;
            (*vdd_stripe_vector)[i].move_range_x_right = to_string(distance);
        }
        else if (i == ((*vdd_stripe_vector).size() - 1))
        {
            int front_index = i - 1;
            (*vdd_stripe_vector)[i].move_range_x_right = (*core_site).right_x_location;
            (*vdd_stripe_vector)[i].move_range_x_left = (*vdd_stripe_vector)[front_index].move_range_x_right;
        }
        else
        {
            double distance = (((stof((*vdd_stripe_vector)[i + 1].start_x_location) - stof((*vdd_stripe_vector)[i].end_x_location)) / 2) + stof((*vdd_stripe_vector)[i].start_x_location));
            (*vdd_stripe_vector)[i].move_range_x_right = to_string(distance);
            (*vdd_stripe_vector)[i].move_range_x_left = (*vdd_stripe_vector)[i - 1].move_range_x_right;
        }
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            double distance = (((stof((*vss_stripe_vector)[i + 1].start_x_location) - stof((*vss_stripe_vector)[i].end_x_location)) / 2) + stof((*vss_stripe_vector)[i].start_x_location));
            (*vss_stripe_vector)[i].move_range_x_left = (*core_site).left_x_location;
            (*vss_stripe_vector)[i].move_range_x_right = to_string(distance);
        }
        else if (i == ((*vss_stripe_vector).size() - 1))
        {
            int front_index = i - 1;
            (*vss_stripe_vector)[i].move_range_x_right = (*core_site).right_x_location;
            (*vss_stripe_vector)[i].move_range_x_left = (*vss_stripe_vector)[front_index].move_range_x_right;
        }
        else
        {
            double distance = (((stof((*vss_stripe_vector)[i + 1].start_x_location) - stof((*vss_stripe_vector)[i].end_x_location)) / 2) + stof((*vss_stripe_vector)[i].start_x_location));
            (*vss_stripe_vector)[i].move_range_x_right = to_string(distance);
            (*vss_stripe_vector)[i].move_range_x_left = (*vss_stripe_vector)[i - 1].move_range_x_right;
        }
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
void getDefPlacedImformation(string DEF_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map)
{
    ifstream def_file(DEF_FILE);
    string def_content;
    int log = 0;
    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("COMPONENT") != string::npos && ((def_content.find("COMPONENTPIN") != string::npos) == false))
            {
                while (getline(def_file, def_content))
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
                cell_instance_power_info.instance_power = stof(ip_content_array[0]);
                power += cell_instance_power_info.instance_power;
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
}