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
const string DEF_FILE = "6t32_Asap7_tg_design_placed_floorplan_transfer.txt";
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
// the bottom loaction
struct Location
{
    string x_location;
    string y_location;
};

struct Stripe
{
   Location start_location;
   Location end_location;
};
struct StripeGroup
{
    Stripe start_stripe;
    Stripe end_stripe;
};
vector<string> splitByPattern(string content, string pattern);
void create_ir_drop_plot(vector<string> *report_content_array, unordered_map<string, Stripe> *x_ir_drop_map);
void create_add_stripe(vector<Location> *stripe_vector, set<string> *add_stripe, string x_drop_location, string y_drop_location);
void ir_drop_plot(string report);
string &trim(string &str);
bool isInRange(string range_max,string range_min,string ir_drop);
void getBoudary(vector<string> report_content_array,Boundary *boundary);
void getStripeLocation( vector<string> *def_content_array,Stripe *stripe);




int main()
{
    

    

    return 0;
}


void ir_drop_plot(string report){
    
    ifstream report_file(report);
    string report_content;


    if (report_file)
    {
        while (getline(report_file, report_content))
        {
            vector<string> report_content_array = splitByPattern(report_content, " ");
            if (report_content_array.size() == 6 && report_content_array[1] == "LISD")
            {
           
              
            }
        }
    }
    else
    {
        cout << "can't open report" << endl;
    }
    report_file.close();

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
