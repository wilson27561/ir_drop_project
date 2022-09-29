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
    // vector<TrackPoint> track_point_vector;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};

string &trim(string &str);
vector<string> splitByPattern(string content, string pattern);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void generateAddStripeTcl(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, string stripe_tcl, string power_stripe_width);
string floatToString(const float value);
int main()
{

    string ADD_STRIPE_TCL = "";
    string DEF_TRANSFER_FILE = "";
    string config_file = "config.txt";
    ifstream config(config_file);
    string config_content;
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
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

            if (config_content_array[0] == "ADD_STRIPE_TCL")
            {
                ADD_STRIPE_TCL = config_content_array[2];
            }
        }
    }
    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    generateAddStripeTcl(&vdd_stripe_vector,  &vss_stripe_vector, ADD_STRIPE_TCL, "0.224");

    return 0;
}

void generateAddStripeTcl(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, string stripe_tcl, string power_stripe_width)
{
    ofstream myfile;
    myfile.open(stripe_tcl);
    float power_stripe_width_float = stof(power_stripe_width);

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        float vdd_start_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location) - (power_stripe_width_float / 2);
        float vdd_end_x_location_float = stof((*vdd_stripe_vector)[i].end_x_location) + (power_stripe_width_float / 2);
        string vdd_start_x_location = floatToString(vdd_start_x_location_float);
        string vdd_end_x_location = floatToString(vdd_end_x_location_float);

        myfile << "addStripe -nets { VDDX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_start_x_location << " " << (*vdd_stripe_vector)[i].start_y_location << " " << vdd_end_x_location << " " << (*vdd_stripe_vector)[i].end_y_location << " }" << endl;
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        float vss_start_x_location_float = stof((*vss_stripe_vector)[i].start_x_location) - (power_stripe_width_float / 2);
        float vss_end_x_location_float = stof((*vss_stripe_vector)[i].end_x_location) + (power_stripe_width_float / 2);
        string vss_start_x_location = floatToString(vss_start_x_location_float);
        string vss_end_x_location = floatToString(vss_end_x_location_float);

        myfile << "addStripe -nets { VSSX } -layer "
               << "M3"
               << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_start_x_location << " " << (*vss_stripe_vector)[i].start_y_location << " " << vss_end_x_location << " " << (*vss_stripe_vector)[i].end_y_location << " }" << endl;
    }

    myfile.close();
};

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site)
{
    cout << "========== getCoreSite start ==========" << endl;
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
    cout << "========== getCoreSite end ==========" << endl;
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
