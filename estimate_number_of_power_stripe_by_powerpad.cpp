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

const float TOTAL_POWER = 4.55424569;
const float M3_SHEET_RESISTANCE_PAD = 0.8119971;
const float wsd = 0.9;
const float M2_SHEET_RESISTANCE = 0.811738;
const float M1_SHEET_RESISTANCE = 1.2;
const float M3_PAD_WIDTH = 1.728;
const float M2_WIDTH = 1.8;
const float VDD_PAD = 0.7;
const float IR_DROP = 0.07;
const float POWER_STRIPE_HEIGHT = 313.92;
const float POWER_STRIPE_WIDTH = 0.224;
const float POWER_RAIL_HEIGHT = 385.344;
const float POWER_RAIL_WIDTH = 0.072;
const int POWER_RAIL_NUMBER = 178;
const float SPACING = 0.072;
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string UP = "UP";
const string DOWN = "DOWN";

const string M3_LAYER = "M3";
const string VERTICAL = "vertical";
const string HORIZONTAL = "horizontal";
const string VDD = "VDDX";
const string VSS = "VSSX";
const string POWERSTRIPEVALUE = "power_stripe_distance";
const string POWERRAILVALUE = "power_rail_distance";

const string DEF_FILE = "def_file/b19/6t49_b19_routing_88_9_transfer.def";

struct ResistLine
{
    string x_location;
    string total_resistance;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
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
};
struct DieArea
{
    string lower_left_x_location;
    string lower_left_y_location;
    string top_right_x_location;
    string top_right_y_location;
};

vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
float getInterval(CoreSite *core_site, int power_stripe_number);
void getCoreSite(string def_file_name, CoreSite *core_site);
int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number, float pad_to_stripe_resist);
void getPowerPadLocation(string def_file_name, vector<PowerPad> *vdd_power_pad, vector<PowerPad> *vss_power_pad);
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad);

void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad);
void getDieArea(string def_file_name, DieArea *die_area);
void setSide(PowerPad *power_pad, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
void getShapeRing(string def_file_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector);
void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector);
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area);
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line);
void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map);
float caculatePowerPadToStripeResistance(vector<PowerPad> *power_pad_vector, map<string, ShapeRing> *shape_map, CoreSite *core_site);
// void countPadToStripeDistance(vector<float> *pad_x_location_vector, string side, CoreSite *core_site, map<string, float> *distance_pad_to_stripe_map, map<string, ShapeRing> *shape_map);
float countPadToStripeDistance(map<string, vector<float>> *power_pad_map, string side, CoreSite *core_site);
float countPadToRing(map<string, ShapeRing> *shape_map, vector<PowerPad> *power_pad_vector);
void setPowerPadDirection(map<string, vector<PowerPad>> *direction_power_pad);
float getPowerPadMiddle(vector<PowerPad> *power_pad_vector, map<string, ShapeRing> *shape_map, string side);
void sortPowerPad(map<string, vector<PowerPad>> *direction_power_pad);
void power_pad_loction_sort(vector<PowerPad> *power_pad_vec, string direction);
void setSinglePad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, vector<string> *side_vec, map<string, float> *power_value_map);
float caculatePowerPad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, map<string, float> *power_stripe_distance_map);
float one_side_power_pad(vector<PowerPad> *power_pad, CoreSite *core_site, string side);
void twice_side_power_pad(map<string, vector<PowerPad>> *direction_power_pad, CoreSite *core_site, string side_first, string side_second, float *power_stripe_distance, float *power_rail_distance);
void setTwicePad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, vector<string> *side_vec, set<string> *side_set, map<string, float> *power_stripe_distance_map);
void countTwicePadDistance(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, string side_first, string side_second, map<string, float> *power_value_map);
float getLongDistance(vector<PowerPad> *power_pad, float first_location, float last_location, string side, float other_side_distance);
float getMiddleLongDistance(vector<PowerPad> *power_pad, float first_location, float last_location, string side, float left_side_distance, float right_side_distance);
void setTriplePad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, vector<string> *side_vec, set<string> *side_set, map<string, float> *power_value_map);
float setPowerStripeValue(float power_stripe_length, float pad_to_ring, PowerPad power_pad, ShapeRing shape_ring, bool is_exists_pad);
float setPowerRailValue(float power_rail_length, PowerPad power_pad, ShapeRing shape_ring, bool is_exists_pad);
int caculate_power_stripes(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, map<string, float> *power_value_map);
int caculateTriplePowerStripe(map<string, float> *power_value_map, set<string> *side_set);
int caculateDoublePowerStripe(map<string, float> *power_value_map, set<string> *side_set);
int caculateSinglePowerStripe(map<string, float> *power_value_map);

int main()
{
    CoreSite core_site;
    DieArea die_area;
    vector<PowerPad> vdd_power_pad_vector;
    vector<PowerPad> vss_power_pad_vector;
    vector<ShapeRing> vdd_shape_ring_vector;
    vector<ShapeRing> vss_shape_ring_vector;
    map<string, ShapeRing> vdd_shape_map;
    map<string, ShapeRing> vss_shape_map;

    map<string, vector<PowerPad>> direction_power_pad;
    map<string, float> power_value_map;
    //TODO 1.計算power stripe 數量　可以重複寫
    //     2. 
    setPowerPadDirection(&direction_power_pad);
    getPowerPadLocation(DEF_FILE, &vdd_power_pad_vector, &vss_power_pad_vector);
    getDieArea(DEF_FILE, &die_area);
    getCoreSite(DEF_FILE, &core_site);
    cout << "core site left : " << core_site.left_x_location << endl;
    cout << "core site right : " << core_site.right_x_location << endl;
    cout << "core site up : " << core_site.up_y_location << endl;
    cout << "core site down : " << core_site.down_y_location << endl;
    setPowerPadSide(&vdd_power_pad_vector, &vss_power_pad_vector, &direction_power_pad, &die_area);
    sortPowerPad(&direction_power_pad);

    getShapeRing(DEF_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);
    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);
    
    //================= 調整side start =========================
    // direction_power_pad.erase(direction_power_pad.find(UP));
    direction_power_pad.erase(direction_power_pad.find(DOWN));
    direction_power_pad.erase(direction_power_pad.find(RIGHT));
    direction_power_pad.erase(direction_power_pad.find(LEFT));
    //================= 調整side end =========================

    caculatePowerPad(&direction_power_pad, &vdd_shape_map, &core_site, &power_value_map);
    float number_of_power_stripes = caculate_power_stripes(&direction_power_pad, &vdd_shape_map, &core_site, &power_value_map);

    float interval = getInterval(&core_site, number_of_power_stripes);

    // cout << "ir-drop : " << 0.7 - IR_DROP << " " << IR_DROP << endl;
    cout << "before power stripe number : " << number_of_power_stripes << endl;
    cout << "before interval : " << interval << endl;

    return 0;
};

int caculate_power_stripes(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, map<string, float> *power_value_map)
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
        }
    }
    
    cout << side_vec.size() << endl;

    if (side_vec.size() == 1)
    {
        number_of_power_stripe = caculateSinglePowerStripe(&(*power_value_map));
    }
    else if (side_vec.size() == 2)
    {
        number_of_power_stripe = caculateDoublePowerStripe(&(*power_value_map), &side_set);
    }
    else if (side_vec.size() == 3)
    {
        number_of_power_stripe = caculateTriplePowerStripe(&(*power_value_map), &side_set);
    }
    else
    {
        cout << "four pad" << endl;
    }
    return number_of_power_stripe;
}
int caculateTriplePowerStripe(map<string, float> *power_value_map, set<string> *side_set)
{
    int m3_number = 0;
    if ((*side_set).count(UP) && (*side_set).count(LEFT) && (*side_set).count(RIGHT))
    {
        float m3_square = POWER_STRIPE_HEIGHT / POWER_STRIPE_WIDTH;
        float m1_square = (POWER_RAIL_HEIGHT / 2) / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);

        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * 2 * m1_current);

        temp_total_current = temp_total_current - temp_current;

        m3_number = temp_total_current / m3_current;
    }
    else if ((*side_set).count(UP) && (*side_set).count(DOWN) && (*side_set).count(LEFT))
    {
        float m3_square = (POWER_STRIPE_HEIGHT / 2) / POWER_STRIPE_WIDTH;
        float m1_square = POWER_RAIL_HEIGHT / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);



        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * m1_current);

        temp_total_current = temp_total_current - temp_current;

      

        m3_number = temp_total_current / m3_current;
        m3_number = m3_number / 2;
    }
    else if ((*side_set).count(UP) && (*side_set).count(DOWN) && (*side_set).count(RIGHT))
    {
        float m3_square = (POWER_STRIPE_HEIGHT / 2) / POWER_STRIPE_WIDTH;
        float m1_square = POWER_RAIL_HEIGHT / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);

        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * m1_current);

        temp_total_current = temp_total_current - temp_current;

        m3_number = temp_total_current / m3_current;
        m3_number = m3_number / 2;
    }
    else if ((*side_set).count(LEFT) && (*side_set).count(RIGHT) && (*side_set).count(DOWN))
    {
        float m3_square = POWER_STRIPE_HEIGHT / POWER_STRIPE_WIDTH;
        float m1_square = (POWER_RAIL_HEIGHT / 2) / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);

        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * 2 * m1_current);

        temp_total_current = temp_total_current - temp_current;

        m3_number = temp_total_current / m3_current;
    }
    else
    {
        cout << "error" << endl;
    }
}

int caculateDoublePowerStripe(map<string, float> *power_value_map, set<string> *side_set)
{
    int m3_number = 0;

    if ((*side_set).count(UP) && (*side_set).count(DOWN))
    {
        float m3_square = (POWER_STRIPE_HEIGHT / 2) / POWER_STRIPE_WIDTH;
        float m1_square = POWER_RAIL_HEIGHT / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);

        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * m1_current);

        temp_total_current = temp_total_current - temp_current;

        m3_number = temp_total_current / m3_current;
        m3_number = m3_number / 2;
    }
    else if ((*side_set).count(RIGHT) && (*side_set).count(LEFT))
    {
        float m3_square = POWER_STRIPE_HEIGHT / POWER_STRIPE_WIDTH;
        float m1_square = (POWER_RAIL_HEIGHT / 2) / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);

        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * 2 * m1_current);

        temp_total_current = temp_total_current - temp_current;

        m3_number = temp_total_current / m3_current;
    }
    else
    {
        float m3_square = POWER_STRIPE_HEIGHT / POWER_STRIPE_WIDTH;
        float m1_square = POWER_RAIL_HEIGHT / POWER_RAIL_WIDTH;
        float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
        float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

        m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
        m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

        float m3_current = IR_DROP / (m3_resistance);
        float m1_current = IR_DROP / (m1_resistance);

        float total_power = TOTAL_POWER * 0.001;
        float temp_total_current = total_power / VDD_PAD;

        float temp_current = m3_current + (POWER_RAIL_NUMBER * m1_current);

        temp_total_current = temp_total_current - temp_current;

        m3_number = temp_total_current / m3_current;
    }

    return m3_number;
}

int caculateSinglePowerStripe(map<string, float> *power_value_map)
{
    float m3_square = POWER_STRIPE_HEIGHT / POWER_STRIPE_WIDTH;
    float m1_square = POWER_RAIL_HEIGHT / POWER_RAIL_WIDTH;
    float m3_resistance = M3_SHEET_RESISTANCE_STRIPE * m3_square;
    float m1_resistance = M1_SHEET_RESISTANCE * m1_square;

    m3_resistance = m3_resistance + (*power_value_map)[POWERSTRIPEVALUE];
    m1_resistance = m1_resistance + (*power_value_map)[{POWERRAILVALUE}];

    float total_power = TOTAL_POWER * 0.001;
    float temp_total_current = total_power / VDD_PAD;


    float m3_current = IR_DROP / (m3_resistance);
    float m1_current = IR_DROP / (m1_resistance);

    float temp = temp_total_current - (POWER_RAIL_NUMBER * m1_current);
    int m3_number = temp / m3_current;


     
    // float temp_current = m3_current + (POWER_RAIL_NUMBER * m1_current);
    // temp_total_current = temp_total_current - temp_current;
    // int m3_number = temp_total_current / m3_current;
    return m3_number;
}

// DOWN 221.508
// DOWN 240.804
// LEFT 132.624
// LEFT 148.032
// RIGHT 178.848
// RIGHT 193.968
// UP 175.284
// UP 193.572

// up   :316.8000
// down :10.0800
// left :10.0800
// right:395.5680

float setPowerStripeValue(float power_stripe_length, PowerPad power_pad, ShapeRing shape_ring, bool is_exists_pad)
{
    float power_value = 0;
    if (is_exists_pad)
    {
        float power_pad_length = stof(power_pad.y_location) - (power_pad.length / 2);
        float pad_to_ring = abs(stof(shape_ring.start_y_location) - power_pad_length);

        cout << "power_pad_length :" <<power_pad_length <<endl;
        cout << "pad_to_ring :" <<pad_to_ring <<endl;
        cout << "power_stripe_length :" <<power_stripe_length <<endl;

        float power_stripe_value = power_stripe_length * M2_SHEET_RESISTANCE;
        float power_pad_value = pad_to_ring * M3_SHEET_RESISTANCE_PAD;
        power_value = power_stripe_value + power_pad_value;
    }
    else
    {
        float power_stripe_value = power_stripe_length * M2_SHEET_RESISTANCE;
        power_value += power_stripe_value;
    }
    return power_value;
}

float setPowerRailValue(float power_rail_length, PowerPad power_pad, ShapeRing shape_ring, bool is_exists_pad)
{
    float power_value = 0;
    if (is_exists_pad)
    {
        float power_pad_length = stof(power_pad.y_location) - (power_pad.length / 2);
        float pad_to_ring = abs(stof(shape_ring.start_x_location) - power_pad_length);

        float power_rail_value = power_rail_length * M2_SHEET_RESISTANCE;
        float power_pad_value = pad_to_ring * M3_SHEET_RESISTANCE_PAD;

        power_value = power_rail_value + power_pad_value;
    }
    else
    {
        float power_rail_value = power_rail_length * M2_SHEET_RESISTANCE;
        power_value += power_rail_value;
    }

    return power_value;
}

float caculatePowerPad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, map<string, float> *power_value_map)
{
    int pad_side = 0;
    vector<string> side_vec;
    set<string> side_set;
    // //================= 調整side start =========================
    // // (*direction_power_pad).erase((*direction_power_pad).find(UP));
    // // (*direction_power_pad).erase((*direction_power_pad).find(DOWN));
    // (*direction_power_pad).erase((*direction_power_pad).find(RIGHT));
    // (*direction_power_pad).erase((*direction_power_pad).find(LEFT));
    // //================= 調整side end =========================

    for (auto direct_pad_iter = (*direction_power_pad).begin(); direct_pad_iter != (*direction_power_pad).end(); ++direct_pad_iter)
    {
        string side = direct_pad_iter->first;

        vector<PowerPad> pad_vec = direct_pad_iter->second;
        if (pad_vec.size() > 0)
        {
            side_vec.push_back(side);
            side_set.insert(side);
        }
    }

    if (side_vec.size() == 1)
    {
        // return resistance pad 到 stripe
        setSinglePad(&(*direction_power_pad), &(*shape_map), &(*core_site), &side_vec, &(*power_value_map));
        cout << "power_stripe_value : " << (*power_value_map)[POWERSTRIPEVALUE] << endl;
        cout << "power_rail_value : " << (*power_value_map)[POWERRAILVALUE] << endl;
    }
    else if (side_vec.size() == 2)
    {
        setTwicePad(&(*direction_power_pad), &(*shape_map), &(*core_site), &side_vec, &side_set, &(*power_value_map));
    }
    else if (side_vec.size() == 3)
    {
        setTriplePad(&(*direction_power_pad), &(*shape_map), &(*core_site), &side_vec, &side_set, &(*power_value_map));
    }
    else
    {
        cout << "four pad" << endl;
    }

    return 0;
}
void setTriplePad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, vector<string> *side_vec, set<string> *side_set, map<string, float> *power_value_map)
{
    float power_stripe_distance = 0;
    float power_rail_distance = 0;

    if ((*side_set).count(UP) && (*side_set).count(LEFT) && (*side_set).count(RIGHT))
    {
        vector<PowerPad> up_power_pad = (*direction_power_pad)[UP];
        vector<PowerPad> left_power_pad = (*direction_power_pad)[LEFT];
        vector<PowerPad> right_power_pad = (*direction_power_pad)[RIGHT];

        float power_stripe_distance_up = getMiddleLongDistance(&up_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), UP, abs(stof(up_power_pad[0].x_location) - stof((*core_site).left_x_location)), abs(stof(up_power_pad[up_power_pad.size() - 1].x_location) - stof((*core_site).right_x_location)));
        float power_rail_distance_left = getLongDistance(&left_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), LEFT, abs(stof((*core_site).left_x_location) - stof(up_power_pad[0].x_location)));
        float power_rail_distance_right = getLongDistance(&right_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), RIGHT, abs(stof((*core_site).right_x_location) - stof(up_power_pad[up_power_pad.size() - 1].y_location)));
        float power_stripe_value = 0;
        float power_rail_value = 0;
        if (power_rail_distance_left >= power_rail_distance_right)
        {
            power_stripe_distance = power_stripe_distance_up;
            power_rail_distance = power_rail_distance_left;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[UP][0], (*shape_map)[UP], false);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[LEFT][0], (*shape_map)[LEFT], true);
        }
        else
        {
            power_stripe_distance = power_stripe_distance_up;
            power_rail_distance = power_rail_distance_right;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[UP][0], (*shape_map)[UP], false);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[RIGHT][0], (*shape_map)[RIGHT], true);
        }

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else if ((*side_set).count(UP) && (*side_set).count(DOWN) && (*side_set).count(LEFT))
    {
        vector<PowerPad> up_power_pad = (*direction_power_pad)[UP];
        vector<PowerPad> down_power_pad = (*direction_power_pad)[DOWN];
        vector<PowerPad> left_power_pad = (*direction_power_pad)[LEFT];

        float power_rail_distance_left = getMiddleLongDistance(&left_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), LEFT, abs(stof(down_power_pad[0].x_location) - stof((*core_site).left_x_location)), abs(stof(up_power_pad[0].x_location) - stof((*core_site).left_x_location)));
        float power_stripe_distance_up = getLongDistance(&up_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), UP, abs(stof((*core_site).up_y_location) - stof(left_power_pad[left_power_pad.size() - 1].y_location)));
        float power_stripe_distance_down = getLongDistance(&down_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), DOWN, abs(stof((*core_site).down_y_location) - stof(left_power_pad[0].y_location)));

        float power_stripe_value = 0;
        float power_rail_value = 0;

        if (power_stripe_distance_up >= power_stripe_distance_down)
        {
            power_stripe_distance = power_stripe_distance_up;
            power_rail_distance = power_rail_distance_left;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[UP][0], (*shape_map)[UP], false);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[LEFT][0], (*shape_map)[LEFT], true);
        }
        else
        {
            power_stripe_distance = power_stripe_distance_down;
            power_rail_distance = power_rail_distance_left;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[UP][0], (*shape_map)[UP], false);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[LEFT][0], (*shape_map)[LEFT], true);
        }

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else if ((*side_set).count(UP) && (*side_set).count(DOWN) && (*side_set).count(RIGHT))
    {
        vector<PowerPad> up_power_pad = (*direction_power_pad)[UP];
        vector<PowerPad> down_power_pad = (*direction_power_pad)[DOWN];
        vector<PowerPad> right_power_pad = (*direction_power_pad)[RIGHT];

        float power_rail_distance_right = getMiddleLongDistance(&right_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), RIGHT, abs(stof(down_power_pad[0].x_location) - stof((*core_site).right_x_location)), abs(stof(up_power_pad[0].x_location) - stof((*core_site).right_x_location)));
        float power_stripe_distance_up = getLongDistance(&up_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), UP, abs(stof((*core_site).up_y_location) - stof(right_power_pad[right_power_pad.size() - 1].y_location)));
        float power_stripe_distance_down = getLongDistance(&down_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), DOWN, abs(stof((*core_site).down_y_location) - stof(right_power_pad[0].y_location)));

        float power_stripe_value = 0;
        float power_rail_value = 0;

        if (power_stripe_distance_up >= power_stripe_distance_down)
        {
            power_stripe_distance = power_stripe_distance_up;
            power_rail_distance = power_rail_distance_right;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[UP][0], (*shape_map)[UP], true);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[RIGHT][0], (*shape_map)[RIGHT], true);
        }
        else
        {
            power_stripe_distance = power_stripe_distance_down;
            power_rail_distance = power_rail_distance_right;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[DOWN][0], (*shape_map)[DOWN], true);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[RIGHT][0], (*shape_map)[RIGHT], true);
        }

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else if ((*side_set).count(LEFT) && (*side_set).count(RIGHT) && (*side_set).count(DOWN))
    {
        vector<PowerPad> down_power_pad = (*direction_power_pad)[DOWN];
        vector<PowerPad> left_power_pad = (*direction_power_pad)[LEFT];
        vector<PowerPad> right_power_pad = (*direction_power_pad)[RIGHT];

        float power_rail_distance_down = getMiddleLongDistance(&down_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), DOWN, abs(stof(left_power_pad[0].y_location) - stof((*core_site).down_y_location)), abs(stof(right_power_pad[0].y_location) - stof((*core_site).down_y_location)));
        float power_stripe_rail_left = getLongDistance(&left_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), LEFT, abs(stof((*core_site).down_y_location) - stof(left_power_pad[0].y_location)));
        float power_stripe_rail_right = getLongDistance(&right_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), RIGHT, abs(stof((*core_site).down_y_location) - stof(right_power_pad[0].y_location)));

        float power_stripe_value = 0;
        float power_rail_value = 0;

        if (power_stripe_rail_left >= power_stripe_rail_right)
        {
            power_stripe_distance = power_rail_distance_down;
            power_rail_distance = power_stripe_rail_left;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[DOWN][0], (*shape_map)[DOWN], true);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[LEFT][0], (*shape_map)[LEFT], true);
        }
        else
        {
            power_stripe_distance = power_rail_distance_down;
            power_rail_distance = power_stripe_rail_right;

            power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[DOWN][0], (*shape_map)[DOWN], true);
            power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[RIGHT][0], (*shape_map)[RIGHT], true);
        }

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else
    {
        cout << "error" << endl;
    }
}

void setTwicePad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, vector<string> *side_vec, set<string> *side_set, map<string, float> *power_value_map)
{
    if ((*side_set).count(UP) && (*side_set).count(DOWN))
    {
        countTwicePadDistance(&(*direction_power_pad), &(*shape_map), &(*core_site), UP, DOWN, &(*power_value_map));
    }
    else if ((*side_set).count(UP) && (*side_set).count(RIGHT))
    {
        countTwicePadDistance(&(*direction_power_pad), &(*shape_map), &(*core_site), UP, RIGHT, &(*power_value_map));
    }
    else if ((*side_set).count(UP) && (*side_set).count(LEFT))
    {
        countTwicePadDistance(&(*direction_power_pad), &(*shape_map), &(*core_site), UP, LEFT, &(*power_value_map));
    }
    else if ((*side_set).count(DOWN) && (*side_set).count(RIGHT))
    {
        countTwicePadDistance(&(*direction_power_pad), &(*shape_map), &(*core_site), DOWN, RIGHT, &(*power_value_map));
    }
    else if ((*side_set).count(DOWN) && (*side_set).count(LEFT))
    {
        countTwicePadDistance(&(*direction_power_pad), &(*shape_map), &(*core_site), DOWN, LEFT, &(*power_value_map));
    }
    else if ((*side_set).count(RIGHT) && (*side_set).count(LEFT))
    {
        countTwicePadDistance(&(*direction_power_pad), &(*shape_map), &(*core_site), LEFT, RIGHT, &(*power_value_map));
    }
    else
    {
        cout << "twice pad error" << endl;
    }
}

void countTwicePadDistance(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, string side_first, string side_second, map<string, float> *power_value_map)
{
    if (side_first == UP && side_second == DOWN)
    {
        float up_distance = one_side_power_pad(&(*direction_power_pad)[UP], &(*core_site), UP);
        float down_distance = one_side_power_pad(&(*direction_power_pad)[DOWN], &(*core_site), DOWN);
        vector<PowerPad> power_pad_vec;

        float long_distance = 0;
        string side = "";
        if (up_distance >= down_distance)
        {
            long_distance = up_distance;
            power_pad_vec = (*direction_power_pad)[UP];
            side = UP;
        }
        else
        {
            long_distance = down_distance;
            power_pad_vec = (*direction_power_pad)[DOWN];
            side = DOWN;
        }

        float power_pad_length = stof(power_pad_vec[0].y_location) - (power_pad_vec[0].length / 2);
        float pad_to_ring = abs(stof((*shape_map)[side].start_y_location) - power_pad_length);

        float power_stripe_distance = long_distance + pad_to_ring;
        float power_rail_distance = power_stripe_distance + abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[UP][0], (*shape_map)[side], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[UP][0], (*shape_map)[side], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
        cout << "UP DOWN : "
             << "power_stripe_distance - " << power_stripe_distance << "power_rail_distance -" << power_rail_distance << endl;
        // TODO 距離
    }
    else if (side_first == UP && side_second == RIGHT)
    {
        float power_stripe_distance = 0;
        float power_rail_distance = 0;
        twice_side_power_pad(&(*direction_power_pad), &(*core_site), UP, RIGHT, &power_stripe_distance, &power_rail_distance);

        float up_power_pad_length = stof((*direction_power_pad)[UP][0].y_location) - ((*direction_power_pad)[UP][0].length / 2);
        float up_pad_to_ring = abs(stof((*shape_map)[UP].start_y_location) - up_power_pad_length);

        float right_power_pad_length = stof((*direction_power_pad)[RIGHT][0].x_location) - ((*direction_power_pad)[RIGHT][0].length / 2);
        float right_pad_to_ring = abs(stof((*shape_map)[RIGHT].start_y_location) - right_power_pad_length);

        power_stripe_distance = power_stripe_distance + up_pad_to_ring;
        power_rail_distance = power_rail_distance + right_pad_to_ring;

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[side_first][0], (*shape_map)[side_first], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[side_second][0], (*shape_map)[side_second], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));

        cout << "UP RIGHT : "
             << "power_stripe_distance - " << power_stripe_distance << " power_rail_distance -" << power_rail_distance << endl;
    }
    else if (side_first == UP && side_second == LEFT)
    {
        float power_stripe_distance = 0;
        float power_rail_distance = 0;
        twice_side_power_pad(&(*direction_power_pad), &(*core_site), UP, LEFT, &power_stripe_distance, &power_rail_distance);
        cout << "UP LEFT : "
             << "power_stripe_distance - " << power_stripe_distance << "power_rail_distance -" << power_rail_distance << endl;

        float up_power_pad_length = stof((*direction_power_pad)[UP][0].y_location) - ((*direction_power_pad)[UP][0].length / 2);
        float up_pad_to_ring = abs(stof((*shape_map)[UP].start_y_location) - up_power_pad_length);

        float left_power_pad_length = stof((*direction_power_pad)[LEFT][0].x_location) - ((*direction_power_pad)[LEFT][0].length / 2);
        float left_pad_to_ring = abs(stof((*shape_map)[LEFT].start_y_location) - left_power_pad_length);

        power_stripe_distance = power_stripe_distance + up_pad_to_ring;
        power_rail_distance = power_rail_distance + left_pad_to_ring;

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[side_first][0], (*shape_map)[side_first], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[side_second][0], (*shape_map)[side_second], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else if (side_first == DOWN && side_second == RIGHT)
    {
        float power_stripe_distance = 0;
        float power_rail_distance = 0;
        twice_side_power_pad(&(*direction_power_pad), &(*core_site), DOWN, RIGHT, &power_stripe_distance, &power_rail_distance);
        cout << "DOWN RIGHT : "
             << "power_stripe_distance - " << power_stripe_distance << "power_rail_distance -" << power_rail_distance << endl;

        float down_power_pad_length = stof((*direction_power_pad)[DOWN][0].y_location) - ((*direction_power_pad)[DOWN][0].length / 2);
        float down_pad_to_ring = abs(stof((*shape_map)[DOWN].start_y_location) - down_power_pad_length);

        float right_power_pad_length = stof((*direction_power_pad)[RIGHT][0].x_location) - ((*direction_power_pad)[RIGHT][0].length / 2);
        float right_pad_to_ring = abs(stof((*shape_map)[RIGHT].start_y_location) - right_power_pad_length);

        power_stripe_distance = power_stripe_distance + down_pad_to_ring;
        power_rail_distance = power_rail_distance + right_pad_to_ring;

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[side_first][0], (*shape_map)[side_first], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[side_second][0], (*shape_map)[side_second], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else if (side_first == DOWN && side_second == LEFT)
    {
        float power_stripe_distance = 0;
        float power_rail_distance = 0;
        twice_side_power_pad(&(*direction_power_pad), &(*core_site), DOWN, LEFT, &power_stripe_distance, &power_rail_distance);
        cout << "DOWN LEFT : "
             << "power_stripe_distance - " << power_stripe_distance << "power_rail_distance -" << power_rail_distance << endl;

        float down_power_pad_length = stof((*direction_power_pad)[DOWN][0].y_location) - ((*direction_power_pad)[DOWN][0].length / 2);
        float down_pad_to_ring = abs(stof((*shape_map)[DOWN].start_y_location) - down_power_pad_length);

        float left_power_pad_length = stof((*direction_power_pad)[LEFT][0].x_location) - ((*direction_power_pad)[LEFT][0].length / 2);
        float left_pad_to_ring = abs(stof((*shape_map)[LEFT].start_y_location) - left_power_pad_length);

        power_stripe_distance = power_stripe_distance + down_pad_to_ring;
        power_rail_distance = power_rail_distance + left_pad_to_ring;

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[side_first][0], (*shape_map)[side_first], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[side_second][0], (*shape_map)[side_second], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else if (side_first == LEFT && side_second == RIGHT)
    {

        float left_distance = one_side_power_pad(&(*direction_power_pad)[LEFT], &(*core_site), LEFT);
        float right_distance = one_side_power_pad(&(*direction_power_pad)[RIGHT], &(*core_site), RIGHT);
        vector<PowerPad> power_pad_vec;

        float long_distance = 0;
        string side = "";
        if (left_distance >= right_distance)
        {
            long_distance = left_distance;
            power_pad_vec = (*direction_power_pad)[LEFT];
            side = LEFT;
        }
        else
        {
            long_distance = right_distance;
            power_pad_vec = (*direction_power_pad)[RIGHT];
            side = RIGHT;
        }

        float power_pad_length = stof(power_pad_vec[0].x_location) - (power_pad_vec[0].length / 2);
        float pad_to_ring = abs(stof((*shape_map)[side].start_x_location) - power_pad_length);
        float power_stripe_distance = long_distance + pad_to_ring;
        float power_rail_distance = power_stripe_distance + abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, (*direction_power_pad)[side_first][0], (*shape_map)[side_first], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, (*direction_power_pad)[side_second][0], (*shape_map)[side_second], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
        cout << "LEFT RIGHT : "
             << "power_stripe_distance - " << power_stripe_distance << "power_rail_distance -" << power_rail_distance << endl;
    }
    else
    {
        cout << "twice pad error" << endl;
    }
}
float getMiddleLongDistance(vector<PowerPad> *power_pad, float first_location, float last_location, string side, float left_side_distance, float right_side_distance)
{
    float long_distance = 0;

    if (side == UP || side == DOWN)
    {
        for (int i = 0; i < (*power_pad).size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - first_location);
                temp_distance = (temp_distance + left_side_distance) / 2;
                if (long_distance < temp_distance)
                {
                    long_distance = abs(stof((*power_pad)[i].x_location) - first_location);
                }
                if ((*power_pad).size() > 1)
                {
                    float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*power_pad)[i + 1].x_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == (*power_pad).size() - 1)
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - last_location);
                temp_distance = (temp_distance + right_side_distance) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = abs(stof((*power_pad)[i].x_location) - last_location);
                }
            }
            else
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*power_pad)[i + 1].x_location)) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
    }
    // 從下面開始選上來 first
    else if (side == RIGHT || side == LEFT)
    {
        for (int i = 0; i < (*power_pad).size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - first_location);
                temp_distance = (temp_distance + left_side_distance) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if ((*power_pad).size() > 1)
                {
                    float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*power_pad)[i + 1].y_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == (*power_pad).size() - 1)
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - last_location);

                temp_distance = (temp_distance + right_side_distance) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = abs(stof((*power_pad)[i].y_location) - last_location);
                }
            }
            else
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*power_pad)[i + 1].y_location)) / 2;
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
    }
    return long_distance;
}

float getLongDistance(vector<PowerPad> *power_pad, float first_location, float last_location, string side, float other_side_distance)
{
    float long_distance = 0;

    if (side == UP || side == DOWN)
    {
        for (int i = 0; i < (*power_pad).size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - first_location);
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if ((*power_pad).size() > 1)
                {
                    float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*power_pad)[i + 1].x_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == (*power_pad).size() - 1)
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - last_location);
                temp_distance = (temp_distance + other_side_distance) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = abs(stof((*power_pad)[i].x_location) - last_location);
                }
            }
            else
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*power_pad)[i + 1].x_location)) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
    }
    // 從下面開始選上來 first
    else if (side == RIGHT || side == LEFT)
    {
        for (int i = 0; i < (*power_pad).size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - first_location);
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if ((*power_pad).size() > 1)
                {
                    float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*power_pad)[i + 1].y_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == (*power_pad).size() - 1)
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - last_location);

                temp_distance = (temp_distance + other_side_distance) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = abs(stof((*power_pad)[i].y_location) - last_location);
                }
            }
            else
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*power_pad)[i + 1].y_location)) / 2;
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
    }
    return long_distance;
}

void twice_side_power_pad(map<string, vector<PowerPad>> *direction_power_pad, CoreSite *core_site, string side_first, string side_second, float *power_stripe_distance, float *power_rail_distance)
{

    if (side_first == UP && side_second == RIGHT)
    {
        vector<PowerPad> up_power_pad = (*direction_power_pad)[UP];
        vector<PowerPad> right_power_pad = (*direction_power_pad)[RIGHT];
        //給定兩個PAD的邊界
        (*power_stripe_distance) = getLongDistance(&up_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), UP, abs(stof((*core_site).up_y_location) - stof(right_power_pad[right_power_pad.size() - 1].y_location)));
        //給定兩個PAD的邊界
        (*power_rail_distance) = getLongDistance(&right_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), RIGHT, abs(stof(up_power_pad[up_power_pad.size() - 1].x_location) - stof((*core_site).right_x_location)));
    }
    else if (side_first == UP && side_second == LEFT)
    {
        vector<PowerPad> up_power_pad = (*direction_power_pad)[UP];
        vector<PowerPad> left_power_pad = (*direction_power_pad)[LEFT];

        (*power_stripe_distance) = getLongDistance(&up_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), UP, abs(stof((*core_site).up_y_location) - stof(left_power_pad[left_power_pad.size() - 1].y_location)));
        (*power_rail_distance) = getLongDistance(&left_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), LEFT, abs(stof(up_power_pad[0].x_location) - stof((*core_site).left_x_location)));
    }
    else if (side_first == DOWN && side_second == RIGHT)
    {
        vector<PowerPad> down_power_pad = (*direction_power_pad)[DOWN];
        vector<PowerPad> right_power_pad = (*direction_power_pad)[RIGHT];

        (*power_stripe_distance) = getLongDistance(&down_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), DOWN, abs(stof((*core_site).down_y_location) - stof(right_power_pad[0].y_location)));
        (*power_rail_distance) = getLongDistance(&right_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), RIGHT, abs(stof(down_power_pad[0].x_location) - stof((*core_site).right_x_location)));
    }
    else if (side_first == DOWN && side_second == LEFT)
    {
        vector<PowerPad> down_power_pad = (*direction_power_pad)[DOWN];
        vector<PowerPad> left_power_pad = (*direction_power_pad)[LEFT];

        (*power_stripe_distance) = getLongDistance(&down_power_pad, stof((*core_site).left_x_location), stof((*core_site).right_x_location), DOWN, abs(stof((*core_site).down_y_location) - stof(left_power_pad[0].y_location)));
        (*power_rail_distance) = getLongDistance(&left_power_pad, stof((*core_site).down_y_location), stof((*core_site).up_y_location), LEFT, abs(stof(down_power_pad[0].x_location) - stof((*core_site).left_x_location)));
    }
    else
    {
        cout << "error twice_side_power_pad " << endl;
    }
}

float one_side_power_pad(vector<PowerPad> *power_pad, CoreSite *core_site, string side)
{
    float long_distance = 0;
    if (side == UP || side == DOWN)
    {
        for (int i = 0; i < (*power_pad).size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*core_site).left_x_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if ((*power_pad).size() > 1)
                {
                    float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*power_pad)[i + 1].x_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == (*power_pad).size() - 1)
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*core_site).right_x_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
            else
            {
                float temp_distance = abs(stof((*power_pad)[i].x_location) - stof((*power_pad)[i + 1].x_location)) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
    }
    else if (side == LEFT || side == RIGHT)
    {
        for (int i = 0; i < (*power_pad).size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*core_site).down_y_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if ((*power_pad).size() > 1)
                {
                    float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*power_pad)[i + 1].y_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == (*power_pad).size() - 1)
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*core_site).up_y_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
            else
            {
                float temp_distance = abs(stof((*power_pad)[i].y_location) - stof((*power_pad)[i + 1].y_location)) / 2;
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
    }
    else
    {
        cout << "one_side_power_pad error" << endl;
    }
    return long_distance;
}

// single distance powerstripe distance and rail distance
void setSinglePad(map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map, CoreSite *core_site, vector<string> *side_vec, map<string, float> *power_value_map)
{
    string side = (*side_vec)[0];

    float power_stripe_distance = 0;
    float power_rail_distance = 0;
    //只有一個
    vector<PowerPad> power_pad = (*direction_power_pad)[side];
    float long_distance = 0;
    if (side == UP || side == DOWN)
    {
        for (int i = 0; i < power_pad.size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof(power_pad[i].x_location) - stof((*core_site).left_x_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if (power_pad.size() > 1)
                {
                    float temp_distance = abs(stof(power_pad[i].x_location) - stof(power_pad[i + 1].x_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == power_pad.size() - 1)
            {
                float temp_distance = abs(stof(power_pad[i].x_location) - stof((*core_site).right_x_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
            else
            {
                float temp_distance = abs(stof(power_pad[i].x_location) - stof(power_pad[i + 1].x_location)) / 2;

                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }

        // float power_pad_length = stof(power_pad[0].y_location) - (power_pad[0].length / 2);
        // float pad_to_ring = abs(stof((*shape_map)[side].start_y_location) - power_pad_length);

        power_stripe_distance = long_distance;
        power_rail_distance = power_stripe_distance + abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));

        

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, power_pad[0], (*shape_map)[side], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, power_pad[0], (*shape_map)[side], true);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
    else
    {
        float long_distance = 0;
        for (int i = 0; i < power_pad.size(); i++)
        {
            if (i == 0)
            {
                float temp_distance = abs(stof(power_pad[i].y_location) - stof((*core_site).down_y_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
                if (power_pad.size() > 1)
                {
                    float temp_distance = abs(stof(power_pad[i].y_location) - stof(power_pad[i + 1].y_location)) / 2;
                    if (long_distance < temp_distance)
                    {
                        long_distance = temp_distance;
                    }
                }
            }
            else if (i == power_pad.size() - 1)
            {
                float temp_distance = abs(stof(power_pad[i].y_location) - stof((*core_site).up_y_location));
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
            else
            {
                float temp_distance = abs(stof(power_pad[i].y_location) - stof(power_pad[i + 1].y_location)) / 2;
                if (long_distance < temp_distance)
                {
                    long_distance = temp_distance;
                }
            }
        }
        float power_pad_length = stof(power_pad[0].x_location) - (power_pad[0].length / 2);
        float pad_to_ring = abs(stof((*shape_map)[side].start_x_location) - power_pad_length);
        power_stripe_distance = long_distance + pad_to_ring;
        power_rail_distance = power_stripe_distance + abs(stof((*core_site).up_y_location) - stof((*core_site).down_y_location));

        float power_stripe_value = setPowerStripeValue(power_stripe_distance, power_pad[0], (*shape_map)[side], true);
        float power_rail_value = setPowerRailValue(power_rail_distance, power_pad[0], (*shape_map)[side], false);

        (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));
        (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_rail_value));
    }
}

void sortPowerPad(map<string, vector<PowerPad>> *direction_power_pad)
{

    for (auto &item : (*direction_power_pad))
    {
        string direction = item.first;
        power_pad_loction_sort(&((*direction_power_pad)[direction]), direction);
    }

    for (auto &item : (*direction_power_pad))
    {
        string direction = item.first;
        if (direction == UP || direction == DOWN)
        {
            for (int i = 0; i < item.second.size(); i++)
            {
                cout << direction << " " << item.second[i].x_location << " ";
                cout << "" << endl;
            }
        }
        else
        {
            for (int i = 0; i < item.second.size(); i++)
            {
                cout << direction << " " << item.second[i].y_location << " ";
                cout << "" << endl;
            }
        }
    }
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

float caculatePowerPadToStripeResistance(vector<PowerPad> *power_pad_vector, map<string, ShapeRing> *shape_map, CoreSite *core_site)
{

    float pad_to_ring_distance = countPadToRing(&(*shape_map), &(*power_pad_vector));

    vector<float> up_pad_vector;
    vector<float> down_pad_vector;
    vector<float> left_pad_vector;
    vector<float> right_pad_vector;

    for (int i = 0; i < (*power_pad_vector).size(); i++)
    {
        if ((*power_pad_vector)[i].side == UP)
        {
            up_pad_vector.push_back(stof((*power_pad_vector)[i].x_location));
        }
        else if ((*power_pad_vector)[i].side == DOWN)
        {
            down_pad_vector.push_back(stof((*power_pad_vector)[i].x_location));
        }
        else if ((*power_pad_vector)[i].side == LEFT)
        {
            left_pad_vector.push_back(stof((*power_pad_vector)[i].y_location));
        }
        else if ((*power_pad_vector)[i].side == RIGHT)
        {
            right_pad_vector.push_back(stof((*power_pad_vector)[i].y_location));
        }
        else
        {
            cout << "power_pad_side has error" << endl;
        }
    }
    //由小到大
    sort(up_pad_vector.begin(), up_pad_vector.end());
    sort(down_pad_vector.begin(), down_pad_vector.end());
    sort(left_pad_vector.begin(), left_pad_vector.end());
    sort(right_pad_vector.begin(), right_pad_vector.end());

    map<string, vector<float>> power_pad_map;
    power_pad_map.insert(pair<string, vector<float>>(UP, up_pad_vector));
    power_pad_map.insert(pair<string, vector<float>>(DOWN, down_pad_vector));
    power_pad_map.insert(pair<string, vector<float>>(RIGHT, right_pad_vector));
    power_pad_map.insert(pair<string, vector<float>>(LEFT, left_pad_vector));

    float top_pad_distance = countPadToStripeDistance(&power_pad_map, UP, &(*core_site));
    float down_pad_distance = countPadToStripeDistance(&power_pad_map, DOWN, &(*core_site));
    float resistance = 0;
    if (top_pad_distance >= down_pad_distance)
    {
        resistance = (top_pad_distance / M2_WIDTH) * M2_SHEET_RESISTANCE + (pad_to_ring_distance / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    }
    else
    {
        resistance = (down_pad_distance / M2_WIDTH) * M2_SHEET_RESISTANCE + (pad_to_ring_distance / M3_PAD_WIDTH) * M3_SHEET_RESISTANCE_PAD;
    }

    return resistance;
}

float countPadToStripeDistance(map<string, vector<float>> *power_pad_map, string side, CoreSite *core_site)
{
    vector<float> pad_vector = (*power_pad_map)[side];
    float temp_power_stripe_distance = 0;
    float temp_distance = 0;
    if (side == UP)
    {
        for (int i = 0; i < pad_vector.size(); i++)
        {
            if (i == 0)
            {
                int last_index = (*power_pad_map)[LEFT].size() - 1;
                float lef_side = abs(stof(core_site->up_y_location) - (*power_pad_map)[LEFT][last_index]);
                float temp_pad_distance = abs(stof(core_site->left_x_location) - pad_vector[i]);
                temp_pad_distance = temp_pad_distance + lef_side;

                if (temp_distance < temp_pad_distance)
                {
                    temp_distance = temp_pad_distance;
                    temp_power_stripe_distance = abs(stof(core_site->left_x_location) - pad_vector[i]);
                }

                float temp_pad_distance_next = abs(pad_vector[i] - pad_vector[i + 1]) / 2;

                if (temp_distance < temp_pad_distance_next)
                {
                    temp_distance = temp_pad_distance_next;
                    temp_power_stripe_distance = temp_pad_distance_next;
                }
            }
            else if (i == pad_vector.size() - 1)
            {
                int last_index = (*power_pad_map)[RIGHT].size() - 1;
                float right_side = abs(stof(core_site->up_y_location) - (*power_pad_map)[RIGHT][last_index]);
                float temp_pad_distance = abs(stof(core_site->right_x_location) - pad_vector[i]);
                temp_pad_distance = temp_pad_distance + right_side;

                if (temp_distance < temp_pad_distance)
                {
                    temp_distance = temp_pad_distance;
                    temp_power_stripe_distance = abs(stof(core_site->right_x_location) - pad_vector[i]);
                }
                break;
            }
            else
            {
                float temp_pad_distance_next = abs(pad_vector[i] - pad_vector[i + 1]) / 2;

                if (temp_distance < temp_pad_distance_next)
                {
                    temp_distance = temp_pad_distance_next;
                    temp_power_stripe_distance = temp_pad_distance_next;
                }
            }
        }
    }
    else if (side == DOWN)
    {
        // float temp_power_stripe_distance = 0;
        // float temp_distance = 0;
        for (int i = 0; i < pad_vector.size(); i++)
        {
            if (i == 0)
            {
                float lef_side = abs(stof(core_site->down_y_location) - (*power_pad_map)[LEFT][0]);
                float temp_pad_distance = abs(stof(core_site->left_x_location) - pad_vector[i]);
                temp_pad_distance = temp_pad_distance + lef_side;
                if (temp_distance < temp_pad_distance)
                {
                    temp_distance = temp_pad_distance;
                    temp_power_stripe_distance = abs(stof(core_site->left_x_location) - pad_vector[i]);
                }

                float temp_pad_distance_next = abs(pad_vector[i] - pad_vector[i + 1]) / 2;
                if (temp_distance < temp_pad_distance_next)
                {
                    temp_distance = temp_pad_distance_next;
                    temp_power_stripe_distance = temp_pad_distance_next;
                }
            }
            else if (i == pad_vector.size() - 1)
            {
                float right_side = abs(stof(core_site->down_y_location) - (*power_pad_map)[RIGHT][0]);
                float temp_pad_distance = abs(stof(core_site->right_x_location) - pad_vector[i]);
                temp_pad_distance = temp_pad_distance + right_side;
                if (temp_distance < temp_pad_distance)
                {
                    temp_distance = temp_pad_distance;
                    temp_power_stripe_distance = abs(stof(core_site->right_x_location) - pad_vector[i]);
                }
            }
            else
            {
                float temp_pad_distance_next = abs(pad_vector[i] - pad_vector[i + 1]) / 2;
                if (temp_distance < temp_pad_distance_next)
                {
                    temp_distance = temp_pad_distance_next;
                    temp_power_stripe_distance = temp_pad_distance_next;
                }
            }
        }
    }

    return temp_power_stripe_distance;
}

float countPadToRing(map<string, ShapeRing> *shape_map, vector<PowerPad> *power_pad_vector)
{
    PowerPad power_pad;
    for (int i = 0; i < (*power_pad_vector).size(); i++)
    {
        if ((*power_pad_vector)[i].side == UP)
        {
            power_pad = (*power_pad_vector)[i];
            break;
        }
    }
    float power_pad_length = stof(power_pad.y_location) - (power_pad.length / 2);
    float y_location = abs(stof((*shape_map)[UP].start_y_location) - power_pad_length);

    return y_location;
}

void getPowerPadLocation(string def_file_name, vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector)
{

    ifstream def_file(def_file_name);
    string def_content;
    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("PINS") != string::npos)
            {
                while (getline(def_file, def_content))
                {
                    if (def_content.find("VDD") != string::npos || def_content.find("VSS") != string::npos)
                    {
                        PowerPad power_pad;
                        setPowerPadInfo(&def_file, def_content, &power_pad);
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
void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad)
{

    vector<string> def_content_array = splitByPattern(def_content, " ");
    (*power_pad).pad_name = def_content_array[1];
    (*power_pad).net_name = def_content_array[4];
    while (getline((*def_file), def_content))
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

void getDieArea(string def_file_name, DieArea *die_area)
{
    ifstream def_file(def_file_name);
    string def_content;
    if (def_file)
    {
        while (getline(def_file, def_content))
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

void getShapeRing(string def_file_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector)
{
    ifstream def_file(def_file_name);
    string def_content;
    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("( * VDD )") != string::npos)
            {
                while (getline(def_file, def_content))
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
                while (getline(def_file, def_content))
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