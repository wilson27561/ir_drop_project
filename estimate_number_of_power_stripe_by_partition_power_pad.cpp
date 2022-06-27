
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

const string DEF_FILE = "def_file/b19/6t49_b19_routing_44_9_53_transfer.def";
const string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
const string IP_REPORT_FILE = "ip_report/print_ip_53.report";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string MIDDLE = "middle";
const float BOUNDARY = 10;
const string UP = "UP";
const string DOWN = "DOWN";
const float TOTAL_POWER = 4.55424569;
const float M3_SHEET_RESISTANCE_PAD = 0.8119971;
const float M3_SHEET_RESISTANCE_STRIPE = 0.9;
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
const string POWERSTRIPEVALUE = "power_stripe_resistance";
const string POWERRAILVALUE = "power_rail_resistance";
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
struct CurrentRange
{
    float left_x_location;
    float right_x_location;
    float range_total_power;
    vector<string> ip_power_vector;
    float pad_to_range;
    vector<PowerStripe> power_stripe_vector;
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
void getCoreSite(string def_file_name, CoreSite *core_site);
void getDefPlacedImformation(string DEF_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
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
void setCurrentRange(CoreSite *core_site, vector<CurrentRange> *current_range_vector);
void getTrack(string def_file_name, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite);
void setTrackInfo(vector<string> def_content_array, unordered_map<string, unordered_map<string, Track>> *layer_track_map, CoreSite *corsite);
void setTrackLocation(Track *track, CoreSite *coreSite);
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad);
void getDieArea(string def_file_name, DieArea *die_area);
void getPowerPadLocation(string def_file_name, vector<PowerPad> *vdd_power_pad, vector<PowerPad> *vss_power_pad);
void setPowerPadDirection(map<string, vector<PowerPad>> *direction_power_pad);
void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
void sortPowerPad(map<string, vector<PowerPad>> *direction_power_pad);
void power_pad_loction_sort(vector<PowerPad> *power_pad_vec, string direction);
void setSide(PowerPad *power_pad, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
int setSinglePad(map<string, vector<PowerPad>> *direction_power_pad, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *shape_map, set<string> *side_set);
void setCurrentRangePowerStripe(map<string, vector<PowerPad>> *direction_power_pad, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *shape_map);
float countPadToRing(map<string, ShapeRing> *shape_map, PowerPad *power_pad, string side);
void coutPadToRing(map<string, ShapeRing> *shape_map, PowerPad *power_pad);
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line);
void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector);
void getShapeRing(string def_file_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector);
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area);
void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map);
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

    getCoreSite(DEF_FILE, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_FILE, &cell_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    setCurrentRange(&core_site, &current_range_vector);
    setIpPowerInStripe(&current_range_vector, &cell_ip_map, &cell_placed_map);
    getTrack(DEF_FILE, &layer_track_map, &core_site);

    setPowerPadDirection(&direction_power_pad);
    getPowerPadLocation(DEF_FILE, &vdd_power_pad_vector, &vss_power_pad_vector);
    getDieArea(DEF_FILE, &die_area);

    setPowerPadSide(&vdd_power_pad_vector, &vss_power_pad_vector, &direction_power_pad, &die_area);
    sortPowerPad(&direction_power_pad);

    getShapeRing(DEF_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);
    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);
    //================= 調整side start =========================
    direction_power_pad.erase(direction_power_pad.find(UP));
    direction_power_pad.erase(direction_power_pad.find(DOWN));
    direction_power_pad.erase(direction_power_pad.find(RIGHT));
    // direction_power_pad.erase(direction_power_pad.find(LEFT));
    //================= 調整side end =========================
    setCurrentRangePowerStripe(&direction_power_pad, &current_range_vector, &layer_track_map, &vdd_shape_map);

    // for (int i = 0; i < current_range_vector.si    float total = 0;ze(); i++)
    // {
    //     // cout << "range total power : " << current_range_vector[i].range_total_power << endl;
    //     total += current_range_vector[i].range_total_power;
    // }
    // cout << "total : " << total << endl;
}

void setCurrentRangePowerStripe(map<string, vector<PowerPad>> *direction_power_pad, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *shape_map)
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
        setSinglePad(&(*direction_power_pad), &(*current_range_vector), &(*layer_track_map), &(*shape_map), &side_set);
    }
    else if (side_vec.size() == 2)
    {
    }
    else if (side_vec.size() == 3)
    {
    }
    else if (side_vec.size() == 4)
    {
    }
    else
    {
        cout << "foul side : " << side_vec.size() << endl;
    }
}
float getPowerPadMiddle(map<string, vector<PowerPad>> *direction_power_pad, string side)
{
    float middle_location = 0;
    if (side == UP)
    {
        float start_x_location = stof((*direction_power_pad)[UP][0].x_location);
        int index_end = (*direction_power_pad)[UP].size() - 1;
        float end_x_location = stof((*direction_power_pad)[UP][index_end].x_location);

        middle_location = abs((start_x_location + end_x_location)) / 2;
    }
    else if (side == DOWN)
    {
        float start_x_location = stof((*direction_power_pad)[DOWN][0].x_location);
        int index_end = (*direction_power_pad)[DOWN].size() - 1;
        float end_x_location = stof((*direction_power_pad)[DOWN][index_end].x_location);

        middle_location = abs((start_x_location + end_x_location)) / 2;
    }
    else if (side == LEFT)
    {
        float start_y_location = stof((*direction_power_pad)[LEFT][0].y_location);
        int index_end = (*direction_power_pad)[LEFT].size() - 1;
        float end_y_location = stof((*direction_power_pad)[LEFT][index_end].y_location);

        middle_location = abs((start_y_location + end_y_location)) / 2;
    }
    else if (side == RIGHT)
    {
        float start_y_location = stof((*direction_power_pad)[RIGHT][0].y_location);
        int index_end = (*direction_power_pad)[RIGHT].size() - 1;
        float end_y_location = stof((*direction_power_pad)[RIGHT][index_end].y_location);

        middle_location = abs((start_y_location + end_y_location)) / 2;
    }
    else
    {
        cout << "getPowerPadMiddle error " << endl;
    }
    return middle_location;
}

void setPowerValueMap(map<string, float> *power_value_map, CurrentRange *currentRange, float middle, float pad_to_ring,CoreSite *core_site)
{
    float current_range_middle = ((*currentRange).left_x_location + (*currentRange).right_x_location) / 2;
    float distance = abs(middle - current_range_middle) / 2;

    float ring_resistance = distance * M2_SHEET_RESISTANCE;
    float pad_to_ring_resistance = distance * M3_SHEET_RESISTANCE_PAD;
    float power_stripe_value = ring_resistance + pad_to_ring_resistance;

    // float 


    // (*power_value_map).insert(pair<string, float>(POWERSTRIPEVALUE, power_stripe_value));




    // (*power_value_map).insert(pair<string, float>(POWERRAILVALUE, power_stripe_value));
}

int setSinglePad(map<string, vector<PowerPad>> *direction_power_pad, vector<CurrentRange> *current_range_vector, unordered_map<string, unordered_map<string, Track>> *layer_track_map, map<string, ShapeRing> *shape_map, set<string> *side_set)
{
    if ((*side_set).count(UP))
    {
        float middle = getPowerPadMiddle(&(*direction_power_pad), UP);
        float pad_to_ring = countPadToRing(&(*shape_map), &(*direction_power_pad)[UP][0], UP);
        cout << "pad_to_ring : " << pad_to_ring << endl;

        for (int i = 0; i < (*current_range_vector).size(); i++)
        {
            map<string, float> power_value_map;
            // float current_range_middle = ((*current_range_vector)[i].left_x_location + (*current_range_vector)[i].right_x_location) / 2;
            // float distance = abs(middle - current_range_middle) / 2;
        }
    }
    else if ((*side_set).count(DOWN))
    {
        float middle = getPowerPadMiddle(&(*direction_power_pad), DOWN);
        float pad_to_ring = countPadToRing(&(*shape_map), &(*direction_power_pad)[DOWN][0], DOWN);
        cout << "pad_to_ring : " << pad_to_ring << endl;
    }
    else if ((*side_set).count(RIGHT))
    {

        float middle = getPowerPadMiddle(&(*direction_power_pad), RIGHT);

        float pad_to_ring = countPadToRing(&(*shape_map), &(*direction_power_pad)[RIGHT][0], RIGHT);
        cout << "pad_to_ring : " << pad_to_ring << endl;
    }
    else if ((*side_set).count(LEFT))
    {
        float middle = getPowerPadMiddle(&(*direction_power_pad), LEFT);
        float pad_to_ring = countPadToRing(&(*shape_map), &(*direction_power_pad)[LEFT][0], LEFT);
        cout << "pad_to_ring : " << pad_to_ring << endl;
    }
    else
    {
        cout << " setSinglePad error " << endl;
    }
    return 0;
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

void setCurrentRange(CoreSite *core_site, vector<CurrentRange> *current_range_vector)
{
    float start = stof((*core_site).left_x_location);
    float end = stof((*core_site).right_x_location);

    for (float i = start; i < end; i += BOUNDARY)
    {
        CurrentRange current_range;
        current_range.left_x_location = i;
        current_range.right_x_location = i + BOUNDARY;
        current_range.range_total_power = 0;

        (*current_range_vector).push_back(current_range);
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
        float power = (*cell_ip_map)[cell_id].instance_power * ratio;
        (*current_range).range_total_power += power;
        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {
        float power = (*cell_ip_map)[cell_id].instance_power;

        (*current_range).range_total_power += power;

        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {
        float ratio = getConsumeRatio(RIGHT, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_power * ratio;
        (*current_range).range_total_power += power;
        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
        float ratio = getConsumeRatio(MIDDLE, left_x_location_float, right_x_location_float, move_range_x_left_float, move_range_x_right_float);
        float power = (*cell_ip_map)[cell_id].instance_power * ratio;
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
    cout << "total : " << power << endl;
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