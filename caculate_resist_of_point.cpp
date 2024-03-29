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
#include <math.h>
#include <iomanip>

const string DEF_FILE = "def_file/b19/6t49_b19_routing_44_9_73_transfer.def";
const string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
const string M3_GRID_RESIST_FILE = "resistance_report/b19_routing_44_9_73_grid_resist_M3.report";
const string M1_GRID_RESIST_FILE = "resistance_report/b19_routing_44_9_73_grid_resist_LISD.report";
const string COMPARE_RESIST_FILE = "resistance_report/b19_routing_44_9_73_grid_resist_compare.report";

const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string MIDDLE = "middle";
const float BOUNDARY = 60;
const string UP = "UP";
const string DOWN = "DOWN";
const string M3 = "M3";
const string X = "X";

// SHEET_RESISTANCE
const float POWER_PAD_TO_POWER_RING_M3_SHEET_RESISTANCE = 0.8119971;
const float POWER_RING_M2_SHEET_RESISTANCE = 0.811738;
const float POWER_RING_M1_SHEET_RESISTANCE = 0.812;
const float POWER_STRIPE_M3_SHEET_RESISTANCE = 0.9;
const float POWER_RAIL_LISD_SHEET_RESISTANCE = 1.0416;

// WIDTH
const float POWER_RING_M2_WIDTH = 1.8;
const float POWER_RING_M1_WIDTH = 1.8;
const float POWER_PAD_TO_POWER_RING_M3_WIDTH = 1.728;
const float POWER_STRIPE_M3_WIDTH = 0.224;
const float POWER_RAIL_LISD_WIDTH = 0.1;

const float SPACING = 0.072;

struct Point
{
    float x_location;
    float y_location;
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

struct GridResistNode
{
    string layer;
    string net;
    float x_location;
    float y_location;
    string voltus_file_resist;
    string model_reist;
    string side;
    float ratio;
};

void getCoreSite(string def_file_name, CoreSite *core_site);
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getDefPlacedImformation(string DEF_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPowerPadDirection(map<string, vector<PowerPad>> *direction_power_pad);
void getPowerPadLocation(string def_file_name, vector<PowerPad> *vdd_power_pad, vector<PowerPad> *vss_power_pad);
void setPowerPadSide(vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
void sortPowerPad(map<string, vector<PowerPad>> *direction_power_pad);
void setPowerPadLengthtWidth(vector<string> *def_content_array, PowerPad *power_pad);
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad);
void setSide(PowerPad *power_pad, map<string, vector<PowerPad>> *direction_power_pad, DieArea *die_area);
void setPowerPadInfo(ifstream *def_file, string def_content, PowerPad *power_pad);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getDieArea(string def_file_name, DieArea *die_area);
void power_pad_loction_sort(vector<PowerPad> *power_pad_vec, string direction);
float twoPointDistance(float start_x_location, float start_y_location, float end_x_location, float end_y_location);
void getShapeRing(string def_file_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector);
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area);
void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map);
void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector);
string floatToString(const float value);
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line);
void getGridResistNode(string grid_file_name, vector<GridResistNode> *grid_node_vector);
float countPadToRing(map<string, ShapeRing> *shape_map, PowerPad *power_pad, string side);
void countResist(vector<PowerPad> *vdd_power_pad_vector, vector<GridResistNode> *grid_node_vector, map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map);
int getShortestResistPad(vector<PowerPad> *vdd_power_pad_vector, GridResistNode *point);
float modelResist(vector<PowerPad> *vdd_power_pad_vector, GridResistNode *grid_node, float pad_to_ring_up_down, float pad_to_ring_left_right);
void outputCompareFile(vector<GridResistNode> *grid_node_vector);
float ratio(float voltus_resist, float model_resist);
int main()
{
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
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
    vector<GridResistNode> m1_grid_node_vector;
    vector<GridResistNode> m3_grid_node_vector;

    getGridResistNode(M3_GRID_RESIST_FILE, &m3_grid_node_vector);
    getGridResistNode(M1_GRID_RESIST_FILE, &m1_grid_node_vector);

    getCoreSite(DEF_FILE, &core_site);
    getDieArea(DEF_FILE, &die_area);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_FILE, &cell_placed_map, &cell_info_map);
    setPowerPadDirection(&direction_power_pad);
    getPowerPadLocation(DEF_FILE, &vdd_power_pad_vector, &vss_power_pad_vector);
    setPowerPadSide(&vdd_power_pad_vector, &vss_power_pad_vector, &direction_power_pad, &die_area);
    sortPowerPad(&direction_power_pad);

    getShapeRing(DEF_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);
    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);

    countResist(&vdd_power_pad_vector, &m1_grid_node_vector, &direction_power_pad, &vdd_shape_map);
    outputCompareFile(&m1_grid_node_vector);
    // countResist(&vdd_power_pad_vector, &m1_grid_node_vector, &direction_power_pad, &vdd_shape_map);

    for (int i = 0; i < vdd_power_pad_vector.size(); i++)
    {
        cout << vdd_power_pad_vector[i].pad_name << " " << vdd_power_pad_vector[i].side << " location -> " << vdd_power_pad_vector[i].x_location << " " << vdd_power_pad_vector[i].y_location << endl;
    }

    return 0;
}

void outputCompareFile(vector<GridResistNode> *grid_node_vector)
{
    ofstream myfile;
    myfile.open(COMPARE_RESIST_FILE);

    for (int i = 0; i < (*grid_node_vector).size(); i++)
    {
        // myfile << (*grid_node_vector)[i].side << "  node location : " << (*grid_node_vector)[i].x_location << " " << (*grid_node_vector)[i].y_location << "    " << (*grid_node_vector)[i].voltus_file_resist << "    " << (*grid_node_vector)[i].model_reist << "    " << ratio(stof((*grid_node_vector)[i].voltus_file_resist), stof( (*grid_node_vector)[i].model_reist)) << endl;
         myfile <<  (*grid_node_vector)[i].model_reist  << endl;
    }
    myfile.close();
}

float ratio(float voltus_resist, float model_resist)
{
    float resist = ( model_resist - voltus_resist) / voltus_resist;

    return resist;
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
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area)
{
    float middle_x_line = stof(die_area->top_right_x_location) / 2;
    float middle_y_line = stof(die_area->top_right_y_location) / 2;
    setRingSide(&(*vdd_shape_ring_vector), middle_x_line, middle_y_line);
    setRingSide(&(*vss_shape_ring_vector), middle_x_line, middle_y_line);
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

void getGridResistNode(string grid_file_name, vector<GridResistNode> *grid_node_vector)
{
    ifstream grid_file(grid_file_name);
    string grid_file_content;
    if (grid_file)
    {
        while (getline(grid_file, grid_file_content))
        {
            vector<string> grid_content_array = splitByPattern(grid_file_content, " ");
            if (grid_content_array.size() == 6)
            {
                // cout << grid_file_content << "  " << grid_content_array.size() << endl;
                GridResistNode grid_reist_node;
                grid_reist_node.voltus_file_resist = grid_content_array[0];
                grid_reist_node.layer = grid_content_array[1];
                grid_reist_node.x_location = stof(grid_content_array[2]);
                grid_reist_node.y_location = stof(grid_content_array[3]);
                grid_reist_node.net = grid_content_array[4];
                (*grid_node_vector).push_back(grid_reist_node);
            }
        }
    }
}

int getShortestResistPad(vector<PowerPad> *vdd_power_pad_vector, GridResistNode *point)
{
    int index = 0;
    float shortest_distance = 0;
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        // cout << "=============" << endl;
        // cout << "point : " << (*point).x_location << " " << (*point).y_location << endl;
        // cout << (*vdd_power_pad_vector)[i].side << "  vdd pad location: " << (*vdd_power_pad_vector)[i].x_location << " " << (*vdd_power_pad_vector)[i].y_location << endl;
        float distance = twoPointDistance((*point).x_location, (*point).y_location, stof((*vdd_power_pad_vector)[i].x_location), stof((*vdd_power_pad_vector)[i].y_location));
        // cout << "distance :" << distance << endl;
        // cout << "=============" << endl;
        if (shortest_distance == 0)
        {
            shortest_distance = distance;
        }
        else if (distance < shortest_distance)
        {
            shortest_distance = distance;
            index = i;
            // cout << "index : " << index << endl;
        }
    }
    // cout << "index : " << index << endl;
    // cout << "shortest_distance : " << shortest_distance << endl;
    return index;
}

void countResist(vector<PowerPad> *vdd_power_pad_vector, vector<GridResistNode> *grid_node_vector, map<string, vector<PowerPad>> *direction_power_pad, map<string, ShapeRing> *shape_map)
{
    float pad_to_ring_up_down = countPadToRing(&(*shape_map), &(*direction_power_pad)[UP][0], UP);
    float pad_to_ring_left_right = countPadToRing(&(*shape_map), &(*direction_power_pad)[LEFT][0], LEFT);

    for (int i = 0; i < (*grid_node_vector).size(); i++)
    {
        // int index = getShortestResistPad(&(*vdd_power_pad_vector), &((*grid_node_vector)[i]));
        float resist = modelResist(&(*vdd_power_pad_vector), &((*grid_node_vector)[i]), pad_to_ring_up_down, pad_to_ring_left_right);
        (*grid_node_vector)[i].model_reist = floatToString(resist);

        // cout << "near pad : " << (*grid_node_vector)[i].side << " " <<  (*grid_node_vector)[i].x_location << " " <<  (*grid_node_vector)[i].y_location <<  " " << (*grid_node_vector)[i].model_reist << endl;
    }
}

float compareModelResist(PowerPad *power_pad, GridResistNode *grid_node, float pad_to_ring_up_down, float pad_to_ring_left_right)
{
    if ((*power_pad).side == UP)
    {
        // pad_ring
        float pad_to_ring_resist_up_down = (pad_to_ring_up_down / POWER_PAD_TO_POWER_RING_M3_WIDTH) * POWER_PAD_TO_POWER_RING_M3_SHEET_RESISTANCE;
        //橫 直
        float horizontal_distance = abs(stof((*power_pad).x_location) - (*grid_node).x_location);
        float vertical_distance = abs(stof((*power_pad).y_location) - (*grid_node).y_location);

        float horizontal_resist = (horizontal_distance / POWER_RING_M2_WIDTH) * POWER_RING_M2_SHEET_RESISTANCE;
        float vertical_resist = (vertical_distance / POWER_STRIPE_M3_WIDTH) * POWER_STRIPE_M3_SHEET_RESISTANCE;

        return (pad_to_ring_resist_up_down + horizontal_resist + vertical_resist);
    }
    else if ((*power_pad).side == DOWN)
    {
        float pad_to_ring_resist_up_down = (pad_to_ring_up_down / POWER_PAD_TO_POWER_RING_M3_WIDTH) * POWER_PAD_TO_POWER_RING_M3_SHEET_RESISTANCE;
        //橫 直
        float horizontal_distance = abs(stof((*power_pad).x_location) - (*grid_node).x_location);
        float vertical_distance = abs(stof((*power_pad).y_location) - (*grid_node).y_location);

        float horizontal_resist = (horizontal_distance / POWER_RING_M2_WIDTH) * POWER_RING_M2_SHEET_RESISTANCE;
        float vertical_resist = (vertical_distance / POWER_STRIPE_M3_WIDTH) * POWER_STRIPE_M3_SHEET_RESISTANCE;

        return (pad_to_ring_resist_up_down + horizontal_resist + vertical_resist);
    }
    else if ((*power_pad).side == LEFT)
    {
        float pad_to_ring_resist_left_right = (pad_to_ring_left_right / POWER_PAD_TO_POWER_RING_M3_WIDTH) * POWER_PAD_TO_POWER_RING_M3_SHEET_RESISTANCE;
        //橫 直
        float horizontal_distance = abs(stof((*power_pad).x_location) - (*grid_node).x_location);
        float vertical_distance = abs(stof((*power_pad).y_location) - (*grid_node).y_location);

        float vertical_resist = (vertical_distance / POWER_RING_M1_WIDTH) * POWER_RING_M1_SHEET_RESISTANCE;
        float horizontal_resist = (horizontal_distance / POWER_RAIL_LISD_WIDTH) * POWER_RAIL_LISD_SHEET_RESISTANCE;

        return (pad_to_ring_resist_left_right + vertical_resist + horizontal_resist);
    }
    else if ((*power_pad).side == RIGHT)
    {
        float pad_to_ring_resist_left_right = (pad_to_ring_left_right / POWER_PAD_TO_POWER_RING_M3_WIDTH) * POWER_PAD_TO_POWER_RING_M3_SHEET_RESISTANCE;
        //橫 直
        float horizontal_distance = abs(stof((*power_pad).x_location) - (*grid_node).x_location);
        float vertical_distance = abs(stof((*power_pad).y_location) - (*grid_node).y_location);

        float vertical_resist = (vertical_distance / POWER_RING_M1_WIDTH) * POWER_RING_M1_SHEET_RESISTANCE;
        float horizontal_resist = (horizontal_distance / POWER_RAIL_LISD_WIDTH) * POWER_RAIL_LISD_SHEET_RESISTANCE;

        return (pad_to_ring_resist_left_right + vertical_resist + horizontal_resist);
    }
    else
    {
        cout << " power side error  " << endl;
    }
}

float modelResist(vector<PowerPad> *vdd_power_pad_vector, GridResistNode *grid_node, float pad_to_ring_up_down, float pad_to_ring_left_right)
{
    float resist = 10000000000000;
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        float resist_node = compareModelResist(&(*vdd_power_pad_vector)[i], &(*grid_node), pad_to_ring_up_down, pad_to_ring_left_right);
        if (resist_node < resist)
        {
            resist = resist_node;
            (*grid_node).side = (*vdd_power_pad_vector)[i].side;
        }
    }
    return resist;
}

float twoPointDistance(float start_x_location, float start_y_location, float end_x_location, float end_y_location)
{
    int start_x_location_int = stoi(floatToString(start_x_location));
    int start_y_location_int = stoi(floatToString(start_y_location));
    int end_x_location_int = stoi(floatToString(end_x_location));
    int end_y_location_int = stoi(floatToString(end_y_location));

    int x_distance = abs(start_x_location_int - end_x_location_int);
    int y_distance = abs(start_y_location_int - end_y_location_int);

    int x_distance_pow = pow(x_distance, 2);
    int y_distance_pow = pow(y_distance, 2);

    int power_add = x_distance_pow + y_distance_pow;
    int distance = pow(power_add, 0.5);

    return distance;
}

string floatToString(const float value)
{
    std::ostringstream out;
    out.precision(6);

    out << value;
    return out.str();
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
void setPowerPadLocation(vector<string> *def_content_array, PowerPad *power_pad)
{
    string x_location = (*def_content_array)[3];
    string y_location = (*def_content_array)[4];
    (*power_pad).x_location = x_location;
    (*power_pad).y_location = y_location;
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