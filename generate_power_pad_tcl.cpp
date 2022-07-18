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

const string DEF_FILE = "def_file/b19/6t49_b19_routing_22_9_72_transfer.def";
const string POWER_PAD_SRIPE_FILE = "stripe_tcl/power_pad_stripe_tcl_22_9_72_2pad.tcl";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string UP = "UP";
const string DOWN = "DOWN";

const string M3_LAYER = "M3";
const string VERTICAL = "vertical";
const string HORIZONTAL = "horizontal";
const string VDD = "VDDX";
const string VSS = "VSSX";
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
struct StripeTcl
{
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string direction;
    string layer;
    string width;
    string voltage;
};

string &trim(string &str);
vector<string> splitByPattern(string content, string pattern);
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
void generatePowerPadTcl(map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector);
string floatToString(const float value);
string generateTcl(StripeTcl stripe_tcl);
int main()
{
    vector<PowerPad> vdd_power_pad_vector;
    vector<PowerPad> vss_power_pad_vector;
    vector<ShapeRing> vdd_shape_ring_vector;
    vector<ShapeRing> vss_shape_ring_vector;
    map<string, ShapeRing> vdd_shape_map;
    map<string, ShapeRing> vss_shape_map;

    DieArea die_area;

    getPowerPadLocation(DEF_FILE, &vdd_power_pad_vector, &vss_power_pad_vector);
    getDieArea(DEF_FILE, &die_area);
    setPowerPadSide(&vdd_power_pad_vector, &vss_power_pad_vector, &die_area);
    getShapeRing(DEF_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);

    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);
    generatePowerPadTcl(&vdd_shape_map, &vss_shape_map, &vdd_power_pad_vector, &vss_power_pad_vector);

    return 0;
}

void generatePowerPadTcl(map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map, vector<PowerPad> *vdd_power_pad_vector, vector<PowerPad> *vss_power_pad_vector)
{
    ofstream myfile;
    myfile.open(POWER_PAD_SRIPE_FILE);
    for (int i = 0; i < (*vdd_power_pad_vector).size(); i++)
    {
        PowerPad power_pad = (*vdd_power_pad_vector)[i];
        ShapeRing shape_ring = (*vdd_shape_map)[power_pad.side];
        float stripe_x_start_location_float = 0;
        float stripe_y_start_location_float = 0;
        float stripe_x_end_location_float = 0;
        float stripe_y_end_location_float = 0;
        if (power_pad.side == UP)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
     
            stripe_x_start_location_float = power_pad_x_loction - (power_pad.width / 2);
            stripe_y_start_location_float = stof(shape_ring.start_y_location) - (shape_ring.width / 2);
            stripe_x_end_location_float = power_pad_x_loction + (power_pad.width / 2);
            stripe_y_end_location_float = power_pad_y_location - power_pad.length / 2;
        }
        else if (power_pad.side == DOWN)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = power_pad_x_loction - (power_pad.width / 2);
            stripe_y_start_location_float = power_pad_y_location + (power_pad.length / 2);
            stripe_x_end_location_float = power_pad_x_loction + power_pad.width / 2;
            stripe_y_end_location_float = stof(shape_ring.start_y_location) + shape_ring.width / 2;

            //  cout << add_stripe_vdd_tcl << endl;
        }
        else if (power_pad.side == LEFT)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = power_pad_x_loction + power_pad.length / 2;
            stripe_y_start_location_float = power_pad_y_location + power_pad.width / 2;
            stripe_x_end_location_float = stof(shape_ring.start_x_location) + shape_ring.width / 2;
            stripe_y_end_location_float = power_pad_y_location - power_pad.width / 2;

            // cout << add_stripe_vdd_tcl << endl;
        }
        else if (power_pad.side == RIGHT)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = stof(power_pad.x_location) - power_pad.length / 2;
            stripe_y_start_location_float = stof(power_pad.y_location) + power_pad.width / 2;
            stripe_x_end_location_float = stof(shape_ring.start_x_location) - shape_ring.width / 2;
            stripe_y_end_location_float = stof(power_pad.y_location) - power_pad.width / 2;
        }
        else
        {
            cout << "power_pad has error" << endl;
        }

        StripeTcl stripe_tcl;
        stripe_tcl.start_x_location = floatToString(stripe_x_start_location_float);
        stripe_tcl.start_y_location = floatToString(stripe_y_start_location_float);
        stripe_tcl.end_x_location = floatToString(stripe_x_end_location_float);
        stripe_tcl.end_y_location = floatToString(stripe_y_end_location_float);
        stripe_tcl.layer = M3_LAYER;
        if (power_pad.side == LEFT || power_pad.side == RIGHT)
        {
            stripe_tcl.direction = HORIZONTAL;
        }
        else
        {
            stripe_tcl.direction = VERTICAL;
        }
        stripe_tcl.width = floatToString(power_pad.width);
        stripe_tcl.voltage = VDD;
        string add_stripe_vdd_tcl = generateTcl(stripe_tcl);

        myfile << add_stripe_vdd_tcl << endl;
    }

    for (int i = 0; i < (*vss_power_pad_vector).size(); i++)
    {
        PowerPad power_pad = (*vss_power_pad_vector)[i];
        ShapeRing shape_ring = (*vss_shape_map)[power_pad.side];
        float stripe_x_start_location_float = 0;
        float stripe_y_start_location_float = 0;
        float stripe_x_end_location_float = 0;
        float stripe_y_end_location_float = 0;
        if (power_pad.side == UP)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = power_pad_x_loction - (power_pad.width / 2);
            stripe_y_start_location_float = stof(shape_ring.start_y_location) - (shape_ring.width / 2);
            stripe_x_end_location_float = power_pad_x_loction + (power_pad.width / 2);
            stripe_y_end_location_float = power_pad_y_location - power_pad.length / 2;
        }
        else if (power_pad.side == DOWN)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = power_pad_x_loction - (power_pad.width / 2);
            stripe_y_start_location_float = power_pad_y_location + (power_pad.length / 2);
            stripe_x_end_location_float = power_pad_x_loction + power_pad.width / 2;
            stripe_y_end_location_float = stof(shape_ring.start_y_location) + shape_ring.width / 2;

            //  cout << add_stripe_vdd_tcl << endl;
        }
        else if (power_pad.side == LEFT)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = power_pad_x_loction + power_pad.length / 2;
            stripe_y_start_location_float = power_pad_y_location + power_pad.width / 2;
            stripe_x_end_location_float = stof(shape_ring.start_x_location) + shape_ring.width / 2;
            stripe_y_end_location_float = power_pad_y_location - power_pad.width / 2;

            // cout << add_stripe_vdd_tcl << endl;
        }
        else if (power_pad.side == RIGHT)
        {
            float power_pad_x_loction = stof(power_pad.x_location);
            float power_pad_y_location = stof(power_pad.y_location);
            stripe_x_start_location_float = stof(power_pad.x_location) - power_pad.length / 2;
            stripe_y_start_location_float = stof(power_pad.y_location) + power_pad.width / 2;
            stripe_x_end_location_float = stof(shape_ring.start_x_location) - shape_ring.width / 2;
            stripe_y_end_location_float = stof(power_pad.y_location) - power_pad.width / 2;
        }
        else
        {
            cout << "power_pad has error" << endl;
        }

        StripeTcl stripe_tcl;
        stripe_tcl.start_x_location = floatToString(stripe_x_start_location_float);
        stripe_tcl.start_y_location = floatToString(stripe_y_start_location_float);
        stripe_tcl.end_x_location = floatToString(stripe_x_end_location_float);
        stripe_tcl.end_y_location = floatToString(stripe_y_end_location_float);
        stripe_tcl.layer = M3_LAYER;
        if (power_pad.side == LEFT || power_pad.side == RIGHT)
        {
            stripe_tcl.direction = HORIZONTAL;
        }
        else
        {
            stripe_tcl.direction = VERTICAL;
        }
        stripe_tcl.width = floatToString(power_pad.width);
        stripe_tcl.voltage = VSS;
        string add_stripe_vdd_tcl = generateTcl(stripe_tcl);

        myfile << add_stripe_vdd_tcl << endl;
    }

    myfile.close();
}

string generateTcl(StripeTcl stripe_tcl)
{
    string add_stripe_vdd_tcl = "addStripe -nets { " + stripe_tcl.voltage + " } -layer " + stripe_tcl.layer + " -direction " + stripe_tcl.direction + " -width " + stripe_tcl.width + " -number_of_sets 1 -area { " + stripe_tcl.start_x_location + " " + stripe_tcl.start_y_location + " " + stripe_tcl.end_x_location + " " + stripe_tcl.end_y_location + " }";
    return add_stripe_vdd_tcl;
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
            cout << "power ring side has error "  << endl;
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
                    if (def_content.find("SHAPE STRIPE") != string::npos ||  def_content.find("+ USE GROUND") != string::npos )
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
    std::ostringstream out;
    out.precision(6);

    out << value;
    return out.str();
}

// for (int i = 0; i < vdd_shape_ring_vector.size(); i++)
// {
//     cout << vdd_shape_ring_vector[i].layer << " " << vdd_shape_ring_vector[i].start_x_location << " " << vdd_shape_ring_vector[i].start_y_location << " " << vdd_shape_ring_vector[i].end_x_location << " " << vdd_shape_ring_vector[i].end_y_location << " " << vdd_shape_ring_vector[i].side << endl;
// }