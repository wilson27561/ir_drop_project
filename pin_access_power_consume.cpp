using namespace std;
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>

const double esp = 1e-6;
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
struct PinAccess
{
    // def information
    string left_bottom_x_location_def;
    string left_bottom_y_location_def;
    string right_top_x_location_def;
    string right_top_y_location_def;
    string middle_x_location_def;
    string middle_y_location_def;
};
struct PinAccessShape
{
    // lef information
    string left_bottom_x_location;
    string left_bottom_y_location;
    string right_top_x_location;
    string right_top_y_location;
    string middle_x_location;
    string middle_y_location;
};

struct Rect
{
    string bottom_x_location;
    string bottom_y_location;
    string top_x_location;
    string top_y_location;
    vector<PinAccessShape> pin_access_shape_vector;
};
// Step1 : 把Pin 改成pin shape
// Step2 : 把Pin放到CellPlacedInfo -> PinAcess (pinname pinAcess_vector)
struct PinShape
{
    string pin_name;
    // layer    rect
    map<string, vector<Rect>> rect_map;
    // map<string, vector<PinShape>> pin_shape_map;
};
struct Pin
{
    string pin_name;
    map<string, vector<PinAccess>> layer_pin_access_map;
};

struct CellInfo
{
    string cell_name;
    string height;
    string width;
    // key:PinName value:PinShape
    map<string, PinShape> pin_shape_map;
};
struct CellPlacedInfo
{
    string cell_id;
    string cell_name;
    string left_x_location;
    string down_y_location;
    string right_x_location;
    string up_y_location;
    string direction;
    map<string, Pin> pin_map;
};
struct CellInstancePowerInfo
{
    string cell_id;
    string cell_name;
    float instance_total_power;
    float internal_power;
    float leakage_power;
    float switching_power;
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
    string width;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};

struct TrackPoint
{
    float x_point;
    float y_point;
};

// main function
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string DEF_TRANSFER_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);

// sub function
void getSizeOfCell(string lef_content, CellInfo *cell_info);
void getPinShapeOfCell(ifstream *lef_file, string left_content, PinShape *pin_shape);
void getPortOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape);
void getLayerOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getPinAccessFromRect(Rect *rect);
void getPinAccessLocationFromDef(PinAccessShape *pin_access_shape, CellPlacedInfo *cell_placed_info, string cell_width, string cell_height, PinAccess *pin_access);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site);
//判斷
bool isVertical(Rect *rect);
bool isPinAccessNameAndLayer(string layer_name, string pin_name);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
// Util
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
bool floatIsEqual(float a, float b);
string floatToString(const float value);
int stringToInt(string num);
int main()
{
    string LEF_FILE = "tech_lef_file/test.lef";
    string DEF_TRANSFER_FILE = "def_file/gpu_nerualNetwork_ricsv_aes_afterRouting/test.def";
    string IP_REPORT_FILE = "power_report/activity_0.2/print_ip_aes.report";
    string v1_width = "0.072";
    string m2_y_pitch = "0.072";
    string m2_y_start = "1.44";
    string m3_x_pitch = "0.072";
    string m3_x_start = "0.18";
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, TrackPoint> m3_track_point_map;
    unordered_map<string, TrackPoint> m2_track_point_map;
    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    setRoutingTrackPoint(&m2_track_point_map, &m3_track_point_map, m3_x_start, m3_x_pitch, m2_y_start, m2_y_pitch, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);

    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    setStripeRange(&vdd_stripe_vector, &vss_stripe_vector, &core_site);

    // step 4 : 取得每一條routing track 蓋到多少cell 的 power 及 蓋到多少 cell的pin access point

    // a. power stripe 在每條power stripe 上 各經過多少 power consuming 跟多少cell 需要牽扯到 power stripe 的 width 還有track 的部分
    // b. 經過每條後 壓到多少pin access point
    // c. 確認pin access 是不是在track 上面



    // for (auto cell_place : cell_placed_map)
    // {
    //     cout << "pin id :" << cell_place.first << endl;
    //     map<string, Pin> pin_map = cell_place.second.pin_map;
    //     for (auto pin : pin_map)
    //     {
    //         cout << "pin name : " << pin.first << endl;
    //         for (auto pin_layer : pin.second.layer_pin_access_map)
    //         {
    //             cout << "layer : " << pin_layer.first << " ";
    //             for (int i = 0; i < pin_layer.second.size(); i++)
    //             {
    //                 cout << "(" << pin_layer.second[i].middle_x_location_def << " " << pin_layer.second[i].middle_y_location_def << ")";
    //             }
    //             std::cout << "" << std::endl;
    //         };
    //     }
    // }

    return 0;
}





void setPinAccessPointFromTrack(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map) {



}

void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site)
{

    int log = 0;
    int start_core_site_y_location_int = (stof((*core_site).down_y_location) * 1000) / 4;
    int end_core_site_y_location_int = (stof((*core_site).up_y_location) * 1000) / 4;
    vector<TrackPoint> m2_track_point_vector;
    float m2_y_start_float = (stof(m2_y_start) * 1000) / 4;
    float m2_y_pitch_float = (stof(m2_y_pitch) * 1000) / 4;
    m2_y_start = floatToString(m2_y_start_float);
    m2_y_pitch = floatToString(m2_y_pitch_float);

    int m2_y_start_int = stringToInt(m2_y_start);
    int m2_y_pitch_int = stringToInt(m2_y_pitch);

    cout << m2_y_start_int << endl;
    cout << m2_y_pitch_int << endl;

    for (int i = m2_y_start_int; i < end_core_site_y_location_int; i += m2_y_pitch_int)
    {
        if (i > start_core_site_y_location_int && i <= end_core_site_y_location_int)
        {
            TrackPoint m2_track_point;
            double i_double = (double)i;
            m2_track_point.y_point = (i_double * 4) / 1000;
            string key_m2_track_point = floatToString(m2_track_point.y_point);
            (*m2_track_point_map).insert(pair<string, TrackPoint>(key_m2_track_point, m2_track_point));

            m2_track_point_vector.push_back(m2_track_point);
        }
        if (i > end_core_site_y_location_int)
        {
            cout << "check break" << endl;
            break;
        }
    };

    // cout << "m2 track size : " << (*m2_track_point_map).size() << endl;
    // cout << "m2 track size : " << m2_track_point_vector.size() << endl;
    // cout << "m2 track point : " << m2_track_point_vector[507].y_point << endl;

    int start_core_site_x_location_int = (stof((*core_site).left_x_location) * 1000) / 4;
    int end_core_site_x_location_int = (stof((*core_site).right_x_location) * 1000) / 4;

    vector<TrackPoint> m3_track_point_vector;

    float m3_x_start_float = (stof(m3_x_start) * 1000) / 4;
    float m3_x_pitch_float = (stof(m3_x_pitch) * 1000) / 4;

    m3_x_start = floatToString(m3_x_start_float);
    m3_x_pitch = floatToString(m3_x_pitch_float);

    int m3_x_start_int = stringToInt(m3_x_start);
    int m3_x_pitch_int = stringToInt(m3_x_pitch);

    for (int i = m3_x_start_int; i < end_core_site_x_location_int; i += m3_x_pitch_int)
    {
        if (i > start_core_site_x_location_int && i <= end_core_site_x_location_int)
        {
            TrackPoint m3_track_point;
            double i_double = (double)i;
            m3_track_point.x_point = (i_double * 4) / 1000;
            string key_m3_track_point = floatToString(m3_track_point.x_point);
            (*m3_track_point_map).insert(pair<string, TrackPoint>(key_m3_track_point, m3_track_point));
            m3_track_point_vector.push_back(m3_track_point);
        }
        if (i > end_core_site_x_location_int)
        {
            cout << "check break" << endl;
            break;
        }
    };
    // cout << "m2 track size : " << (*m3_track_point_map).size() << endl;
    // cout << "m2 track size : " << m3_track_point_vector.size() << endl;
    // cout << "m3 track point : " << m3_track_point_vector[509].x_point << endl;
};

int stringToInt(string num)
{
    stringstream ss;
    int num_int;
    ss << num;
    ss >> num_int;

    ss.str("");
    ss.clear();
    return num_int;
}

void setStripeRange(vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
{
    setRange(&(*vdd_stripe_vector), &(*core_site));
    setRange(&(*vss_stripe_vector), &(*core_site));
}

void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
{
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            (*stripe_vector)[i].move_range_x_left = (*core_site).left_x_location;
            float middle_x_power_stripe = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_right = floatToString(middle_x_power_stripe);
        }
        else if (i == ((*stripe_vector).size() - 1))
        {
            (*stripe_vector)[i].move_range_x_right = (*core_site).right_x_location;
            float middle_x_power_stripe = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_left = middle_x_power_stripe;
        }
        else
        {
            float middle_x_power_stripe_right = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
            float middle_x_power_stripe_left = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_right = middle_x_power_stripe_right;
            (*stripe_vector)[i].move_range_x_left = middle_x_power_stripe_left;
        }
    }
}

//只讀到Stripe 就 break
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site)
{

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
                cell_instance_power_info.instance_total_power = stof(ip_content_array[0]);
                power += cell_instance_power_info.instance_total_power;
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
    // cout << "total : " << power << endl;
    cout << " count power consuming of total ip report : " << power << endl;
}

void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map)
{
    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    int log = 0;
    if (def_transfer_file)
    {
        while (getline(def_transfer_file, def_content))
        {
            if (def_content.find("COMPONENT") != string::npos && ((def_content.find("COMPONENTPIN") != string::npos) == false))
            {
                while (getline(def_transfer_file, def_content))
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
                        if (log % 10000 == 0)
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
        // cell 的 右上資訊
        string left_x_location = (*cell_placed_info).left_x_location;
        string down_y_location = (*cell_placed_info).down_y_location;
        float left_x_float = stof(left_x_location);
        float down_y_float = stof(down_y_location);
        string cell_width = (*cell_info_map)[cell_name].width;
        string cell_height = (*cell_info_map)[cell_name].height;
        float right_x_float = left_x_float + stof(cell_width);
        float up_y_float = down_y_float + stof(cell_height);
        (*cell_placed_info).right_x_location = to_string(right_x_float);
        (*cell_placed_info).up_y_location = to_string(up_y_float);
        cout << cell_name << " " << left_x_float << " " << down_y_float << " " << right_x_float << " " << up_y_float << " " << (*cell_placed_info).direction << endl;

        if ((*cell_placed_info).cell_id == "U145431")
        {

            map<string, PinShape> pin_shape_map = (*cell_info_map)[cell_name].pin_shape_map;
            map<string, Pin> pin_map;
            // 將 pin access 放入 cell placed imformation
            for (auto pin_shape : pin_shape_map)
            {
                Pin pin;
                map<string, vector<Rect>> rect_map = pin_shape.second.rect_map;
                cout << pin_shape.first << endl;
                string pin_name = pin_shape.first;
                map<string, vector<PinAccess>> pin_access_layer_map;
                for (auto rect : rect_map)
                {
                    string layer_name = rect.first;
                    vector<Rect> rect_vector = rect.second;
                    vector<PinAccess> pin_access_vector;
                    for (int i = 0; i < rect_vector.size(); i++)
                    {
                        vector<PinAccessShape> pin_access_shape_vector = rect_vector[i].pin_access_shape_vector;
                        if (pin_access_shape_vector.size() > 0)
                        {
                            for (int i = 0; i < pin_access_shape_vector.size(); i++)
                            {
                                PinAccess pin_access;
                                // cout << cell_name << " " << left_x_float << " " << down_y_float << " " << right_x_float << " " << up_y_float << " " << (*cell_placed_info).direction << endl;
                                cout << pin_access_shape_vector[i].left_bottom_x_location << " " << pin_access_shape_vector[i].left_bottom_y_location << " " << pin_access_shape_vector[i].right_top_x_location << " " << pin_access_shape_vector[i].right_top_y_location << endl;
                                getPinAccessLocationFromDef(&(pin_access_shape_vector[i]), &(*cell_placed_info), cell_width, cell_height, &pin_access);
                                pin_access_vector.push_back(pin_access);
                            }
                            cout << "" << endl;
                        }
                    }
                    pin_access_layer_map.insert(pair<string, vector<PinAccess>>(layer_name, pin_access_vector));
                }
                pin.layer_pin_access_map = pin_access_layer_map;
                pin_map.insert(pair<string, Pin>(pin_name, pin));
            }
            (*cell_placed_info).pin_map = pin_map;
        }
    }
    else
    {
        cout << "there some error in cell_info" << endl;
    }
}

void getPinAccessLocationFromDef(PinAccessShape *pin_access_shape, CellPlacedInfo *cell_placed_info, string cell_width, string cell_height, PinAccess *pin_access)
{
    float cell_place_left_x = stof((*cell_placed_info).left_x_location);
    float cell_place_down_y = stof((*cell_placed_info).down_y_location);
    float cell_width_float = stof(cell_width);
    float cell_height_float = stof(cell_height);
    if ((*cell_placed_info).direction == "N")
    {
        // float left_bottom_x_location_float = stof((*pin_access).left_bottom_x_location);
        // float left_bottom_y_location_float = stof((*pin_access).left_bottom_y_location);
        // float right_top_x_location_float = stof((*pin_access).right_top_x_location);
        // float right_top_y_location_float = stof((*pin_access).right_top_y_location);

        // (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + left_bottom_x_location_float);
        // (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + left_bottom_y_location_float);
        // (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + right_top_x_location_float);
        // (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + right_top_y_location_float);

        float middle_x_location_float = stof((*pin_access_shape).middle_x_location);
        float middle_y_location_float = stof((*pin_access_shape).middle_y_location);

        (*pin_access).middle_x_location_def = floatToString(cell_place_left_x + middle_x_location_float);
        (*pin_access).middle_y_location_def = floatToString(cell_place_down_y + middle_y_location_float);

        // cout << "pin : " << (*pin_access).left_bottom_x_location_def << " " << (*pin_access).left_bottom_y_location_def << " " << (*pin_access).right_top_x_location_def << " " << (*pin_access).right_top_y_location_def << endl;
        cout << "pin : "
             << "(" << (*pin_access).middle_x_location_def << " " << (*pin_access).middle_y_location_def << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "S")
    {
        float middle_x_location_float = stof((*pin_access_shape).middle_x_location);
        float middle_y_location_float = stof((*pin_access_shape).middle_y_location);

        float left_bottom_x_distance = abs(cell_width_float - middle_x_location_float);
        float right_top_y_distance = abs(cell_height_float - middle_y_location_float);

        (*pin_access).middle_x_location_def = floatToString(cell_place_left_x + left_bottom_x_distance);
        (*pin_access).middle_y_location_def = floatToString(cell_place_down_y + right_top_y_distance);

        cout << "pin : "
             << "(" << (*pin_access).middle_x_location_def << " " << (*pin_access).middle_y_location_def << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FN")
    {
        // float left_bottom_x_location_float = stof((*pin_access).left_bottom_x_location);
        // float left_bottom_y_location_float = stof((*pin_access).left_bottom_y_location);
        // float right_top_x_location_float = stof((*pin_access).right_top_x_location);
        // float right_top_y_location_float = stof((*pin_access).right_top_y_location);
        // float left_bottom_x_distance = abs(cell_width_float - left_bottom_x_location_float);
        // (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + left_bottom_x_distance);
        // (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + left_bottom_y_location_float);
        // float right_top_x_distance = abs(cell_width_float - right_top_x_location_float);
        // (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + right_top_x_distance);
        // (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + right_top_y_location_float);

        float middle_x_location_float = stof((*pin_access_shape).middle_x_location);
        float middle_y_location_float = stof((*pin_access_shape).middle_y_location);

        float middle_x_distance_float = abs(cell_width_float - middle_x_location_float);
        (*pin_access).middle_x_location_def = floatToString(cell_place_left_x + middle_x_distance_float);
        (*pin_access).middle_y_location_def = floatToString(cell_place_down_y + middle_y_location_float);

        cout << "pin : "
             << "(" << (*pin_access).middle_x_location_def << " " << (*pin_access).middle_y_location_def << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FS")
    {
        // float left_bottom_x_location_float = stof((*pin_access).left_bottom_x_location);
        // float left_bottom_y_location_float = stof((*pin_access).left_bottom_y_location);
        // float right_top_x_location_float = stof((*pin_access).right_top_x_location);
        // float right_top_y_location_float = stof((*pin_access).right_top_y_location);
        // float left_bottom_y_distance = abs(cell_height_float - left_bottom_y_location_float);
        // (*pin_access).left_bottom_x_location_def = floatToString(cell_place_left_x + left_bottom_x_location_float);
        // (*pin_access).left_bottom_y_location_def = floatToString(cell_place_down_y + left_bottom_y_distance);
        // float right_top_y_distance = abs(cell_height_float - right_top_y_location_float);
        // (*pin_access).right_top_x_location_def = floatToString(cell_place_left_x + right_top_x_location_float);
        // (*pin_access).right_top_y_location_def = floatToString(cell_place_down_y + right_top_y_distance);

        float middle_x_location_float = stof((*pin_access_shape).middle_x_location);
        float middle_y_location_float = stof((*pin_access_shape).middle_y_location);

        (*pin_access).middle_x_location_def = floatToString(cell_place_left_x + middle_x_location_float);
        float top_y_distance_float = abs(cell_height_float - middle_y_location_float);
        (*pin_access).middle_y_location_def = floatToString(cell_place_down_y + top_y_distance_float);

        // cout << "pin : "
        //      << "(" << (*pin_access).left_bottom_x_location_def << " " << (*pin_access).left_bottom_y_location_def << ")"
        //      << " "
        //      << "(" << (*pin_access).right_top_x_location_def << " " << (*pin_access).right_top_y_location_def << ")" << endl;

        cout << "pin : "
             << "(" << (*pin_access).middle_x_location_def << " " << (*pin_access).middle_y_location_def << ")" << endl;
    }
    else
    {
        cout << " getPinAccessLocationFromDef error " << endl;
    }
};

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
            (*cell_placed_info).direction = (*def_content_array)[i + 4];
            break;
        }
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
                CellInfo cell_info;
                vector<string> lef_content_array = splitByPattern(lef_content, " ");
                string cell_name = lef_content_array[1];
                cell_info.cell_name = cell_name;
                map<string, PinShape> pin_shape_map;
                while (getline(lef_file, lef_content))
                {
                    vector<string> lef_content_array = splitByPattern(lef_content, " ");
                    if (lef_content.find("SIZE") != string::npos && lef_content.find("BY") != string::npos)
                    {
                        getSizeOfCell(lef_content, &cell_info);
                    }
                    if (lef_content.find("PIN") != string::npos)
                    {
                        PinShape pin_shape;
                        map<string, vector<Rect>> rect_map;
                        pin_shape.rect_map = rect_map;
                        getPinShapeOfCell(&lef_file, lef_content, &pin_shape);
                        pin_shape_map.insert(pair<string, PinShape>(pin_shape.pin_name, pin_shape));
                        cell_info.pin_shape_map = pin_shape_map;
                    }
                    if ((lef_content_array[0] == "END") && (lef_content_array[1] == cell_info.cell_name))
                    {
                        (*cell_info_map).insert(pair<string, CellInfo>(cell_info.cell_name, cell_info));
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

void getSizeOfCell(string lef_content, CellInfo *cell_info)
{
    vector<string> lef_content_array = splitByPattern(lef_content, " ");
    (*cell_info).height = lef_content_array[3];
    (*cell_info).width = lef_content_array[1];
}

void getPinShapeOfCell(ifstream *lef_file, string lef_content, PinShape *pin_shape)
{
    vector<string> lef_content_array = splitByPattern(lef_content, " ");
    (*pin_shape).pin_name = lef_content_array[1];
    // cout << (*pin).pin_name << endl;
    while (getline(*lef_file, lef_content))
    {
        vector<string> lef_content_array = splitByPattern(lef_content, " ");
        if ((lef_content_array[0] == "PORT"))
        {
            getPortOfPinShape(&(*lef_file), lef_content, &(*pin_shape));
        }
        if ((lef_content_array[0] == "END") && (lef_content_array[1] == (*pin_shape).pin_name))
        {
            // cout << lef_content << endl;
            break;
        }
    }
}

void getPortOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape)
{
    // cout << lef_content << endl;
    vector<Rect> rect_vector;
    string layer_name;
    while (getline(*lef_file, lef_content))
    {
        vector<string> lef_content_array = splitByPattern(lef_content, " ");
        if ((lef_content_array[0] == "LAYER"))
        {
            if (rect_vector.size() > 0)
            {
                (*pin_shape).rect_map.insert(pair<string, vector<Rect>>(layer_name, rect_vector));
                rect_vector.clear();
            }
            layer_name = lef_content_array[1];
        }
        if ((lef_content_array[0] == "RECT"))
        {
            Rect rect;
            vector<string> lef_content_array = splitByPattern(lef_content, " ");
            rect.bottom_x_location = lef_content_array[1];
            rect.bottom_y_location = lef_content_array[2];
            rect.top_x_location = lef_content_array[3];
            rect.top_y_location = lef_content_array[4];
            // cout << "====================" << endl;
            if (isPinAccessNameAndLayer(layer_name, pin_shape->pin_name))
            {
                // cout << lef_content << endl;
                getPinAccessFromRect(&rect);
            }
            // cout << "=====================" << endl;
            rect_vector.push_back(rect);
        }
        if ((lef_content_array[0] == "END"))
        {
            if (rect_vector.size() > 0)
            {
                (*pin_shape).rect_map.insert(pair<string, vector<Rect>>(layer_name, rect_vector));
                rect_vector.clear();
            }
            // cout << lef_content << endl;
            break;
        }
    }
}
bool isPinAccessNameAndLayer(string layer_name, string pin_name)
{
    if ((layer_name == "M1" || layer_name == "M2") && pin_name != "VDD" && pin_name != "VSS")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool lessThanOrEqualTo(float a, float b)
{
    if (a < b)
    {
        return true;
    }
    else if (floatIsEqual(a, b))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void getPinAccessFromRect(Rect *rect)
{
    float bottom_x_location_float = stof((*rect).bottom_x_location);
    float bottom_y_location_float = stof((*rect).bottom_y_location);
    float top_x_location_float = stof((*rect).top_x_location);
    float top_y_location_float = stof((*rect).top_y_location);
    vector<PinAccessShape> pin_access_shape_vector;
    // step 1: 判斷是 垂直還是水平
    if (isVertical(&(*rect)))
    {
        for (float i = bottom_y_location_float; i < top_y_location_float; i += 0.072)
        {
            // cout << " i " << i;

            float middle_x_location = (abs(bottom_x_location_float - top_x_location_float) / 2) + bottom_x_location_float;
            float middle_y_location = i + ((0.072) / 2);

            float left_bottom_x_location = bottom_x_location_float;
            float left_bottom_y_location = i;
            float right_top_x_location = top_x_location_float;
            float right_top_y_location = i + 0.072;
            PinAccessShape pin_access_shape;

            pin_access_shape.middle_x_location = floatToString(middle_x_location);
            pin_access_shape.middle_y_location = floatToString(middle_y_location);
            pin_access_shape.left_bottom_x_location = floatToString(left_bottom_x_location);
            pin_access_shape.left_bottom_y_location = floatToString(left_bottom_y_location);
            pin_access_shape.right_top_x_location = floatToString(right_top_x_location);
            pin_access_shape.right_top_y_location = floatToString(right_top_y_location);
            if (right_top_y_location < top_y_location_float || floatIsEqual(right_top_y_location, top_y_location_float))
            {
                pin_access_shape_vector.push_back(pin_access_shape);
            }

            // cout << " middle " << middle_x_location << " " << middle_y_location;
            // cout <<" vertical " << " left_bottom_x_location : " << pin_access.left_bottom_x_location << " left_bottom_y_location : " << pin_access.left_bottom_y_location << " right_top_x_location : " << pin_access.right_top_x_location << " right_top_y_location : " << pin_access.right_top_y_location << endl;
        }
        (*rect).pin_access_shape_vector = pin_access_shape_vector;
        // cout << "" << endl;
    }
    else
    {
        for (float i = bottom_x_location_float; i < top_x_location_float; i += 0.072)
        {
            // cout << " i " << i;

            float middle_x_locaiton = i + ((0.072) / 2);
            float middle_y_location = (abs(bottom_y_location_float - top_y_location_float) / 2) + bottom_y_location_float;
            // cout << " middle " << middle_x_locaiton << " " << middle_y_location;

            float left_bottom_x_location = i;
            float left_bottom_y_location = bottom_y_location_float;
            float right_top_x_location = i + 0.072;
            float right_top_y_location = top_y_location_float;

            PinAccessShape pin_access_shape;
            pin_access_shape.middle_x_location = floatToString(middle_x_locaiton);
            pin_access_shape.middle_y_location = floatToString(middle_y_location);
            pin_access_shape.left_bottom_x_location = floatToString(left_bottom_x_location);
            pin_access_shape.left_bottom_y_location = floatToString(left_bottom_y_location);
            pin_access_shape.right_top_x_location = floatToString(right_top_x_location);
            pin_access_shape.right_top_y_location = floatToString(right_top_y_location);
            if (right_top_x_location < top_x_location_float || floatIsEqual(right_top_x_location, top_x_location_float))
            {
                pin_access_shape_vector.push_back(pin_access_shape);
            }
            // cout << " horizontal " << " left_bottom_x_location : " << pin_access.left_bottom_x_location << " left_bottom_y_location : " << pin_access.left_bottom_y_location << " right_top_x_location : " << pin_access.right_top_x_location << " right_top_y_location : " << pin_access.right_top_y_location << endl;
        }
        (*rect).pin_access_shape_vector = pin_access_shape_vector;
        // cout << "" << endl;
    }
}

bool isVertical(Rect *rect)
{
    float bottom_x_location_float = stof((*rect).bottom_x_location);
    float bottom_y_location_float = stof((*rect).bottom_y_location);
    float top_x_location_float = stof((*rect).top_x_location);
    float top_y_location_float = stof((*rect).top_y_location);

    float width = abs(bottom_x_location_float - top_x_location_float);
    float height = abs(bottom_y_location_float - top_y_location_float);

    if (width > height)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site)
{
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

bool floatIsEqual(float a, float b)
{
    if (fabs(a - b) <= esp)
    {
        return true;
    }
    else
    {
        return false;
    }
}
string floatToString(const float value)
{
    std::ostringstream out;
    out.precision(6);

    out << value;
    return out.str();
}

// for (auto cellInfo : cell_info_map)
// {
//     cout << "cell name : " << cellInfo.first << endl;
//     map<string, PinShape> pin_shape_map = cellInfo.second.pin_shape_map;
//     for (auto pin_shape : pin_shape_map)
//     {
//         cout << pin_shape.second.pin_name << " " << pin_shape.second.rect_map.size() << endl;
//         for (auto rect : pin_shape.second.rect_map)
//         {
//             cout << rect.first << endl;
//             for (int i = 0; i < rect.second.size(); i++)
//             {
//                 cout << rect.second[i].bottom_x_location << " " << rect.second[i].bottom_y_location << " " << rect.second[i].top_x_location << " " << rect.second[i].top_y_location << " "  << rect.second[i].pin_access_vector.size() << endl;
//             }
//         }
//     }
// }