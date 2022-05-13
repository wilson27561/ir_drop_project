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
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string IR_DROP_LAYER = "LISD";
const string DEF_FILE_ORI = "def_file/b19/6t49_b19_routing_88_9_39_transfer.def";
const string IR_DROP_FILE = "ir_drop_report/print_irdrop_PD1_25C_avg_1_6t49_b19_routing_88_2_LISD.enc_VDDX.report";
const string IR_COUNT_REPORT_FILE = "ir_drop_report/ir_report_all.txt";
const float POWER_STRIPE_WIDTH = 0.224;
struct Via
{
    string via_name;
    string x_location;
    string y_location;
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
    float ir_drop_voltus = 0;
};
struct IrDropNode{
    float ir_drop;
    string layer;
    string x_location;
    string y_loaction;
    string net;
    string node_id;

};

void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector);
void transferStripeVectorToMap(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, map<string, Stripe> *vdd_stripe_map, map<string, Stripe> *vss_stripe_map);
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
void getIrDropFromVoltus(map<string, Stripe> *vdd_stripe_map);
void printPowerStripeIrDropFromVoltus(map<string, Stripe> *vdd_stripe_map,vector<Stripe> *vdd_stripe_vector);
int main()
{
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    map<string, Stripe> vdd_stripe_map;
    map<string, Stripe> vss_stripe_map;
    getStripeLocation(DEF_FILE_ORI, &vdd_stripe_vector, &vss_stripe_vector);
    transferStripeVectorToMap(&vdd_stripe_vector, &vss_stripe_vector, &vdd_stripe_map, &vss_stripe_map);
    getIrDropFromVoltus( &vdd_stripe_map);
    printPowerStripeIrDropFromVoltus(&vdd_stripe_map, &vdd_stripe_vector);

   
    

    return 0;
}
void printPowerStripeIrDropFromVoltus(map<string, Stripe> *vdd_stripe_map,vector<Stripe> *vdd_stripe_vector){
    ofstream myfile;
    myfile.open(IR_COUNT_REPORT_FILE);

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        string start_x_location = (*vdd_stripe_vector)[i].start_x_location;
        if((*vdd_stripe_map).count((*vdd_stripe_vector)[i].start_x_location)){
            float ir_drop_voltus_float = (*vdd_stripe_map)[(*vdd_stripe_vector)[i].start_x_location].ir_drop_voltus;
            ir_drop_voltus_float = ir_drop_voltus_float*0.001;
            myfile<<  0.7 - ir_drop_voltus_float << endl;
        }
    }
    myfile.close();
    

}

void getIrDropFromVoltus(map<string, Stripe> *vdd_stripe_map)
{
    ifstream ir_drop_file(IR_DROP_FILE);
    string ir_drop_content;

    if (ir_drop_file)
    {
        while (getline(ir_drop_file, ir_drop_content))
        {
            vector<string> ir_drop_content_array = splitByPattern(ir_drop_content, " ");
             if(ir_drop_content_array.size() == 6)
             {
          
                  IrDropNode ir_drop_node;
                  ir_drop_node.ir_drop = stof(ir_drop_content_array[0]);
                  ir_drop_node.layer = ir_drop_content_array[1];
                  ir_drop_node.x_location = ir_drop_content_array[2];
                  ir_drop_node.y_loaction = ir_drop_content_array[3];
                  ir_drop_node.net = ir_drop_content_array[4];
                  ir_drop_node.node_id = ir_drop_content_array[5];
                
                  if((*vdd_stripe_map).count(ir_drop_node.x_location)){
                      Stripe vdd_stripe = (*vdd_stripe_map)[ir_drop_node.x_location];
                      if(vdd_stripe.ir_drop_voltus < ir_drop_node.ir_drop){
                        (*vdd_stripe_map)[ir_drop_node.x_location].ir_drop_voltus = ir_drop_node.ir_drop;
                      }
                  }
             }
        }
        
    }
    else
    {
        cout << "can't found ir drop report" << endl;
    }
}

//只讀到Stripe 就 break
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector)
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