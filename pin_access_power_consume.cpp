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
#include <time.h>
const double esp = 1e-6;
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";

struct Rect
{
    string bottom_x_location;
    string bottom_y_location;
    string top_x_location;
    string top_y_location;
};

struct PinShape
{
    string pin_name;
    map<string, vector<Rect>> rect_map;
};
struct PinAccessPoint
{
    string middle_x_location;
    string middle_y_location;
};
struct PinAccessCost
{
    // key : FN N FS  // value access cost
    unordered_map<string, string> vertical_direction_cost_map;
};
struct CellInfo
{
    string cell_name;
    string height;
    string width;
    // key:PinName value:PinShape
    map<string, PinShape> pin_shape_map;
    // key:PinName value:Pin Access Point
    map<string, vector<PinAccessPoint>> odd_pin_access_point_map;
    // key:PinName value:Pin Access Point
    map<string, vector<PinAccessPoint>> even_pin_access_point_map;

    // key : stripe_width key2 : location , value : cost
    unordered_map<string, unordered_map<string, PinAccessCost>> odd_pin_access_cost_map;
    unordered_map<string, unordered_map<string, PinAccessCost>> even_pin_access_cost_map;
};
struct PinAccess
{
    // def information
    string left_bottom_x_location_def;
    string left_bottom_y_location_def;
    string right_top_x_location_def;
    string right_top_y_location_def;
    // string middle_x_location_def;
    // string middle_y_location_def;
    // bool isPinAccessPoint;
};
struct TrackPoint
{
    string x_point;
    string y_point;
    vector<string> power_cell_id_vector;
    bool isRoutingTrack;
    float total_pin_access_power_consum_cost;
    float pin_access_cost;
    float power_consuming_cost;
};
struct Pin
{
    string pin_name;
    map<string, vector<PinAccess>> layer_pin_access_map;
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
    // key pin name value pin_access
    map<string, vector<PinAccessPoint>> pin_access_point_map;
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
    string move_range_y_down;
    string move_range_y_up;
    string width;
    string length;
    string vdd_track_x_location;
    string vss_track_x_location;
    string vdd_track_y_location;
    string vss_track_y_location;
    string layer;
    vector<string> ip_power_vector;
    vector<TrackPoint> track_point_vector;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct TrackInfo
{
    string layer;
    string start;
    string pitch;
};

struct TrackOfCell
{
    string start_x_location;
    string end_x_location;
    string start_y_location;
    string end_y_location;
    string layer;
    string pitch;
};

struct CellRange
{
    vector<string> cell_id_vector;
    string start_x_location;
    string end_x_location;

    /* data */
};

struct CompareTrack
{
    string x_point;
    string y_point;
    float pin_access_cost;
    float power_cost;
    float pin_access_cost_to_average;
    float power_consum_cost_to_average;
    float power_consum_and_pin_access_to_average;
    // pinaccess_power
    string type;
    /* data */
};
struct StripeInfo
{
    string layer;
    string original_vss_stripe_width;
    string minmum_spacing;
    string maximum_spacing;
};

const string positive_positive = "positive_positive";
const string positive_negative = "positive_negative";
const string negative_positive = "negative_positive";
const string negative_negative = "negative_negative";
const int PIN_ACCESS_LESS_WEIGHT = 10;
const string ODD = "odd";
const string EVEN = "even";

// main function
void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string DEF_TRANSFER_FILE, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);

// sub function
int getCoverTrack(float power_stripe_width);
void getSizeOfCell(string lef_content, CellInfo *cell_info);
void getPinShapeOfCell(ifstream *lef_file, string left_content, PinShape *pin_shape);
void getPortOfPinShape(ifstream *lef_file, string lef_content, PinShape *pin_shape);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
void setStripeRange(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void setRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setRoutingTrackPoint(unordered_map<string, TrackPoint> *m2_track_point_map, unordered_map<string, TrackPoint> *m3_track_point_map, string m3_x_start, string m3_x_pitch, string m2_y_start, string m2_y_pitch, CoreSite *core_site);
void setRoutingTrackPowerConsuming(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, TrackInfo> *track_info_map);
void setRoutingTrackNumberOfPinAccess(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info);
void isOnTrackPin(map<string, PinShape> *pin_shape_map, map<string, vector<PinAccessPoint>> *pin_access_point_map, string track_point_x, string track_point_y);
void getAddStripeCost(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, TrackInfo> *track_info_map);
void getLeftCellPinAccessPoint(unordered_map<string, CellInfo> *cell_info_map);
void setCellPinAccessPoint(CellInfo *cell_info);
void getStripeRangeCost(Stripe *stripe, string layer, float track_pitch, Stripe *odd_vss_stripe, Stripe *even_vss_stripe);
void generateAddStripeTcl(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, string move_stripe_name);
void transferPinAccessLocationFromDef(CellPlacedInfo *cell_placed_info, PinAccessPoint *pin_access_point, PinAccessPoint *pin_access_point_def, string cell_width, string cell_height);
float getPinAccessPointOfPlaced(float power_stripe_width_float, TrackPoint *m3_track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *m3_track_info);
void generateLogFile(vector<Stripe> *stripe_vector, string log_file_name);
void setCellRange(string power_stripe_width, CoreSite *core_site, vector<CellRange> *cell_range_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void getRoutingTrackPowerConsuming(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void setStripeLength(Stripe *stripe);
void transferStripeToMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector);
void generateTrackInfoMap(unordered_map<string, TrackInfo> *track_info_map);
void setOddLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void setEvenLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site);
void transferMovingRangeToTrack(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, TrackInfo> *track_info_map);
void setOddTrackRange(vector<Stripe> *stripe_vector, TrackInfo *track_info);
void setEvenTrackRange(vector<Stripe> *stripe_vector, TrackInfo *track_info);
// float getTrackPinAccessPointCost(string layer, float power_stripe_width_float, TrackPoint *track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info);
float getTrackPinAccessPointCost(string layer, string power_stripe_width, TrackPoint *track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info);
bool isInStripeRange(Stripe *vdd_stripe, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map);
string transferTrackLocationType(string layer, TrackPoint *track_point, CellPlacedInfo *cell_placed_info);
bool isOddLayer(string layer);
bool isVertical(Rect *rect);
bool isOddTrackCell(string x_location, TrackInfo *m3_track_info);
bool isPinAccessNameAndLayer(string layer_name, string pin_name);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
bool isInOddTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width);
bool isCoverPinAccess(string layer, PinAccessPoint *pin_access_point_def, TrackPoint *track_point, float power_stripe_width);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool floatIsEqual(float a, float b);
bool isInCellRange(CellRange *cell_range, CellPlacedInfo *cell_place_info, string power_stripe_width);
bool isInEvenStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
bool isInOddStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map);
void setEvenTrackPointInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *track_info);
void setCellStripeRange(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
bool isInEvenTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width);
void setOddTrackPointInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *track_info);
void setRoutingTrackNumberOfPinAccess(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, unordered_map<string, TrackInfo> *track_info_map);
void setOddTrackPinAccessCost(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info);
void setTrackPinAccessCost(ofstream *myfile, string layer, vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info);
float z_score_normalization(float original, float arithmetic_mean, float standard_deviation);
bool sortTrackPointVector(TrackPoint track_point_a, TrackPoint track_point_b);
void sortStripeMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map);
void getStripeLocationFromStripeTcl(string stripe_tcl_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, set<string> *stripe_width_set);
void transferStripePositionFromTcl(string layer, Stripe *stripe, string start_x_location, string start_y_location, string end_x_location, string end_y_location);
void getCompareTrackType(CompareTrack *compare_track, float average_z_score_pin_access_cost, float average_z_score_power_cost);
void getLefCellPinAccessPointCost(unordered_map<string, CellInfo> *cell_info_map, set<string> *stripe_width_set);
void setTrackPointCost(string type, CellInfo *cell_info, string stripe_width);
void transferPinAccessLocationFromRect(string direction, PinAccessPoint *pin_access_point, string cell_width, string cell_height, PinAccessPoint *transfer_pin_access_point);
double caculatePinAccessCost(double total_pin_access_size, double cover_pin_access_size);
void setStripeInfo(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, StripeInfo> *stripe_info_map);
void setStripeRangeCost(string layer, Stripe *stripe, unordered_map<string, StripeInfo> *stripe_info_map, unordered_map<string, TrackInfo> *track_info_map);
void setAddStripePosition(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, StripeInfo> *stripe_info_map, unordered_map<string, TrackInfo> *track_info_map);
// Util
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
string floatToString(const float value);
int stringToInt(string num);

bool sortOddStripeLocationVector(Stripe stripe_a, Stripe stripe_b);
bool sortEvenStripeLocationVector(Stripe stripe_a, Stripe stripe_b);
bool sortCompareTrack(CompareTrack compare_track_a, CompareTrack compare_track_b);
float standardDeviation(vector<float> *num_vec);
float arithmeticMean(vector<float> *num_vec);
float z_score_normalization(float original, float arithmetic_mean, float standard_deviation);
int tranferLocationFloatToInt(float location);
float convertInnovusPoint(int number);
bool isCoverPinAccessCost(string layer, PinAccessPoint *transfer_pin_access_point, float track_location, float power_stripe_width);

int main(int argc, char *argv[])
{
    cout << "-------------------------- pin_access_power_consume.cpp start --------------------------" << endl;
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    vector<CellRange> cell_range_vector;
    unordered_map<string, CellInstancePowerInfo> cell_ip_map;
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, TrackPoint> m3_track_point_map;
    unordered_map<string, TrackPoint> m2_track_point_map;
    unordered_map<string, vector<Stripe>> vdd_stripe_map;
    unordered_map<string, vector<Stripe>> vss_stripe_map;
    unordered_map<string, TrackInfo> track_info_map;
    set<string> stripe_width_set;
    unordered_map<string, StripeInfo> stripe_info_map;

    string config_file = argv[1];
    // string config_file = "config/config_gpu.txt";
    string excute_time = "log_file/excute_time" + config_file;
    ofstream myfile;
    myfile.open(excute_time);
    string LEF_FILE = "";
    string IP_REPORT_FILE = "";
    string ADD_STRIPE_TCL = "";
    string DEF_TRANSFER_FILE = "";
    string DECREASE_WIRE_STRIPE_TCL = "";
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
            if (config_content_array[0] == "LEF_FILE")
            {
                LEF_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "IP_REPORT_FILE")
            {
                IP_REPORT_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "DECREASE_WIRE_STRIPE_TCL")
            {
                DECREASE_WIRE_STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "ADD_STRIPE_TCL")
            {
                ADD_STRIPE_TCL = config_content_array[2];
            }
        }
    }

    double START, END;
    START = clock();
    // TODO step 1 : Smin Smax range
    // TODO step 2 : even Layer cost caculate

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    generateTrackInfoMap(&track_info_map);
    getIpPowerReport(IP_REPORT_FILE, &cell_ip_map);
    // use def for moving
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    transferStripeToMap(&vdd_stripe_map, &vss_stripe_map, &vdd_stripe_vector, &vss_stripe_vector);

    // use tcl for moving
    // getStripeLocationFromStripeTcl(DECREASE_WIRE_STRIPE_TCL, &vdd_stripe_map, &vss_stripe_map, &core_site, &stripe_width_set);
    // sortStripeMap(&vdd_stripe_map, &vss_stripe_map);
    // setStripeInfo(&vdd_stripe_map, &vss_stripe_map, &stripe_info_map);
    // setStripeRange(&vdd_stripe_map, &vss_stripe_map, &core_site);
    // transferMovingRangeToTrack(&vdd_stripe_map, &vss_stripe_map, &track_info_map);
    // getLefCellImformation(LEF_FILE, &cell_info_map);
    // getLeftCellPinAccessPoint(&cell_info_map);
    // getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);
    // setCellStripeRange(&vdd_stripe_map, &cell_ip_map, &cell_placed_map);
    // getLefCellPinAccessPointCost(&cell_info_map, &stripe_width_set);
    // setRoutingTrackPowerConsuming(&vdd_stripe_map, &cell_placed_map, &track_info_map);
    // setRoutingTrackNumberOfPinAccess(&vdd_stripe_map, &cell_placed_map, &cell_ip_map, &cell_info_map, &track_info_map);
    // // Smin 原本最小間距的 1/2 Smax 原本最小間距的 2 倍
    // setAddStripePosition(&vdd_stripe_map, &vss_stripe_map, &stripe_info_map, &track_info_map);
    // generateAddStripeTcl(&vdd_stripe_map, &vss_stripe_map, ADD_STRIPE_TCL);

    // // getAddStripeCost(&vdd_stripe_map, &vss_stripe_map, &track_info_map);
    // // generateAddStripeTcl(&vdd_stripe_map, &vss_stripe_map, ADD_STRIPE_TCL);

    cout
        << endl
        << "Program exctuing time" << (double)clock() / CLOCKS_PER_SEC << " S" << endl;
    myfile << "chip : " << config_file << endl;
    myfile << endl
           << "Program exctuing time" << (double)clock() / CLOCKS_PER_SEC << " S" << endl;
    myfile.close();
    // test
    // getRoutingTrackPowerConsuming(&vdd_stripe_vector, &cell_placed_map, &cell_ip_map);
    // setRoutingTrackPoint(&m2_track_point_map, &m3_track_point_map, m3_x_start, m3_x_pitch, m2_y_start, m2_y_pitch, &core_site);
    // generateLogFile(&vdd_stripe_map["M3"], "track_cost_position.txt");
    cout << "-------------------------- pin_access_power_consume.cpp end --------------------------" << endl;

    return 0;
}
void getStripeRangeCost(Stripe *stripe, string layer, float track_pitch, Stripe *odd_vss_stripe, Stripe *even_vss_stripe)
{

    if (isOddLayer(layer))
    {
        float vdd_stripe_cost = 1000000000000;
        float vss_stripe_cost = 1000000000000;
        string vdd_track = "0";
        string vss_track = "0";

        float power_stripe_width_float = stof((*stripe).width);
        vector<TrackPoint> track_point = (*stripe).track_point_vector;
        // sorting 出 vdd stripe範圍裡所有的track 其中 power consuming pin access cost 最小的地方
        sort(track_point.begin(), track_point.end(), sortTrackPointVector);

        vdd_track = track_point[0].x_point;
        // cout << "track point x : " << track_point[0].x_point << endl;
        float vdd_track_float = stof(vdd_track);

        int cover_track = getCoverTrack(power_stripe_width_float);
        float cover_distance = (cover_track * track_pitch) + 0.036;
        if (cover_distance <= 0.36)
        {
            cover_distance = 0.36 + 0.036;
        }
        float cover_range_left = vdd_track_float - cover_distance;
        float cover_range_right = vdd_track_float + cover_distance;
        string odd_vss_power_stripe_width = (*odd_vss_stripe).width;
        float odd_vss_power_stripe_width_float = stof(odd_vss_power_stripe_width);

        // vss 要不放在 vdd鄰近的範圍內 track cost 第二低的位置
        for (int i = 1; i < track_point.size(); i++)
        {
            float vss_track_point = stof(track_point[i].x_point);
            float vss_track_point_left = vss_track_point - odd_vss_power_stripe_width_float;
            float vss_track_point_right = vss_track_point + odd_vss_power_stripe_width_float;
            if (((vss_track_point_right < cover_range_left) && (vss_track_point_left < cover_range_left)) || ((vss_track_point_left > cover_range_right) && (vss_track_point_right > cover_range_right)))
            {
                vss_track = track_point[i].x_point;
                break;
            }
            // if (!(vss_track_point >= cover_range_left && vss_track_point <= cover_range_right))
            // {
            //     vss_track = track_point[i].x_point;
            //     break;
            // }
        }
        // cout << (*stripe).start_x_location << " vdd change to " << (*stripe).vdd_track_x_location << endl;
        // cout << (*stripe).start_x_location << " vss change to " << (*stripe).vss_track_x_location << endl;
        // 將 vdd track 放入 stripe x track location 內
        (*stripe).vdd_track_x_location = vdd_track;
        cout << "odd layer : " << endl;
        cout << "vdd track x location : " << (*stripe).vdd_track_x_location << "   cover distance : " << cover_distance << endl;
        (*stripe).vss_track_x_location = vss_track;
    }
    else
    {
        float vdd_stripe_cost = 1000000000000;
        float vss_stripe_cost = 1000000000000;
        string vdd_track = "0";
        string vss_track = "0";

        float power_stripe_width_float = stof((*stripe).width);
        vector<TrackPoint> track_point = (*stripe).track_point_vector;
        sort(track_point.begin(), track_point.end(), sortTrackPointVector);

        vdd_track = track_point[0].y_point;
        // cout << "track  y : " << track_point[0].y_point << endl;
        float vdd_track_float = stof(vdd_track);

        int cover_track = getCoverTrack(power_stripe_width_float);
        float cover_distance = (cover_track * track_pitch) + 0.036;
        float cover_range_down = vdd_track_float - cover_distance;
        float cover_range_up = vdd_track_float + cover_distance;
        string even_vss_power_stripe_width = (*even_vss_stripe).width;

        float even_vss_power_stripe_width_float = stof(even_vss_power_stripe_width);

        for (int i = 1; i < track_point.size(); i++)
        {
            float vss_track_point = stof(track_point[i].y_point);
            float vss_track_point_down = vss_track_point - even_vss_power_stripe_width_float;
            float vss_track_point_up = vss_track_point + even_vss_power_stripe_width_float;

            if (((vss_track_point_up < cover_range_down) && (vss_track_point_down < cover_range_up)) || ((vss_track_point_up > cover_range_up) && (vss_track_point_down > cover_range_down)))
            {
                vss_track = track_point[i].y_point;
                break;
            }

            // if (!(vss_track_point >= cover_range_down && vss_track_point <= cover_range_up))
            // {
            //     vss_track = track_point[i].y_point;
            //     break;
            // }
        }

        // cout << (*stripe).start_y_location << " vdd change to " << (*stripe).vdd_track_x_location << endl;
        // cout << (*stripe).start_y_location << " vss change to " << (*stripe).vss_track_x_location << endl;

        (*stripe).vdd_track_y_location = vdd_track;
        cout << "even layer : " << endl;
        cout << "vdd track x location : " << (*stripe).vdd_track_x_location << "   cover distance : " << cover_distance << endl;
        (*stripe).vss_track_y_location = vss_track;
    };
}
void getAddStripeCost(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, TrackInfo> *track_info_map)
{
    cout << "========== getAddStripeCost start ==========" << endl;

    Stripe odd_vss_stripe;
    Stripe even_vss_stripe;
    // 取得 vss stripe width
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        for (int i = 0; i < vss_stripe_map_it->second.size(); i++)
        {
            if (isOddLayer(layer))
            {
                odd_vss_stripe = vss_stripe_map_it->second[i];
                break;
            }
            else
            {
                even_vss_stripe = vss_stripe_map_it->second[i];
                break;
            }
        }
    }

    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        // cout << "layer : " << layer << endl;
        // cout << "pitch : " << (*track_info_map)[layer].pitch << endl;
        float track_pitch_float = stof((*track_info_map)[layer].pitch);
        for (int i = 0; i < vdd_stripe_map_it->second.size(); i++)
        {
            getStripeRangeCost(&(vdd_stripe_map_it->second[i]), layer, track_pitch_float, &odd_vss_stripe, &even_vss_stripe);
        }
    }
    cout << "========== getAddStripeCost end ==========" << endl;
}

void setAddStripePosition(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, StripeInfo> *stripe_info_map, unordered_map<string, TrackInfo> *track_info_map)
{
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        float track_pitch_float = stof((*track_info_map)[layer].pitch);
        for (int i = 0; i < vdd_stripe_map_it->second.size(); i++)
        {
            // getStripeRangeCost(&(vdd_stripe_map_it->second[i]), layer, track_pitch_float, &odd_vss_stripe, &even_vss_stripe);
            setStripeRangeCost(layer, &(vdd_stripe_map_it->second[i]), &(*stripe_info_map), &(*track_info_map));
        }
    }
}

void setStripeRangeCost(string layer, Stripe *stripe, unordered_map<string, StripeInfo> *stripe_info_map, unordered_map<string, TrackInfo> *track_info_map)
{
    if (isOddLayer(layer))
    {
        float vdd_stripe_cost = 1000000000000;
        float vss_stripe_cost = 1000000000000;
        string vdd_track = "0";
        string vss_track = "0";

        float power_stripe_width_float = stof((*stripe).width);
        vector<TrackPoint> track_point = (*stripe).track_point_vector;

        // sorting 出 vdd stripe範圍裡所有的track 其中 power consuming pin access cost 最小的地方
        sort(track_point.begin(), track_point.end(), sortTrackPointVector);
        vdd_track = track_point[0].x_point;

        // cout << "track point x : " << track_point[0].x_point << endl;
        float vdd_track_float = stof(vdd_track);
        float minimum_spacing = stof(((*stripe_info_map)[layer].minmum_spacing)) / 2;
        float vss_strip_width = stof((*stripe_info_map)[layer].original_vss_stripe_width);

        float track_pitch = stof((*track_info_map)[layer].pitch);

        int cover_track = getCoverTrack(minimum_spacing);
        float cover_distance = (cover_track * track_pitch) + 0.036;

        if (cover_distance <= 0.36)
        {
            cover_distance = 0.36 + 0.036;
        }

        float cover_range_left = vdd_track_float - cover_distance;
        float cover_range_right = vdd_track_float + cover_distance;
        // string odd_vss_power_stripe_width = (*odd_vss_stripe).width;
        // float odd_vss_power_stripe_width_float = stof(odd_vss_power_stripe_width);

        // vss 要不放在 vdd鄰近的範圍內 track cost 第二低的位置
        for (int i = 1; i < track_point.size(); i++)
        {
            float vss_track_point = stof(track_point[i].x_point);
            float vss_track_point_left = vss_track_point - vss_strip_width;
            float vss_track_point_right = vss_track_point + vss_strip_width;
            if (((vss_track_point_right < cover_range_left) && (vss_track_point_left < cover_range_left)) || ((vss_track_point_left > cover_range_right) && (vss_track_point_right > cover_range_right)))
            {
                vss_track = track_point[i].x_point;
                break;
            }
            // if (!(vss_track_point >= cover_range_left && vss_track_point <= cover_range_right))
            // {
            //     vss_track = track_point[i].x_point;
            //     break;
            // }
        }
        // cout << (*stripe).start_x_location << " vdd change to " << (*stripe).vdd_track_x_location << endl;
        // cout << (*stripe).start_x_location << " vss change to " << (*stripe).vss_track_x_location << endl;
        // 將 vdd track 放入 stripe x track location 內
        (*stripe).vdd_track_x_location = vdd_track;
        cout << "odd layer : " << endl;
        cout << "vdd track x location : " << (*stripe).vdd_track_x_location << "   cover distance : " << cover_distance << endl;
        (*stripe).vss_track_x_location = vss_track;
    }
    else
    {
        float vdd_stripe_cost = 1000000000000;
        float vss_stripe_cost = 1000000000000;
        string vdd_track = "0";
        string vss_track = "0";

        float power_stripe_width_float = stof((*stripe).width);
        vector<TrackPoint> track_point = (*stripe).track_point_vector;
        sort(track_point.begin(), track_point.end(), sortTrackPointVector);

        float vdd_track_float = stof(vdd_track);
        float minimum_spacing = stof(((*stripe_info_map)[layer].minmum_spacing)) / 2;
        float vss_strip_width = stof((*stripe_info_map)[layer].original_vss_stripe_width);
        float track_pitch = stof((*track_info_map)[layer].pitch);

        vdd_track = track_point[0].y_point;
        // cout << "track  y : " << track_point[0].y_point << endl;

        int cover_track = getCoverTrack(minimum_spacing);
        float cover_distance = (cover_track * track_pitch) + 0.036;
        float cover_range_down = vdd_track_float - cover_distance;
        float cover_range_up = vdd_track_float + cover_distance;

        for (int i = 1; i < track_point.size(); i++)
        {
            float vss_track_point = stof(track_point[i].y_point);
            float vss_track_point_down = vss_track_point - vss_strip_width;
            float vss_track_point_up = vss_track_point + vss_strip_width;

            if (((vss_track_point_up < cover_range_down) && (vss_track_point_down < cover_range_up)) || ((vss_track_point_up > cover_range_up) && (vss_track_point_down > cover_range_down)))
            {
                vss_track = track_point[i].y_point;
                break;
            }

            // if (!(vss_track_point >= cover_range_down && vss_track_point <= cover_range_up))
            // {
            //     vss_track = track_point[i].y_point;
            //     break;
            // }
        }

        // cout << (*stripe).start_y_location << " vdd change to " << (*stripe).vdd_track_x_location << endl;
        // cout << (*stripe).start_y_location << " vss change to " << (*stripe).vss_track_x_location << endl;

        (*stripe).vdd_track_y_location = vdd_track;
        cout << "even layer : " << endl;
        cout << "vdd track x location : " << (*stripe).vdd_track_x_location << "   cover distance : " << cover_distance << endl;
        (*stripe).vss_track_y_location = vss_track;
    };
}

void setStripeInfo(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, StripeInfo> *stripe_info_map)
{

    unordered_map<string, vector<Stripe>> all_stripe_map;
    // vss stripe width info
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        StripeInfo stripe_info;
        stripe_info.layer = layer;
        for (int i = 0; i < vss_stripe_map_it->second.size(); i++)
        {
            if (isOddLayer(layer))
            {
                string width = vss_stripe_map_it->second[i].width;
                stripe_info.original_vss_stripe_width = width;
                break;
            }
            else
            {
                string width = vss_stripe_map_it->second[i].width;
                stripe_info.original_vss_stripe_width = width;
                break;
            }
        }

        (*stripe_info_map).insert(pair<string, StripeInfo>(layer, stripe_info));
        vector<Stripe> stripe_vector;
        all_stripe_map.insert(pair<string, vector<Stripe>>(layer, stripe_vector));
    }
    // spacing
    // step all stripe spacing
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        if (all_stripe_map.count(layer) != 0)
        {
            all_stripe_map[layer].insert(all_stripe_map[layer].end(), (*vdd_stripe_map)[layer].begin(), (*vdd_stripe_map)[layer].end());
        }
        else
        {
            cout << "no this layer" << endl;
        }
    }
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        if (all_stripe_map.count(layer) != 0)
        {
            all_stripe_map[layer].insert(all_stripe_map[layer].end(), (*vss_stripe_map)[layer].begin(), (*vss_stripe_map)[layer].end());
        }
        else
        {
            cout << "no this layer" << endl;
        }
    }
    for (auto all_stripe_map_it = all_stripe_map.begin(); all_stripe_map_it != all_stripe_map.end(); ++all_stripe_map_it)
    {
        string layer = all_stripe_map_it->first;
        vector<Stripe> stripe_vector = all_stripe_map[layer];
        cout << " layer : " << layer << " " << all_stripe_map_it->second.size() << endl;

        if (isOddLayer(layer))
        {
            float minimum_section = 100000000;
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            for (int i = 1; i < stripe_vector.size(); i++)
            {

                float spacing = stof(stripe_vector[i].start_x_location) - stof(stripe_vector[i - 1].start_x_location);
                spacing = abs(spacing);
                if (spacing <= minimum_section)
                {
                    minimum_section = spacing;
                }
            }
            (*stripe_info_map)[layer].minmum_spacing = floatToString(minimum_section);
        }
        else
        {
            float minimum_section = 100000000;
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            for (int i = 1; i < stripe_vector.size(); i++)
            {
                float spacing = stof(stripe_vector[i].start_y_location) - stof(stripe_vector[i - 1].start_y_location);
                spacing = abs(spacing);
                if (spacing <= minimum_section)
                {
                    minimum_section = spacing;
                }
            }
            (*stripe_info_map)[layer].minmum_spacing = floatToString(minimum_section);
        }
    }
}

void setAddStripePostion(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, TrackInfo> *track_info_map)
{
    Stripe odd_vss_stripe;
    Stripe even_vss_stripe;
    // ================== 取得 vss stripe width start =========================
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        for (int i = 0; i < vss_stripe_map_it->second.size(); i++)
        {
            if (isOddLayer(layer))
            {
                odd_vss_stripe = vss_stripe_map_it->second[i];
                break;
            }
            else
            {
                even_vss_stripe = vss_stripe_map_it->second[i];
                break;
            }
        }
    }
    // ================== 取得 vss stripe width end =========================
}
void getLefCellPinAccessPointCost(unordered_map<string, CellInfo> *cell_info_map, set<string> *stripe_width_set)
{

    // step 1: set stripe width
    for (auto cell_info_map_it = (*cell_info_map).begin(); cell_info_map_it != (*cell_info_map).end(); ++cell_info_map_it)
    {
        string cell_name = cell_info_map_it->first;

        unordered_map<string, unordered_map<string, PinAccessCost>> odd_pin_access_cost_map;
        unordered_map<string, unordered_map<string, PinAccessCost>> even_pin_access_cost_map;
        for (auto string_width : (*stripe_width_set))
        {
            unordered_map<string, PinAccessCost> pin_access_cost_map;
            // width
            odd_pin_access_cost_map.insert(pair<string, unordered_map<string, PinAccessCost>>(string_width, pin_access_cost_map));
            even_pin_access_cost_map.insert(pair<string, unordered_map<string, PinAccessCost>>(string_width, pin_access_cost_map));
        }
        (*cell_info_map)[cell_name].odd_pin_access_cost_map = odd_pin_access_cost_map;
        (*cell_info_map)[cell_name].even_pin_access_cost_map = even_pin_access_cost_map;
    }

    // step 2 : set pin access cost
    for (auto cell_info_map_it = (*cell_info_map).begin(); cell_info_map_it != (*cell_info_map).end(); ++cell_info_map_it)
    {
        string cell_name = cell_info_map_it->first;
        // key 1 : width                    key 2 : location
        unordered_map<string, unordered_map<string, PinAccessCost>> odd_pin_access_cost_map = (*cell_info_map)[cell_name].odd_pin_access_cost_map;
        unordered_map<string, unordered_map<string, PinAccessCost>> even_pin_access_cost_map = (*cell_info_map)[cell_name].even_pin_access_cost_map;

        for (auto odd_pin_access_cost_map_it = odd_pin_access_cost_map.begin(); odd_pin_access_cost_map_it != odd_pin_access_cost_map.end(); ++odd_pin_access_cost_map_it)
        {
            string stripe_width = odd_pin_access_cost_map_it->first;
            // ODD Track
            setTrackPointCost(ODD, &(*cell_info_map)[cell_name], stripe_width);
        }

        for (auto even_pin_access_cost_map_it = even_pin_access_cost_map.begin(); even_pin_access_cost_map_it != even_pin_access_cost_map.end(); ++even_pin_access_cost_map_it)
        {
            string stripe_width = even_pin_access_cost_map_it->first;
            // EVEN Track
            setTrackPointCost(EVEN, &(*cell_info_map)[cell_name], stripe_width);
        }
    }
    //     if ("U66546" == (*cell_placed_info).cell_id)
    // {
    //     cout << "track location " << track_location << endl;
    //     cout << "cell_placed_left_x_location : " << cell_placed_left_x_location << endl;
    //     cout << isOddTrackCell(cell_placed_left_x_location, &(*track_info)) << endl;
    // }

    // for (auto odd_pin_access_cost_map_it = (*cell_info_map)["AO22x1_ASAP7_6t_fix"].odd_pin_access_cost_map["0.224"].begin(); odd_pin_access_cost_map_it != (*cell_info_map)["AO22x1_ASAP7_6t_fix"].odd_pin_access_cost_map["0.224"].end(); ++odd_pin_access_cost_map_it)
    // {
    //     cout << "location : " << odd_pin_access_cost_map_it->first << endl;
    // }
    // cout << "check" << endl;

    // if ((*cell_info_map)["INVxp33_ASAP7_6t_fix"].even_pin_access_cost_map["0.224"].find("0.108") != ((*cell_info_map)["AO22x1_ASAP7_6t_fix"].even_pin_access_cost_map["0.224"].end()))
    // {
    //     cout << "check in side" << endl;
    // }
    // else
    // {
    //     cout << "did not exsist" << endl;
    // }

    // cout << "check in side : " << (*cell_info_map)["INVxp33_ASAP7_6t_fix"].even_pin_access_cost_map["0.224"]["0.108"].vertical_direction_cost_map["S"] << endl;
    // cout << "check in side : " << (*cell_info_map)["INVxp33_ASAP7_6t_fix"].even_pin_access_cost_map["0.224"]["0.108"].vertical_direction_cost_map["FS"] << endl;
    // cout << "check in side : " << (*cell_info_map)["INVxp33_ASAP7_6t_fix"].even_pin_access_cost_map["0.224"]["0.108"].vertical_direction_cost_map["N"] << endl;
    // cout << "check in side : " << (*cell_info_map)["INVxp33_ASAP7_6t_fix"].even_pin_access_cost_map["0.224"]["0.108"].vertical_direction_cost_map["FN"] << endl;
    //    if ("U58984" == (*cell_placed_info).cell_id)
    // {
    //     cout << "check " << endl;
    // }

    // unordered_map<string, string> vertical_direction_cost_map = odd_pin_access_cost_map["0.224"]["0.108"].vertical_direction_cost_map["S"];
    // if ((odd_pin_access_cost_map[power_stripe_width].count(track_location)) != true)
    // {
    //     cout << "has error in transferTrackLocationType " << endl;
    // }
}
void setTrackPointCost(string type, CellInfo *cell_info, string stripe_width)
{
    vector<string> place_direction;
    place_direction.push_back("FN");
    place_direction.push_back("N");
    place_direction.push_back("FS");
    place_direction.push_back("S");
    float odd_start_m3_track_float = 0.036;
    float even_start_m3_track_float = 0.108;
    float m3_track_step_float = 0.144;

    float cell_width_float = stof((*cell_info).width);
    int even_start_m3_track_int = tranferLocationFloatToInt(even_start_m3_track_float);
    int odd_start_m3_track_int = tranferLocationFloatToInt(odd_start_m3_track_float);
    int cell_width_int = tranferLocationFloatToInt(cell_width_float);
    int m3_track_step_int = tranferLocationFloatToInt(m3_track_step_float);

    // cout << "track : " << odd_start_m3_track_int << endl;
    // cout << "track : " << even_start_m3_track_int << endl;
    // cout << "track : " << m3_track_step_int << endl;
    map<string, vector<PinAccessPoint>> even_pin_access_point_map = (*cell_info).even_pin_access_point_map;
    map<string, vector<PinAccessPoint>> odd_pin_access_point_map = (*cell_info).odd_pin_access_point_map;
    // (*cell_info).width;
    // cout << "cell info width : " << cell_width_int << endl;

    if (type == ODD)
    {
        for (int i = odd_start_m3_track_int; i < cell_width_int; i += m3_track_step_int)
        {
            float step = convertInnovusPoint(i);
            string step_str = floatToString(step);
            PinAccessCost pin_access_cost;
            for (int k = 0; k < place_direction.size(); k++)
            {
                string place_direction_type = place_direction[k];
                double total_pin_access_cost_of_cell_double = 0;
                for (auto odd_pin_access_point_map_it = odd_pin_access_point_map.begin(); odd_pin_access_point_map_it != odd_pin_access_point_map.end(); ++odd_pin_access_point_map_it)
                {
                    string pin_name = odd_pin_access_point_map_it->first;
                    // if (pin_name == "A2" && place_direction_type == "FS")
                    // {
                    //     cout << "break" << endl;
                    // }
                    int total_pin_access_size = odd_pin_access_point_map[pin_name].size();
                    int cover_pin_access_size = 0;
                    for (int j = 0; j < odd_pin_access_point_map[pin_name].size(); j++)
                    {
                        // 根據當下 placed  FN的位置去做計算
                        PinAccessPoint transfer_pin_access_point;
                        transferPinAccessLocationFromRect(place_direction_type, &odd_pin_access_point_map[pin_name][j], (*cell_info).width, (*cell_info).height, &transfer_pin_access_point);
                        if (isCoverPinAccessCost("M3", &transfer_pin_access_point, step, stof(stripe_width)))
                        {
                            cover_pin_access_size += 1;
                        }
                    }
                    // if (pin_name == "A2" && place_direction_type == "FS")
                    // {
                    //     cout << "break" << endl;
                    // }
                    double total_pin_access_size_double = (double)total_pin_access_size;
                    double cover_pin_access_size_double = (double)cover_pin_access_size;

                    double pin_access_cost_of_cell_double = caculatePinAccessCost(total_pin_access_size, cover_pin_access_size);

                    // cout << " pin name                 : " << pin_name << endl;
                    // cout << " total_pin_access_size    : " << total_pin_access_size << endl;
                    // cout << " cover_pin_access_size    : " << cover_pin_access_size << endl;
                    // cout << " step                     : " << step << endl;
                    // cout << " stripe width             : " << stripe_width << endl;
                    // cout << " place_direction_type     : " << place_direction_type << endl;
                    // cout << " pin_access_cost_of_cell  : " << pin_access_cost_of_cell_double << endl;
                    // cout << " ------------------------------------------- " << endl;

                    // cout << "pin_access_cost_of_cell : " << pin_access_cost_of_cell << endl;
                    total_pin_access_cost_of_cell_double += pin_access_cost_of_cell_double;
                }
                string total_pin_access_cost_of_cell_str = to_string(total_pin_access_cost_of_cell_double);
                pin_access_cost.vertical_direction_cost_map.insert(pair<string, string>(place_direction_type, total_pin_access_cost_of_cell_str));
            }
            (*cell_info).odd_pin_access_cost_map[stripe_width].insert(pair<string, PinAccessCost>(step_str, pin_access_cost));
            // cout << "check in side : " << (*cell_info).even_pin_access_cost_map[stripe_width]["0.108"].direction_cost_map["FN"] << endl;
        }
    }

    else
    {
        // width                             location
        // unordered_map<string, unordered_map<string, PinAccessCost>> even_pin_access_cost_map = (*cell_info).even_pin_access_cost_map;
        // unordered_map<string, PinAccessCost> even_pin_access_cost_location_map = (*cell_info).even_pin_access_cost_map[stripe_width];
        // 在這條track 上 碰到幾個pin access point
        for (int i = even_start_m3_track_int; i < cell_width_int; i += m3_track_step_int)
        {
            float step = convertInnovusPoint(i);
            string step_str = floatToString(step);
            PinAccessCost pin_access_cost;
            for (int k = 0; k < place_direction.size(); k++)
            {
                string place_direction_type = place_direction[k];
                double total_pin_access_cost_of_cell_double = 0;
                for (auto even_pin_access_point_map_it = even_pin_access_point_map.begin(); even_pin_access_point_map_it != even_pin_access_point_map.end(); ++even_pin_access_point_map_it)
                {
                    string pin_name = even_pin_access_point_map_it->first;
                    // if(pin_name == "Y"){
                    //     cout << "break" << endl;
                    // }
                    int total_pin_access_size = even_pin_access_point_map[pin_name].size();
                    int cover_pin_access_size = 0;
                    for (int j = 0; j < even_pin_access_point_map[pin_name].size(); j++)
                    {
                        // 根據當下 placed  FN的位置去做計算
                        PinAccessPoint transfer_pin_access_point;
                        transferPinAccessLocationFromRect(place_direction_type, &even_pin_access_point_map[pin_name][j], (*cell_info).width, (*cell_info).height, &transfer_pin_access_point);
                        if (isCoverPinAccessCost("M3", &transfer_pin_access_point, step, stof(stripe_width)))
                        {
                            cover_pin_access_size += 1;
                        }
                    }
                    double total_pin_access_size_double = (double)total_pin_access_size;
                    double cover_pin_access_size_double = (double)cover_pin_access_size;

                    double pin_access_cost_of_cell_double = caculatePinAccessCost(total_pin_access_size, cover_pin_access_size);

                    // cout << " pin name                 : " << pin_name << endl;
                    // cout << " total_pin_access_size    : " << total_pin_access_size << endl;
                    // cout << " cover_pin_access_size    : " << cover_pin_access_size << endl;
                    // cout << " step                     : " << step << endl;
                    // cout << " stripe width             : " << stripe_width << endl;
                    // cout << " place_direction_type     : " << place_direction_type << endl;
                    // cout << " pin_access_cost_of_cell  : " << pin_access_cost_of_cell_double << endl;
                    // cout << " ------------------------------------------- " << endl;

                    // cout << "pin_access_cost_of_cell : " << pin_access_cost_of_cell << endl;
                    total_pin_access_cost_of_cell_double += pin_access_cost_of_cell_double;
                }
                string total_pin_access_cost_of_cell_str = to_string(total_pin_access_cost_of_cell_double);
                pin_access_cost.vertical_direction_cost_map.insert(pair<string, string>(place_direction_type, total_pin_access_cost_of_cell_str));
            }
            (*cell_info).even_pin_access_cost_map[stripe_width].insert(pair<string, PinAccessCost>(step_str, pin_access_cost));
            // cout << "check in side : " << (*cell_info).even_pin_access_cost_map[stripe_width]["0.108"].direction_cost_map["FN"] << endl;
        }
    }
}

double caculatePinAccessCost(double total_pin_access_size, double cover_pin_access_size)
{
    double cost = 0;
    if (total_pin_access_size <= 3)
    {
        double temp = (double)(cover_pin_access_size / total_pin_access_size);
        cost = temp * PIN_ACCESS_LESS_WEIGHT;
    }
    else
    {

        cost = (double)(cover_pin_access_size / total_pin_access_size);
    }

    return cost;
}

bool isCoverPinAccessCost(string layer, PinAccessPoint *transfer_pin_access_point, float track_location, float power_stripe_width)
{
    int cover_track = getCoverTrack(power_stripe_width);
    float cover_distance = (cover_track * 0.144) + 0.036;
    if (isOddLayer(layer))
    {
        float track_point_x_left = track_location - cover_distance;
        float track_point_x_right = track_location + cover_distance;

        float pin_access_x_left = stof((*transfer_pin_access_point).middle_x_location) - (0.072 / 2);
        float pin_access_x_right = stof((*transfer_pin_access_point).middle_x_location) + (0.072 / 2);

        if (track_point_x_left < pin_access_x_left && track_point_x_right < pin_access_x_left)
        {
            return false;
        }
        else if (track_point_x_left > pin_access_x_left && track_point_x_right > pin_access_x_right)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        float track_point_y_down_float = track_location - cover_distance;
        float track_point_y_up_float = track_location + cover_distance;

        float pin_access_y_down_float = stof((*transfer_pin_access_point).middle_y_location) - (0.072 / 2);
        float pin_access_y_up_float = stof((*transfer_pin_access_point).middle_y_location) + (0.072 / 2);

        if (track_point_y_down_float < pin_access_y_down_float && track_point_y_up_float < pin_access_y_down_float)
        {
            return false;
        }
        else if (track_point_y_down_float > pin_access_y_down_float && track_point_y_down_float > pin_access_y_up_float)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

void transferPinAccessLocationFromRect(string direction, PinAccessPoint *pin_access_point, string cell_width, string cell_height, PinAccessPoint *transfer_pin_access_point)
{
    float pin_access_x_point = stof((*pin_access_point).middle_x_location);
    float pin_access_y_point = stof((*pin_access_point).middle_y_location);
    float cell_width_float = stof(cell_width);
    float cell_height_float = stof(cell_height);

    if (direction == "N")
    {
        (*transfer_pin_access_point).middle_x_location = floatToString(pin_access_x_point);
        (*transfer_pin_access_point).middle_y_location = floatToString(pin_access_x_point);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if (direction == "S")
    {

        float temp_middle_x_float = abs(cell_width_float - pin_access_x_point);
        float temp_middle_y_float = abs(cell_height_float - pin_access_y_point);

        (*transfer_pin_access_point).middle_x_location = floatToString(temp_middle_x_float);
        (*transfer_pin_access_point).middle_y_location = floatToString(temp_middle_y_float);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if (direction == "FN")
    {

        float temp_middle_x_float = abs(cell_width_float - pin_access_x_point);

        (*transfer_pin_access_point).middle_x_location = floatToString(temp_middle_x_float);
        (*transfer_pin_access_point).middle_y_location = floatToString(pin_access_y_point);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if (direction == "FS")
    {

        float temp_middle_y_float = abs(cell_height_float - pin_access_y_point);

        (*transfer_pin_access_point).middle_x_location = floatToString(pin_access_x_point);
        (*transfer_pin_access_point).middle_y_location = floatToString(temp_middle_y_float);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else
    {
        cout << " transferPinAccessLocationFromRect error " << endl;
    }
};

int tranferLocationFloatToInt(float location)
{
    float temp = location * 1000.0;
    temp = round(temp);
    int temp_int = (int)temp;
    int ans = temp_int / 4;
    return ans;
}

void sortStripeMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map)
{
    cout << "========== sortStripeMap start ==========" << endl;
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        vector<Stripe> stripe_vector = (*vdd_stripe_map)[layer];
        if (isOddLayer(layer))
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            (*vdd_stripe_map)[layer] = stripe_vector;
        }
        else
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            (*vdd_stripe_map)[layer] = stripe_vector;
        }
    }
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        vector<Stripe> stripe_vector = (*vss_stripe_map)[layer];
        if (isOddLayer(layer))
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            (*vss_stripe_map)[layer] = stripe_vector;
        }
        else
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            (*vss_stripe_map)[layer] = stripe_vector;
        }
    }
    cout << "========== sortStripeMap end ==========" << endl;
}
void transferStripePositionFromTcl(string layer, Stripe *stripe, string start_x_location, string start_y_location, string end_x_location, string end_y_location)
{

    float start_x_location_float = stof(start_x_location);
    float end_x_location_float = stof(end_x_location);
    float start_y_loaction_float = stof(start_y_location);
    float end_y_location_float = stof(end_y_location);

    float middle_x_location_float = (start_x_location_float + end_x_location_float) / 2;
    float middle_y_location_float = (start_y_loaction_float + start_y_loaction_float) / 2;

    if (isOddLayer(layer))
    {
        (*stripe).start_x_location = floatToString(middle_x_location_float);
        (*stripe).start_y_location = start_y_location;
        (*stripe).end_x_location = floatToString(middle_x_location_float);
        (*stripe).end_y_location = end_y_location;
    }
    else
    {
        (*stripe).start_x_location = start_x_location;
        (*stripe).start_y_location = floatToString(middle_y_location_float);
        (*stripe).end_x_location = end_x_location;
        (*stripe).end_y_location = floatToString(middle_y_location_float);
    }
};

void getStripeLocationFromStripeTcl(string stripe_tcl_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, set<string> *stripe_width_set)
{
    cout << "========== getStripeLocationFromStripeTcl start ==========" << endl;
    ifstream stripe_tcl_file(stripe_tcl_file_name);
    string stripe_tcl_content;

    if (stripe_tcl_file)
    {
        while (getline(stripe_tcl_file, stripe_tcl_content))
        {
            vector<string> stripe_tcl_content_array = splitByPattern(stripe_tcl_content, " ");
            // cout << stripe_tcl_content << endl;
            Stripe stripe;
            stripe.layer = stripe_tcl_content_array[6];
            stripe.width = stripe_tcl_content_array[10];
            transferStripePositionFromTcl(stripe.layer, &stripe, stripe_tcl_content_array[17], stripe_tcl_content_array[18], stripe_tcl_content_array[19], stripe_tcl_content_array[20]);
            stripe.net_name = stripe_tcl_content_array[3];
            setStripeLength(&stripe);
            (*stripe_width_set).insert(stripe.width);
            // cout << stripe.net_name << " " << stripe.layer << " " << stripe.width << " " << stripe.length << " " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << endl;
            vector<string> ip_power_vector;
            stripe.ip_power_vector = ip_power_vector;
            if (stripe.net_name == "VDDX")
            {
                if ((*vdd_stripe_map).count(stripe.layer))
                {
                    (*vdd_stripe_map)[stripe.layer].push_back(stripe);
                }
                else
                {
                    vector<Stripe> layer_vdd_stripe_vector;
                    layer_vdd_stripe_vector.push_back(stripe);
                    string layer = stripe.layer;
                    (*vdd_stripe_map).insert(pair<string, vector<Stripe>>(layer, layer_vdd_stripe_vector));
                }
            }
            else if (stripe.net_name == "VSSX")
            {
                if ((*vss_stripe_map).count(stripe.layer))
                {
                    (*vss_stripe_map)[stripe.layer].push_back(stripe);
                }
                else
                {
                    vector<Stripe> layer_vss_stripe_vector;
                    layer_vss_stripe_vector.push_back(stripe);
                    string layer = stripe.layer;
                    (*vss_stripe_map).insert(pair<string, vector<Stripe>>(layer, layer_vss_stripe_vector));
                }
            }

            // cout << stripe.net_name << " " << stripe.layer << " " << stripe.width << " " << stripe.length << " " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << endl;
        }
    }
    else
    {
        cout << "can't found file" << endl;
    }
    stripe_tcl_file.close();
    cout << "========== getStripeLocationFromStripeTcl end ==========" << endl;
}

void generateAddStripeTcl(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, string move_stripe_name)
{
    Stripe odd_vss_stripe;
    Stripe even_vss_stripe;

    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        for (int i = 0; i < vss_stripe_map_it->second.size(); i++)
        {
            if (isOddLayer(layer))
            {
                odd_vss_stripe = vss_stripe_map_it->second[i];
                break;
            }
            else
            {
                even_vss_stripe = vss_stripe_map_it->second[i];
                break;
            }
        }
    }

    ofstream myfile;
    myfile.open(move_stripe_name);
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;

        for (int i = 0; i < vdd_stripe_map_it->second.size(); i++)
        {
            if (isOddLayer(layer))
            {
                Stripe stripe = vdd_stripe_map_it->second[i];
                float power_stripe_width_float = stof(stripe.width);
                float vdd_track_cost = stof(stripe.vdd_track_x_location);
                float vdd_left_track_cost = vdd_track_cost - (power_stripe_width_float / 2);
                float vdd_right_track_cost = vdd_track_cost + (power_stripe_width_float / 2);

                string vdd_left_track_cost_str = floatToString(vdd_left_track_cost);
                string vdd_right_track_cost_str = floatToString(vdd_right_track_cost);

                float vss_track_x_cost = stof(stripe.vss_track_x_location);
                float vss_left_track_cost = vss_track_x_cost - (power_stripe_width_float / 2);
                float vss_right_track_cost = vss_track_x_cost + (power_stripe_width_float / 2);

                string vss_left_track_cost_str = floatToString(vss_left_track_cost);
                string vss_right_track_cost_str = floatToString(vss_right_track_cost);
                cout << "========== range " << i << "  end ==========" << endl;
                myfile << "addStripe -nets { VDDX } -layer "
                       << layer
                       << " -direction vertical -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vdd_left_track_cost_str << " " << stripe.start_y_location << " " << vdd_right_track_cost_str << " " << stripe.end_y_location << " }" << endl;
                // myfile << "addStripe -nets { VSSX } -layer "
                //        << layer
                //        << " -direction vertical -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_left_track_cost << " " << odd_vss_stripe.start_y_location << " " << vss_right_track_cost << " " << odd_vss_stripe.end_y_location << " }" << endl;
                // myfile << "addStripe -nets { VSSX } -layer "
                //        << "M3"
                //        << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_left_track_cost_str << " " << (*stripe_vector)[i].start_y_location << " " << vss_right_track_cost_str << " " << (*stripe_vector)[i].end_y_location << " }" << endl;
            }
            else
            {
                Stripe stripe = vdd_stripe_map_it->second[i];
                float power_stripe_width_float = stof(stripe.width);
                float vdd_track_cost = stof(stripe.vdd_track_y_location);
                float vdd_down_track_cost = vdd_track_cost - (power_stripe_width_float / 2);
                float vdd_up_track_cost = vdd_track_cost + (power_stripe_width_float / 2);

                string vdd_down_track_cost_str = floatToString(vdd_down_track_cost);
                string vdd_up_track_cost_str = floatToString(vdd_up_track_cost);

                float vss_track_y_cost = stof(stripe.vss_track_y_location);
                float vss_down_track_cost = vss_track_y_cost - (power_stripe_width_float / 2);
                float vss_up_track_cost = vss_track_y_cost + (power_stripe_width_float / 2);

                string vss_down_track_cost_str = floatToString(vss_down_track_cost);
                string vss_up_track_cost_str = floatToString(vss_up_track_cost);

                myfile << "addStripe -nets { VDDX } -layer "
                       << layer
                       << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << vdd_down_track_cost_str << " " << stripe.end_x_location << " " << vdd_up_track_cost_str << " }" << endl;
                // myfile << "addStripe -nets { VSSX } -layer "
                //        << layer
                //        << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << even_vss_stripe.start_x_location << " " << vss_down_track_cost_str << " " << even_vss_stripe.end_x_location << " " << vss_up_track_cost_str << " }" << endl;

                // myfile << "addStripe -nets { VSSX } -layer "
                //        << "M3"
                //        << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_left_track_cost_str << " " << (*stripe_vector)[i].start_y_location << " " << vss_right_track_cost_str << " " << (*stripe_vector)[i].end_y_location << " }" << endl;
            }
        }
    }
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;

        for (int i = 0; i < (*vss_stripe_map)[layer].size(); i++)
        {
            if (isOddLayer(layer))
            {
                Stripe stripe = (*vss_stripe_map)[layer][i];
                float power_stripe_width_float = stof(stripe.width);
                string power_stripe_width = floatToString(power_stripe_width_float);
                float vss_start_x_location_float = stof(stripe.start_x_location) - (power_stripe_width_float / 2);
                float vss_end_x_location_float = stof(stripe.start_x_location) + (power_stripe_width_float / 2);
                string vss_start_x_location = floatToString(vss_start_x_location_float);
                string vss_end_x_location = floatToString(vss_end_x_location_float);
                myfile << "addStripe -nets { VSSX } -layer "
                       << layer
                       << " -direction vertical -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << vss_start_x_location << " " << stripe.start_y_location << " " << vss_end_x_location << " " << stripe.end_y_location << " }" << endl;
            }
            else
            {
                Stripe stripe = (*vss_stripe_map)[layer][i];
                float power_stripe_width_float = stof(stripe.width);
                string power_stripe_width = floatToString(power_stripe_width_float);
                float vss_start_y_location_float = stof(stripe.start_y_location) - (power_stripe_width_float / 2);
                float vss_end_y_location_float = stof(stripe.start_y_location) + (power_stripe_width_float / 2);
                string vss_start_y_location = floatToString(vss_start_y_location_float);
                string vss_end_y_location = floatToString(vss_end_y_location_float);
                myfile << "addStripe -nets { VSSX } -layer "
                       << layer
                       << " -direction horizontal -width " << power_stripe_width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << vss_start_y_location << " " << stripe.end_x_location << " " << vss_end_y_location << " }" << endl;
            }
        }
    }

    myfile.close();
}

bool sortTrackPointVector(TrackPoint track_point_a, TrackPoint track_point_b)
{
    return track_point_a.total_pin_access_power_consum_cost < track_point_b.total_pin_access_power_consum_cost;
}

void setRoutingTrackNumberOfPinAccess(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, unordered_map<string, TrackInfo> *track_info_map)
{
    cout << "========== setRoutingTrackNumberOfPinAccess start ==========" << endl;
    ofstream myfile;
    myfile.open("stripe_cost.txt");
    for (auto stripe_map_it = (*stripe_map).begin(); stripe_map_it != (*stripe_map).end(); ++stripe_map_it)
    {
        string layer = stripe_map_it->first;
        setTrackPinAccessCost(&myfile, layer, &(*stripe_map)[layer], &(*cell_placed_map), &(*cell_ip_map), &(*cell_info_map), &(*track_info_map)[layer]);
    }
    myfile.close();
    cout << "========== setRoutingTrackNumberOfPinAccess end ==========" << endl;
}
void setTrackPinAccessCost(ofstream *myfile, string layer, vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info)
{
    vector<float> total_pin_access_cost_vector;
    vector<float> total_power_consuming_vector;
    int track_size = 0;

    cout << "========== setTrackPinAccessCost start ==========" << endl;

    // step 1 : get pin_access_cost and power_consuming_cost of each track
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        cout << "========== " << (*stripe_vector)[i].layer << " "
             << "stripe" << i << " start ========== " << endl;
        // if (i != 44)
        // {
        //     continue;
        // }

        for (int j = 0; j < (*stripe_vector)[i].track_point_vector.size(); j++)
        {

            TrackPoint track_point = (*stripe_vector)[i].track_point_vector[j];
            float total_cell_pin_access_power_consume_cost = 0;
            // float power_stripe_width_float = stof((*stripe_vector)[i].width);
            string power_stripe_width = (*stripe_vector)[i].width;
            float track_of_total_pin_access_cost = 0;
            float track_of_total_power_consuming = 0;
            for (int k = 0; k < track_point.power_cell_id_vector.size(); k++)
            {

                string cell_id = track_point.power_cell_id_vector[k];
                CellPlacedInfo cell_placed_info = (*cell_placed_map)[cell_id];
                // if (i == 44)
                // {
                //     cout << "cell id " << cell_id << endl;
                // }
                // cost function
                // float cell_pin_access_cost = getTrackPinAccessPointCost(layer, power_stripe_width_float, &track_point, &cell_placed_info, &(*cell_ip_map), &(*cell_info_map), &(*track_info));
                float cell_pin_access_cost = getTrackPinAccessPointCost(layer, power_stripe_width, &track_point, &cell_placed_info, &(*cell_info_map), &(*track_info));
                float power_consuming_cost = ((*cell_ip_map)[cell_id].instance_total_power * 10000);
                // power_consuming_cost = 1 / power_consuming_cost;

                track_of_total_pin_access_cost += cell_pin_access_cost;
                track_of_total_power_consuming += power_consuming_cost;
            }
            (*stripe_vector)[i].track_point_vector[j].pin_access_cost = track_of_total_pin_access_cost;
            (*stripe_vector)[i].track_point_vector[j].power_consuming_cost = track_of_total_power_consuming;
            total_pin_access_cost_vector.push_back(track_of_total_pin_access_cost);
            total_power_consuming_vector.push_back(track_of_total_power_consuming);
        }
        track_size += (*stripe_vector)[i].track_point_vector.size();
        cout << "========== stripe " << i << " end ==========" << endl;
    }
    // // step 2 : get standard deviation and average
    float pin_access_cost_standard_deviation = standardDeviation(&total_pin_access_cost_vector);
    float power_consuming_cost_standard_deviation = standardDeviation(&total_power_consuming_vector);
    float pin_access_cost_arithmetic_mean = arithmeticMean(&total_pin_access_cost_vector);
    float power_consuming_cost_arithmetic_mean = arithmeticMean(&total_power_consuming_vector);

    // step 3 : z-score normalization
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        (*myfile) << "======================== stripe " << i << " end ========================" << endl;
        float total_pin_access_cost = 0;
        float total_power_consuming_cost = 0;
        vector<CompareTrack> compare_track_vector;

        for (int j = 0; j < (*stripe_vector)[i].track_point_vector.size(); j++)
        {
            TrackPoint track_point = (*stripe_vector)[i].track_point_vector[j];
            (*myfile) << "---------- track point " << track_point.x_point << " " << track_point.y_point << " end ----------" << endl;
            float original_power_comsing_cost = track_point.power_consuming_cost;
            float original_pin_access_cost = track_point.pin_access_cost;
            // (*myfile) << "power_consuming         :" << original_power_comsing_cost << " ----------" << endl;
            (*myfile) << "pin_access_cost         :" << original_pin_access_cost << " ----------" << endl;
            float z_score_power_cost = z_score_normalization(original_power_comsing_cost, power_consuming_cost_arithmetic_mean, power_consuming_cost_standard_deviation);
            float z_score_pin_access_cost = z_score_normalization(original_pin_access_cost, pin_access_cost_arithmetic_mean, pin_access_cost_standard_deviation);
            // (*myfile) << "z_score_power_cost      :" << (1 / z_score_power_cost) << " ----------" << endl;
            // (*myfile) << "z_score_pin_access_cost :" << z_score_pin_access_cost << " ----------" << endl;
            (*stripe_vector)[i].track_point_vector[j].power_consuming_cost = z_score_power_cost;
            (*stripe_vector)[i].track_point_vector[j].pin_access_cost = z_score_pin_access_cost;
            // ======================== log for stripe cost ====================================
            CompareTrack compare_track;
            compare_track.pin_access_cost = z_score_pin_access_cost;
            compare_track.power_cost = (1 / z_score_power_cost);
            compare_track.x_point = track_point.x_point;
            compare_track.y_point = track_point.y_point;
            compare_track_vector.push_back(compare_track);

            total_pin_access_cost += z_score_pin_access_cost;
            total_power_consuming_cost += (1 / z_score_power_cost);
            // ======================== log for stripe cost ====================================
            //========================== TODO change cost ===================================
            // (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = (1 / z_score_power_cost) + z_score_pin_access_cost;
            // (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = z_score_power_cost;
            (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = z_score_pin_access_cost;
            //========================== TODO change cost ===================================
            // (*myfile) << "total_cost            :" << (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost << " ----------" << endl;
        }
        // 範圍內 z-cost pin access cost +  z-cost power cost最大
        float average_z_score_pin_access_cost = total_pin_access_cost / ((*stripe_vector)[i].track_point_vector.size());
        float average_z_score_power_cost = total_power_consuming_cost / ((*stripe_vector)[i].track_point_vector.size());
        // (*myfile) << "average z_score_pin_access_cost                 :" << (total_pin_access_cost / ((*stripe_vector)[i].track_point_vector.size())) << " ----------" << endl;
        // (*myfile) << "average_z_score_power_cost                      :" << (total_power_consuming_cost / ((*stripe_vector)[i].track_point_vector.size())) << " ----------" << endl;

        for (int i = 0; i < compare_track_vector.size(); i++)
        {
            getCompareTrackType(&compare_track_vector[i], average_z_score_pin_access_cost, average_z_score_power_cost);
            float temp_pin_access = abs(average_z_score_pin_access_cost - (compare_track_vector)[i].pin_access_cost);
            float temp_power = abs(average_z_score_power_cost - (compare_track_vector)[i].power_cost);
            compare_track_vector[i].power_consum_and_pin_access_to_average = temp_pin_access + temp_power;
        }
        sort(compare_track_vector.begin(), compare_track_vector.end(), sortCompareTrack);
        for (int i = 0; i < compare_track_vector.size(); i++)
        {
            // (*myfile) << "track point                                 :" << (compare_track_vector)[i].x_point << " " << (compare_track_vector)[i].y_point << "----------" << endl;
            // (*myfile) << "track_type                                  :" << compare_track_vector[i].type << "----------" << endl;
            // (*myfile) << "power_consum_and_pin_access_to_average      :" << compare_track_vector[i].power_consum_and_pin_access_to_average << " ----------" << endl;
        }
    }

    cout << "========== setTrackPinAccessCost end ==========" << endl;
}

// void setTrackPinAccessCost(ofstream *myfile, string layer, vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info)
// {
//     vector<float> total_pin_access_cost_vector;
//     vector<float> total_power_consuming_vector;
//     int track_size = 0;

//     cout << "========== setTrackPinAccessCost start ==========" << endl;

//     // step 1 : get pin_access_cost and power_consuming_cost of each track
//     for (int i = 0; i < (*stripe_vector).size(); i++)
//     {
//         cout << "========== " << (*stripe_vector)[i].layer << " "
//              << "stripe" << i << " start ========== " << endl;
//         for (int j = 0; j < (*stripe_vector)[i].track_point_vector.size(); j++)
//         {

//             TrackPoint track_point = (*stripe_vector)[i].track_point_vector[j];
//             float total_cell_pin_access_power_consume_cost = 0;
//             float power_stripe_width_float = stof((*stripe_vector)[i].width);
//             float track_of_total_pin_access_cost = 0;
//             float track_of_total_power_consuming = 0;
//             for (int k = 0; k < track_point.power_cell_id_vector.size(); k++)
//             {
//                 string cell_id = track_point.power_cell_id_vector[k];
//                 CellPlacedInfo cell_placed_info = (*cell_placed_map)[cell_id];
//                 // cost function
//                 float cell_pin_access_cost = getTrackPinAccessPointCost(layer, power_stripe_width_float, &track_point, &cell_placed_info, &(*cell_ip_map), &(*cell_info_map), &(*track_info));
//                 float power_consuming_cost = ((*cell_ip_map)[cell_id].instance_total_power * 10000);
//                 // power_consuming_cost = 1 / power_consuming_cost;

//                 track_of_total_pin_access_cost += cell_pin_access_cost;
//                 track_of_total_power_consuming += power_consuming_cost;
//             }
//             (*stripe_vector)[i].track_point_vector[j].pin_access_cost = track_of_total_pin_access_cost;
//             (*stripe_vector)[i].track_point_vector[j].power_consuming_cost = track_of_total_power_consuming;
//             total_pin_access_cost_vector.push_back(track_of_total_pin_access_cost);
//             total_power_consuming_vector.push_back(track_of_total_power_consuming);
//         }
//         track_size += (*stripe_vector)[i].track_point_vector.size();
//         cout << "========== stripe " << i << " end ==========" << endl;
//     }
//     // step 2 : get standard deviation and average
//     float pin_access_cost_standard_deviation = standardDeviation(&total_pin_access_cost_vector);
//     float power_consuming_cost_standard_deviation = standardDeviation(&total_power_consuming_vector);
//     float pin_access_cost_arithmetic_mean = arithmeticMean(&total_pin_access_cost_vector);
//     float power_consuming_cost_arithmetic_mean = arithmeticMean(&total_power_consuming_vector);

//     // step 3 : z-score normalization
//     for (int i = 0; i < (*stripe_vector).size(); i++)
//     {
//         (*myfile) << "======================== stripe " << i << " end ========================" << endl;
//         float total_pin_access_cost = 0;
//         float total_power_consuming_cost = 0;
//         vector<CompareTrack> compare_track_vector;

//         for (int j = 0; j < (*stripe_vector)[i].track_point_vector.size(); j++)
//         {
//             TrackPoint track_point = (*stripe_vector)[i].track_point_vector[j];
//             // (*myfile) << "---------- track point " << track_point.x_point << " " << track_point.y_point << " end ----------" << endl;
//             float original_power_comsing_cost = track_point.power_consuming_cost;
//             float original_pin_access_cost = track_point.pin_access_cost;
//             // (*myfile) << "power_consuming         :" << original_power_comsing_cost << " ----------" << endl;
//             // (*myfile) << "pin_access_cost         :" << original_pin_access_cost << " ----------" << endl;
//             float z_score_power_cost = z_score_normalization(original_power_comsing_cost, power_consuming_cost_arithmetic_mean, power_consuming_cost_standard_deviation);
//             float z_score_pin_access_cost = z_score_normalization(original_pin_access_cost, pin_access_cost_arithmetic_mean, pin_access_cost_standard_deviation);
//             // (*myfile) << "z_score_power_cost      :" << (1 / z_score_power_cost) << " ----------" << endl;
//             // (*myfile) << "z_score_pin_access_cost :" << z_score_pin_access_cost << " ----------" << endl;
//             (*stripe_vector)[i].track_point_vector[j].power_consuming_cost = z_score_power_cost;
//             (*stripe_vector)[i].track_point_vector[j].pin_access_cost = z_score_pin_access_cost;
//             // ======================== log for stripe cost ====================================
//             CompareTrack compare_track;
//             compare_track.pin_access_cost = z_score_pin_access_cost;
//             compare_track.power_cost = (1 / z_score_power_cost);
//             compare_track.x_point = track_point.x_point;
//             compare_track.y_point = track_point.y_point;
//             compare_track_vector.push_back(compare_track);

//             total_pin_access_cost += z_score_pin_access_cost;
//             total_power_consuming_cost += (1 / z_score_power_cost);
//             // ======================== log for stripe cost ====================================
//             //========================== TODO change cost ===================================
//             // (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = (1 / z_score_power_cost) * 0.2 + z_score_pin_access_cost;
//             // (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = z_score_power_cost;
//             (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost = z_score_pin_access_cost;
//             //========================== TODO change cost ===================================
//             // (*myfile) << "total_cost            :" << (*stripe_vector)[i].track_point_vector[j].total_pin_access_power_consum_cost << " ----------" << endl;
//         }
//         // 範圍內 z-cost pin access cost +  z-cost power cost最大
//         float average_z_score_pin_access_cost = total_pin_access_cost / ((*stripe_vector)[i].track_point_vector.size());
//         float average_z_score_power_cost = total_power_consuming_cost / ((*stripe_vector)[i].track_point_vector.size());
//         (*myfile) << "average z_score_pin_access_cost                 :" << (total_pin_access_cost / ((*stripe_vector)[i].track_point_vector.size())) << " ----------" << endl;
//         (*myfile) << "average_z_score_power_cost                      :" << (total_power_consuming_cost / ((*stripe_vector)[i].track_point_vector.size())) << " ----------" << endl;

//         for (int i = 0; i < compare_track_vector.size(); i++)
//         {
//             getCompareTrackType(&compare_track_vector[i], average_z_score_pin_access_cost, average_z_score_power_cost);
//             float temp_pin_access = abs(average_z_score_pin_access_cost - (compare_track_vector)[i].pin_access_cost);
//             float temp_power = abs(average_z_score_power_cost - (compare_track_vector)[i].power_cost);
//             compare_track_vector[i].power_consum_and_pin_access_to_average = temp_pin_access + temp_power;
//         }
//         sort(compare_track_vector.begin(), compare_track_vector.end(), sortCompareTrack);
//         for (int i = 0; i < compare_track_vector.size(); i++)
//         {
//             (*myfile) << "track point                                 :" << (compare_track_vector)[i].x_point << " " << (compare_track_vector)[i].y_point << "----------" << endl;
//             (*myfile) << "track_type                                  :" << compare_track_vector[i].type << "----------" << endl;
//             (*myfile) << "power_consum_and_pin_access_to_average      :" << compare_track_vector[i].power_consum_and_pin_access_to_average << " ----------" << endl;
//         }
//     }

//     cout << "========== setTrackPinAccessCost end ==========" << endl;
// }
bool sortCompareTrack(CompareTrack compare_track_a, CompareTrack compare_track_b)
{
    return compare_track_a.power_consum_and_pin_access_to_average > compare_track_b.power_consum_and_pin_access_to_average;
}
void getCompareTrackType(CompareTrack *compare_track, float average_z_score_pin_access_cost, float average_z_score_power_cost)
{
    if ((*compare_track).pin_access_cost >= average_z_score_pin_access_cost && (*compare_track).power_cost >= average_z_score_power_cost)
    {
        (*compare_track).type = positive_positive;
    }
    else if ((*compare_track).pin_access_cost <= average_z_score_pin_access_cost && (*compare_track).power_cost <= average_z_score_power_cost)
    {
        (*compare_track).type = negative_negative;
    }
    else if ((*compare_track).pin_access_cost >= average_z_score_pin_access_cost && (*compare_track).power_cost <= average_z_score_power_cost)
    {
        (*compare_track).type = positive_negative;
    }
    else if ((*compare_track).pin_access_cost <= average_z_score_pin_access_cost && (*compare_track).power_cost >= average_z_score_power_cost)
    {
        (*compare_track).type = negative_positive;
    }
    else
    {
        cout << (*compare_track).pin_access_cost << endl;
        cout << (*compare_track).power_cost << endl;
        cout << average_z_score_pin_access_cost << endl;
        cout << average_z_score_power_cost << endl;
        cout << "=================== getCompareTrackType error ==============" << endl;
    }

    // const string positive_positive = "positive_positive";
    // const string positive_negative = "positive_negative";
    // const string negative_positive = "negative_positive";
    // const string negative_negative = "negative_negative";
}

float z_score_normalization(float original, float arithmetic_mean, float standard_deviation)
{
    float z_score = (original - arithmetic_mean) / standard_deviation;
    return z_score;
}

float arithmeticMean(vector<float> *num_vec)
{

    float total = 0;
    for (int i = 0; i < (*num_vec).size(); i++)
    {
        total += (*num_vec)[i];
    }

    float average = total / (*num_vec).size();
    return average;
}

//
float standardDeviation(vector<float> *num_vec)
{
    float sum = 0;
    float std1 = 0;
    float mean = 0;
    for (int i = 0; i < (*num_vec).size(); i++)
    {
        sum += (*num_vec)[i];
    };
    mean = sum / (*num_vec).size();
    for (int i = 0; i < (*num_vec).size(); i++)
    {
        std1 += ((*num_vec)[i] - mean) * ((*num_vec)[i] - mean);
    }
    std1 /= (*num_vec).size();
    std1 = sqrt(std1);
    return std1;
}
string transferTrackLocationType(string layer, TrackPoint *track_point, CellPlacedInfo *cell_placed_info)
{

    if (isOddLayer(layer))
    {
        float track_x_point = stof((*track_point).x_point);

        float left_x_location = stof((*cell_placed_info).left_x_location);
        float temp = track_x_point - left_x_location;

        string track_location = floatToString(temp);
        // if ((*cell_placed_info).cell_id == "FE_OFC6745_wb_databus_instruction_fifo_n43963")
        // {
        //     cout << "track_x_point : " << track_x_point << endl;
        //     cout << "left_x_location : " << left_x_location << endl;
        //     cout << "temp : " << temp << endl;
        //     cout << "track_location : " << track_location << endl;
        // }

        return track_location;
    }
    else
    {
        float track_y_point = stof((*track_point).y_point);
        float down_x_locatiion = stof((*cell_placed_info).down_y_location);
        float temp = track_y_point - down_x_locatiion;
        string track_location = floatToString(temp);
        return track_location;
    }
}

float getTrackPinAccessPointCost(string layer, string power_stripe_width, TrackPoint *track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info)
{

    // float m3_track_point_middle_float = stof((*track_point).x_point);
    string cell_placed_left_x_location = (*cell_placed_info).left_x_location;
    string cell_name = (*cell_placed_info).cell_name;
    float total_pin_access_cost = 0;

    unordered_map<string, unordered_map<string, PinAccessCost>> odd_pin_access_cost_map = (*cell_info_map)[cell_name].odd_pin_access_cost_map;
    unordered_map<string, unordered_map<string, PinAccessCost>> even_pin_access_cost_map = (*cell_info_map)[cell_name].even_pin_access_cost_map;
    string track_location = transferTrackLocationType(layer, &(*track_point), &(*cell_placed_info));
    // if ("U66546" == (*cell_placed_info).cell_id)
    // {
    //     cout << "track location " << track_location << endl;
    //     cout << "cell_placed_left_x_location : " << cell_placed_left_x_location << endl;
    //     cout << isOddTrackCell(cell_placed_left_x_location, &(*track_info)) << endl;
    // }

    if (isOddTrackCell(cell_placed_left_x_location, &(*track_info)))
    {
        //     // key : stripe_width key2 : location , value : cost
        //     unordered_map<string, unordered_map<string, PinAccessCost>> odd_pin_access_cost_map;
        string direction = (*cell_placed_info).direction;
        unordered_map<string, string> vertical_direction_cost_map = odd_pin_access_cost_map[power_stripe_width][track_location].vertical_direction_cost_map;
        if (!(odd_pin_access_cost_map[power_stripe_width].find(track_location) != odd_pin_access_cost_map[power_stripe_width].end()))
        {
            cout << "odd_pin_access_cost_map key error!" << endl;
        }
        string cost = vertical_direction_cost_map[direction];
        total_pin_access_cost = stof(cost);
    }
    else
    {
        string direction = (*cell_placed_info).direction;
        unordered_map<string, string> vertical_direction_cost_map = even_pin_access_cost_map[power_stripe_width][track_location].vertical_direction_cost_map;
        if (!(even_pin_access_cost_map[power_stripe_width].find(track_location) != even_pin_access_cost_map[power_stripe_width].end()))
        {
            cout << "even_pin_access_cost_map key error!" << endl;
        }
        string cost = vertical_direction_cost_map[direction];
        total_pin_access_cost = stof(cost);
    }
    return total_pin_access_cost;
}
// struct CellInfo
// {
//     string cell_name;
//     string height;
//     string width;
//     // key:PinName value:PinShape
//     map<string, PinShape> pin_shape_map;
//     // key:PinName value:Pin Access Point
//     map<string, vector<PinAccessPoint>> odd_pin_access_point_map;
//     // key:PinName value:Pin Access Point
//     map<string, vector<PinAccessPoint>> even_pin_access_point_map;

//     // key : stripe_width key2 : location , value : cost
//     unordered_map<string, unordered_map<string, PinAccessCost>> odd_pin_access_cost_map;
//     unordered_map<string, unordered_map<string, PinAccessCost>> even_pin_access_cost_map;
// };

// float getTrackPinAccessPointCost(string layer, float power_stripe_width_float, TrackPoint *track_point, CellPlacedInfo *cell_placed_info, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellInfo> *cell_info_map, TrackInfo *track_info)
// {
//     // float m3_track_point_middle_float = stof((*track_point).x_point);

//     string cell_placed_left_x_location = (*cell_placed_info).left_x_location;
//     string cell_placed_right_x_location = (*cell_placed_info).right_x_location;

//     string cell_name = (*cell_placed_info).cell_name;
//     string direction = (*cell_placed_info).direction;
//     string cell_width = (*cell_info_map)[cell_name].width;
//     string cell_height = (*cell_info_map)[cell_name].height;
//     float total_pin_access_cost = 0;

//     if (isOddTrackCell(cell_placed_left_x_location, &(*track_info)))
//     {

//         for (auto odd_pin_access_point_iter = ((*cell_info_map)[cell_name].odd_pin_access_point_map).begin(); odd_pin_access_point_iter != ((*cell_info_map)[cell_name].odd_pin_access_point_map).end(); ++odd_pin_access_point_iter)
//         {
//             int pin_access_point = 0;
//             for (int i = 0; i < (odd_pin_access_point_iter->second).size(); i++)
//             {
//                 PinAccessPoint pin_access_point_def;
//                 transferPinAccessLocationFromDef(&(*cell_placed_info), &(odd_pin_access_point_iter->second)[i], &pin_access_point_def, cell_width, cell_height);
//                 //  cout << (cell_placed_info.cell_id.find("U15399") != string::npos) << endl;
//                 // if ((*cell_placed_info).cell_id.find("U15399") != string::npos)
//                 // {
//                 //     cout << "(" << pin_access_point_def.middle_x_location << " " << pin_access_point_def.middle_y_location << ")" << endl;
//                 // }

//                 // float pin_access_point_x_def_float = stof(pin_access_point_def.middle_x_location);
//                 if (isCoverPinAccess(layer, &pin_access_point_def, &(*track_point), power_stripe_width_float))
//                 {
//                     pin_access_point += 1;
//                 }
//             }
//             float pin_access_cost = (float)pin_access_point / (odd_pin_access_point_iter->second).size();
//             // cout << "pin name : " << odd_pin_access_point_iter->first << endl;
//             // cout << "pin_access_point : " << pin_access_point << " " << (odd_pin_access_point_iter->second).size() << endl;
//             // cout << "pin cost : " << pin_access_cost << endl;
//             total_pin_access_cost += pin_access_cost;
//         }
//     }
//     else
//     {
//         for (auto even_pin_access_point_iter = ((*cell_info_map)[cell_name].even_pin_access_point_map).begin(); even_pin_access_point_iter != ((*cell_info_map)[cell_name].even_pin_access_point_map).end(); ++even_pin_access_point_iter)
//         {
//             int pin_access_point = 0;
//             for (int i = 0; i < (even_pin_access_point_iter->second).size(); i++)
//             {
//                 PinAccessPoint pin_access_point_def;
//                 transferPinAccessLocationFromDef(&(*cell_placed_info), &(even_pin_access_point_iter->second)[i], &pin_access_point_def, cell_width, cell_height);
//                 // float pin_access_point_x_def_float = stof(pin_access_point_def.middle_x_location);

//                 // if ((*cell_placed_info).cell_id.find("U15399") != string::npos)
//                 // {
//                 //     cout << "(" << pin_access_point_def.middle_x_location << " " << pin_access_point_def.middle_y_location << ")" << endl;
//                 // }
//                 if (isCoverPinAccess(layer, &pin_access_point_def, &(*track_point), power_stripe_width_float))
//                 {
//                     pin_access_point += 1;
//                 }
//             }
//             float pin_access_cost = (float)pin_access_point / (even_pin_access_point_iter->second).size();
//             total_pin_access_cost += pin_access_cost;
//             // cout << "pin name : " << odd_pin_access_point_iter->first << endl;
//             // cout << "pin_access_point : " << pin_access_point << " " << (odd_pin_access_point_iter->second).size() << endl;
//             // cout << "pin cost : " << pin_access_cost << endl;
//         }
//     }
//     if (total_pin_access_cost == 0)
//     {
//         total_pin_access_cost = 1;
//     }
//     return total_pin_access_cost;
// }
bool isCoverPinAccess(string layer, PinAccessPoint *pin_access_point_def, TrackPoint *track_point, float power_stripe_width)
{
    int cover_track = getCoverTrack(power_stripe_width);
    float cover_distance = (cover_track * 0.144) + 0.036;
    if (isOddLayer(layer))
    {
        float track_point_x_left = stof((*track_point).x_point) - cover_distance;
        float track_point_x_right = stof((*track_point).x_point) + cover_distance;

        float pin_access_x_left = stof((*pin_access_point_def).middle_x_location) - (0.072 / 2);
        float pin_access_x_right = stof((*pin_access_point_def).middle_x_location) + (0.072 / 2);

        if (track_point_x_left < pin_access_x_left && track_point_x_right < pin_access_x_left)
        {
            return false;
        }
        else if (track_point_x_left > pin_access_x_left && track_point_x_right > pin_access_x_right)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        float track_point_y_down_float = stof((*track_point).y_point) - cover_distance;
        float track_point_y_up_float = stof((*track_point).y_point) + cover_distance;

        float pin_access_y_down_float = stof((*pin_access_point_def).middle_y_location) - (0.072 / 2);
        float pin_access_y_up_float = stof((*pin_access_point_def).middle_y_location) + (0.072 / 2);

        if (track_point_y_down_float < pin_access_y_down_float && track_point_y_up_float < pin_access_y_down_float)
        {
            return false;
        }
        else if (track_point_y_down_float > pin_access_y_down_float && track_point_y_down_float > pin_access_y_up_float)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
};

int getCoverTrack(float power_stripe_width)
{
    float half_power_stripe_width = power_stripe_width / 2;
    int temp_pitch = (int)(half_power_stripe_width / 0.144);
    int track = 0;
    float temp_width = temp_pitch * 0.144;
    temp_width = half_power_stripe_width - temp_width;
    if (floatIsEqualOrLess(temp_width, 0.036))
    {
        track = temp_pitch;
    }
    else
    {
        track = temp_pitch + 1;
    }
    return track;
}
void transferPinAccessLocationFromDef(CellPlacedInfo *cell_placed_info, PinAccessPoint *pin_access_point, PinAccessPoint *pin_access_point_def, string cell_width, string cell_height)
{

    float cell_place_left_x = stof((*cell_placed_info).left_x_location);
    float cell_place_down_y = stof((*cell_placed_info).down_y_location);

    float cell_width_float = stof(cell_width);
    float cell_height_float = stof(cell_height);

    float pin_access_x_point = stof((*pin_access_point).middle_x_location);
    float pin_access_y_point = stof((*pin_access_point).middle_y_location);

    if ((*cell_placed_info).direction == "N")
    {
        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + pin_access_x_point);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + pin_access_y_point);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "S")
    {

        float temp_middle_x_float = abs(cell_width_float - pin_access_x_point);
        float temp_middle_y_float = abs(cell_height_float - pin_access_y_point);

        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + temp_middle_x_float);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + temp_middle_y_float);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FN")
    {

        float temp_middle_x_float = abs(cell_width_float - pin_access_x_point);

        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + temp_middle_x_float);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + pin_access_y_point);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else if ((*cell_placed_info).direction == "FS")
    {

        float temp_middle_y_float = abs(cell_height_float - pin_access_y_point);

        (*pin_access_point_def).middle_x_location = floatToString(cell_place_left_x + pin_access_x_point);
        (*pin_access_point_def).middle_y_location = floatToString(cell_place_down_y + temp_middle_y_float);

        // cout << "pin : (" << (*pin_access_point_def).middle_x_location << " " << (*pin_access_point_def).middle_y_location << ")" << endl;
    }
    else
    {
        cout << " getPinAccessLocationFromDef error " << endl;
    }
};

bool isOddTrackCell(string x_location, TrackInfo *m3_track_info)
{

    float m3_x_float_temp = stof(x_location) - stof((*m3_track_info).start);
    float temp_pitch_start = m3_x_float_temp / (stof((*m3_track_info).pitch));
    int temp_pitch_start_int = (int)temp_pitch_start;
    temp_pitch_start_int += 1;
    float start_x = (temp_pitch_start_int * (stof((*m3_track_info).pitch))) + stof((*m3_track_info).start);
    float distance = start_x - stof(x_location);
    // if (x_location == "36")
    // {
    //     cout << " stof(x_location)     : " << stof(x_location) << endl;
    //     cout << "(*m3_track_info).start: " << stof((*m3_track_info).start) << endl;
    //     cout << "stof((*m3_track_info).pitch)*2: " << (stof((*m3_track_info).pitch) * 2) << endl;
    //     cout << "m3_x_float_temp      : " << m3_x_float_temp << endl;
    //     cout << "temp_pitch_start     : " << temp_pitch_start << endl;
    //     cout << "temp_pitch_start_int : " << temp_pitch_start_int << endl;
    //     cout << "start_x              : " << start_x << endl;
    //     cout << "distance             : " << distance << endl;
    // }

    if (floatIsEqual(distance, 0.108))
    {
        return false;
    }
    else
    {
        return true;
    }
    return true;
}

void setRoutingTrackPowerConsuming(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, TrackInfo> *track_info_map)
{

    for (auto stripe_map_it = (*stripe_map).begin(); stripe_map_it != (*stripe_map).end(); ++stripe_map_it)
    {
        string layer = stripe_map_it->first;
        if (isOddLayer(layer))
        {
            setOddTrackPointInStripe(&(*stripe_map)[layer], &(*cell_placed_map), &(*track_info_map)[layer]);
        }
        else
        {
            setEvenTrackPointInStripe(&(*stripe_map)[layer], &(*cell_placed_map), &(*track_info_map)[layer]);
        };
    }
};

void setEvenTrackPointInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *track_info)
{

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        // cout << "start_move_range_y_down : " << (*stripe_vector)[i].move_range_y_down << endl;
        // cout << "start_move_range_y_up : " << (*stripe_vector)[i].move_range_y_up << endl;
        float power_stripe_width_float = stof((*stripe_vector)[i].width);
        float start_move_range_y_down = (stof((*stripe_vector)[i].move_range_y_down) * 1000) / 4;
        float start_move_range_y_up = (stof((*stripe_vector)[i].move_range_y_up) * 1000) / 4;
        // cout << "start : " << (*track_info).start << endl;
        // cout << "pitch : " << (*track_info).pitch << endl;

        float y_start_float = (stof((*track_info).start) * 1000) / 4;
        float y_pitch_float = (stof((*track_info).pitch) * 1000) / 4;
        // cout << "y_start_float : " << y_start_float << endl;
        // cout << "y_pitch_float : " << y_pitch_float << endl;

        int y_start_int = (int)y_start_float;
        int y_pitch_int = (int)y_pitch_float;
        // cout << "y_start_int : " << y_start_int << endl;
        // cout << "y_pitch_int : " << y_pitch_int << endl;
        cout << " ----- moving range start -----" << i << endl;
        cout << (*stripe_vector)[i].move_range_y_down << " " << (*stripe_vector)[i].move_range_y_up << endl;
        vector<TrackPoint> track_point_vector;
        vector<string> ip_power_vector = (*stripe_vector)[i].ip_power_vector;
        for (int j = start_move_range_y_down; j <= start_move_range_y_up; j += y_pitch_int)
        {
            float y_track_point = (j * 4) / 1000.0;
            vector<string> power_cell_id_vector;
            TrackPoint track_point;
            track_point.y_point = floatToString(y_track_point);
            // cout << "track point " << track_point.y_point << endl;
            track_point.power_cell_id_vector = power_cell_id_vector;
            for (int k = 0; k < ip_power_vector.size(); k++)
            {
                string cell_id = ip_power_vector[k];
                if (isInEvenTrackStripeRange(&track_point, &(*cell_placed_map)[cell_id], power_stripe_width_float))
                {
                    track_point.power_cell_id_vector.push_back(cell_id);
                }
            }
            track_point_vector.push_back(track_point);
        };

        cout << " ----- moving range end   -----" << endl;
        (*stripe_vector)[i].track_point_vector = track_point_vector;
    }
}

void setOddTrackPointInStripe(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *track_info)
{

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        float power_stripe_width_float = stof((*stripe_vector)[i].width);
        // int start_move_range_x_left = (stof((*stripe_vector)[i].move_range_x_left) * 1000) / 4;
        // int start_move_range_x_right = (stof((*stripe_vector)[i].move_range_x_right) * 1000) / 4;
        // if ((*stripe_vector)[i].move_range_x_left == "523.764")
        // {
        //     float start_move_range_x_left_float = stof((*stripe_vector)[i].move_range_x_left);
        //     float temp = start_move_range_x_left_float * 1000.0;
        //     temp = round(temp);
        //     int temp_int = (int)temp;
        //     int ans = temp_int / 4;
        //     cout << "ans : " << ans << endl;
        // }

        int start_move_range_x_left = tranferLocationFloatToInt(stof((*stripe_vector)[i].move_range_x_left));

        int start_move_range_x_right = tranferLocationFloatToInt(stof((*stripe_vector)[i].move_range_x_right));
        // float x_start_float = (stof((*track_info).start) * 1000) / 4;
        // float x_pitch_float = (stof((*track_info).pitch) * 1000) / 4;
        // int x_start_int = (int)x_start_float;
        // int x_pitch_int = (int)x_pitch_float;
        int x_start_int = tranferLocationFloatToInt(stof((*track_info).start));
        int x_pitch_int = tranferLocationFloatToInt(stof((*track_info).pitch));

        cout << " ----- moving range start -----" << endl;

        cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << endl;
        vector<TrackPoint> track_point_vector;
        vector<string> ip_power_vector = (*stripe_vector)[i].ip_power_vector;

        // if ((*stripe_vector)[i].move_range_x_left == "523.764")
        // {
        //     cout << "check in side" << endl;
        //     cout << "move_range_x_left   : " << (*stripe_vector)[i].move_range_x_left << endl;
        //     cout << "move_range_x_right  :" << (*stripe_vector)[i].move_range_x_right << endl;
        //     cout << "(*track_info).start : " << (*track_info).start << endl;
        //     cout << "(*track_info).pitch :" << (*track_info).pitch << endl;
        //     cout << "start_move_range_x_left : " << start_move_range_x_left << endl;
        //     cout << "start_move_range_x_right : " << start_move_range_x_right << endl;
        //     cout << "x_pitch_int : " << x_pitch_int << endl;
        // }
        for (int j = start_move_range_x_left; j <= start_move_range_x_right; j += x_pitch_int)
        {
            float x_track_point = (j * 4) / 1000.0;
            // if (i == 44)
            // {
            //     cout << "before j " << j << endl;
            // }
            // float x_track_point = convertInnovusPoint(j);
            // if (i == 44)
            // {
            //     cout << "after j " << j << endl;
            // }

            vector<string> power_cell_id_vector;
            TrackPoint track_point;
            track_point.x_point = floatToString(x_track_point);
            // cout << "track point :" << track_point.x_point << endl;
            track_point.power_cell_id_vector = power_cell_id_vector;

            for (int k = 0; k < ip_power_vector.size(); k++)
            {
                string cell_id = ip_power_vector[k];
                if (isInOddTrackStripeRange(&track_point, &(*cell_placed_map)[cell_id], power_stripe_width_float))
                {
                    track_point.power_cell_id_vector.push_back(cell_id);
                }
            }
            track_point_vector.push_back(track_point);
        }
        cout << " ----- moving range end -----" << endl;
        (*stripe_vector)[i].track_point_vector = track_point_vector;
    }
};

// �@��routing track �q�L�h��cell
bool isInEvenTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width)
{

    float track_point_down = stof((*track_point).y_point);
    float track_point_up = stof((*track_point).y_point);
    float cell_placed_down = stof((*cell_placed_info).down_y_location);
    float cell_placed_up = stof((*cell_placed_info).up_y_location);

    // cell in power stripe middle
    if (cell_placed_down >= track_point_down && cell_placed_up <= track_point_up)
    {
        return true;
    }
    // cell across power stripe up
    else if (cell_placed_down >= track_point_down && cell_placed_down <= track_point_up && cell_placed_up >= track_point_up)
    {
        return true;
    }
    // cell across power stripe down
    else if (cell_placed_down <= track_point_down && cell_placed_up >= track_point_down && cell_placed_up <= track_point_up)
    {
        return true;

    } // cell across all power stripe
    else if (cell_placed_down <= track_point_down && cell_placed_up >= track_point_up)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// bool isInEvenTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width)
// {

//     float track_point_down = stof((*track_point).y_point) - (power_stripe_width / 2);
//     float track_point_up = stof((*track_point).y_point) + (power_stripe_width / 2);
//     float cell_placed_down = stof((*cell_placed_info).down_y_location);
//     float cell_placed_up = stof((*cell_placed_info).up_y_location);

//     // cell in power stripe middle
//     if (cell_placed_down >= track_point_down && cell_placed_up <= track_point_up)
//     {
//         return true;
//     }
//     // cell across power stripe up
//     else if (cell_placed_down >= track_point_down && cell_placed_down <= track_point_up && cell_placed_up >= track_point_up)
//     {
//         return true;
//     }
//     // cell across power stripe down
//     else if (cell_placed_down <= track_point_down && cell_placed_up >= track_point_down && cell_placed_up <= track_point_up)
//     {
//         return true;

//     } // cell across all power stripe
//     else if (cell_placed_down <= track_point_down && cell_placed_up >= track_point_up)
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

// �@��routing track �q�L�h��cell �u�ѳo�̦����D
// bool isInOddTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width)
// {

//     float track_point_left = stof((*track_point).x_point) - (power_stripe_width / 2);
//     float track_point_right = stof((*track_point).x_point) + (power_stripe_width / 2);
//     float cell_placed_left = stof((*cell_placed_info).left_x_location);
//     float cell_placed_right = stof((*cell_placed_info).right_x_location);
//     // cout << " track_point " << track_point_left << " " << track_point_right << endl;
//     // cout << " cell_placed " << cell_placed_left << " " << cell_placed_right << endl;

//     // cell in power stripe middle
//     if (cell_placed_left >= track_point_left && cell_placed_right <= track_point_right)
//     {
//         return true;
//     }
//     // cell across power stripe left
//     else if (cell_placed_left <= track_point_left && cell_placed_right >= track_point_left && cell_placed_right <= track_point_right)
//     {
//         return true;
//     }
//     // cell across power stripe right
//     else if (cell_placed_left >= track_point_left && cell_placed_left <= track_point_right && cell_placed_right >= track_point_right)
//     {
//         return true;
//     }
//     // cell across all power stripe
//     else if (cell_placed_left <= track_point_left && cell_placed_right >= track_point_right)
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }

//     cout << "========== isInTrackStripeRange end ==========" << endl;
// }
bool isInOddTrackStripeRange(TrackPoint *track_point, CellPlacedInfo *cell_placed_info, float power_stripe_width)
{

    float track_point_left = stof((*track_point).x_point);
    float track_point_right = stof((*track_point).x_point);
    float cell_placed_left = stof((*cell_placed_info).left_x_location);
    float cell_placed_right = stof((*cell_placed_info).right_x_location);
    // cout << " track_point " << track_point_left << " " << track_point_right << endl;
    // cout << " cell_placed " << cell_placed_left << " " << cell_placed_right << endl;

    // cell in power stripe middle
    if (cell_placed_left >= track_point_left && cell_placed_right <= track_point_right)
    {
        return true;
    }
    // cell across power stripe left
    else if (cell_placed_left <= track_point_left && cell_placed_right >= track_point_left && cell_placed_right <= track_point_right)
    {
        return true;
    }
    // cell across power stripe right
    else if (cell_placed_left >= track_point_left && cell_placed_left <= track_point_right && cell_placed_right >= track_point_right)
    {
        return true;
    }
    // cell across all power stripe
    else if (cell_placed_left <= track_point_left && cell_placed_right >= track_point_right)
    {
        return true;
    }
    else
    {
        return false;
    }

    cout << "========== isInTrackStripeRange end ==========" << endl;
}

void setCellStripeRange(unordered_map<string, vector<Stripe>> *stripe_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    for (auto stripe_map_it = (*stripe_map).begin(); stripe_map_it != (*stripe_map).end(); ++stripe_map_it)
    {
        string layer = stripe_map_it->first;
        if (isOddLayer(layer))
        {
            cout << "========== start setIpPowerInStripe isOddLayer ==========" << endl;
            for (auto cell_ip_it = (*cell_ip_map).begin(); cell_ip_it != (*cell_ip_map).end(); ++cell_ip_it)
            {
                string cell_id = cell_ip_it->first;
                CellInstancePowerInfo cell_instance_power_info = cell_ip_it->second;

                for (int i = 0; i < (*stripe_map)[layer].size(); i++)
                {
                    vector<string> ip_power_vector;
                    if (isInOddStripeRange(&(*stripe_map)[layer][i], cell_id, &(*cell_placed_map), &(*cell_ip_map)))
                    {
                        (*stripe_map)[layer][i].ip_power_vector.push_back(cell_id);
                    }
                }
            }
            cout << "========== end setIpPowerInStripe isOddLayer ==========" << endl;
        }
        else
        {
            cout << "========== start setIpPowerInStripe isEvenLayer ==========" << endl;
            for (auto cell_ip_it = (*cell_ip_map).begin(); cell_ip_it != (*cell_ip_map).end(); ++cell_ip_it)
            {
                string cell_id = cell_ip_it->first;
                CellInstancePowerInfo cell_instance_power_info = cell_ip_it->second;
                for (int i = 0; i < (*stripe_map)[layer].size(); i++)
                {
                    if (isInEvenStripeRange(&(*stripe_map)[layer][i], cell_id, &(*cell_placed_map), &(*cell_ip_map)))
                    {
                        // cout << "stripe :" << (*stripe_map)[layer][i].move_range_x_left << " " << (*stripe_map)[layer][i].move_range_x_right << endl;
                        // cout << "cell id : " << cell_id << endl;
                        (*stripe_map)[layer][i].ip_power_vector.push_back(cell_id);
                    }
                }
            }
            cout << "========== end setIpPowerInStripe isEvenLayer ==========" << endl;
        }
    }
}
bool isInEvenStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{

    string up_y_location = (*cell_placed_map)[cell_id].up_y_location;
    string down_y_location = (*cell_placed_map)[cell_id].down_y_location;

    float move_range_y_down_float = stof((*stripe_vector).move_range_y_down);
    float move_range_y_up_float = stof((*stripe_vector).move_range_y_up);

    float up_y_location_float = stof(up_y_location);
    float down_y_location_float = stof(down_y_location);

    // case 1 : middle of down stripe
    if ((up_y_location_float > move_range_y_down_float) && (down_y_location_float < move_range_y_down_float) && (up_y_location_float < move_range_y_up_float))
    {
        return true;
    } // case 2 : middle of up stripe
    else if ((up_y_location_float > move_range_y_up_float) && (down_y_location_float < move_range_y_up_float) && (down_y_location_float > move_range_y_down_float))
    {
        return true;
    } // case 3 : over all of stripe range
    else if ((up_y_location_float > move_range_y_up_float) && (down_y_location_float < move_range_y_down_float))
    {

        return true;
    } // case 4 : in power srtripe range
    else if (floatIsEqualOrLess(up_y_location_float, move_range_y_up_float) && floatIsEqualOrMore(up_y_location_float, move_range_y_down_float) && floatIsEqualOrLess(down_y_location_float, move_range_y_up_float) && floatIsEqualOrMore(down_y_location_float, move_range_y_down_float))
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool isInOddStripeRange(Stripe *stripe_vector, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{
    string left_x_location = (*cell_placed_map)[cell_id].left_x_location;
    string right_x_location = (*cell_placed_map)[cell_id].right_x_location;

    float move_range_x_left_float = stof((*stripe_vector).move_range_x_left);
    float move_range_x_right_float = stof((*stripe_vector).move_range_x_right);

    float left_x_location_float = stof(left_x_location);
    float right_x_location_float = stof(right_x_location);

    // case 1 : middle of left stripe
    if ((left_x_location_float < move_range_x_left_float) && (right_x_location_float > move_range_x_left_float) && (left_x_location_float < move_range_x_right_float) && (right_x_location_float < move_range_x_right_float))
    {

        // cout << " middle of left stripe " << endl;
        return true;
    } // case 2 : in stripe moving location
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
    {

        // cout << " in stripe moving location " << endl;
        return true;
    } // case 3 : middle of right stripe
    else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
    {

        // cout << " middle of right stripe " << endl;
        return true;
    } // case4 : over stripe moving location
    else if (left_x_location_float < move_range_x_left_float && left_x_location_float < move_range_x_right_float && right_x_location_float > move_range_x_right_float && right_x_location_float > move_range_x_left_float)
    {
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

void transferMovingRangeToTrack(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, unordered_map<string, TrackInfo> *track_info_map)
{
    cout << "========== transferMovingRangeToTrack start ==========" << endl;
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        TrackInfo track_info = (*track_info_map)[layer];
        if (isOddLayer(layer))
        {
            setOddTrackRange(&(*vdd_stripe_map)[layer], &(*track_info_map)[layer]);
        }
        else
        {
            setEvenTrackRange(&(*vdd_stripe_map)[layer], &(*track_info_map)[layer]);
        }
    }

    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        TrackInfo track_info = (*track_info_map)[layer];
        if (isOddLayer(layer))
        {
            setOddTrackRange(&(*vss_stripe_map)[layer], &(*track_info_map)[layer]);
        }
        else
        {
            setEvenTrackRange(&(*vss_stripe_map)[layer], &(*track_info_map)[layer]);
        }
    }
    cout << "========== transferMovingRangeToTrack end ==========" << endl;
}
void setOddTrackRange(vector<Stripe> *stripe_vector, TrackInfo *track_info)
{
    float track_start = stof((*track_info).start);
    // float track_pitch = stof((*track_info).pitch) * 2;
    float track_pitch = stof((*track_info).pitch);

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        // cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << " " << endl;
        float move_range_x_left_float = stof((*stripe_vector)[i].move_range_x_left);
        float move_range_x_right_float = stof((*stripe_vector)[i].move_range_x_right);

        float move_range_left_x_float_temp = move_range_x_left_float - track_start;
        float temp_pitch_start = move_range_left_x_float_temp / track_pitch;
        int temp_pitch_start_int = (int)temp_pitch_start;
        float start_x = (temp_pitch_start_int * track_pitch) + track_start;

        float move_range_x_right_float_temp = move_range_x_right_float - track_start;
        float temp_pitch_end = move_range_x_right_float_temp / track_pitch;
        int temp_pitch_end_int = (int)temp_pitch_end;

        temp_pitch_end_int -= 1;
        float end_x = (temp_pitch_end_int * track_pitch) + track_start;

        (*stripe_vector)[i].move_range_x_left = floatToString(start_x);
        (*stripe_vector)[i].move_range_x_right = floatToString(end_x);
    }
}
void setEvenTrackRange(vector<Stripe> *stripe_vector, TrackInfo *track_info)
{
    float track_start = stof((*track_info).start);
    float track_pitch = stof((*track_info).pitch);

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        float move_range_y_down_float = stof((*stripe_vector)[i].move_range_y_down);
        float move_range_y_up_float = stof((*stripe_vector)[i].move_range_y_up);

        float move_range_y_down_float_temp = move_range_y_down_float - track_start;
        float temp_pitch_start = move_range_y_down_float_temp / track_pitch;
        int temp_pitch_start_int = (int)temp_pitch_start;
        float start_x = (temp_pitch_start_int * track_pitch) + track_start;

        float move_range_y_up_float_temp = move_range_y_up_float - track_start;
        float temp_pitch_end = move_range_y_up_float_temp / track_pitch;
        int temp_pitch_end_int = (int)temp_pitch_end;

        temp_pitch_end_int -= 1;
        float end_x = (temp_pitch_end_int * track_pitch) + track_start;
        (*stripe_vector)[i].move_range_x_left = floatToString(start_x);
        (*stripe_vector)[i].move_range_x_right = floatToString(end_x);
    }
}

void setStripeRange(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site)
{
    cout << "========== setStripeRange start ==========" << endl;
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        if (isOddLayer(layer))
        {
            setOddLayerRange(&((*vdd_stripe_map)[layer]), &(*core_site));
        }
        else
        {
            setEvenLayerRange(&((*vdd_stripe_map)[layer]), &(*core_site));
        };
    }
    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        if (isOddLayer(layer))
        {
            setOddLayerRange(&((*vss_stripe_map)[layer]), &(*core_site));
        }
        else
        {
            setEvenLayerRange(&((*vss_stripe_map)[layer]), &(*core_site));
        };
    }
    cout << "========== setStripeRange end ==========" << endl;
};
void setOddLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
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
            (*stripe_vector)[i].move_range_x_left = floatToString(middle_x_power_stripe);
        }
        else
        {
            float middle_x_power_stripe_right = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i + 1].start_x_location)) / 2;
            float middle_x_power_stripe_left = (stof((*stripe_vector)[i].start_x_location) + stof((*stripe_vector)[i - 1].start_x_location)) / 2;
            (*stripe_vector)[i].move_range_x_right = floatToString(middle_x_power_stripe_right);
            (*stripe_vector)[i].move_range_x_left = floatToString(middle_x_power_stripe_left);
        }
    }
}
void setEvenLayerRange(vector<Stripe> *stripe_vector, CoreSite *core_site)
{
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        if (i == 0)
        {
            (*stripe_vector)[i].move_range_y_down = (*core_site).down_y_location;
            float middle_y_power_stripe = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i + 1].start_y_location)) / 2;
            (*stripe_vector)[i].move_range_y_up = floatToString(middle_y_power_stripe);
        }
        else if (i == ((*stripe_vector).size() - 1))
        {
            (*stripe_vector)[i].move_range_y_up = (*core_site).up_y_location;
            float middle_y_power_stripe = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i - 1].start_y_location)) / 2;
            (*stripe_vector)[i].move_range_y_down = floatToString(middle_y_power_stripe);
        }
        else
        {
            float middle_y_power_stripe_up = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i + 1].start_y_location)) / 2;
            float middle_y_power_stripe_down = (stof((*stripe_vector)[i].start_y_location) + stof((*stripe_vector)[i - 1].start_y_location)) / 2;
            (*stripe_vector)[i].move_range_y_up = floatToString(middle_y_power_stripe_up);
            (*stripe_vector)[i].move_range_y_down = floatToString(middle_y_power_stripe_down);
        }
    }
}

void transferStripeToMap(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector)
{
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        if ((*vdd_stripe_map).count((*vdd_stripe_vector)[i].layer) == 1)
        {
            (*vdd_stripe_map)[(*vdd_stripe_vector)[i].layer].push_back((*vdd_stripe_vector)[i]);
        }
        else
        {
            vector<Stripe> layer_vdd_stripe_vector;
            layer_vdd_stripe_vector.push_back((*vdd_stripe_vector)[i]);
            string layer = (*vdd_stripe_vector)[i].layer;
            (*vdd_stripe_map).insert(pair<string, vector<Stripe>>(layer, layer_vdd_stripe_vector));
        }
    }
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        vector<Stripe> stripe_vector = (*vdd_stripe_map)[layer];
        if (isOddLayer(layer))
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            (*vdd_stripe_map)[layer] = stripe_vector;
        }
        else
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            (*vdd_stripe_map)[layer] = stripe_vector;
        }
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        if ((*vss_stripe_map).count((*vss_stripe_vector)[i].layer) == 1)
        {
            (*vss_stripe_map)[(*vss_stripe_vector)[i].layer].push_back((*vss_stripe_vector)[i]);
        }
        else
        {
            vector<Stripe> layer_vss_stripe_vector;
            layer_vss_stripe_vector.push_back((*vss_stripe_vector)[i]);
            string layer = (*vss_stripe_vector)[i].layer;
            (*vss_stripe_map).insert(pair<string, vector<Stripe>>(layer, layer_vss_stripe_vector));
        }
    }

    for (auto vss_stripe_map_it = (*vss_stripe_map).begin(); vss_stripe_map_it != (*vss_stripe_map).end(); ++vss_stripe_map_it)
    {
        string layer = vss_stripe_map_it->first;
        vector<Stripe> stripe_vector = (*vss_stripe_map)[layer];
        if (isOddLayer(layer))
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortOddStripeLocationVector);
            (*vss_stripe_map)[layer] = stripe_vector;
        }
        else
        {
            sort(stripe_vector.begin(), stripe_vector.end(), sortEvenStripeLocationVector);
            (*vss_stripe_map)[layer] = stripe_vector;
        }
    }

    vector<Stripe> stripe_vector = (*vdd_stripe_map)["M4"];
    cout << stripe_vector.size() << endl;
    float stripe_resource_total = 0;
    for (int i = 0; i < stripe_vector.size(); i++)
    {
        float start_x_location = stof(stripe_vector[i].start_x_location);
        float end_x_location = stof(stripe_vector[i].end_x_location);
        float distance = abs(start_x_location - end_x_location);
        float stripe_width = stof(stripe_vector[i].width);
        float stripe_resource = distance * stripe_width;
        stripe_resource_total += stripe_resource;
    }
    cout << "stripe_resource_total : " << stripe_resource_total << endl;
}

void getRoutingTrackPowerConsuming(vector<Stripe> *stripe_vector, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{
    ofstream myfile;
    myfile.open("track_power");
    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        cout << " ======== stripe range " << i << " ===========" << endl;

        for (int j = 0; j < (*stripe_vector)[i].track_point_vector.size(); j++)
        {
            TrackPoint track_point = (*stripe_vector)[i].track_point_vector[j];

            float total_power_consume = 0;
            float total_cell = 0;
            for (int k = 0; k < track_point.power_cell_id_vector.size(); k++)
            {
                string cell_id = track_point.power_cell_id_vector[k];
                CellPlacedInfo cell_placed_info = (*cell_placed_map)[cell_id];
                float y_location = stof(cell_placed_info.down_y_location);
                if (y_location >= 162.144)
                {

                    float power = (*cell_ip_map)[cell_id].instance_total_power;
                    total_power_consume += power;
                    total_cell += 1;
                }
            }
        }
    }
    myfile.close();
}

void getLeftCellPinAccessPoint(unordered_map<string, CellInfo> *cell_info_map)
{
    cout << "========== getLeftCellPinAccessPoint start ==========" << endl;
    for (auto cell_info_iter = (*cell_info_map).begin(); cell_info_iter != (*cell_info_map).end(); ++cell_info_iter)
    {
        setCellPinAccessPoint(&(cell_info_iter->second));
    }
    cout << "========== getLeftCellPinAccessPoint end ==========" << endl;
}

void setCellPinAccessPoint(CellInfo *cell_info)
{
    float odd_start_m3_track_float = 0.036;
    float even_start_m3_track_float = 0.108;
    float end_m3_track_float = stof((*cell_info).width);
    float m3_pitch_track = 0.072;
    map<string, vector<PinAccessPoint>> odd_pin_access_point_map;
    map<string, vector<PinAccessPoint>> even_pin_access_point_map;
    (*cell_info).odd_pin_access_point_map = odd_pin_access_point_map;
    (*cell_info).even_pin_access_point_map = even_pin_access_point_map;

    float start_m2_track_float = 0.0;
    float end_m2_track_float = stof((*cell_info).height);
    float m2_pitch_track = 0.072;

    // odd
    for (float i = odd_start_m3_track_float; floatIsEqualOrLess(i, end_m3_track_float); i += m3_pitch_track)
    {
        for (float j = start_m2_track_float; floatIsEqualOrLess(j, end_m2_track_float); j += m2_pitch_track)
        {
            isOnTrackPin(&((*cell_info).pin_shape_map), &((*cell_info).odd_pin_access_point_map), floatToString(i), floatToString(j));
        }
    }

    for (float i = even_start_m3_track_float; floatIsEqualOrLess(i, end_m3_track_float); i += m3_pitch_track)
    {
        for (float j = start_m2_track_float; floatIsEqualOrLess(j, end_m2_track_float); j += m2_pitch_track)
        {
            isOnTrackPin(&((*cell_info).pin_shape_map), &((*cell_info).even_pin_access_point_map), floatToString(i), floatToString(j));
        }
    }
}

void isOnTrackPin(map<string, PinShape> *pin_shape_map, map<string, vector<PinAccessPoint>> *pin_access_point_map, string track_point_x, string track_point_y)
{

    float track_point_x_float_right = stof(track_point_x) + 0.036;
    float track_point_x_float_left = stof(track_point_x) - 0.036;
    float track_point_y_float_top = stof(track_point_y) + 0.036;
    float track_point_y_float_bottom = stof(track_point_y) - 0.036;

    bool is_ontrack_pin = false;
    string pin_name;
    for (auto pin_shape_iter = (*pin_shape_map).begin(); pin_shape_iter != (*pin_shape_map).end(); ++pin_shape_iter)
    {
        pin_name = pin_shape_iter->first;

        // map<string, vector<PinAccess>> layer_pin_access_map = pin_iter->second.layer_pin_access_map;
        for (auto rect_iter = pin_shape_iter->second.rect_map.begin(); rect_iter != pin_shape_iter->second.rect_map.end(); ++rect_iter)
        {
            string layer_name = rect_iter->first;
            if (layer_name == "M1")
            {
                // vector<PinAccess> pin_access_vector = layer_pin_access_iter->second;

                for (int i = 0; i < rect_iter->second.size(); i++)
                {

                    float rect_bottom_x_float = stof((rect_iter->second)[i].bottom_x_location);
                    float rect_bottom_y_float = stof((rect_iter->second)[i].bottom_y_location);
                    float rect_top_x_float = stof((rect_iter->second)[i].top_x_location);
                    float rect_top_y_float = stof((rect_iter->second)[i].top_y_location);
                    // on vertical shape
                    if (floatIsEqual(rect_bottom_x_float, track_point_x_float_left) && floatIsEqual(rect_top_x_float, track_point_x_float_right) && floatIsEqualOrMore(track_point_y_float_bottom, rect_bottom_y_float) && floatIsEqualOrLess(track_point_y_float_top, rect_top_y_float))
                    {
                        is_ontrack_pin = true;
                    }
                    // on horizontal shape
                    else if (floatIsEqual(rect_bottom_y_float, track_point_y_float_bottom) && floatIsEqual(rect_top_y_float, track_point_y_float_top) && floatIsEqualOrMore(track_point_x_float_left, rect_bottom_x_float) && floatIsEqualOrLess(track_point_x_float_right, rect_top_x_float))
                    {
                        is_ontrack_pin = true;
                    }
                    if (is_ontrack_pin)
                    {
                        break;
                    }
                }
            }
            if (is_ontrack_pin)
            {
                break;
            }
        }
        if (is_ontrack_pin)
        {
            break;
        }
    }

    if (is_ontrack_pin)
    {
        PinAccessPoint pin_access_point;
        pin_access_point.middle_x_location = track_point_x;
        pin_access_point.middle_y_location = track_point_y;

        if ((*pin_access_point_map).count(pin_name))
        {
            (*pin_access_point_map)[pin_name].push_back(pin_access_point);
        }
        else
        {
            vector<PinAccessPoint> pin_access_point_vector;
            pin_access_point_vector.push_back(pin_access_point);
            (*pin_access_point_map).insert(pair<string, vector<PinAccessPoint>>(pin_name, pin_access_point_vector));
        }
    }
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

    // cout << m2_y_start_int << endl;
    // cout << m2_y_pitch_int << endl;

    for (int i = m2_y_start_int; i < end_core_site_y_location_int; i += m2_y_pitch_int)
    {
        if (i > start_core_site_y_location_int && i <= end_core_site_y_location_int)
        {
            TrackPoint m2_track_point;
            double i_double = (double)i;
            m2_track_point.y_point = floatToString((i_double * 4) / 1000);
            string key_m2_track_point = m2_track_point.y_point;

            m2_track_point_vector.push_back(m2_track_point);
            (*m2_track_point_map).insert(pair<string, TrackPoint>(key_m2_track_point, m2_track_point));
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

    int isRoutingTrackCount = 1;
    for (int i = m3_x_start_int; i < end_core_site_x_location_int; i += m3_x_pitch_int)
    {
        if (i > start_core_site_x_location_int && i <= end_core_site_x_location_int)
        {
            TrackPoint m3_track_point;
            double i_double = (double)i;
            m3_track_point.x_point = floatToString((i_double * 4) / 1000);
            string key_m3_track_point = m3_track_point.x_point;
            vector<string> power_cell_id_vector;
            m3_track_point.power_cell_id_vector = power_cell_id_vector;

            if ((isRoutingTrackCount % 2) == 1)
            {
                m3_track_point.isRoutingTrack = true;
                // cout << "isRouting track point : " << m3_track_point.x_point << " " << m3_track_point.isRoutingTrack << endl;
            }
            else
            {
                // cout << "isRouting track point : " << m3_track_point.x_point << " " << m3_track_point.isRoutingTrack << endl;
                m3_track_point.isRoutingTrack = false;
            }

            m3_track_point_vector.push_back(m3_track_point);
            (*m3_track_point_map).insert(pair<string, TrackPoint>(key_m3_track_point, m3_track_point));
        }
        if (i > end_core_site_x_location_int)
        {
            cout << "check break" << endl;
            break;
        }
        isRoutingTrackCount += 1;
    };
    // cout << "m2 track size : " << (*m3_track_point_map).size() << endl;
    // cout << "m2 track size : " << m3_track_point_vector.size() << endl;
    // cout << "m3 track point : " << m3_track_point_vector[509].x_point << endl;
    cout << "========== setRoutingTrackPoint end ==========" << endl;
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
                            stripe.end_x_location = def_content_array[11];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.layer = def_content_array[1];
                            stripe.net_name = NET_NAME_VDD;
                            vector<string> ip_power_vector;
                            stripe.ip_power_vector = ip_power_vector;

                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                setStripeLength(&stripe);
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
                            stripe.end_x_location = def_content_array[11];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.layer = def_content_array[1];
                            stripe.net_name = NET_NAME_VSS;
                            vector<string> ip_power_vector;
                            stripe.ip_power_vector = ip_power_vector;
                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                setStripeLength(&stripe);
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
void setStripeLength(Stripe *stripe)
{
    if (isOddLayer((*stripe).layer))
    {
        float length = abs(stof((*stripe).start_y_location) - stof((*stripe).end_y_location));
        (*stripe).length = floatToString(length);
    }
    else
    {
        float length = abs(stof((*stripe).start_x_location) - stof((*stripe).end_x_location));
        (*stripe).length = floatToString(length);
    }
}
bool isOddLayer(string layer)
{
    if (layer == "M3" || layer == "M5" || layer == "M7" || layer == "M9")
    {
        return true;
    }
    else if (layer == "M4" || layer == "M6" || layer == "M8")
    {
        return false;
    }
    else
    {
        cout << "error is Odd Layer" << endl;
        return false;
    }
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
    else if (((*stripe).start_y_location == (*stripe).end_y_location))
    {
        float x_distance = abs(stof((*stripe).start_x_location) - stof((*stripe).end_x_location));
        float core_site_x_distance = abs(stof((*core_site).left_x_location) - stof((*core_site).right_x_location)) / 2;
        if (x_distance < core_site_x_distance)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return false;
        cout << " isPowerStripe error  << " << endl;
    }
}

void getIpPowerReport(string ip_report, unordered_map<string, CellInstancePowerInfo> *cell_ip_map)
{
    cout << "========== getIpPowerReport start ==========" << endl;
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
    cout << "========== getIpPowerReport end ==========" << endl;
}

void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map)
{

    ifstream def_transfer_file(def_transfer_file_name);
    string def_content;
    bool is_placed_end = false;
    int log = 0;
    if (def_transfer_file)
    {
        while (getline(def_transfer_file, def_content))
        {
            // check 這部分
            if (def_content.find("COMPONENTS") != string::npos && ((def_content.find("COMPONENTPIN") != string::npos) == false))
            {
                while (getline(def_transfer_file, def_content))
                {
                    if (def_content.find("PLACED") != string::npos)
                    {
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
                        cout << " end def data " << endl;
                        is_placed_end = true;
                        break;
                    }
                }
            }
            if (is_placed_end)
            {
                break;
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
        string cell_width = (*cell_info_map)[cell_name].width;
        string cell_height = (*cell_info_map)[cell_name].height;
        float right_x_float = left_x_float + stof(cell_width);
        float up_y_float = down_y_float + stof(cell_height);
        (*cell_placed_info).right_x_location = floatToString(right_x_float);
        (*cell_placed_info).up_y_location = floatToString(up_y_float);
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
            (*cell_placed_info).direction = (*def_content_array)[i + 4];
            break;
        }
    }
}
bool sortOddStripeLocationVector(Stripe stripe_a, Stripe stripe_b)
{
    return stof(stripe_a.start_x_location) < stof(stripe_b.start_x_location);
}
bool sortEvenStripeLocationVector(Stripe stripe_a, Stripe stripe_b)
{
    return stof(stripe_a.start_y_location) < stof(stripe_b.start_y_location);
}

void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map)
{
    cout << "========== getLefCellImformation start ==========" << endl;
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
    cout << "========== getLefCellImformation end ==========" << endl;
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
                rect_vector.push_back(rect);
            }
            // cout << "=====================" << endl;
            // rect_vector.push_back(rect);
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

float convertInnovusPoint(int number)
{
    return (number * 4) / 1000.0;
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

bool floatIsEqual(float a, float b)
{
    string a_str = floatToString(a);
    string b_str = floatToString(b);
    if (a_str == b_str)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool floatIsEqualOrLess(float a, float b)
{
    if (floatIsEqual(a, b))
    {
        return true;
    }
    else if (a < b)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool floatIsEqualOrMore(float a, float b)
{
    if (floatIsEqual(a, b))
    {
        return true;
    }
    else if (a > b)
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
    std::stringstream buf;
    std::ostringstream oss;

    buf.precision(3);
    buf.setf(std::ios::fixed);
    buf << value;
    string str = buf.str();
    buf.clear();

    return str;
}
void generateLogFile(vector<Stripe> *vdd_stripe_vector, string log_file)
{
    // �������ʶZ�� �̤j�Z�����ʦh�� �̤p�Z�����ʦh�� �зǮt�h��
    ofstream myfile;
    myfile.open(log_file);
    vector<float> distance_vector;
    float total_distance = 0;
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        myfile << "---------------------------------------------" << endl;
        myfile << "stripe index       : " << i << endl;
        float start_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);
        float track_cost_float = stof((*vdd_stripe_vector)[i].vdd_track_x_location);
        float migration_distance = abs((start_x_location_float - track_cost_float));
        total_distance += migration_distance;
        myfile << "migration distance : " << migration_distance << endl;
        distance_vector.push_back(migration_distance);
        myfile << "moving range     : " << (*vdd_stripe_vector)[i].move_range_x_left << " " << (*vdd_stripe_vector)[i].move_range_x_right << endl;
        if (i != ((*vdd_stripe_vector).size() - 1))
        {
            myfile << "spacing          : " << (stof((*vdd_stripe_vector)[i].start_x_location) - stof((*vdd_stripe_vector)[i + 1].start_x_location)) << endl;
        }
    }
    sort(distance_vector.begin(), distance_vector.end());
    int last_index = distance_vector.size() - 1;
    float average = total_distance / distance_vector.size();
    float standard_deviation = standardDeviation(&distance_vector);
    myfile << "================================================" << endl;
    myfile << "number of stripe       : " << distance_vector.size() << endl;
    myfile << "total_distance         : " << total_distance << endl;
    myfile << "The shortest distance  : " << distance_vector[0] << endl;
    myfile << "The logest distance    : " << distance_vector[last_index] << endl;
    myfile << "The average distance   : " << average << endl;
    myfile << "The standard deviation : " << standard_deviation << endl;

    myfile.close();
}
void generateTrackInfoMap(unordered_map<string, TrackInfo> *track_info_map)
{
    // TRACKS Y 0.18 DO 0.33025 STEP 0.216 LAYER M4 ;
    // TRACKS X 0.18 DO 0.611 STEP 0.144 LAYER M3;
    TrackInfo m3_track_info;
    TrackInfo m4_track_info;

    string m3_x_pitch = "0.144";
    string m3_x_start = "0.18";
    string m4_x_pitch = "0.216";
    string m4_x_start = "0.18";
    m3_track_info.layer = "M3";
    m3_track_info.start = m3_x_start;
    m3_track_info.pitch = m3_x_pitch;
    m4_track_info.layer = "M4";
    m4_track_info.start = m4_x_start;
    m4_track_info.pitch = m4_x_pitch;
    (*track_info_map).insert(pair<string, TrackInfo>(m3_track_info.layer, m3_track_info));
    (*track_info_map).insert(pair<string, TrackInfo>(m4_track_info.layer, m4_track_info));
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
// cout << " after " << endl;
// cout << "" << endl;
// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;
//     map<string, Pin> pin_map = cell_place.second.pin_map;
//     for (auto pin : pin_map)
//     {
//         cout << "pin name : " << pin.first << endl;
//         for (auto pin_layer : pin.second.layer_pin_access_map)
//         {
//             cout << "layer : " << pin_layer.first << " " << pin_layer.second.size();
//             for (int i = 0; i < pin_layer.second.size(); i++)
//             {
//                 cout << "(" << pin_layer.second[i].middle_x_location_def << " " << pin_layer.second[i].middle_y_location_def << ")";
//             }
//             std::cout << "" << std::endl;
//         };
//     }
// }
// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;
//     map<string, Pin> pin_map = cell_place.second.pin_map;
//     for (auto pin : pin_map)
//     {
//         cout << "pin name : " << pin.first << endl;
//         for (auto pin_layer : pin.second.layer_pin_access_map)
//         {
//             cout << "layer : " << pin_layer.first << " " << pin_layer.second.size();
//             for (int i = 0; i < pin_layer.second.size(); i++)
//             {
//                 // cout << "(" << pin_layer.second[i].middle_x_location_def << " " << pin_layer.second[i].middle_y_location_def << ")";
//             }
//             std::cout << "" << std::endl;
//         };
//     }
// }
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

// for (auto cell_place : cell_placed_map["U148369"].pin_map)
// {
//     cout << cell_place.first << endl;

//     for (auto cell_layer : cell_place.second.layer_pin_access_map)
//     {
//         for (int i = 0; i < cell_layer.second.size(); i++)
//         {
//             cout << cell_place.first << "  (" << cell_layer.second[i].middle_x_location_def << " " << cell_layer.second[i].middle_y_location_def << ")" << endl;
//         }
//     }
// }
// for (auto m3_track_point : m3_track_point_map)
// {
//     if (m3_track_point.second.power_cell_id_vector.size() > 0)
//     {

//         cout << "track point :" << m3_track_point.second.x_point << " " << m3_track_point.second.power_cell_id_vector.size() << endl;
//         for (int i = 0; i < m3_track_point.second.power_cell_id_vector.size(); i++)
//         {
//             cout << m3_track_point.second.power_cell_id_vector[i] << endl;
//         }
//     }
// }

// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;
//     for (auto pin_access_map : cell_place.second.pin_access_point_map)
//     {
//         cout << "pin name : " << pin_access_map.first << endl;
//         for (auto pin_access_pin : pin_access_map.second)
//         {
//             cout << " (" << pin_access_pin.middle_x_location_def << " " << pin_access_pin.middle_y_location_def << ") " << endl;
//         }
//     }
// }

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
//                 cout << rect.second[i].bottom_x_location << " " << rect.second[i].bottom_y_location << " " << rect.second[i].top_x_location << " " << rect.second[i].top_y_location << " "  << endl;
//             }
//         }
//     }
// }
// for (auto cellInfo : cell_info_map)
// {
//     cout << "cell name : " << cellInfo.first << endl;
//     map<string, vector<PinAccessPoint>> pin_access_map = cellInfo.second.even_pin_access_point_map;
//     for (auto pin_access : pin_access_map)
//     {
//         vector<PinAccessPoint> pin_access_vector = pin_access.second;
//         for (int i = 0; i < pin_access_vector.size(); i++)
//         {
//             if (cellInfo.first == "A2O1A1Ixp33_ASAP7_6t_fix")
//             {
//                 cout << "(" << floatToString(stof(pin_access_vector[i].middle_x_location_def) + 561.96) << " " << floatToString(stof(pin_access_vector[i].middle_y_location_def) + 78.336) << ")" << endl;
//             }
//         }
//     }
// }

// int total_track = 0;
// int total_track_cell_id = 0;
// for (int i = 0; i < vdd_stripe_vector.size(); i++)
// {
//     for (int j = 0; j < vdd_stripe_vector[i].track_point_vector.size(); j++)
//     {
//         TrackPoint track_point = vdd_stripe_vector[i].track_point_vector[j];
//         cout << "track poin cost : " << track_point.x_point << "  " << track_point.total_pin_access_power_consum_cost << endl;
//     }
// }

// cout << "track size : " << total_track << endl;
// cout << "total_track_cell_id : " << total_track_cell_id << endl;

// for (auto cell_place : cell_placed_map)
// {
//     cout << "cell id :" << cell_place.first << endl;

//     cout << cell_place.second.left_x_location << " " << cell_place.second.down_y_location << " " << cell_place.second.right_x_location << " " << cell_place.second.up_y_location << endl;
// }

// // �N cell ��J stripe range ����
// void setCellStripeRange(vector<Stripe> *vdd_stripe_vector, unordered_map<string, CellInstancePowerInfo> *cell_ip_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
// {
//     cout << "========== setCellStripeRange start ==========" << endl;
//     for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
//     {
//         for (auto iter = (*cell_ip_map).begin(); iter != (*cell_ip_map).end(); ++iter)
//         {
//             string cell_id = iter->first;
//             if (isInStripeRange((&(*vdd_stripe_vector)[i]), cell_id, &(*cell_placed_map)))
//             {
//                 (*vdd_stripe_vector)[i].ip_power_vector.push_back(cell_id);
//             }
//         }
//     }
//     cout << "========== setCellStripeRange end ==========" << endl;
// }

// bool isInStripeRange(Stripe *vdd_stripe, string cell_id, unordered_map<string, CellPlacedInfo> *cell_placed_map)
// {
//     string move_range_x_left = (*vdd_stripe).move_range_x_left;
//     string move_range_x_right = (*vdd_stripe).move_range_x_right;
//     string left_x_location = (*cell_placed_map)[cell_id].left_x_location;
//     string right_x_location = (*cell_placed_map)[cell_id].right_x_location;
//     float move_range_x_left_float = stof(move_range_x_left);
//     float move_range_x_right_float = stof(move_range_x_right);
//     float left_x_location_float = stof(left_x_location);
//     float right_x_location_float = stof(right_x_location);

//     // case 1 : middle of left stripe
//     if ((left_x_location_float < move_range_x_left_float) && (right_x_location_float > move_range_x_left_float) && (left_x_location_float < move_range_x_right_float) && (right_x_location_float < move_range_x_right_float))
//     {
//         return true;
//     } // case 2 : in stripe moving location
//     else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float <= move_range_x_right_float)
//     {
//         return true;
//     } // case 3 : middle of right stripe
//     else if (left_x_location_float >= move_range_x_left_float && right_x_location_float >= move_range_x_left_float && left_x_location_float <= move_range_x_right_float && right_x_location_float >= move_range_x_right_float)
//     {
//         return true;
//     } // case 4 : out side
//     else
//     {
//         return false;
//     }
// }

// void setRoutingTrackPowerConsuming(vector<Stripe> *stripe_vector, unordered_map<string, TrackPoint> *m3_track_point_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, TrackInfo *m3_track_info)
// {
//     cout << "========== setRoutingTrackPowerConsuming start ==========" << endl;

//     // float power_stripe_width_float = stof(power_stripe_width);
//     for (int i = 0; i < (*stripe_vector).size(); i++)
//     {
//         float power_stripe_width_float = stof((*stripe_vector)[i].width);
//         int start_move_range_x_left = (stof((*stripe_vector)[i].move_range_x_left) * 1000) / 4;
//         int start_move_range_x_right = (stof((*stripe_vector)[i].move_range_x_right) * 1000) / 4;

//         float m3_x_start_float = (stof((*m3_track_info).start) * 1000) / 4;
//         float m3_x_pitch_float = (stof((*m3_track_info).pitch) * 1000) / 4;

//         int m3_x_start_int = (int)m3_x_start_float;
//         m3_x_pitch_float = m3_x_pitch_float * 2;
//         int m3_x_pitch_int = (int)m3_x_pitch_float;

//         cout << " ----- moving range start -----" << endl;
//         cout << (*stripe_vector)[i].move_range_x_left << " " << (*stripe_vector)[i].move_range_x_right << endl;
//         vector<TrackPoint> track_point_vector;
//         for (int j = start_move_range_x_left; j <= start_move_range_x_right; j += m3_x_pitch_float)
//         {
//             float x_track_point = (j * 4) / 1000.0;

//             vector<string> power_cell_id_vector;
//             TrackPoint track_point;
//             track_point.x_point = floatToString(x_track_point);
//             track_point.power_cell_id_vector = power_cell_id_vector;
//             vector<string> ip_power_vector = (*stripe_vector)[i].ip_power_vector;
//             for (int k = 0; k < ip_power_vector.size(); k++)
//             {
//                 string cell_id = ip_power_vector[k];
//                 if (isInOddTrackStripeRange(&track_point, &(*cell_placed_map)[cell_id], power_stripe_width_float))
//                 {
//                     track_point.power_cell_id_vector.push_back(cell_id);
//                 }
//             }
//             track_point_vector.push_back(track_point);
//         }
//         cout << " ----- moving range end -----" << endl;
//         (*stripe_vector)[i].track_point_vector = track_point_vector;
//     }

//     cout << "========== setRoutingTrackPowerConsuming end ==========" << endl;
// }

//    for (auto vdd_stripe_map_it = vdd_stripe_map.begin(); vdd_stripe_map_it != vdd_stripe_map.end(); ++vdd_stripe_map_it)
//     {
//         string layer = vdd_stripe_map_it->first;
//         cout << " layer : " << layer << " size : " << vdd_stripe_map_it->second.size() << endl;
//         vector<Stripe> stripe_vector = vdd_stripe_map_it->second;
//         for (int i = 0; i < stripe_vector.size(); i++)
//         {
//             Stripe stripe = stripe_vector[i];
//             cout << stripe.net_name << " " << stripe.layer << " " << stripe.width << " " << stripe.length << " " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << endl;
//         }
//     }

//   for (auto vdd_stripe_map_it = vdd_stripe_map.begin(); vdd_stripe_map_it != vdd_stripe_map.end(); ++vdd_stripe_map_it)
//     {
//         string layer = vdd_stripe_map_it->first;
//         cout << " layer : " << layer << " size : " << vdd_stripe_map_it->second.size() << endl;
//         vector<Stripe> stripe_vector = vdd_stripe_map_it->second;
//         for (int i = 0; i < stripe_vector.size(); i++)
//         {
//             Stripe stripe = stripe_vector[i];
//             cout << stripe.net_name << " " << stripe.layer << " " << stripe.width << " " << stripe.length << " " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << endl;
//         }
//     }
// for (auto even_pin_access_cost_map_it = even_pin_access_cost_map.begin(); even_pin_access_cost_map_it != even_pin_access_cost_map.end(); ++even_pin_access_cost_map_it)
// {
//     string stripe_width = even_pin_access_cost_map_it->first;
//     cout << "stripe_width : " << stripe_width << endl;
//     unordered_map<string, PinAccessCost> pin_access_cost_map = even_pin_access_cost_map_it->second;
//     for (auto pin_access_cost_map_it = pin_access_cost_map.begin(); pin_access_cost_map_it != pin_access_cost_map.end(); ++pin_access_cost_map_it)
//     {
//         string location = pin_access_cost_map_it->first;
//         cout << "location : " << location << endl;
//         unordered_map<string, string> pin_access_cost_of_cell_map = pin_access_cost_map_it->second.direction_cost_map;
//         for (auto pin_access_cost_of_cell_map_it = pin_access_cost_of_cell_map.begin(); pin_access_cost_of_cell_map_it != pin_access_cost_of_cell_map.end(); ++pin_access_cost_of_cell_map_it)
//         {
//             cout << "direction : " << pin_access_cost_of_cell_map_it->first << endl;
//             cout << "cost      : " << pin_access_cost_of_cell_map_it->second << endl;
//         }
//     }
// }
// for (auto odd_pin_access_cost_map_it = even_pin_access_cost_map.begin(); odd_pin_access_cost_map_it != odd_pin_access_cost_map.end(); ++odd_pin_access_cost_map_it)
// {
//     string stripe_width = odd_pin_access_cost_map_it->first;
//     cout << "stripe_width : " << stripe_width << endl;
//     unordered_map<string, PinAccessCost> pin_access_cost_map = odd_pin_access_cost_map_it->second;
//     for (auto pin_access_cost_map_it = pin_access_cost_map.begin(); pin_access_cost_map_it != pin_access_cost_map.end(); ++pin_access_cost_map_it)
//     {
//         string location = pin_access_cost_map_it->first;
//         cout << "location : " << location << endl;
//         unordered_map<string, string> pin_access_cost_of_cell_map = pin_access_cost_map_it->second.vertical_direction_cost_map;
//         for (auto pin_access_cost_of_cell_map_it = pin_access_cost_of_cell_map.begin(); pin_access_cost_of_cell_map_it != pin_access_cost_of_cell_map.end(); ++pin_access_cost_of_cell_map_it)
//         {
//             cout << "direction : " << pin_access_cost_of_cell_map_it->first << endl;
//             cout << "cost      : " << pin_access_cost_of_cell_map_it->second << endl;
//         }
//     }
// }

// for (auto vdd_stripe_map_it = vdd_stripe_map.begin(); vdd_stripe_map_it != vdd_stripe_map.end(); ++vdd_stripe_map_it)
// {
//     string layer = vdd_stripe_map_it->first;
//     cout << " layer : " << layer << " size : " << vdd_stripe_map_it->second.size() << endl;
//     vector<Stripe> stripe_vector = vdd_stripe_map_it->second;
//     for (int i = 0; i < stripe_vector.size(); i++)
//     {
//         Stripe stripe = stripe_vector[i];
//         cout <<  "size : " << stripe.ip_power_vector.size() << endl;
//         for (int j = 0; j < stripe.ip_power_vector.size(); j++)
//         {
//             cout <<  stripe.ip_power_vector[j] << endl;
//         }

//         // cout << stripe.net_name << " " << stripe.layer << " " << stripe.width << " " << stripe.length << " " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << endl;
//     }
// }