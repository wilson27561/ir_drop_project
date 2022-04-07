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

const string DEF_FILE = "6t49_b19_routing_88_9_floorplan_transfer.def";
const string EFFR_FILE_VDD = "VDDX.effr";
const string ADD_STRIPE_FILE = "add_stripe_PD9_6t49_b19_routing_88_9_stripe_change_range.tcl";
const string LEF_FILE = "characterization_6T_ALL_20200610area_4x.lef";
const string IP_REPORT_FILE = "VDDX_ip_drop.report";
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string STRIPE_METAL = "M3";
// the bottom loaction
struct Location
{
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
};
struct Range
{
    Stripe star_stripe;
    Stripe end_stripe;
};
struct ResistNode
{
    string reff;
    string instance_id;
    string pin;
    string cell_name;
    string x_location;
    string y_location;
    string layer;
};

struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
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
    string instance_power;
};
struct CellBoundary
{
    string lef_x_location;
    string right_x_location;
    vector<string> cell_id_vector;
};

vector<string> splitByPattern(string content, string pattern);
vector<string> splitByPatternEffrReport(string content, string pattern);
string &trim(string &str);
void getCoreSite(string def_file_name, CoreSite *core_site);
void getStripeMovingRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, vector<Range> *vdd_range_vector, vector<Range> *vss_range_vector);
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_map);
void getDefPlacedImformation(string DEF_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getIpBoudary(unordered_map<string, CellInstancePowerInfo> *cell_ip_map, vector<CellBoundary> *cell_boundary_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void setCellBoundary(vector<CellBoundary> *cell_boundary_vector, CellInstancePowerInfo *cell_instance_power_info, unordered_map<string, CellPlacedInfo> *cell_placed_map);
bool adjustCellBoundary(CellBoundary *cell_boundary, CellPlacedInfo *cell_placed_info);
void setPlacePosition(vector<string> *def_content_array,CellPlacedInfo *cell_placed_info);
bool AllisNum(string str); 
int main()
{
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    vector<Range> vdd_range_vector;
    vector<Range> vss_range_vector;
    map<string, ResistLine> resist_line_vdd_map;
    map<string, vector<ResistNode>> resist_node_vdd_map;
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    vector<CellBoundary> cell_boundary_vector;

    //   //算出middle line
    //   //將它分成區段
    // getStripeLocation(DEF_FILE, &vdd_stripe_vector, &vss_stripe_vector, &vdd_range_vector, &vss_range_vector);
    // getCoreSite( DEF_FILE, &core_site);
    // getStripeMovingRange(&vdd_stripe_vector, &vss_stripe_vector, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_FILE, &cell_placed_map, &cell_info_map);
    // TODO boundary 之後可以寫在裡面
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    // TODO 會有 FILLER 隔開logic gate
    getIpBoudary(&cell_ip_map, &cell_boundary_vector, &cell_placed_map);

    
    




    return 0;
}

void getIpBoudary(unordered_map<string, CellInstancePowerInfo> *cell_ip_map, vector<CellBoundary> *cell_boundary_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{

    for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
    {
        string cell_id = iter->first;
        CellInstancePowerInfo cell_instance_power_info = iter->second;
        setCellBoundary(&(*cell_boundary_vector), &cell_instance_power_info, &(*cell_placed_map));
    }
}
void setCellBoundary(vector<CellBoundary> *cell_boundary_vector, CellInstancePowerInfo *cell_instance_power_info, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    string cell_id = cell_instance_power_info->cell_id;
    string cell_name = cell_instance_power_info->cell_name;
  
    CellPlacedInfo cell_placed_info;

    if((*cell_placed_map).count(cell_id)){
      cell_placed_info = (*cell_placed_map)[cell_id];
    }else{
       cout << "cell get error " << endl;
    }

    if ((*cell_boundary_vector).size() == 0)
    {

        CellBoundary cell_boundary;
        vector<string> cell_id_vector;
        
        cell_boundary.lef_x_location = cell_placed_info.left_x_location;
        cell_boundary.right_x_location = cell_placed_info.right_x_location;
        cell_id_vector.push_back(cell_id);
        cell_boundary.cell_id_vector = cell_id_vector;
        (*cell_boundary_vector).push_back(cell_boundary);
    }
    else
    {
        bool need_add = false;
        for (int i = 0; i < (*cell_boundary_vector).size(); i++)
        {
            CellBoundary cell_boundary = (*cell_boundary_vector)[i];
            if (adjustCellBoundary(&cell_boundary, &cell_placed_info))
            {
                need_add = true;
            }
        }
        if (need_add)
        {
            CellBoundary cell_boundary;
            cell_boundary.lef_x_location = cell_placed_info.left_x_location;
            cell_boundary.right_x_location = cell_placed_info.right_x_location;
            vector<string> cell_id_vector;
            cell_id_vector.push_back(cell_placed_info.cell_id);
            cell_boundary.cell_id_vector = cell_id_vector;
            (*cell_boundary_vector).push_back(cell_boundary);
        }
    }
}

bool adjustCellBoundary(CellBoundary *cell_boundary, CellPlacedInfo *cell_placed_info)
{
    float boundary_left_location = stof(cell_boundary->lef_x_location);
    float boundary_right_location = stof(cell_boundary->right_x_location);

    float placed_left_location = stof(cell_placed_info->left_x_location);
    float placed_right_location = stof(cell_placed_info->right_x_location);
    // case 1 left oversize right inside
    if (placed_right_location >= boundary_left_location && placed_right_location <= boundary_right_location && placed_left_location <= boundary_left_location)
    {
        (*cell_boundary).lef_x_location = to_string(placed_left_location);
        (*cell_boundary).cell_id_vector.push_back((*cell_placed_info).cell_id);
        return false;
    } // case 2 left inside right inside
    else if (placed_left_location >= boundary_left_location && placed_right_location <= boundary_right_location)
    {
        (*cell_boundary).cell_id_vector.push_back((*cell_placed_info).cell_id);
        return false;

    } // case 3 left inside right oversize
    else if (placed_left_location >= boundary_left_location && placed_left_location <= boundary_right_location && placed_right_location >= boundary_right_location)
    {
        (*cell_boundary).right_x_location = to_string(placed_right_location);
        (*cell_boundary).cell_id_vector.push_back((*cell_placed_info).cell_id);
        return false;

    } // case 4 left oversize right oversize
    else if (placed_left_location <= boundary_left_location && placed_right_location >= boundary_right_location)
    {
        (*cell_boundary).lef_x_location = to_string(placed_left_location);
        (*cell_boundary).right_x_location = to_string(placed_right_location);
        (*cell_boundary).cell_id_vector.push_back((*cell_placed_info).cell_id);
        return false;
    } // case 5 outside
    else
    {
        return true;
    }
}

void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    ifstream ip_file(ip_report);
    string ip_content;
    if (ip_file)
    {
        while (getline(ip_file, ip_content))
        {
            if (ip_content.find("Cell") != string::npos and ip_content.find("Instance") != string::npos)
            {
                continue;
            }
            else if (ip_content.find("Range") != string::npos and ip_content.find("Total") != string::npos)
            {
                break;
            }
            else
            {
                vector<string> ip_content_array = splitByPattern(ip_content, " ");
                CellInstancePowerInfo cell_instance_power_info;
                cell_instance_power_info.instance_power = ip_content_array[0];
                cell_instance_power_info.cell_name = ip_content_array[1];
                cell_instance_power_info.cell_id = ip_content_array[2];
                (*cell_ip_map).insert(pair<string, CellInstancePowerInfo>(cell_instance_power_info.cell_id, cell_instance_power_info));
            }
        }
    }
}

void getDefPlacedImformation(string DEF_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map)
{
    ifstream def_file(DEF_FILE);
    string def_content;

    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("COMPONENT") != string::npos and ((def_content.find("COMPONENTPIN") != string::npos) == false))
            {
                while (getline(def_file, def_content))
                {
                    if (def_content.find("PLACED") != string::npos)
                    {
                        //TODO 這裡可以下log
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        CellPlacedInfo cell_placed_info;
                        cell_placed_info.cell_id = def_content_array[1];
                        cell_placed_info.cell_name = def_content_array[2];
                        setPlacePosition( &def_content_array, &cell_placed_info);
                        getCellLocation(&cell_placed_info, &(*cell_info_map));

                        (*cell_placed_map).insert(pair<string, CellPlacedInfo>(cell_placed_info.cell_id, cell_placed_info));

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
void setPlacePosition(vector<string> *def_content_array,CellPlacedInfo *cell_placed_info){
    int index = 0;
    for (int i = 0; i < (*def_content_array).size(); i++)
    {
        string content = (*def_content_array)[i];
        if(content == "("){
            (*cell_placed_info).left_x_location = (*def_content_array)[i+1];
             (*cell_placed_info).down_y_location =  (*def_content_array)[i+2];
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

void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map)
{

    ifstream lef_file(LEF_FILE);
    string lef_content;

    if (lef_file)
    {
        while (getline(lef_file, lef_content))
        {
            if (lef_content.find("MACRO") != string::npos and lef_content.find("6t_fix") != string::npos)
            {
                vector<string> lef_content_array = splitByPattern(lef_content, " ");
                string cell_name = lef_content_array[1];
                while (getline(lef_file, lef_content))
                {
                    if (lef_content.find("SIZE") != string::npos and lef_content.find("BY") != string::npos)
                    {
                        vector<string> lef_content_array = splitByPattern(lef_content, " ");
                        string cell_width = lef_content_array[1];
                        string cell_height = lef_content_array[3];
                        CellInfo cell;
                        cell.cell_name = cell_name;
                        cell.cell_height = cell_height;
                        cell.cell_width = cell_width;
                        (*cell_info_map).insert(pair<string, CellInfo>(cell_name, cell));
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

void getStripeMovingRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
{

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            double distance = (((stof((*vdd_stripe_vector)[i + 1].start_x_location) - stof((*vdd_stripe_vector)[i].end_x_location)) / 2) + stof((*vdd_stripe_vector)[i].start_x_location));
            (*vdd_stripe_vector)[i].move_range_x_left = (*vdd_stripe_vector)[i].start_x_location;
            (*vdd_stripe_vector)[i].move_range_x_right = to_string(distance);
        }
        else if (i == ((*vdd_stripe_vector).size() - 1))
        {
            int front_index = i - 1;
            (*vdd_stripe_vector)[i].move_range_x_right = (*vdd_stripe_vector)[i].start_x_location;
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
}

//只讀到Stripe 就 break
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, vector<Range> *vdd_range_vector, vector<Range> *vss_range_vector)
{

    ifstream def_file(DEF_FILE);
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

                            (*vdd_stripe_vector).push_back(stripe);
                            int index = (*vdd_stripe_vector).size();
                            if ((*vdd_stripe_vector).size() >= 2)
                            {
                                Range stripe_range;
                                stripe_range.end_stripe = (*vdd_stripe_vector)[index - 1];
                                stripe_range.star_stripe = (*vdd_stripe_vector)[index - 2];
                                (*vdd_range_vector).push_back(stripe_range);
                            }
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
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location = def_content_array[12];
                            stripe.net_name = NET_NAME_VSS;
                            (*vss_stripe_vector).push_back(stripe);
                            int index = (*vss_stripe_vector).size();
                            if ((*vss_stripe_vector).size() >= 2)
                            {
                                Range stripe_range;
                                stripe_range.end_stripe = (*vss_stripe_vector)[index - 1];
                                stripe_range.star_stripe = (*vss_stripe_vector)[index - 2];
                                (*vss_range_vector).push_back(stripe_range);
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

    def_file.close();
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
bool AllisNum(string str)  
{  
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    } 
    return true;
}

    // for (auto item :cell_ip_map )
    // {
    //    cout << item.second.cell_id << " : " << cell_placed_map[item.second.cell_id].left_x_location << " " << cell_placed_map[item.second.cell_id].right_x_location << endl;
    // }
    

    // for (int i = 0; i < cell_boundary_vector.size(); i++)
    // {
    //     cout << cell_boundary_vector[i].lef_x_location << "   " << cell_boundary_vector[i].right_x_location << endl;
    // }
    