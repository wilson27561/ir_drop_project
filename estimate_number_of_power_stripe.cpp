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

const float TOTAL_POWER = 4.743;
// const float M3_SHEET_RESISTANCE = 3.1445326;
// const float M1_SHEET_RESISTANCE = 3.1445326;
// const float M1_SHEET_RESISTANCE = 3.1445326;
const float M3_SHEET_RESISTANCE_PAD = 0.8119971;
const float M3_SHEET_RESISTANCE_STRIPE = 0.9;
const float M2_SHEET_RESISTANCE = 0.811738;
const float M1_SHEET_RESISTANCE = 1.2;
const float M3_PAD_WIDTH = 1.728;
const float M2_WIDTH = 1.8;
// const float V1_SHEET_RESISTANCE = 8.7348129;
// const float V2_SHEET_RESISTANCE = 8.7348129;
const float VDD_PAD = 0.7;
const float IR_DROP = 0.03;
const float POWER_STRIPE_HEIGHT = 313.92;
// const float POWER_STRIPE_HEIGHT = 156.96;
const float POWER_STRIPE_WIDTH = 0.224;
const float POWER_RAIL_HEIGHT = 385.344;
// const float POWER_RAIL_HEIGHT = 192.672;
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

const string DEF_FILE = "def_file/b19/6t32_powerpad_design_placed_52_transfer.def";

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
int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number,float pad_to_stripe_resist);
void getPowerPadLocation(string def_file_name, vector<PowerPad> *vdd_power_pad, vector<PowerPad> *vss_power_pad);
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad);
void getDieArea(string def_file_name, DieArea *die_area);
void setSide(PowerPad *power_pad, DieArea *die_area);
void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, DieArea *die_area);
void getShapeRing(string def_file_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector);
void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector);
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area);
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line);
void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map);
float caculatePowerPadToStripeResistance(vector<PowerPad> *power_pad_vector, map<string, ShapeRing> *shape_map, CoreSite *core_site);
// void countPadToStripeDistance(vector<float> *pad_x_location_vector, string side, CoreSite *core_site, map<string, float> *distance_pad_to_stripe_map, map<string, ShapeRing> *shape_map);
float countPadToStripeDistance(map<string, vector<float>> *power_pad_map, string side, CoreSite *core_site);
float countPadToRing(map<string, ShapeRing> *shape_map, vector<PowerPad> *power_pad_vector);

float getPowerPadMiddle(vector<PowerPad> *power_pad_vector, map<string, ShapeRing> *shape_map, string side);
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

    getPowerPadLocation(DEF_FILE, &vdd_power_pad_vector, &vss_power_pad_vector);
    getDieArea(DEF_FILE, &die_area);
    setPowerPadSide(&vdd_power_pad_vector, &vss_power_pad_vector, &die_area);
    getShapeRing(DEF_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);
    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);
    getCoreSite(DEF_FILE, &core_site);

    float pad_to_stripe_resistance = caculatePowerPadToStripeResistance(&vdd_power_pad_vector, &vdd_shape_map, &core_site);
    cout << pad_to_stripe_resistance << endl;
    float power_stripes_number = caculate_power_stripe(TOTAL_POWER, IR_DROP, M3_SHEET_RESISTANCE_PAD, M1_SHEET_RESISTANCE, VDD_PAD, POWER_STRIPE_HEIGHT, POWER_STRIPE_WIDTH, POWER_RAIL_HEIGHT, POWER_RAIL_WIDTH, POWER_RAIL_NUMBER,pad_to_stripe_resistance);

    float interval = getInterval(&core_site, power_stripes_number);
   
    cout << "ir-drop : " << 0.7 - IR_DROP << " "  << IR_DROP << endl;
    cout << "before power stripe number : " << power_stripes_number << endl;
    cout << "before interval : " << interval << endl;

    return 0;
};
int caculate_power_stripe(float total_power, float ir_drop, float m3_sheet_resistance, float m1_sheet_resistance, float vdd_pad, float power_stripe_height, float power_stripe_width, float power_rail_height, float power_rail_width, int power_rail_number,float pad_to_stripe_resist)
{
    float m3_square = (power_stripe_height/2) / power_stripe_width;
    float m1_square = (power_rail_height/2) / power_rail_width;
    float m3_resistance = m3_sheet_resistance * m3_square;
    float m1_resistance = m1_sheet_resistance * m1_square;

    m3_resistance = m3_resistance+pad_to_stripe_resist; // pad to stripe resistance


    total_power = total_power * 0.001;

    float m3_current = ir_drop / (m3_resistance);
    float m1_current = ir_drop / (m1_resistance);

    float temp_total_current = (total_power / vdd_pad);

    float temp_current = ((power_rail_number * 2  * m1_current));
   

    temp_total_current = temp_total_current - temp_current;

    int m3_number = temp_total_current / m3_current;
    m3_number = m3_number/2 ;
    return m3_number;
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

void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, DieArea *die_area)
{
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        setSide(&(*vdd_power_pad_vector)[i], &(*die_area));
    }
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        setSide(&(*vss_power_pad_vector)[i], &(*die_area));
    }
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
void setSide(PowerPad *power_pad, DieArea *die_area)
{
    if (stof(power_pad->x_location) == stof(die_area->lower_left_x_location))
    {
        power_pad->side = LEFT;
    }
    else if (stof((power_pad->y_location)) == stof(die_area->lower_left_y_location))
    {
        power_pad->side = DOWN;
    }
    else if (stof(power_pad->x_location) == stof(die_area->top_right_x_location))
    {
        power_pad->side = RIGHT;
    }
    else if (stof(power_pad->y_location) == stof(die_area->top_right_y_location))
    {
        power_pad->side = UP;
    }
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

// float getPowerPadMiddle(vector<PowerPad> *power_pad_vector, map<string, ShapeRing> *shape_map, string side)
// {
//     float y_location = 0;
//     //距離與voltus 有些許落差 有時間再改
//     for (int i = 0; i < (*power_pad_vector).size(); i++)
//     {
//         if ((*power_pad_vector)[i].side == side)
//         {
//             if (side == UP)
//             {
//                 y_location = stof((*power_pad_vector)[i].y_location) - ((*power_pad_vector)[i].length / 2);
//                 if ((*shape_map).count(side))
//                 {
//                     y_location = abs(stof((*shape_map)[side].start_y_location) - y_location);
//                     cout << "UP : " << y_location << endl;
//                 }
//                 else
//                 {
//                     cout << "side error" << endl;
//                 }
//             }
//             else if (side == DOWN)
//             {
//                 float y_location = stof((*power_pad_vector)[i].y_location) + ((*power_pad_vector)[i].length / 2);
//                 if ((*shape_map).count(side))
//                 {
//                     y_location = abs(stof((*shape_map)[side].start_y_location) - y_location);
//                     cout << "DOWN : " << y_location << endl;
//                 }
//                 else
//                 {
//                     cout << "side error" << endl;
//                 }
//             }
//             else
//             {
//                 cout << " side error " << endl;
//             }

//             break;
//         }
//     }
//     return y_location;
// }