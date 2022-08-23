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

const string REPORT_VSS_FILE = "print_irdrop_PD1_25C_avg_1_6t49_b19_routing_88_2_LISD.enc_VSSX.report";
const string REPORT_VDD_FILE = "print_irdrop_PD1_25C_avg_1_6t49_b19_routing_88_2_LISD.enc_VDDX.report";
// const string DEF_TRANSFER_FILE = "6t32_Asap7_tg_design_placed_floorplan_transfer.txt";
const string ADD_STRIPE_FILE = "add_stripe_PD1_6t49_b19_routing_88_avg_1_min_stripe.tcl";
const string STRIPE_METAL = "M4";
const string VDD_RANGE_MIN = "237";
const string VDD_RANGE_MAX = "242";
const string VSS_RANGE_MIN = "137";
const string VSS_RANGE_MAX = "146";

struct Boundary
{
    float left;
    float right;
    float up;
    float down;
};

struct Node
{
    string node_id;
    string ir;
    string layer;
    string x_location;
    string y_location;
    string net;
};
struct Stripe
{
    // key : y_location , value : node
    map<string, vector<Node>> y_node_map;
};
// the bottom loaction
struct Location
{
    string x_location;
    string y_location;
};

vector<string> splitByPattern(string content, string pattern);
void create_ir_drop_plot(vector<string> *report_content_array, unordered_map<string, Stripe> *x_ir_drop_map);
void create_add_stripe(vector<Location> *stripe_vector, set<string> *add_stripe, string x_drop_location, string y_drop_location);
void ir_drop_plot(string report, set<string> *add_stripe, vector<Location> *stripe_vector, string range_min, string range_max);
string &trim(string &str);
bool isInRange(string range_max, string range_min, string ir_drop);
void getBoudary(vector<string> report_content_array, Boundary *boundary);

int main()
{

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
        }
    }

    ifstream def_file(DEF_TRANSFER_FILE);
    string def_content;
    vector<Location> vdd_stripe_vector;
    vector<Location> vss_stripe_vector;

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
                            Location location;
                            location.x_location = def_content_array[7];
                            location.y_location = def_content_array[8];
                            vdd_stripe_vector.push_back(location);
                        }
                    }
                }
                // VSSX stripe
                while (getline(def_file, def_content))
                {
                    if (def_content.find("NEW") != string::npos)
                    {
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        if (def_content_array[4] == "SHAPE" && def_content_array[5] == "STRIPE" && def_content_array.size() == 14)
                        {
                            cout << "VSSX :" << def_content << " " << def_content_array.size() << endl;
                            Location location;
                            location.x_location = def_content_array[7];
                            location.y_location = def_content_array[8];
                            vss_stripe_vector.push_back(location);
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

    set<string> vss_add_stripe;
    set<string> vdd_add_stripe;
    cout << "vdd_stripe_vector size : " << vdd_stripe_vector.size() << endl;
    cout << "vss_stripe_vector size : " << vss_stripe_vector.size() << endl;

    // ir_drop_plot(REPORT_VDD_FILE,&vdd_add_stripe, &vdd_stripe_vector,VDD_RANGE_MIN,VDD_RANGE_MAX);
    // ir_drop_plot(REPORT_VSS_FILE,&vss_add_stripe, &vss_stripe_vector,VSS_RANGE_MIN,VSS_RANGE_MAX);

    // ofstream myfile;
    // myfile.open(ADD_STRIPE_FILE);

    // for (set<string>::iterator it = vdd_add_stripe.begin(); it != vdd_add_stripe.end(); ++it) {
    //       myfile << "addStripe -nets { VDDX } -layer "<< STRIPE_METAL << " -direction horizontal -width 0.216 -set_to_set_distance 12.88 -number_of_sets 1  -area {" << *it << "}" << endl;

    // }
    // for (set<string>::iterator it = vss_add_stripe.begin(); it != vss_add_stripe.end(); ++it) {
    //       myfile << "addStripe -nets { VSSX } -layer "<< STRIPE_METAL << " -direction horizontal -width 0.216 -set_to_set_distance 12.88 -number_of_sets 1  -area {" << *it << "}" << endl;
    // }
    // myfile.close();

    return 0;
}

void ir_drop_plot(string report, set<string> *add_stripe, vector<Location> *stripe_vector, string range_min, string range_max)
{

    ifstream report_file(report);
    string report_content;
    Boundary boundary;

    if (report_file)
    {
        while (getline(report_file, report_content))
        {
            vector<string> report_content_array = splitByPattern(report_content, " ");
            if (report_content_array.size() == 6 && report_content_array[1] == "LISD")
            {
                // if(isInRange(range_max,range_min, report_content_array[0])){
                //       cout<< "node id : " << report_content_array[5] <<  "  " << report_content_array[4] << "  ";
                //       create_add_stripe(&(*stripe_vector), &(*add_stripe), report_content_array[2], report_content_array[3]);
                // }
                getBoudary(report_content_array, &boundary);
            }
        }
    }
    else
    {
        cout << "can't open report" << endl;
    }
    report_file.close();
}

void getBoudary(vector<string> report_content_array, Boundary *boundary)
{
    string ir_drop = report_content_array[0];
    string x_location = report_content_array[2];
    string y_location = report_content_array[3];
    float num_x_location = stof(x_location);
    float num_y_location = stof(y_location);
    if (num_x_location > (*boundary).left)
    {
        (*boundary).left = num_x_location;
    }
    if (num_x_location < (*boundary).right)
    {
        (*boundary).right = num_x_location;
    }
    if (num_y_location > (*boundary).up)
    {
        (*boundary).up = num_y_location;
    }
    if (num_y_location < (*boundary).down)
    {
        (*boundary).down = num_y_location;
    }
}

bool isInRange(string range_max, string range_min, string ir_drop)
{
    float num_range_max = stof(range_max);
    float num_range_min = stof(range_min);
    float num_ir_drop = stof(ir_drop);

    if (num_ir_drop >= num_range_min && num_ir_drop <= num_range_max)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void create_add_stripe(vector<Location> *stripe_vector, set<string> *add_stripe, string x_drop_location, string y_drop_location)
{

    for (int i = 0; i < ((*stripe_vector).size() - 1); i++)
    {
        string x_stripe_start_location = (*stripe_vector)[i].x_location;
        string y_stripe_start_location = (*stripe_vector)[i].y_location;
        string x_stripe_end_location = (*stripe_vector)[i + 1].x_location;
        string y_stripe_end_location = (*stripe_vector)[i + 1].y_location;
        float x_stripe_start = std::stof(x_stripe_start_location);
        float y_stripe_start = std::stof(y_stripe_start_location);
        float x_stripe_end = std::stof(x_stripe_end_location);
        float y_stripe_end = std::stof(y_stripe_end_location);
        float x_drop = std::stof(x_drop_location);
        float y_drop = std::stof(y_drop_location);
        string y_drop_end_location = to_string(y_drop + 3);

        if ((x_stripe_start <= x_drop) && (x_drop <= x_stripe_end))
        {
            string stripe_location = x_stripe_start_location + " " + y_drop_location + " " + x_stripe_end_location + " " + y_drop_end_location;
            cout << stripe_location << endl;

            break;
        }
    }
}

void create_ir_drop_plot(vector<string> *report_content_array, unordered_map<string, Stripe> *x_ir_drop_map)
{
    Node node;
    node.ir = (*report_content_array)[0];
    node.layer = (*report_content_array)[1];
    node.x_location = (*report_content_array)[2];
    node.y_location = (*report_content_array)[3];
    node.net = (*report_content_array)[4];
    node.node_id = (*report_content_array)[5];

    if ((*x_ir_drop_map).count(node.x_location))
    {
        (*x_ir_drop_map)[node.x_location].y_node_map[node.y_location].push_back(node);
    }
    else
    {
        Stripe stripe;
        map<string, vector<Node>> y_node_map;
        vector<Node> node_vector;
        y_node_map.insert(pair<string, vector<Node>>(node.y_location, node_vector));
        (*x_ir_drop_map).insert(pair<string, Stripe>(node.x_location, stripe));
        (*x_ir_drop_map)[node.x_location].y_node_map[node.y_location].push_back(node);
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

//   int i=0;
//    for (const auto &item1 :  x_ir_drop_map)
// {
//     for (const auto &item2 : item1.second.y_node_map)
//     {
//         for (const auto &item3 : item2.second)
//         {
//           cout << "id : " << item3.node_id << endl;
//           i++;
//         }
//     }
// }