
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
const string LEFT = "left";
const string RIGHT = "right";
const string MIDDLE = "middle";
const float BOUNDARY = 10;
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct CurrentRange
{
    float left_x_location;
    float right_x_location;
    float range_total_power;
    vector<string> ip_power_vector;
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
int main()
{
    CoreSite core_site;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    vector<CurrentRange> current_range_vector;
    unordered_map<string, unordered_map<string, Track>> layer_track_map;

    getCoreSite(DEF_FILE, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_FILE, &cell_placed_map, &cell_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    setCurrentRange(&core_site, &current_range_vector);
    setIpPowerInStripe(&current_range_vector, &cell_ip_map, &cell_placed_map);
    getTrack(DEF_FILE, &layer_track_map, &core_site);



    float total = 0;
    for (int i = 0; i < current_range_vector.size(); i++)
    {
        // cout << "range total power : " << current_range_vector[i].range_total_power << endl;
        total += current_range_vector[i].range_total_power;
    }
    cout << "total : " << total << endl;
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