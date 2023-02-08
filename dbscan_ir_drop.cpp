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
#include <stdio.h>
#include <cmath>
// =========== dbscan.h start ==========
#include "D:\ir_drop_project\header\dbscan.h"
// =========== dbscan.h end ==========
#include <cstdio>
#include <ctime>
// =========== kmeans.h start ==========
// #include "D:\ir_drop_project\header\kmeans.h"
// #include "D:\ir_drop_project\header\kmeansPlusPlus.h"
// =========== kmeans.h end ==========
struct Track
{
    string track_x_location;
};
struct Stripe
{
    string net_name;
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string add_stripe_middle_x_location;
    string add_stripe_middle_y_location;
    string width;
    string length;
    string layer;
    long double delta_ir_drop;
    float ir_drop_cost;
    vector<string> ip_power_vector;
    unordered_map<string, string> m4_track_point_map;
};
struct CoreSite
{

    string left_x_location;
    string right_x_location;
    string up_y_location;
    string down_y_location;
};
struct IrDropPoint
{
    string ir_drop;
    string layer;
    string x_location;
    string y_location;
    string down_middle_location;
    string up_middle_location;
};
struct CellPlacedInfo
{
    string cell_id;
    string cell_name;
    string left_x_location;
    string down_y_location;
    string right_x_location;
    string up_y_location;
    string middle_x_location;
    string middle_y_location;
    vector<IrDropPoint> ir_drop_point_vector;
    string ir_drop_count;
};
struct Row
{
    string down_boundary;
    string up_boundary;
    string middle_location;
    vector<string> cell_id_vector;
};
struct Cluster
{
    string cluster_id;
    vector<Point> point_vector;
    vector<CellPlacedInfo> cell_placed_info_vector;
    float left_x_location = 1000000;
    float down_y_location = 1000000;
    float right_x_location = 0;
    float up_y_location = 0;
    float height;
    float width;

    string average_ir_drop;
    vector<Stripe> add_stripe_vector;
};

struct CellInfo
{
    string cell_name;
    string cell_height;
    string cell_width;
};

struct RowBlock
{
    string left_x_location;
    string down_y_location;
    string right_x_location;
    string up_y_location;
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
struct DieArea
{
    string lower_left_x_location;
    string lower_left_y_location;
    string top_right_x_location;
    string top_right_y_location;
};
#define MINIMUM_POINTS 6 // minimum number of cluster
// #define EPSILON (0.864 * 0.864) // distance for clustering, metre^2
#define EPSILON 1.728 // distance for clustering, metre^2
// Util
vector<string>
splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool floatIsEqual(float a, float b);
bool isInCluster(Cluster *rcSour, Cluster *rcDest);

void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void printClusterReport(vector<Point> *points, unordered_map<string, Cluster> *cluster_map);
void printResults(vector<Point> &points, int num_points);
void readBenchmarkData(vector<Point> &points, string file_name);

void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void setIrDropReport(string method, vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map, string range_start, string range_end, string ir_drop_percent);
void printIrDropReport(unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
void getRowCore(unordered_map<string, Row> *row_map, CoreSite *core_site);
void setCellInRowCore(unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void reviseCellPlacedPosition(CellPlacedInfo *cell_placed_info);
void setIrDropPointInRow(unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
string getIrDropMiddleLocation(string y_location);
void setIrDropCost(unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *cell_location_map);
void printCellIrDropPointMap(unordered_map<string, CellPlacedInfo> *cell_placed_map, string ir_point_file);
void transferCellIrDropPointMapToVector(unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<CellPlacedInfo> *cell_placed_vector);
void setIrDropInCell(string middle_location, string ir_drop_x_location, IrDropPoint *ir_drop_point, unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void getBoudaryClusterAndIrDrop(unordered_map<string, Cluster> *cluster_map, unordered_map<string, CellPlacedInfo> *cell_location_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<Cluster> *cluster_vector);
float innovusPointIntToFloat(int number);
int innovusPointFloatToInt(float location);
void getStripeLocation(string def_transfer_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, string *power_stripe_width_resource);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
void setIrDropInCluster(unordered_map<string, Cluster> *cluster_map, unordered_map<string, CellPlacedInfo> *cell_location_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<Cluster> *cluster_vector);
bool isOddLayer(string layer);
void setClusterBoundary(vector<Cluster> *cluster_vector);
void transferStripePositionFromTcl(string layer, Stripe *stripe, string start_x_location, string start_y_location, string end_x_location, string end_y_location);
void setStripeLength(Stripe *stripe);
void transferToPositionMap(vector<Stripe> *vdd_stripe_vector, unordered_map<string, Stripe> *position_vdd_stripe_map);
string getLeftStripeLocation(vector<Stripe> *vdd_stripe_vector, string left_point_location);
string getRightStripeLocation(vector<Stripe> *vdd_stripe_vector, string right_point_location);
void getRightStripe(vector<Stripe> *vdd_stripe_vector, Stripe *right_stripe, string right_point_location);
void getLeftStripe(vector<Stripe> *vdd_stripe_vector, Stripe *right_stripe, string right_point_location);
void getStripeLocationFromStripeTcl(string stripe_tcl_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, set<string> *stripe_width_set, string *power_stripe_resource_width);
bool sortClusterIrDropVector(Cluster cluster_a, Cluster cluster_b);
void generateAddPowerStripe(vector<Cluster> *cluster_vector, vector<Cluster> *revise_cluster_vector, unordered_map<string, vector<Stripe>> *vdd_stripe_map);
void generateAddPowerStripeTclLimitResource(unordered_map<string, vector<Stripe>> *vdd_stripe_map, vector<Cluster> *cluster_vector, string add_stripe_file_name, string dbscan_log_file, bool isLimitResource);
void getAddStripeTclFirstMethod(unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector);
void getAddStripeTclSecondMethod(unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector);
void getAddStripeTclThirdMethod(unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector);
void generateStripeTrack(vector<Stripe> *stripe_vector, CoreSite *core_site);
bool deisngRuleCheckAddPowerStripe(Stripe *left_stripe, Stripe *right_stripe, string *middle_y_location, unordered_map<string, Stripe> *position_design_rule_vdd_stripe_map);
string transferToTrackPoint(string middle_y_location, string layer);
bool rowBlockRuleCheck(vector<RowBlock> *row_block_vector, Stripe *add_Stripe, string middle_y_location_string);
bool trackCanNotBeAddStripe(unordered_map<string, Stripe> *position_design_rule_vdd_stripe_map, string middle_y_location, string left_location, string right_location);
void removeTrackPointFromPosition(string left_x_location, string right_x_location, Stripe *left_stripe, Stripe *right_stripe, string middle_y_location, unordered_map<string, Stripe> *position_design_rule_vdd_stripe_map);
bool sortIrDropPoint(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b);
void setIrInPowerStripe(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
bool isInOddPowerStripeWidth(Stripe *stripe, string power_stripe_width, IrDropPoint *ir_drop_point);
bool isInEvenPowerStripeWidth(Stripe *stripe, string power_stripe_width, IrDropPoint *ir_drop_point);
bool sortRiselocation(Stripe stripe_a, Stripe stripe_b);
bool sortDropLocation(Stripe stripe_a, Stripe stripe_b);
bool sortDeltaIrDropRise(Stripe stripe_a, Stripe stripe_b);
bool sortDeltaIrDrop(Stripe stripe_a, Stripe stripe_b);
void setShapeRingLocation(ShapeRing *shape_ring, vector<string> *def_content_array, vector<ShapeRing> *shape_ring_vector);
void setRingSide(vector<ShapeRing> *shape_ring_vector, float middle_x_line, float middle_y_line);
void getShapeRing(string def_file_name, vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector);
void tansferShapeRing(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, map<string, ShapeRing> *vdd_shape_map, map<string, ShapeRing> *vss_shape_map);
void setShapeRingSide(vector<ShapeRing> *vdd_shape_ring_vector, vector<ShapeRing> *vss_shape_ring_vector, DieArea *die_area);
void getDieArea(string def_file_name, DieArea *die_area);
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string M3 = "M3";
const string M4 = "M4";
const string M1 = "M1";
const string LEFT = "LEFT";
const string RIGHT = "RIGHT";
const string UP = "UP";
const string DOWN = "DOWN";
// 294.048(一條power stripe 長度)
// const string POWER_STRIPE_RESOURCE_HEIGHT = "588.096";
string POWER_STRIPE_RESOURCE_HEIGHT = "";
const string POWER_STRIPE_RESOURCE_WIDTH = "0.224";
const string M4_TRACK_STEP = "0.216";
const string M4_TRACK_START = "0.18";
const int CHECK_RULE_DISTANCE = 5;
// 50%
const string IR_DROP_PERCENT = "0.5";
const int CHECK_POWER_STRIPE_DISTANCE = 10;
const int FIRSTMETHODNUMBEROFIRPOINT = 20;
const int SECONDMETHODNUMBEROFIRPOINT = 20;
const int ADDSTRIPEOFCELLROW = 4;
const string METHOD_3 = "METHOD_3";
const string METHOD_2 = "METHOD_2";
const string METHOD_1 = "METHOD_1";
const float METHOD_2_IR_DROP_PERCENT = 0.07;
int main(int argc, char *argv[])
// int main()
{
    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    unordered_map<string, vector<IrDropPoint>> ir_drop_point_map;
    vector<Row> boundary_row;
    unordered_map<string, Row> row_map;
    vector<string> ir_drop_file_vector;
    vector<Cluster> cluster_vector;
    vector<Cluster> revise_cluster_vector;
    unordered_map<string, Cluster> cluster_map;
    unordered_map<string, CellPlacedInfo> cell_location_map;
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    unordered_map<string, vector<Stripe>> vdd_stripe_map;
    unordered_map<string, vector<Stripe>> vss_stripe_map;
    set<string> stripe_width_set;
    vector<ShapeRing> vdd_shape_ring_vector;
    vector<ShapeRing> vss_shape_ring_vector;
    map<string, ShapeRing> vdd_shape_map;
    map<string, ShapeRing> vss_shape_map;
    DieArea die_area;

    string config_file = argv[1];
    // string config_file = "config/config_gpu.txt";
    string excute_time = "log_file/excute_time" + config_file;
    ofstream myfile;
    myfile.open(excute_time);
    string LEF_FILE = "";
    string IR_REPORT_FILE_M1 = "";
    string DEF_TRANSFER_FILE = "";
    string ADD_STRIPE_FOR_CLUSTER_TCL = "";
    string DBSCAN_LOG_FILE = "";
    string IR_REPORT_FILE_M3 = "";
    string STRIPE_TCL = "";
    string IR_DROP_POINT_FILE = "";
    // string POWER_STRIPE_RESOURCE_HEIGHT = "";
    // add power stripe config
    const string IR_DROP_RANGE_START = "0.632";
    const string IR_DROP_RANGE_END = "0.64";
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
            if (config_content_array[0] == "ADD_STRIPE_FOR_CLUSTER_TCL")
            {
                ADD_STRIPE_FOR_CLUSTER_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "DBSCAN_LOG_FILE")
            {
                DBSCAN_LOG_FILE = config_content_array[2];
            }
            if (config_content_array[0] == "IR_REPORT_FILE_M1")
            {
                IR_REPORT_FILE_M1 = config_content_array[2];
            }
            if (config_content_array[0] == "ADD_STRIPE_TCL")
            {
                STRIPE_TCL = config_content_array[2];
            }
            if (config_content_array[0] == "IR_REPORT_FILE_M3")
            {
                IR_REPORT_FILE_M3 = config_content_array[2];
            }
            if (config_content_array[0] == "IR_DROP_POINT_FILE")
            {
                IR_DROP_POINT_FILE = config_content_array[2];
            }
        }
    }

    double START, END;
    START = clock();

    // string ir_drop_file = "ir_report/M1_ir_drop_point_25.report";
    ir_drop_file_vector.push_back(IR_REPORT_FILE_M1);
    ir_drop_file_vector.push_back(IR_REPORT_FILE_M3);

    // ===========  ir drop map start ===========
    // setIrDropReport(METHOD_1, &ir_drop_file_vector, &ir_drop_point_map, IR_DROP_RANGE_START, IR_DROP_RANGE_END, IR_DROP_PERCENT);
    setIrDropReport(METHOD_2, &ir_drop_file_vector, &ir_drop_point_map, IR_DROP_RANGE_START, IR_DROP_RANGE_END, IR_DROP_PERCENT);
    // setIrDropReport(METHOD_3, &ir_drop_file_vector, &ir_drop_point_map, IR_DROP_RANGE_START, IR_DROP_RANGE_END, IR_DROP_PERCENT);

    // for (auto ir_drop_point_map_it = ir_drop_point_map.begin(); ir_drop_point_map_it != ir_drop_point_map.end(); ++ir_drop_point_map_it)
    // {
    //     cout << ir_drop_point_map_it->second.size() << endl;
    // }
    // cout << ir_drop_point_map.size() << endl;

    // =========== ir drop map end ===========

    // // =========== def file start ===========
    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);
    getRowCore(&row_map, &core_site);
    setCellInRowCore(&row_map, &cell_placed_map);
    setIrDropPointInRow(&row_map, &cell_placed_map, &ir_drop_point_map);
    setIrDropCost(&cell_placed_map, &cell_location_map);
    printCellIrDropPointMap(&cell_placed_map, IR_DROP_POINT_FILE);

    // // // get Stripe from def
    getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_map, &vss_stripe_map, &core_site, &POWER_STRIPE_RESOURCE_HEIGHT);
    generateStripeTrack(&vdd_stripe_map[M3], &core_site);
    setIrInPowerStripe(&vdd_stripe_map, &ir_drop_point_map);
    // // get Stripe from tcl
    // // getStripeLocationFromStripeTcl(STRIPE_TCL, &vdd_stripe_map, &vss_stripe_map, &core_site, &stripe_width_set, &POWER_STRIPE_RESOURCE_HEIGHT);
    // // cout << "size : " << vdd_stripe_map[M3].size() << endl;
    // for (int i = 0; i < vdd_stripe_map[M3].size(); i++)
    // {
    //     cout << "VDDX : " << vdd_stripe_map[M3][i].start_x_location << " " << vdd_stripe_map[M3][i].width << " " << vdd_stripe_map[M3][i].delta_ir_drop << endl;
    // }
    getDieArea(DEF_TRANSFER_FILE, &die_area);
    getShapeRing(DEF_TRANSFER_FILE, &vdd_shape_ring_vector, &vss_shape_ring_vector);
    setShapeRingSide(&vdd_shape_ring_vector, &vss_shape_ring_vector, &die_area);
    tansferShapeRing(&vdd_shape_ring_vector, &vss_shape_ring_vector, &vdd_shape_map, &vss_shape_map);

    // // // // =========== def file end ===========

    // // // // =========== dbscan  cluster algorithm start ===========
    vector<Point> points;
    // read point data
    readBenchmarkData(points, IR_DROP_POINT_FILE);
    // constructor
    DBSCAN ds(MINIMUM_POINTS, EPSILON, points);
    // main loop
    ds.run();
    // result of DBSCAN algorithm
    // printResults(ds.m_points, ds.getTotalPointSize());
    printClusterReport(&(ds.m_points), &cluster_map);
    setIrDropInCluster(&cluster_map, &cell_location_map, &cell_placed_map, &cluster_vector);
    // // // =========== dbscan cluster algorithm end ===========

    // // =========== get Add Power Stripe start ============

    // method 1 serious 20 point :
    // getAddStripeTclFirstMethod(&cell_location_map, &cluster_vector);
    // setClusterBoundary(&cluster_vector);
    // generateAddPowerStripe(&cluster_vector, &vdd_stripe_map);
    // generateAddPowerStripeTclLimitResource(&vdd_stripe_map, &cluster_vector, ADD_STRIPE_FOR_CLUSTER_TCL, DBSCAN_LOG_FILE, false);

    // method 2 serious 5% point :
    getAddStripeTclSecondMethod(&cell_location_map, &cluster_vector);
    setClusterBoundary(&cluster_vector);
    generateAddPowerStripe(&cluster_vector, &revise_cluster_vector, &vdd_stripe_map);
    generateAddPowerStripeTclLimitResource(&vdd_stripe_map, &cluster_vector, ADD_STRIPE_FOR_CLUSTER_TCL, DBSCAN_LOG_FILE, false);
    // method 3 serious range 1  :
    // getAddStripeTclThirdMethod(&cell_location_map, &cluster_vector);
    // setClusterBoundary(&cluster_vector);
    // generateAddPowerStripe(&cluster_vector, &vdd_stripe_map);
    // generateAddPowerStripeTclLimitResource(&vdd_stripe_map, &cluster_vector, ADD_STRIPE_FOR_CLUSTER_TCL, DBSCAN_LOG_FILE, false);

    // =========== get Add Power Stripe end   ============

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
bool isInOddPowerStripeWidth(Stripe *stripe, string power_stripe_width, IrDropPoint *ir_drop_point)
{
    string start_x_location = (*stripe).start_x_location;
    float power_stripe_width_float = stof(power_stripe_width);
    float x_left_float = stof(start_x_location) - power_stripe_width_float;
    float x_right_float = stof(start_x_location) + power_stripe_width_float;

    float ir_drop_point_x_location = stof((*ir_drop_point).x_location);

    if (ir_drop_point_x_location >= x_left_float && ir_drop_point_x_location <= x_right_float)
    {
        return true;
    }
    else
    {
        return false;
    }
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
            if ((def_content.find("( * VDD )") != string::npos))
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

void setIrInPowerStripe(unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
{

    // step 1 : count delta ir drop
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        if ((*ir_drop_point_map).count(layer))
        {
            vector<IrDropPoint> ir_drop_point_vector = (*ir_drop_point_map)[layer];
            if (isOddLayer(layer))
            {
                for (int i = 0; i < (*vdd_stripe_map)[layer].size(); i++)
                {
                    vector<IrDropPoint> sort_ir_drop_point_vector;
                    for (int j = 0; j < ir_drop_point_vector.size(); j++)
                    {
                        if (isInOddPowerStripeWidth(&(*vdd_stripe_map)[layer][i], (*vdd_stripe_map)[layer][i].width, &ir_drop_point_vector[j]))
                        {
                            sort_ir_drop_point_vector.push_back(ir_drop_point_vector[j]);
                        }
                    }

                    sort(sort_ir_drop_point_vector.begin(), sort_ir_drop_point_vector.end(), sortIrDropPoint);
                    int last_index = sort_ir_drop_point_vector.size() - 1;
                    float delta_ir_drop = stof(sort_ir_drop_point_vector[0].ir_drop) - stof(sort_ir_drop_point_vector[last_index].ir_drop);
                    (*vdd_stripe_map)[layer][i].delta_ir_drop = delta_ir_drop;
                }
            }
            else
            {

                for (int i = 0; i < (*vdd_stripe_map)[layer].size(); i++)
                {
                    vector<IrDropPoint> sort_ir_drop_point_vector;
                    for (int j = 0; j < ir_drop_point_vector.size(); j++)
                    {
                        if (isInEvenPowerStripeWidth(&(*vdd_stripe_map)[layer][i], (*vdd_stripe_map)[layer][i].width, &ir_drop_point_vector[j]))
                        {
                            sort_ir_drop_point_vector.push_back(ir_drop_point_vector[j]);
                        }
                    }
                    sort(sort_ir_drop_point_vector.begin(), sort_ir_drop_point_vector.end(), sortIrDropPoint);
                    int last_index = sort_ir_drop_point_vector.size() - 1;
                    float delta_ir_drop = stof(sort_ir_drop_point_vector[0].ir_drop) - stof(sort_ir_drop_point_vector[last_index].ir_drop);
                    (*vdd_stripe_map)[layer][i].delta_ir_drop = delta_ir_drop;
                }
            }
        }
    }

    // step 2 : count delta ir drop cost
    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        vector<Stripe> stripe_vector = vdd_stripe_map_it->second;
        sort(stripe_vector.begin(), stripe_vector.end(), sortDeltaIrDrop);
        float maximum_delta_ir_drop = stripe_vector[0].delta_ir_drop - stripe_vector[(stripe_vector.size() - 1)].delta_ir_drop;
        float minimum_ir_drop = stripe_vector[(stripe_vector.size() - 1)].delta_ir_drop;

        for (int i = 0; i < (*vdd_stripe_map)[layer].size(); i++)
        {
            float delata_ir_drop = (*vdd_stripe_map)[layer][i].delta_ir_drop;
            float temp_delta_ir = delata_ir_drop - minimum_ir_drop;
            (*vdd_stripe_map)[layer][i].ir_drop_cost = temp_delta_ir / maximum_delta_ir_drop;
        }
    }

    for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    {
        string layer = vdd_stripe_map_it->first;
        for (int i = 0; i < (*vdd_stripe_map)[layer].size(); i++)
        {
            cout << "layer : " << layer << " stripe location : " << (*vdd_stripe_map)[layer][i].start_x_location << " delta ir drop : " << (*vdd_stripe_map)[layer][i].delta_ir_drop << endl;
        }
    }
    //  =================================
    // for (auto vdd_stripe_map_it = (*vdd_stripe_map).begin(); vdd_stripe_map_it != (*vdd_stripe_map).end(); ++vdd_stripe_map_it)
    // {
    //     string layer = vdd_stripe_map_it->first;
    //     for (int i = 0; i < (*vdd_stripe_map)[layer].size(); i++)
    //     {
    //         cout << "layer : " << layer << " stripe location : " << (*vdd_stripe_map)[layer][i].start_x_location << " delta ir drop : " <<  (*vdd_stripe_map)[layer][i].ir_drop_cost << endl;
    //     }
    // }
}
bool sortIrDropPoint(IrDropPoint ir_drop_point_a, IrDropPoint ir_drop_point_b)
{
    return (stof(ir_drop_point_a.ir_drop) > stof(ir_drop_point_b.ir_drop));
}
bool sortDeltaIrDrop(Stripe stripe_a, Stripe stripe_b)
{
    return stripe_a.delta_ir_drop > stripe_b.delta_ir_drop;
}

bool sortDeltaIrDropRise(Stripe stripe_a, Stripe stripe_b)
{
    return stripe_a.delta_ir_drop < stripe_b.delta_ir_drop;
}

// 小到大
bool sortRiseLocation(Stripe stripe_a, Stripe stripe_b)
{
    return stof(stripe_a.start_x_location) < stof(stripe_b.start_x_location);
}
// 大到小
bool sortDropLocation(Stripe stripe_a, Stripe stripe_b)
{
    return stof(stripe_a.start_x_location) > stof(stripe_b.start_x_location);
}

bool isInEvenPowerStripeWidth(Stripe *stripe, string power_stripe_width, IrDropPoint *ir_drop_point)
{
    string start_y_location = (*stripe).start_y_location;
    float power_stripe_width_float = stof(power_stripe_width);
    float y_down_float = stof(start_y_location) - power_stripe_width_float;
    float y_up_float = stof(start_y_location) + power_stripe_width_float;

    float ir_drop_point_y_location = stof((*ir_drop_point).y_location);

    if (ir_drop_point_y_location >= y_down_float && ir_drop_point_y_location <= y_up_float)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void generateAddPowerStripeTclLimitResource(unordered_map<string, vector<Stripe>> *vdd_stripe_map, vector<Cluster> *cluster_vector, string add_stripe_file_name, string dbscan_log_file, bool isLimitResource)
{

    cout << "========== generateAddPowerStripeTclLimitResource start ==========" << endl;
    ofstream myfile;
    myfile.open(dbscan_log_file);
    float power_stripe_resource_length = stof(POWER_STRIPE_RESOURCE_HEIGHT);
    ofstream add_stripe_file;
    add_stripe_file.open(add_stripe_file_name);
    float metal_resource = 0;
    cout << "cluster size : " << (*cluster_vector).size() << endl;
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        myfile << "cluster_id : " << (*cluster_vector)[i].cluster_id << endl;
        myfile << "average_ir_drop : " << (*cluster_vector)[i].average_ir_drop << endl;
        myfile << "left_x_location : " << (*cluster_vector)[i].left_x_location << endl;
        myfile << "right_x_location : " << (*cluster_vector)[i].right_x_location << endl;
        myfile << "down_y_location : " << (*cluster_vector)[i].down_y_location << endl;
        myfile << "up_y_location : " << (*cluster_vector)[i].up_y_location << endl;
        myfile << "add_stripe_size : " << (*cluster_vector)[i].add_stripe_vector.size() << endl;
        if ((*cluster_vector)[i].add_stripe_vector.size() == 1)
        {
            Stripe stripe = (*cluster_vector)[i].add_stripe_vector[0];
            myfile << "addStripe -nets { VDDX } -layer "
                   << stripe.layer
                   << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << " }" << endl;
        }

        for (int j = 0; j < (*cluster_vector)[i].add_stripe_vector.size(); j++)
        {
            Stripe stripe = (*cluster_vector)[i].add_stripe_vector[j];
            myfile << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << endl;
            if (power_stripe_resource_length >= 0 && !isLimitResource)
            {
                float stripe_width = abs(stof(stripe.start_x_location) - stof(stripe.end_x_location));
                if (isLimitResource)
                {
                    power_stripe_resource_length -= stripe_width;
                }
                add_stripe_file << "addStripe -nets { VDDX } -layer "
                                << stripe.layer
                                << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << " }" << endl;
                float single_power_stripe_resource = stripe_width * stof(POWER_STRIPE_RESOURCE_WIDTH);
                metal_resource += single_power_stripe_resource;
            }
        }
        myfile << " ================== " << endl;
        myfile << " ------------------------------ " << endl;
    };
    myfile << "add power stripe metal using resource : " << metal_resource << endl;

    vector<Stripe> stripe_vector = (*vdd_stripe_map)[M3];
    float original_metal_resource = 0;

    for (int i = 0; i < stripe_vector.size(); i++)
    {
        float power_stripe_length = abs(stof(stripe_vector[i].start_y_location) - stof(stripe_vector[i].end_y_location));
        float power_stripe_width = stof(stripe_vector[i].width);
        float power_stripe_resource = power_stripe_length * power_stripe_width;

        original_metal_resource += power_stripe_resource;
    }
    myfile << "original metal using resource : " << original_metal_resource << endl;

    myfile.close();
    add_stripe_file.close();
    cout << "========== generateAddPowerStripeTclLimitResource end ==========" << endl;
}

bool isInCluster(Cluster *rcSour, Cluster *rcDest)
{
    bool b_Result = true;
    // 如果第一个矩形的左边x坐标大于第二个矩形右边的x坐标，肯定不相交
    if ((*rcSour).left_x_location > ((*rcDest).left_x_location + (*rcDest).width) && ((*rcSour).left_x_location + (*rcSour).width) > (*rcDest).left_x_location)
        b_Result = false;
    // 如果第一个矩形的右边x坐标小于第二个矩形左边的x坐标，肯定不相交
    if ((*rcSour).left_x_location < ((*rcDest).left_x_location + (*rcDest).width) && ((*rcSour).left_x_location + (*rcSour).width) < (*rcDest).left_x_location)
        b_Result = false;
    // 如果第一个矩形的底边y坐标大于第二个矩形顶边的y坐标，肯定不相交
    if ((*rcSour).up_y_location > ((*rcDest).up_y_location - (*rcDest).height) && ((*rcSour).up_y_location - (*rcSour).height) > (*rcDest).up_y_location)
        b_Result = false;
    // 如果第一个矩形的顶边y坐标小于第二个矩形底边的y坐标，肯定不相交
    if ((*rcSour).up_y_location < ((*rcDest).up_y_location - (*rcDest).height) && ((*rcSour).up_y_location - (*rcSour).height) < (*rcDest).up_y_location)
        b_Result = false;
    // 其它情况，均会相交
    return b_Result;
}

// float innovusPointIntToFloat(int number);
// int innovusPointFloatToInt(float location);
void generateStripeTrack(vector<Stripe> *stripe_vector, CoreSite *core_site)
{
    float m4_track_step = stof(M4_TRACK_STEP);
    int m4_track_step_int = innovusPointFloatToInt(m4_track_step);
    float m4_track_start = stof(M4_TRACK_START);
    int m4_track_start_int = innovusPointFloatToInt(m4_track_start);

    float core_site_up_float = stof((*core_site).up_y_location);
    int core_site_up_int = innovusPointFloatToInt(core_site_up_float);
    float core_site_down_float = stof((*core_site).down_y_location);
    int core_site_down_int = innovusPointFloatToInt(core_site_down_float);

    int core_site_up_int_temp = core_site_up_int - m4_track_start_int;
    int temp_pitch_up = core_site_up_int_temp / m4_track_step_int;
    int end_track_int = (temp_pitch_up * m4_track_step_int) + m4_track_start_int;
    float end_track_float = innovusPointIntToFloat(end_track_int);

    int core_site_down_int_temp = core_site_down_int - m4_track_start_int;
    int temp_pitch_down = core_site_down_int_temp / m4_track_step_int;
    temp_pitch_down += 1;
    int start_track_int = (temp_pitch_down * m4_track_step_int) + m4_track_start_int;
    float start_track_float = innovusPointIntToFloat(start_track_int);

    for (int i = 0; i < (*stripe_vector).size(); i++)
    {
        for (int j = start_track_int; j <= end_track_int; j += m4_track_step_int)
        {
            float innovus_point_float = innovusPointIntToFloat(j);
            string innovus_point = floatToString(innovus_point_float);
            (*stripe_vector)[i].m4_track_point_map.insert(pair<string, string>(innovus_point, innovus_point));
        }
    }
}

void transferToPositionMap(vector<Stripe> *vdd_stripe_vector, unordered_map<string, Stripe> *position_vdd_stripe_map)
{
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        cout << "position : " << (*vdd_stripe_vector)[i].start_x_location << endl;
        (*position_vdd_stripe_map).insert(pair<string, Stripe>((*vdd_stripe_vector)[i].start_x_location, (*vdd_stripe_vector)[i]));
    }
};

// void getLeftStripe(vector<Stripe> *vdd_stripe_vector, Stripe *left_stripe, string left_point_location)
// {
//     float distance = 100000000;
//     float left_point_location_float = stof(left_point_location);
//     for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
//     {
//         float stripe_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);

//         float stripe_width_float = stof((*vdd_stripe_vector)[i].width);
//         stripe_width_float = stripe_width_float / 2;

//         stripe_x_location_float = stripe_x_location_float - stripe_width_float;

//         float temp_distance = abs((stripe_x_location_float - left_point_location_float));

//         if (stripe_x_location_float <= left_point_location_float && temp_distance <= distance)
//         {
//             distance = temp_distance;
//             (*left_stripe) = (*vdd_stripe_vector)[i];
//         }
//     }
// }
// const int CHECK_POWER_STRIPE_DISTANCE = 5;
//     bool sortRiselocation(Stripe stripe_a, Stripe stripe_b)
// bool sortDropLocation(Stripe stripe_a, Stripe stripe_b)
void getLeftStripe(vector<Stripe> *vdd_stripe_vector, Stripe *left_stripe, string left_point_location)
{
    // cout << " ----- getLeftStripe start -----" << endl;
    float left_point_location_float = stof(left_point_location);
    vector<Stripe> delta_ir_drop_stripe_vector;
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        float stripe_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);
        float stripe_width_float = stof((*vdd_stripe_vector)[i].width);
        stripe_width_float = stripe_width_float / 2;

        stripe_x_location_float = stripe_x_location_float - stripe_width_float;

        if (stripe_x_location_float <= left_point_location_float)
        {
            delta_ir_drop_stripe_vector.push_back((*vdd_stripe_vector)[i]);
        }
    }
    // cout << "check in side 1 -------" << endl;
    // step 1 sort distance
    sort(delta_ir_drop_stripe_vector.begin(), delta_ir_drop_stripe_vector.end(), sortDropLocation);
    vector<Stripe> distance_ir_drop_stripe_vector;
    // cout << "check in side 2 -------" << endl;
    for (int i = 0; i < CHECK_POWER_STRIPE_DISTANCE; i++)
    {
        distance_ir_drop_stripe_vector.push_back(delta_ir_drop_stripe_vector[i]);
    }
    // cout << "check in side 3 -------" << endl;
    // step2 sort delta ir_drop

    // cout << distance_ir_drop_stripe_vector[0].delta_ir_drop << endl;
    sort(distance_ir_drop_stripe_vector.begin(), distance_ir_drop_stripe_vector.end(), sortDeltaIrDropRise);
    // cout << "check in side 4 -------" << endl;

    // cout << distance_ir_drop_stripe_vector[0].start_x_location << endl;
    (*left_stripe) = distance_ir_drop_stripe_vector[0];
    // cout << " ----- getLeftStripe end -----" << endl;
}
void getRightStripe(vector<Stripe> *vdd_stripe_vector, Stripe *right_stripe, string right_point_location)
{
    // cout << " ----- RightStripe start -----" << endl;
    float right_point_location_float = stof(right_point_location);
    vector<Stripe> delta_ir_drop_stripe_vector;
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        float stripe_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);

        float stripe_width_float = stof((*vdd_stripe_vector)[i].width);
        stripe_width_float = stripe_width_float / 2;
        stripe_x_location_float = stripe_x_location_float + stripe_width_float;

        if (stripe_x_location_float >= right_point_location_float)
        {
            (*right_stripe) = (*vdd_stripe_vector)[i];
            delta_ir_drop_stripe_vector.push_back((*vdd_stripe_vector)[i]);
        }
    }
    // step 1 sort distance
    sort(delta_ir_drop_stripe_vector.begin(), delta_ir_drop_stripe_vector.end(), sortRiseLocation);
    vector<Stripe> distance_ir_drop_stripe_vector;
    for (int i = 0; i < CHECK_POWER_STRIPE_DISTANCE; i++)
    {
        distance_ir_drop_stripe_vector.push_back(delta_ir_drop_stripe_vector[i]);
    }
    // step2 sort delta ir_drop
    sort(distance_ir_drop_stripe_vector.begin(), distance_ir_drop_stripe_vector.end(), sortDeltaIrDropRise);
    (*right_stripe) = distance_ir_drop_stripe_vector[0];
    // cout << " ----- getRightStripe end -----" << endl;
}

// void getRightStripe(vector<Stripe> *vdd_stripe_vector, Stripe *right_stripe, string right_point_location)
// {
//     float distance = 100000000;

//     float right_point_location_float = stof(right_point_location);
//     for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
//     {
//         float stripe_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);

//         float stripe_width_float = stof((*vdd_stripe_vector)[i].width);
//         stripe_width_float = stripe_width_float / 2;
//         stripe_x_location_float = stripe_x_location_float + stripe_width_float;
//         float temp_distance = abs((stripe_x_location_float - right_point_location_float));

//         if (stripe_x_location_float >= right_point_location_float && temp_distance <= distance)
//         {
//             distance = temp_distance;
//             (*right_stripe) = (*vdd_stripe_vector)[i];
//         }
//     }
// }

bool trackIsBeUsed(unordered_map<string, string> *track_map, string middle_y_location)
{
    if ((*track_map).count(middle_y_location) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool trackCanNotBeAddStripe(unordered_map<string, Stripe> *position_design_rule_vdd_stripe_map, string middle_y_location, string left_location, string right_location)
{
    bool isUsed = false;

    for (auto position_design_rule_vdd_stripe_map_it = (*position_design_rule_vdd_stripe_map).begin(); position_design_rule_vdd_stripe_map_it != (*position_design_rule_vdd_stripe_map).end(); ++position_design_rule_vdd_stripe_map_it)
    {
        string position = position_design_rule_vdd_stripe_map_it->first;
        string stripe_x_location = (*position_design_rule_vdd_stripe_map)[position].start_x_location;
        float stripe_x_location_float = stof(stripe_x_location);
        float right_x_location_float = stof(right_location);
        float left_location_float = stof(left_location);
        if (left_location_float <= stripe_x_location_float && stripe_x_location_float <= right_x_location_float)
        {
            if (trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[stripe_x_location].m4_track_point_map, middle_y_location))
            {
                isUsed = true;
            }
        }
    }
    return isUsed;
}

bool deisngRuleCheckAddPowerStripe(Stripe *left_stripe, Stripe *right_stripe, string *middle_y_location, unordered_map<string, Stripe> *position_design_rule_vdd_stripe_map)
{
    // addStripe -nets { VDDX } -layer M4 -direction horizontal -width 0.224 -set_to_set_distance 12.88 -number_of_sets 1  -area { 282.308 230.720 308.164 230.944 }
    // 上面三條Track 下面三條Track
    string left_x_location = (*left_stripe).start_x_location;
    string right_x_location = (*right_stripe).start_x_location;
    string middle_y_location_temp = (*middle_y_location);

    int middle_y_location_int = innovusPointFloatToInt(stof((*middle_y_location)));
    int lef_x_location_int = innovusPointFloatToInt(stof(left_x_location));
    int right_x_location_int = innovusPointFloatToInt(stof(right_x_location));
    int m4_track_step_int = innovusPointFloatToInt(stof(M4_TRACK_STEP));
    // cout << "middle_y_location " << (*middle_y_location) << endl;
    // cout << "position left : " << ((*position_design_rule_vdd_stripe_map).count(left_x_location) != 0) << endl;
    // cout << "position right : " << ((*position_design_rule_vdd_stripe_map).count(right_x_location) != 0) << endl;
    // cout << "middle_y_location_before : " << (*middle_y_location) << endl;
    // cout << "left_x_location : " << left_x_location << endl;
    // cout << "right_x_location : " << right_x_location << endl;
    // cout << "track_is_be_used_left : " << trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map, middle_y_location_temp) << endl;
    // cout << "track_is_be_used_right : " << trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map, middle_y_location_temp) << endl;
    // cout << "trackCanNotBeAddStripe : " << trackCanNotBeAddStripe(&(*position_design_rule_vdd_stripe_map), middle_y_location_temp, left_x_location, right_x_location) << endl;

    if ((*position_design_rule_vdd_stripe_map).count(left_x_location) != 0 && (*position_design_rule_vdd_stripe_map).count(right_x_location) != 0)
    {
        if (trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map, middle_y_location_temp) || trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map, middle_y_location_temp) || trackCanNotBeAddStripe(&(*position_design_rule_vdd_stripe_map), middle_y_location_temp, left_x_location, right_x_location))
        {
            // cout << " track is be used " << endl;
            // 進行for迴圈跑上下三條track 奇數偶數判斷
            string revise_middle_location = "";
            for (int i = 1; i <= 3; i++)
            {
                int temp_track_int = i * m4_track_step_int;
                int up_track_middle_point_int = middle_y_location_int + temp_track_int;
                int down_track_middle_point_int = middle_y_location_int - temp_track_int;
                string up_track_middle_point = floatToString(innovusPointIntToFloat(up_track_middle_point_int));
                string down_track_middle_point = floatToString(innovusPointIntToFloat(down_track_middle_point_int));
                // if (middle_y_location_temp == "130.860")
                // {
                //     cout << "index                   : " << i << endl;
                //     cout << "up_track_middle_point   : " << up_track_middle_point << endl;
                //     cout << "down_track_middle_point : " << down_track_middle_point << endl;
                //     cout << "---------------------------------------- out" << endl;
                // }

                if ((!trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map, up_track_middle_point)) && (!trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map, up_track_middle_point)) && (!trackCanNotBeAddStripe(&(*position_design_rule_vdd_stripe_map), middle_y_location_temp, left_x_location, right_x_location)))
                {

                    revise_middle_location = up_track_middle_point;
                    (*middle_y_location) = revise_middle_location;
                    removeTrackPointFromPosition(left_x_location, right_x_location, &(*left_stripe), &(*right_stripe), revise_middle_location, &(*position_design_rule_vdd_stripe_map));
                    break;
                }
                if ((!trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map, down_track_middle_point)) && (!trackIsBeUsed(&(*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map, down_track_middle_point)) && (!trackCanNotBeAddStripe(&(*position_design_rule_vdd_stripe_map), middle_y_location_temp, left_x_location, right_x_location)))
                {
                    revise_middle_location = down_track_middle_point;
                    (*middle_y_location) = revise_middle_location;
                    removeTrackPointFromPosition(left_x_location, right_x_location, &(*left_stripe), &(*right_stripe), revise_middle_location, &(*position_design_rule_vdd_stripe_map));
                    break;
                }
            }
            if (revise_middle_location.length() == 0)
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
            removeTrackPointFromPosition(left_x_location, right_x_location, &(*left_stripe), &(*right_stripe), middle_y_location_temp, &(*position_design_rule_vdd_stripe_map));
            // cout << "middle_y_location_temp : " << middle_y_location_temp << endl;
            // cout << "middle_y_location_after : " << (*middle_y_location) << endl;

            return true;
        }
    }
    else
    {
        cout << "position_design_rule_vdd_stripe_map error  " << endl;
        return false;
    }
}

void removeTrackPointFromPosition(string left_x_location, string right_x_location, Stripe *left_stripe, Stripe *right_stripe, string middle_y_location, unordered_map<string, Stripe> *position_design_rule_vdd_stripe_map)
{
    (*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map.erase(middle_y_location);
    (*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map.erase(middle_y_location);
    int middle_y_location_int = innovusPointFloatToInt(stof(middle_y_location));
    int m4_track_step_int = innovusPointFloatToInt(stof(M4_TRACK_STEP));
    // M4 design rule 間距 上下三條
    for (int i = 1; i <= CHECK_RULE_DISTANCE; i++)
    {

        int temp_track_int = i * m4_track_step_int;
        int up_track_middle_point_int = middle_y_location_int + temp_track_int;
        int down_track_middle_point_int = middle_y_location_int - temp_track_int;

        string up_track_middle_point = floatToString(innovusPointIntToFloat(up_track_middle_point_int));
        string down_track_middle_point = floatToString(innovusPointIntToFloat(down_track_middle_point_int));
        // if (middle_y_location == "130.860")
        // {
        //     cout << "index                   : " << i << endl;
        //     cout << "up_track_middle_point   : " << up_track_middle_point << endl;
        //     cout << "down_track_middle_point : " << down_track_middle_point << endl;
        //     cout << "---------------------------------------- in" << endl;
        // }
        (*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map.erase(up_track_middle_point);
        (*position_design_rule_vdd_stripe_map)[left_x_location].m4_track_point_map.erase(down_track_middle_point);
        (*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map.erase(up_track_middle_point);
        (*position_design_rule_vdd_stripe_map)[right_x_location].m4_track_point_map.erase(down_track_middle_point);
    }
}

string transferToTrackPoint(string middle_y_location, string layer)
{
    if (isOddLayer(layer))
    {
        return "";
    }
    else
    {
        float m4_track_step = stof(M4_TRACK_STEP);
        int m4_track_step_int = innovusPointFloatToInt(m4_track_step);
        float m4_track_start = stof(M4_TRACK_START);
        int m4_track_start_int = innovusPointFloatToInt(m4_track_start);

        float middle_y_location_float = stof(middle_y_location);
        int middle_y_location_int = innovusPointFloatToInt(middle_y_location_float);

        int middle_y_location_temp = middle_y_location_int - m4_track_start_int;
        int temp_pitch = middle_y_location_temp / m4_track_step_int;
        int middle_y_location_temp_int = (temp_pitch * m4_track_step_int) + m4_track_start_int;
        float middle_y_location_temp_float = innovusPointIntToFloat(middle_y_location_temp_int);
        string middle_y_location_string = floatToString(middle_y_location_temp_float);
        return middle_y_location_string;
    }
}
// struct RowBlock
// {
//     string left_x_location;
//     string down_y_location;
//     string right_x_location;
//     string up_y_location;
// };
bool rowBlockRuleCheck(vector<RowBlock> *row_block_vector, Stripe *add_Stripe, string middle_y_location_string)
{
    bool rowBlockCheck = false;
    string add_stripe_left_x_location = (*add_Stripe).start_x_location;
    string add_stripe_end_x_location = (*add_Stripe).end_x_location;
    cout << "add_stripe_left_x_location : " << add_stripe_left_x_location << endl;
    cout << "add_stripe_end_x_location : " << add_stripe_end_x_location << endl;

    float add_stripe_left_x_location_float = stof(add_stripe_left_x_location);
    float add_stripe_end_x_location_float = stof(add_stripe_end_x_location);
    float middle_y_location_float = stof(middle_y_location_string);
    cout << "middle_y_location_float : " << middle_y_location_float << endl;

    int middle_y_location_int = innovusPointFloatToInt(middle_y_location_float);
    int step_int = innovusPointFloatToInt(0.864);
    int boundary_distance_int = step_int * 3;
    int down_boundary_int = middle_y_location_int - boundary_distance_int;
    int up_boundary_int = middle_y_location_int + boundary_distance_int;

    float down_boudary_float = innovusPointIntToFloat(down_boundary_int);
    float up_boundary_float = innovusPointIntToFloat(up_boundary_int);

    cout << "down_boudary_float : " << down_boudary_float << endl;
    cout << "up_boundary_float  : " << up_boundary_float << endl;

    RowBlock row_block;
    row_block.left_x_location = add_stripe_left_x_location;
    row_block.right_x_location = add_stripe_end_x_location;
    row_block.down_y_location = floatToString(down_boudary_float);
    row_block.up_y_location = floatToString(up_boundary_float);

    if (middle_y_location_string == "227.196")
    {
    }

    if ((*row_block_vector).size() == 0)
    {
        (*row_block_vector).push_back(row_block);
    }
    else
    {
        for (int i = 0; i < (*row_block_vector).size(); i++)
        {
            // if (middle_y_location_string == "227.196")
            // {
            //     cout << "(*row_block_vector)[i].left_x_location : " << (*row_block_vector)[i].left_x_location << endl;
            //     cout << "(*row_block_vector)[i].right_x_location : " << (*row_block_vector)[i].right_x_location << endl;
            //     cout << "(*row_block_vector) size : " << (*row_block_vector).size() << endl;
            // }
            if (((*row_block_vector)[i].left_x_location == add_stripe_left_x_location) && ((*row_block_vector)[i].right_x_location == add_stripe_end_x_location))
            {
                // cout << "row block check left_x_location : " << (*row_block_vector)[i].left_x_location << endl;
                // cout << "row block check right_x_location : " << (*row_block_vector)[i].right_x_location << endl;
                float row_block_down_y_location_float = stof(row_block.down_y_location);
                float row_block_up_y_location_float = stof(row_block.up_y_location);

                if (middle_y_location_float >= row_block_down_y_location_float && middle_y_location_float <= row_block_up_y_location_float)
                {
                    rowBlockCheck = true;
                }
            }
        }
        (*row_block_vector).push_back(row_block);
    }
    cout << "rowBlockCheck : " << rowBlockCheck << "row Block size : " << (*row_block_vector).size() << endl;
    return rowBlockCheck;
}

void generateAddPowerStripe(vector<Cluster> *cluster_vector, vector<Cluster> *revise_cluster_vector, unordered_map<string, vector<Stripe>> *vdd_stripe_map)
{
    cout << "========== generateAddPowerStripe start ========== " << endl;
    // const string POWER_STRIPE_RESOURCE_HEIGHT = "588.096";
    // const string POWER_STRIPE_RESOURCE_WIDTH = "0.224";
    vector<Stripe> vdd_stripe_vector = (*vdd_stripe_map)[M3];
    unordered_map<string, Stripe> position_design_rule_vdd_stripe_map;
    transferToPositionMap(&(*vdd_stripe_map)[M3], &position_design_rule_vdd_stripe_map);

    float power_stripe_resource_height = stof(POWER_STRIPE_RESOURCE_HEIGHT);
    sort((*cluster_vector).begin(), (*cluster_vector).end(), sortClusterIrDropVector);
    int step_int = innovusPointFloatToInt(0.864);
    int over_float_cell_row_int = ADDSTRIPEOFCELLROW * 2;
    int add_stripe_limit = over_float_cell_row_int * step_int;

    vector<RowBlock> row_block_vector;

    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        // step 1 : 離 cluster left 最近的power stripe 但要靠左一點
        Stripe left_stripe;
        Stripe right_stripe;

        // string lef_point_location = getLeftStripeLocation(&vdd_stripe_vector, floatToString((*cluster_vector)[i].left_x_location));
        getLeftStripe(&vdd_stripe_vector, &left_stripe, floatToString((*cluster_vector)[i].left_x_location));
        // step 2 : 離 cluster right 最近的power stripe 但要靠右一點
        // string right_point_location = getRightStripeLocation(&vdd_stripe_vector, floatToString((*cluster_vector)[i].right_x_location));
        getRightStripe(&vdd_stripe_vector, &right_stripe, floatToString((*cluster_vector)[i].right_x_location));

        float left_stripe_width_float = stof(left_stripe.width);
        left_stripe_width_float = left_stripe_width_float / 2;
        float left_start_x_location_float = stof(left_stripe.start_x_location);
        float left_boundary_float = left_start_x_location_float - left_stripe_width_float;
        string left_boundary_string = floatToString(left_boundary_float);

        float right_stripe_width_float = stof(right_stripe.width);
        right_stripe_width_float = right_stripe_width_float / 2;
        float right_start_x_location_float = stof(right_stripe.start_x_location);
        float right_boundary_float = right_start_x_location_float + right_stripe_width_float;
        string right_boundary_string = floatToString(right_boundary_float);

        // cout << "lef_point_location   : " << lef_point_location << endl;
        // cout << "right_point_location : " << right_point_location << endl;
        // cout << "left_stripe          : " << left_stripe.start_x_location << "  " << left_stripe.width << endl;
        // cout << "right_stripe         : " << right_stripe.start_x_location << "  " << right_stripe.width << endl;
        // cout << "--------------------------------------" << endl;

        // step 3 : 每5個cell row 加一條 power stripe 若低於五個 則加在中間的power stripe
        float up_point_location = (*cluster_vector)[i].up_y_location;
        float down_point_location = (*cluster_vector)[i].down_y_location;
        // float innovusPointIntToFloat(int number);
        // int innovusPointFloatToInt(float location);
        int up_point_location_int = innovusPointFloatToInt(up_point_location);
        int down_point_location_int = innovusPointFloatToInt(down_point_location);
        int vertical_distance = abs(up_point_location_int - down_point_location_int);

        // cout << "vertical_distance : " << innovusPointIntToFloat(vertical_distance) << endl;
        // cout << "add_stripe_limit : " << innovusPointIntToFloat(add_stripe_limit) << endl;

        if (vertical_distance < add_stripe_limit)
        {

            int middle_location_int = (up_point_location_int + down_point_location_int) / 2;
            float middle_y_location_float = innovusPointIntToFloat(middle_location_int);
            string middle_y_location_string = floatToString(middle_y_location_float);

            middle_y_location_string = transferToTrackPoint(middle_y_location_string, M4);
            // cout << "cluster boundary : " << (*cluster_vector)[i].left_x_location << (*cluster_vector)[i].right_x_location << " " << (*cluster_vector)[i].down_y_location << " " << (*cluster_vector)[i].up_y_location << endl;
            // cout << "is not over_flow : " << endl;
            // cout << "up_point_location : " << up_point_location << endl;
            // cout << "down_point_location : " << down_point_location << endl;
            // cout << "middle_y_location_float :" << middle_y_location_float << endl;
            // cout << "---------------------" << endl;
            // float power_stripe_float = stof(POWER_STRIPE_RESOURCE_WIDTH);
            // float down_stripe_float = middle_y_location_float - (power_stripe_float / 2);
            // float up_stripe_float = middle_y_location_float + (power_stripe_float / 2);
            // Stripe stripe;
            // stripe.start_x_location = lef_point_location;
            // stripe.end_x_location = right_point_location;
            // stripe.start_y_location = floatToString(down_stripe_float);
            // stripe.end_y_location = floatToString(up_stripe_float);
            // stripe.width = POWER_STRIPE_RESOURCE_WIDTH;
            // stripe.layer = M4;

            // step 4 : 在這邊做 design rule check

            if (deisngRuleCheckAddPowerStripe(&left_stripe, &right_stripe, &middle_y_location_string, &position_design_rule_vdd_stripe_map))
            {

                // addStripe -nets { VDDX } -layer M4 -direction horizontal -width 0.224 -set_to_set_distance 12.88 -number_of_sets 1  -area { 282.308 230.720 308.164 230.944 }

                // cout << "cluster boundary : " << (*cluster_vector)[i].up_y_location << " " << (*cluster_vector)[i].down_y_location << endl;
                // if (floatToString((*cluster_vector)[i].up_y_location) == "274.896" && floatToString((*cluster_vector)[i].down_y_location) == "273.168")
                // {
                //     for (int j = 0; j < (*cluster_vector)[i].point_vector.size(); j++)
                //     {
                //         cout << (*cluster_vector)[i].point_vector[j].x << "," << (*cluster_vector)[i].point_vector[j].y << endl;
                //     }
                // }
                // string right_boundary_string = floatToString(right_boundary_float);
                //      string left_boundary_string = floatToString(left_boundary_float);
                float power_stripe_float = stof(POWER_STRIPE_RESOURCE_WIDTH);
                float down_stripe_float = stof(middle_y_location_string) - (power_stripe_float / 2);
                float up_stripe_float = stof(middle_y_location_string) + (power_stripe_float / 2);
                Stripe stripe;
                stripe.start_x_location = left_boundary_string;
                stripe.end_x_location = right_boundary_string;
                stripe.start_y_location = floatToString(down_stripe_float);
                stripe.end_y_location = floatToString(up_stripe_float);
                stripe.width = POWER_STRIPE_RESOURCE_WIDTH;
                stripe.layer = M4;

                // if (!(rowBlockRuleCheck(&row_block_vector, &stripe, middle_y_location_string)))
                // {
                // cout << "addStripe -nets { VDDX } -layer "
                //      << stripe.layer
                //      << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << " }" << endl;
                (*cluster_vector)[i].add_stripe_vector.push_back(stripe);
                // }
            }

            // (*cluster_vector)[i].add_stripe_vector.push_back(stripe);
        }
        else
        {
            // cout << "is  over_flow : " << endl;
            // // cout << up_point_location << endl;
            // // cout << down_point_location << endl;
            // // int count = 1;
            // // cout << "------------------------------------------" << endl;
            // // cout << "cluster id " << (*cluster_vector)[i].cluster_id << endl;
            int count = 1;
            for (int j = down_point_location_int; j <= up_point_location_int; j += step_int)
            {
                int up_point_float_end_int = 0;
                int down_point_start_int = 0;
                if (count == 1)
                {
                    down_point_start_int = j;
                }
                if ((count % 5 == 0))
                {
                    up_point_float_end_int = j;
                    int middle_point_int = (up_point_float_end_int + down_point_start_int) / 2;
                    float middle_y_point_float = innovusPointIntToFloat(middle_point_int);
                    string middle_y_point_string = floatToString(middle_y_point_float);
                    middle_y_point_string = transferToTrackPoint(middle_y_point_string, M4);
                    // float power_stripe_float = stof(POWER_STRIPE_RESOURCE_WIDTH);
                    // float down_stripe_float = middle_y_point_float - (power_stripe_float / 2);
                    // float up_stripe_float = middle_y_point_float + (power_stripe_float / 2);

                    // step 4 : 在這邊做 design rule check

                    if (deisngRuleCheckAddPowerStripe(&left_stripe, &right_stripe, &middle_y_point_string, &position_design_rule_vdd_stripe_map))
                    {
                        middle_y_point_float = stof(middle_y_point_string);

                        float power_stripe_float = stof(POWER_STRIPE_RESOURCE_WIDTH);
                        float down_stripe_float = middle_y_point_float - (power_stripe_float / 2);
                        float up_stripe_float = middle_y_point_float + (power_stripe_float / 2);
                        Stripe stripe;
                        stripe.start_x_location = left_boundary_string;
                        stripe.end_x_location = right_boundary_string;
                        stripe.start_y_location = floatToString(down_stripe_float);
                        stripe.end_y_location = floatToString(up_stripe_float);
                        stripe.width = POWER_STRIPE_RESOURCE_WIDTH;
                        stripe.layer = M4;

                        // cout << "addStripe -nets { VDDX } -layer "
                        //      << stripe.layer
                        //      << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << " }" << endl;
                        // (*cluster_vector)[i].add_stripe_vector.push_back(stripe);

                        // if (!(rowBlockRuleCheck(&row_block_vector, &stripe, middle_y_point_string)))
                        // {
                        cout << "addStripe -nets { VDDX } -layer "
                             << stripe.layer
                             << " -direction horizontal -width " << stripe.width << " -set_to_set_distance 12.88 -number_of_sets 1  -area { " << stripe.start_x_location << " " << stripe.start_y_location << " " << stripe.end_x_location << " " << stripe.end_y_location << " }" << endl;
                        (*cluster_vector)[i].add_stripe_vector.push_back(stripe);
                        // }
                    }
                    // Stripe stripe;
                    // stripe.start_x_location = lef_point_location;
                    // stripe.end_x_location = right_point_location;
                    // stripe.start_y_location = floatToString(down_stripe_float);
                    // stripe.end_y_location = floatToString(up_stripe_float);
                    // stripe.layer = M4;
                    // stripe.width = POWER_STRIPE_RESOURCE_WIDTH;
                    // (*cluster_vector)[i].add_stripe_vector.push_back(stripe);

                    count = 1;
                }

                count += 1;
            }
        }
    }
    cout << "========== generateAddPowerStripe end ========== " << endl;
}

// void generateAddPowerStripe(vector<Cluster> *cluster_vector, vector<Cluster> *revise_cluster_vector, unordered_map<string, vector<Stripe>> *vdd_stripe_map)
// {
//     cout << "========== generateAddPowerStripe start ========== " << endl;
//     // const string POWER_STRIPE_RESOURCE_HEIGHT = "588.096";
//     // const string POWER_STRIPE_RESOURCE_WIDTH = "0.224";
//     vector<Stripe> vdd_stripe_vector = (*vdd_stripe_map)[M3];

//     float power_stripe_resource_height = stof(POWER_STRIPE_RESOURCE_HEIGHT);
//     sort((*cluster_vector).begin(), (*cluster_vector).end(), sortClusterIrDropVector);
//     int step_int = innovusPointFloatToInt(0.864);
//     int over_float_cell_row_int = ADDSTRIPEOFCELLROW * 2;
//     int add_stripe_limit = over_float_cell_row_int * step_int;

//     for (int i = 0; i < (*cluster_vector).size(); i++)
//     {
//         // step 1 : 離 cluster left 最近的power stripe 但要靠左一點
//         string lef_point_location = getLeftStripeLocation(&vdd_stripe_vector, floatToString((*cluster_vector)[i].left_x_location));

//         // step 2 : 離 cluster right 最近的power stripe 但要靠右一點
//         string right_point_location = getRightStripeLocation(&vdd_stripe_vector, floatToString((*cluster_vector)[i].right_x_location));
//         // step 3 : 每5個cell row 加一條 power stripe 若低於五個 則加在中間的power stripe
//         float up_point_location = (*cluster_vector)[i].up_y_location;
//         float down_point_location = (*cluster_vector)[i].down_y_location;
//         // float innovusPointIntToFloat(int number);
//         // int innovusPointFloatToInt(float location);
//         int up_point_location_int = innovusPointFloatToInt(up_point_location);
//         int down_point_location_int = innovusPointFloatToInt(down_point_location);
//         int vertical_distance = abs(up_point_location_int - down_point_location_int);

//         // cout << "vertical_distance : " << innovusPointIntToFloat(vertical_distance) << endl;
//         // cout << "add_stripe_limit : " << innovusPointIntToFloat(add_stripe_limit) << endl;

//         if (vertical_distance < add_stripe_limit)
//         {

//             int middle_location_int = (up_point_location_int + down_point_location_int) / 2;
//             float middle_y_location_float = innovusPointIntToFloat(middle_location_int);

//             // cout << "is not over_flow : " << endl;
//             // cout << "up_point_location : " << up_point_location << endl;
//             // cout << "down_point_location : " << down_point_location << endl;
//             // cout << "middle_y_location_float :" << middle_y_location_float << endl;
//             // cout << "---------------------" << endl;
//             float power_stripe_float = stof(POWER_STRIPE_RESOURCE_WIDTH);
//             float down_stripe_float = middle_y_location_float - (power_stripe_float / 2);
//             float up_stripe_float = middle_y_location_float + (power_stripe_float / 2);
//             Stripe stripe;
//             stripe.start_x_location = lef_point_location;
//             stripe.end_x_location = right_point_location;
//             stripe.start_y_location = floatToString(down_stripe_float);
//             stripe.end_y_location = floatToString(up_stripe_float);
//             stripe.width = POWER_STRIPE_RESOURCE_WIDTH;
//             stripe.layer = M4;
//             (*cluster_vector)[i].add_stripe_vector.push_back(stripe);
//         }
//         else
//         {
//             // cout << "is  over_flow : " << endl;
//             // cout << up_point_location << endl;
//             // cout << down_point_location << endl;
//             // int count = 1;
//             // cout << "------------------------------------------" << endl;
//             // cout << "cluster id " << (*cluster_vector)[i].cluster_id << endl;
//             int count = 1;
//             for (int j = down_point_location_int; j <= up_point_location_int; j += step_int)
//             {
//                 int up_point_float_end_int = 0;
//                 int down_point_start_int = 0;
//                 if (count == 1)
//                 {
//                     down_point_start_int = j;
//                 }
//                 if ((count % 5 == 0))
//                 {
//                     up_point_float_end_int = j;
//                     int middle_point_int = (up_point_float_end_int + down_point_start_int) / 2;
//                     float middle_y_point_float = innovusPointIntToFloat(middle_point_int);
//                     float power_stripe_float = stof(POWER_STRIPE_RESOURCE_WIDTH);
//                     float down_stripe_float = middle_y_point_float - (power_stripe_float / 2);
//                     float up_stripe_float = middle_y_point_float + (power_stripe_float / 2);

//                     Stripe stripe;
//                     stripe.start_x_location = lef_point_location;
//                     stripe.end_x_location = right_point_location;
//                     stripe.start_y_location = floatToString(down_stripe_float);
//                     stripe.end_y_location = floatToString(up_stripe_float);
//                     stripe.layer = M4;
//                     stripe.width = POWER_STRIPE_RESOURCE_WIDTH;
//                     (*cluster_vector)[i].add_stripe_vector.push_back(stripe);

//                     count = 1;
//                 }

//                 count += 1;
//             }
//         }
//     }

//     cout << "========== generateAddPowerStripe end ========== " << endl;
// }

string getLeftStripeLocation(vector<Stripe> *vdd_stripe_vector, string left_point_location)
{
    float distance = 100000000;
    string add_stripe_x_location = "";
    float left_point_location_float = stof(left_point_location);

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        float stripe_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);

        float stripe_width_float = stof((*vdd_stripe_vector)[i].width);
        stripe_width_float = stripe_width_float / 2;

        stripe_x_location_float = stripe_x_location_float - stripe_width_float;

        float temp_distance = abs((stripe_x_location_float - left_point_location_float));

        if (stripe_x_location_float <= left_point_location_float && temp_distance <= distance)
        {
            distance = temp_distance;
            add_stripe_x_location = floatToString(stripe_x_location_float);
        }
    }

    return add_stripe_x_location;
}

string getRightStripeLocation(vector<Stripe> *vdd_stripe_vector, string right_point_location)
{
    float distance = 100000000;
    string add_stripe_x_location = "";
    float right_point_location_float = stof(right_point_location);
    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        float stripe_x_location_float = stof((*vdd_stripe_vector)[i].start_x_location);

        float stripe_width_float = stof((*vdd_stripe_vector)[i].width);
        stripe_width_float = stripe_width_float / 2;
        stripe_x_location_float = stripe_x_location_float + stripe_width_float;
        float temp_distance = abs((stripe_x_location_float - right_point_location_float));

        if (stripe_x_location_float >= right_point_location_float && temp_distance <= distance)
        {
            distance = temp_distance;
            add_stripe_x_location = floatToString(stripe_x_location_float);
        }
    }

    return add_stripe_x_location;
}

void setClusterBoundary(vector<Cluster> *cluster_vector)
{
    sort((*cluster_vector).begin(), (*cluster_vector).end(), sortClusterIrDropVector);
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        for (int j = 0; j < (*cluster_vector)[i].cell_placed_info_vector.size(); j++)
        {
            float point_x_location = stof((*cluster_vector)[i].cell_placed_info_vector[j].middle_x_location);
            float point_y_location = stof((*cluster_vector)[i].cell_placed_info_vector[j].middle_y_location);

            if ((*cluster_vector)[i].left_x_location > point_x_location)
            {
                (*cluster_vector)[i].left_x_location = point_x_location;
            }
            if ((*cluster_vector)[i].right_x_location < point_x_location)
            {
                (*cluster_vector)[i].right_x_location = point_x_location;
            }
            if ((*cluster_vector)[i].up_y_location < point_y_location)
            {
                (*cluster_vector)[i].up_y_location = point_y_location;
            }
            if ((*cluster_vector)[i].down_y_location > point_y_location)
            {
                (*cluster_vector)[i].down_y_location = point_y_location;
            }
        }

        float height = abs((*cluster_vector)[i].up_y_location - (*cluster_vector)[i].down_y_location);
        float width = abs((*cluster_vector)[i].left_x_location - (*cluster_vector)[i].right_x_location);
        (*cluster_vector)[i].height = height;
        (*cluster_vector)[i].width = width;
    }

    // for (int i = 0; i < (*cluster_vector).size(); i++)
    // {
    //     cout << "cluster id : " << (*cluster_vector)[i].cluster_id << endl;
    //     cout << "ir drop count  : " << (*cluster_vector)[i].average_ir_drop << endl;
    //     cout << "location   : " << (*cluster_vector)[i].left_x_location << " " << (*cluster_vector)[i].down_y_location << " " << (*cluster_vector)[i].right_x_location << " " << (*cluster_vector)[i].up_y_location << endl;
    // }
}

bool sortClusterIrDropVector(Cluster cluster_a, Cluster cluster_b)
{
    return stof(cluster_a.average_ir_drop) > stof(cluster_b.average_ir_drop);
}
bool sortCellPlacedIrDropVector(CellPlacedInfo cell_placed_a, CellPlacedInfo cell_placed_b)
{
    return stof(cell_placed_a.ir_drop_count) > stof(cell_placed_b.ir_drop_count);
}
// Method 1 : 每個 cluster 最嚴重的20個中心點去比
void getAddStripeTclFirstMethod(unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector)
{
    // sort(track_point.begin(), track_point.end(), sortTrackPointVector);

    // step 1 : 挑出最嚴重的 20 point
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        if ((*cluster_vector)[i].point_vector.size() > FIRSTMETHODNUMBEROFIRPOINT)
        {
            // cout << "before : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << (*cluster_vector)[i].cell_placed_info_vector.size() << endl;
            vector<CellPlacedInfo> cell_placed_info_vector = (*cluster_vector)[i].cell_placed_info_vector;
            int pop_size = cell_placed_info_vector.size();
            sort(cell_placed_info_vector.begin(), cell_placed_info_vector.end(), sortCellPlacedIrDropVector);
            for (int j = (FIRSTMETHODNUMBEROFIRPOINT + 1); j <= pop_size; j++)
            {
                cell_placed_info_vector.pop_back();
            }
            float total_ir_drop = 0;
            for (int j = 0; j < cell_placed_info_vector.size(); j++)
            {
                float ir_drop_count_float = stof(cell_placed_info_vector[j].ir_drop_count);
                total_ir_drop += ir_drop_count_float;
            }

            float average_ir_drop = total_ir_drop / cell_placed_info_vector.size();
            (*cluster_vector)[i].average_ir_drop = floatToString(average_ir_drop);
            (*cluster_vector)[i].cell_placed_info_vector = cell_placed_info_vector;
            // cout << "after : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << cell_placed_info_vector.size() << endl;
        };
    }
}

// Method 2 : 整個IR DROP 的點 前5% 最嚴重的部分去改善
void getAddStripeTclSecondMethod(unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector)
{
    // sort(track_point.begin(), track_point.end(), sortTrackPointVector);

    // step 1 :
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        if ((*cluster_vector)[i].point_vector.size() > 0)
        {
            // cout << "before : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << (*cluster_vector)[i].cell_placed_info_vector.size() << endl;
            vector<CellPlacedInfo> cell_placed_info_vector = (*cluster_vector)[i].cell_placed_info_vector;
            int pop_size = cell_placed_info_vector.size();
            sort(cell_placed_info_vector.begin(), cell_placed_info_vector.end(), sortCellPlacedIrDropVector);
            float total_ir_drop = 0;
            for (int j = 0; j < cell_placed_info_vector.size(); j++)
            {
                float ir_drop_count_float = stof(cell_placed_info_vector[j].ir_drop_count);
                total_ir_drop += ir_drop_count_float;
            }

            float average_ir_drop = total_ir_drop / cell_placed_info_vector.size();
            (*cluster_vector)[i].average_ir_drop = floatToString(average_ir_drop);
            (*cluster_vector)[i].cell_placed_info_vector = cell_placed_info_vector;
            // cout << "after : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << cell_placed_info_vector.size() << endl;
        };
    }
}

// Method 3 : 整個IR DROP range 1 去改善
void getAddStripeTclThirdMethod(unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector)
{
    cout << "========== getAddStripeTclThirdMethod start ========== " << endl;
    // sort(track_point.begin(), track_point.end(), sortTrackPointVector);

    // step 1 :
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        if ((*cluster_vector)[i].point_vector.size() > FIRSTMETHODNUMBEROFIRPOINT)
        {
            // cout << "before : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << (*cluster_vector)[i].cell_placed_info_vector.size() << endl;
            vector<CellPlacedInfo> cell_placed_info_vector = (*cluster_vector)[i].cell_placed_info_vector;
            int pop_size = cell_placed_info_vector.size();
            sort(cell_placed_info_vector.begin(), cell_placed_info_vector.end(), sortCellPlacedIrDropVector);
            float total_ir_drop = 0;
            for (int j = 0; j < cell_placed_info_vector.size(); j++)
            {
                float ir_drop_count_float = stof(cell_placed_info_vector[j].ir_drop_count);
                total_ir_drop += ir_drop_count_float;
            }

            float average_ir_drop = total_ir_drop / cell_placed_info_vector.size();
            (*cluster_vector)[i].average_ir_drop = floatToString(average_ir_drop);
            (*cluster_vector)[i].cell_placed_info_vector = cell_placed_info_vector;
            // cout << "after : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << cell_placed_info_vector.size() << endl;
        };
    }
    cout << "========== getAddStripeTclThirdMethod end ========== " << endl;
}

void setIrDropInCluster(unordered_map<string, Cluster> *cluster_map, unordered_map<string, CellPlacedInfo> *cell_location_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<Cluster> *cluster_vector)
{

    for (auto cluster_map_it = (*cluster_map).begin(); cluster_map_it != (*cluster_map).end(); ++cluster_map_it)
    {
        string cluster_id = cluster_map_it->first;
        // 左 下 右 上
        vector<Point> point_vector = (*cluster_map)[cluster_id].point_vector;
        float tota_ir_drop = 0;
        for (int i = 0; i < point_vector.size(); i++)
        {
            string x_location = floatToString(point_vector[i].x);
            string y_location = floatToString(point_vector[i].y);
            string point_location = x_location + "_" + y_location;

            if ((*cell_location_map).count(point_location) != 0)
            {
                string cell_id = (*cell_location_map)[point_location].cell_id;
                string ir_drop = (*cell_placed_map)[cell_id].ir_drop_count;
                (*cluster_map)[cluster_id].cell_placed_info_vector.push_back((*cell_placed_map)[cell_id]);
                float ir_drop_float = stof(ir_drop);
                tota_ir_drop += ir_drop_float;
            }
            else
            {
                cout << "cell location error " << endl;
            }
        }
        float average = tota_ir_drop / (point_vector.size());
        (*cluster_map)[cluster_id].average_ir_drop = floatToString(average);
    }

    for (auto cluster_map_it = (*cluster_map).begin(); cluster_map_it != (*cluster_map).end(); ++cluster_map_it)
    {
        string cluster_id = cluster_map_it->first;
        Cluster cluster = cluster_map_it->second;
        cluster.cluster_id = cluster_id;
        if (cluster.cluster_id != "-1")
        {
            (*cluster_vector).push_back(cluster_map_it->second);
        }
        // cout << "cluster id : " << cluster_id << " ir_drop_count : " << cluster_map_it->second.average_ir_drop << endl;
        // cout << "location : " << cluster_map_it->second.left_x_location << " " << cluster_map_it->second.down_y_location << " " << cluster_map_it->second.right_x_location << " " << cluster_map_it->second.up_y_location << endl;
        // cout << "===================================================================" << endl;
    }

    cout << " getBoudaryClusterAndIrDrop end" << endl;
}

void printCellIrDropPointMap(unordered_map<string, CellPlacedInfo> *cell_placed_map, string ir_point_file)
{
    ofstream myfile;
    myfile.open(ir_point_file);
    vector<CellPlacedInfo> cell_placed_info_vector;
    for (auto cell_placed_map_it = (*cell_placed_map).begin(); cell_placed_map_it != (*cell_placed_map).end(); ++cell_placed_map_it)
    {
        string cell_id = cell_placed_map_it->first;
        string middle_x_location = (*cell_placed_map)[cell_id].middle_x_location;
        string middle_y_location = (*cell_placed_map)[cell_id].middle_y_location;
        float ir_drop_count = stof(cell_placed_map_it->second.ir_drop_count);

        if (ir_drop_count > 0)
        {
            cell_placed_info_vector.push_back((*cell_placed_map)[cell_id]);
        }
    }

    myfile << cell_placed_info_vector.size() << endl;
    for (int i = 0; i < cell_placed_info_vector.size(); i++)
    {
        myfile << cell_placed_info_vector[i].middle_x_location << "," << cell_placed_info_vector[i].middle_y_location << ","
               << "0,1" << endl;
        // myfile << cell_placed_info_vector[i].cell_id << " " << cell_placed_info_vector[i].middle_x_location << " " << cell_placed_info_vector[i].middle_y_location << endl;
    }

    myfile.close();
}

void setIrDropCost(unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *cell_location_map)
{
    for (auto cell_placed_map_it = (*cell_placed_map).begin(); cell_placed_map_it != (*cell_placed_map).end(); ++cell_placed_map_it)
    {
        float total_ir_drop_count = 0;
        string cell_id = cell_placed_map_it->first;
        vector<IrDropPoint> ir_drop_vector = cell_placed_map_it->second.ir_drop_point_vector;
        for (int i = 0; i < ir_drop_vector.size(); i++)
        {
            float ir_drop_count = stof(ir_drop_vector[i].ir_drop);
            total_ir_drop_count += ir_drop_count;
        }
        float size_float = stof(to_string(ir_drop_vector.size()));
        float average = total_ir_drop_count / size_float;
        (*cell_placed_map)[cell_id].ir_drop_count = floatToString(average);
    }
    int cell_count = 0;
    ofstream myfile;
    string file_name = "ir_drop_report_log_file.txt";
    myfile.open(file_name);
    for (auto cell_placed_map_it = (*cell_placed_map).begin(); cell_placed_map_it != (*cell_placed_map).end(); ++cell_placed_map_it)
    {
        string cell_id = cell_placed_map_it->first;

        if (stof((*cell_placed_map)[cell_id].ir_drop_count) > 0)
        {
            string middle_location = (*cell_placed_map)[cell_id].middle_x_location + "_" + (*cell_placed_map)[cell_id].middle_y_location;
            (*cell_location_map).insert(pair<string, CellPlacedInfo>(middle_location, (*cell_placed_map)[cell_id]));
            cell_count++;
            myfile << "cell id : " << (*cell_placed_map)[cell_id].cell_id << " ir_drop_count : " << (*cell_placed_map)[cell_id].ir_drop_count << endl;
        }
    }
    myfile.close();
    // cout << "cell count : " << cell_count << endl;
}

void setIrDropPointInRow(unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
{
    vector<IrDropPoint> ir_drop_point_vector = (*ir_drop_point_map)[M1];

    // for (auto ir_drop_point_map_it = (*ir_drop_point_map).begin(); ir_drop_point_map_it != (*ir_drop_point_map).end(); ++ir_drop_point_map_it)
    // {
    // string layer = ir_drop_point_map_it->first;
    int log = 0;
    for (int i = 0; i < (*ir_drop_point_map)[M1].size(); i++)
    {
        log++;
        string down_middle_location = (*ir_drop_point_map)[M1][i].down_middle_location;
        string up_middle_location = (*ir_drop_point_map)[M1][i].up_middle_location;
        string ir_drop_x_location = (*ir_drop_point_map)[M1][i].x_location;
        if (log % 1000 == 0)
        {
            cout << "ir data setting: " << log << endl;
        }
        if ((*row_map).count(down_middle_location) != 0)
        {
            setIrDropInCell(down_middle_location, ir_drop_x_location, &(*ir_drop_point_map)[M1][i], &(*row_map), &(*cell_placed_map));
        }
        else
        {
            // cout << "y_location      : " << ir_drop_point_map_it->second[i].y_location << endl;
            // cout << "middle_location : " << middle_location << endl;
            cout << "row map error down" << endl;
        }
        if ((*row_map).count(up_middle_location) != 0)
        {

            setIrDropInCell(up_middle_location, ir_drop_x_location, &(*ir_drop_point_map)[M1][i], &(*row_map), &(*cell_placed_map));
        }
        else
        {
            // cout << "y_location      : " << ir_drop_point_map_it->second[i].y_location << endl;
            // cout << "middle_location : " << middle_location << endl;
            cout << "row map error up" << endl;
        }
    }
    // }
    for (auto cell_placed_map_it = (*cell_placed_map).begin(); cell_placed_map_it != (*cell_placed_map).end(); ++cell_placed_map_it)
    {
        string cell_id = cell_placed_map_it->first;
        int cell_size = (*cell_placed_map)[cell_id].ir_drop_point_vector.size();
        // cout << " cell id   : " << cell_id << " cell size : " << cell_size << endl;
    }
}

void setIrDropInCell(string middle_location, string ir_drop_x_location, IrDropPoint *ir_drop_point, unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    for (int j = 0; j < (*row_map)[middle_location].cell_id_vector.size(); j++)
    {
        string cell_id = (*row_map)[middle_location].cell_id_vector[j];
        string left_x_location = (*cell_placed_map)[cell_id].left_x_location;
        string right_x_location = (*cell_placed_map)[cell_id].right_x_location;

        float left_x_location_float = stof(left_x_location);
        int left_x_location_int = innovusPointFloatToInt(left_x_location_float);
        float right_x_location_float = stof(right_x_location);
        int right_x_location_int = innovusPointFloatToInt(right_x_location_float);
        float ir_drop_x_location_float = stof(ir_drop_x_location);
        int ir_drop_x_location_int = innovusPointFloatToInt(ir_drop_x_location_float);

        if (ir_drop_x_location_int >= left_x_location_int && ir_drop_x_location_int <= right_x_location_int)
        {
            (*cell_placed_map)[cell_id].ir_drop_point_vector.push_back((*ir_drop_point));
            break;
        }
    }
}

bool isFillerCell(CellPlacedInfo *cell_placed_info)
{
    string cell_name = (*cell_placed_info).cell_name;
    if (cell_name.find("FILLER") != string::npos)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setCellInRowCore(unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    for (auto cell_placed_map_it = (*cell_placed_map).begin(); cell_placed_map_it != (*cell_placed_map).end(); ++cell_placed_map_it)
    {
        string middle_y_location = cell_placed_map_it->second.middle_y_location;
        string cell_id = cell_placed_map_it->first;
        if ((*row_map).count(middle_y_location) != 0)
        {
            // if (!isFillerCell(&cell_placed_map_it->second))
            // {
            (*row_map)[middle_y_location].cell_id_vector.push_back(cell_id);
            // }
        }
        else
        {
            cout << "cell id : " << cell_id << endl;
            cout << "middle_y_location : " << middle_y_location << endl;
            cout << "cell placed is error" << endl;
        }
    }

    // for (auto row_map_it = (*row_map).begin(); row_map_it != (*row_map).end(); ++row_map_it)
    // {
    //     cout << "row size : " << row_map_it->second.cell_id_vector.size() << endl;
    // }
}

void getRowCore(unordered_map<string, Row> *row_map, CoreSite *core_site)
{
    string step = "0.864";
    string down_boundary = (*core_site).down_y_location;
    string up_boundary = (*core_site).up_y_location;

    int step_point_step = innovusPointFloatToInt(stof(step));
    int down_boundary_step = innovusPointFloatToInt(stof(down_boundary));
    int up_boundary_step = innovusPointFloatToInt(stof(up_boundary));
    up_boundary_step = up_boundary_step - step_point_step;

    for (int i = down_boundary_step; i <= up_boundary_step; i += step_point_step)
    {
        Row row;
        float down_boundary_float = innovusPointIntToFloat(i);

        int up_boundary_int = i + step_point_step;
        float up_boundary_float = innovusPointIntToFloat(up_boundary_int);

        int boundry_step = (i + up_boundary_int);
        boundry_step = boundry_step / 2;
        float boundry_step_float = innovusPointIntToFloat(boundry_step);

        row.down_boundary = floatToString(down_boundary_float);
        row.up_boundary = floatToString(up_boundary_float);
        row.middle_location = floatToString(boundry_step_float);

        // (*boundary_row).push_back(row);
        (*row_map).insert(pair<string, Row>(row.middle_location, row));
    }

    // for (const auto &kv : (*row_map))
    // {
    //     std::cout << kv.first << " has value up " << kv.second.up_boundary << std::endl;
    //     std::cout << kv.first << " has value down " << kv.second.down_boundary << std::endl;
    // }
}
int innovusPointFloatToInt(float location)
{
    float temp = location * 1000.0;
    temp = round(temp);
    int temp_int = (int)temp;
    int ans = temp_int / 4;
    return ans;
}
float innovusPointIntToFloat(int number)
{
    return (number * 4) / 1000.0;
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
                        // string middle_location = cell_placed_info.middle_x_location + "_" + cell_placed_info.middle_y_location;
                        (*cell_placed_map).insert(pair<string, CellPlacedInfo>(cell_placed_info.cell_id, cell_placed_info));
                        // (*cell_location_map).insert(pair<string, CellPlacedInfo>(middle_location, cell_placed_info));
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

        float middle_y_location = (down_y_float + up_y_float) / 2;
        float middle_x_location = (right_x_float + left_x_float) / 2;

        (*cell_placed_info).middle_x_location = floatToString(middle_x_location);
        (*cell_placed_info).middle_y_location = floatToString(middle_y_location);
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
            reviseCellPlacedPosition(&(*cell_placed_info));
            break;
        }
    }
}
void reviseCellPlacedPosition(CellPlacedInfo *cell_placed_info)
{
    float left_x_location_float = stof((*cell_placed_info).left_x_location);
    float down_y_location_float = stof((*cell_placed_info).down_y_location);

    if (left_x_location_float > 4000)
    {
        // cout << "check in side : " << left_x_location_float << endl;
        left_x_location_float = left_x_location_float / 4000;
        (*cell_placed_info).left_x_location = floatToString(left_x_location_float);
    }
    if (down_y_location_float > 4000)
    {
        // cout << "check in side : " << down_y_location_float << endl;
        down_y_location_float = down_y_location_float / 4000;
        (*cell_placed_info).down_y_location = floatToString(down_y_location_float);
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
void printClusterReport(vector<Point> *points, unordered_map<string, Cluster> *cluster_map)
{

    for (int i = 0; i < (*points).size(); i++)
    {
        string cluster_id = to_string((*points)[i].clusterID);
        // cout << "cluster_id :" << cluster_id << endl;
        if ((*cluster_map).count(cluster_id) == 0)
        {
            vector<Point> point_vector;
            point_vector.push_back((*points)[i]);
            Cluster cluster;
            cluster.point_vector = point_vector;
            cluster.cluster_id = cluster_id;
            (*cluster_map).insert(pair<string, Cluster>(cluster_id, cluster));
        }
        else
        {
            (*cluster_map)[cluster_id].point_vector.push_back((*points)[i]);
        }
    }
    cout << "size : " << (*cluster_map).size() << endl;

    for (auto cluster_id_map_it = (*cluster_map).begin(); cluster_id_map_it != (*cluster_map).end(); ++cluster_id_map_it)
    {
        string cluster_id = cluster_id_map_it->first;
        ofstream myfile;
        string file_name = "cluster_report/cluster_id_report_" + cluster_id;
        myfile.open(file_name);

        for (int i = 0; i < (*cluster_map)[cluster_id].point_vector.size(); i++)
        {
            myfile << (*cluster_map)[cluster_id].point_vector[i].x << "," << (*cluster_map)[cluster_id].point_vector[i].y << endl;
            // cout << cluster_id_map[cluster_id][i].x << "," << cluster_id_map[cluster_id][i].y << endl;
        }
        myfile.close();
    }
}

void printIrDropReport(unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
{
    ofstream myfile;
    myfile.open("ir_drop_report");
    for (auto ir_drop_point_map_it = (*ir_drop_point_map).begin(); ir_drop_point_map_it != (*ir_drop_point_map).end(); ++ir_drop_point_map_it)
    {
        string layer = ir_drop_point_map_it->first;
        myfile << (*ir_drop_point_map)[layer].size() << endl;
        for (int i = 0; i < (*ir_drop_point_map)[layer].size(); i++)
        {
            myfile << (*ir_drop_point_map)[layer][i].x_location << "," << (*ir_drop_point_map)[layer][i].y_location << ","
                   << "0,1" << endl;
            // cout << (*ir_drop_point_map)[layer][i].x_location << " " << (*ir_drop_point_map)[layer][i].y_location << " " << (*ir_drop_point_map)[layer][i].ir_drop << endl;
        }
    }
    myfile.close();
}
// ,string y_location
void getIrDropMiddleLocation(IrDropPoint *ir_drop_point)
{
    string step = "0.864";
    string y_location = (*ir_drop_point).y_location;
    int step_point_int = innovusPointFloatToInt(stof(step));
    step_point_int = step_point_int / 2;
    int y_location_int = innovusPointFloatToInt(stof(y_location));
    int down_y_location_int = y_location_int - step_point_int;
    int up_y_location_int = y_location_int + step_point_int;

    // float y_location_float = innovusPointIntToFloat(y_location_int);
    // string y_location_str = floatToString(y_location_float);

    float up_y_location_float = innovusPointIntToFloat(up_y_location_int);
    float down_y_location_float = innovusPointIntToFloat(down_y_location_int);
    (*ir_drop_point).down_middle_location = floatToString(down_y_location_float);
    (*ir_drop_point).up_middle_location = floatToString(up_y_location_float);

    // cout << "y_location_float : " << y_location << endl;
    // cout << "y_location_float : " << y_location_float << endl;

    // return y_location_str;
}

void setIrDropReport(string method, vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map, string range_start, string range_end, string ir_drop_percent)
{
    cout << "========== setIrDropReport start ==========" << endl;
    // float range_start_float = stof(range_start);
    // float range_end_float = stof(range_end);
    // float ir_drop_percent_float = stof(ir_drop_percent);

    // range_start_float = 0.7 - range_start_float;
    // range_end_float = 0.7 - range_end_float;
    // //    0.51 ~ 0.44
    // float range = range_start_float - range_end_float;
    // range = range * ir_drop_percent_float;

    // range_start_float = range_start_float * 1000;
    // range_start_float += 1;
    // range_end_float = range_end_float * 1000;
    // range_end_float -= 1;
    // cout << "range_start_float : " << range_start_float << endl;
    // cout << "range_end_float : " << range_end_float << endl;

    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {
       
        string ir_drop_file_name = (*ir_drop_file_vector)[i];
         cout << "ir_drop_file_name : " << ir_drop_file_name << endl;

        vector<string> ir_file_content_array = splitByPattern(ir_drop_file_name, "/");

        string file_name_detail = ir_file_content_array[2];

        vector<string> file_content_array = splitByPattern(file_name_detail, "_");
        string layer = file_content_array[4];
        vector<IrDropPoint> ir_drop_point_vector;

        if (file_content_array.size() != 6)
        {
            cout << "ir_drop_report_error" << endl;
        }

        ifstream ir_file(ir_drop_file_name);

        string ir_content;
        int log = 0;
        if (ir_file)
        {
            while (getline(ir_file, ir_content))
            {
                if (ir_content.find("ir") != string::npos && ir_content.find("layer") != string::npos)
                {
                    continue;
                }
                else if (ir_content.find("Range") != string::npos && ir_content.find("Total") != string::npos)
                {
                    break;
                }
                else
                {
                    vector<string> ir_content_array = splitByPattern(ir_content, " ");
                    IrDropPoint ir_drop_point;
                    ir_drop_point.ir_drop = ir_content_array[0];
                    ir_drop_point.layer = ir_content_array[1];
                    ir_drop_point.x_location = ir_content_array[2];
                    ir_drop_point.y_location = ir_content_array[3];
                    // ir_drop_point.down_middle_location = getIrDropMiddleLocation(ir_drop_point.y_location);
                    getIrDropMiddleLocation(&ir_drop_point);
                    float ir_drop = stof(ir_drop_point.ir_drop);

                    // 51 ~ 44
                    // 0.649V -  0.656V

                    // if ((method == METHOD_1))
                    // {
                    //     if (ir_drop >= range_end_float && ir_drop <= range_start_float)
                    //     {
                    //         ir_drop_point_vector.push_back(ir_drop_point);
                    //     }
                    // }
                    if ((method == METHOD_2))
                    {

                        ir_drop_point_vector.push_back(ir_drop_point);
                    }

                    // if ((method == METHOD_3))
                    // {

                    //     if (ir_drop >= range_end_float && ir_drop <= range_start_float)
                    //     {
                    //         ir_drop_point_vector.push_back(ir_drop_point);
                    //     }
                    // }

                    log++;
                    if (log % 1000 == 0)
                    {
                        cout << "ir data : " << log << endl;
                    }
                }
            }
        }
        else
        {
            cout << " read ir file error " << endl;
        }
        cout << "ir_drop_size : " << ir_drop_point_vector.size() << endl;
        vector<IrDropPoint> revise_ir_drop_point;
        if (method == METHOD_2)
        {
            if (layer == M1)
            {
                cout << "before_size : " << ir_drop_point_vector.size() << endl;

                int ir_drop_size = (METHOD_2_IR_DROP_PERCENT * ir_drop_point_vector.size());

                for (int i = 0; i < ir_drop_size; i++)
                {
                    IrDropPoint ir_drop_point = ir_drop_point_vector[i];
                    revise_ir_drop_point.push_back(ir_drop_point);
                }
                cout << "after_size : " << revise_ir_drop_point.size() << endl;
                (*ir_drop_point_map).insert(pair<string, vector<IrDropPoint>>(layer, revise_ir_drop_point));
            }
            else
            {
                (*ir_drop_point_map).insert(pair<string, vector<IrDropPoint>>(layer, ir_drop_point_vector));
            }
        }
    }

    // for (auto ir_drop_point_map_it = (*ir_drop_point_map).begin(); ir_drop_point_map_it != (*ir_drop_point_map).end(); ++ir_drop_point_map_it)
    // {
    //     string layer = ir_drop_point_map_it->first;
    //     for (int i = 0; i < (*ir_drop_point_map)[layer].size(); i++)
    //     {
    //         cout << (*ir_drop_point_map)[layer][i].x_location << " " << (*ir_drop_point_map)[layer][i].y_location << endl;
    //         cout << (*ir_drop_point_map)[layer][i].up_middle_location << " " << (*ir_drop_point_map)[layer][i].down_middle_location << endl;
    //         cout << "=========================================" << endl;
    //     }
    // }
    cout << "========== setIrDropReport end ==========" << endl;
}

void getStripeLocation(string def_transfer_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, string *power_stripe_width_resource)
{
    cout << "========== getStripeLocation start ==========" << endl;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;

    (*vdd_stripe_map).insert(pair<string, vector<Stripe>>(M3, vdd_stripe_vector));
    (*vss_stripe_map).insert(pair<string, vector<Stripe>>(M3, vss_stripe_vector));
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
                            Stripe stripe;

                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.net_name = NET_NAME_VDD;
                            stripe.layer = def_content_array[1];

                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                setStripeLength(&stripe);
                                stripe.start_x_location = floatToString(stof(stripe.start_x_location));
                                stripe.end_x_location = floatToString(stof(stripe.end_x_location));
                                stripe.start_y_location = floatToString(stof(stripe.start_y_location));
                                stripe.end_y_location = floatToString(stof(stripe.end_y_location));
                                (*power_stripe_width_resource) = stripe.length;
                                (*vdd_stripe_map)[M3].push_back(stripe);

                                // (*vdd_stripe_vector).push_back(stripe);
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
                            // cout << "VSSX :" << def_content << " " << def_content_array.size() << endl;
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location = def_content_array[12];
                            stripe.width = def_content_array[2];
                            stripe.net_name = NET_NAME_VSS;
                            stripe.layer = def_content_array[1];
                            if (isPowerStripe(&stripe, &(*core_site)))
                            {
                                setStripeLength(&stripe);
                                stripe.start_x_location = floatToString(stof(stripe.start_x_location));
                                stripe.end_x_location = floatToString(stof(stripe.end_x_location));
                                stripe.start_y_location = floatToString(stof(stripe.start_y_location));
                                stripe.end_y_location = floatToString(stof(stripe.end_y_location));
                                (*power_stripe_width_resource) = stripe.length;
                                (*vss_stripe_map)[M3].push_back(stripe);
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
void getStripeLocationFromStripeTcl(string stripe_tcl_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, set<string> *stripe_width_set, string *power_stripe_resource_width)
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
            (*power_stripe_resource_width) = stripe.length;

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

void readBenchmarkData(vector<Point> &points, string file_name)
{
    // load point cloud
    FILE *stream;
    stream = fopen(file_name.c_str(), "ra");

    unsigned int minpts, num_points, cluster, i = 0;
    double epsilon;
    fscanf(stream, "%u\n", &num_points);

    Point *p = (Point *)calloc(num_points, sizeof(Point));

    while (i < num_points)
    {
        fscanf(stream, "%f,%f,%f,%d\n", &(p[i].x), &(p[i].y), &(p[i].z), &cluster);
        p[i].clusterID = UNCLASSIFIED;
        points.push_back(p[i]);
        ++i;
    }

    free(p);
    fclose(stream);
}
void printResults(vector<Point> &points, int num_points)
{
    int i = 0;
    printf("Number of points: %u\n"
           " x     y     z     cluster_id\n"
           "-----------------------------\n",
           num_points);
    while (i < num_points)
    {
        printf("%5.2lf %5.2lf %5.2lf: %d\n",
               points[i].x,
               points[i].y, points[i].z,
               points[i].clusterID);
        ++i;
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

// =========== Kmeans  cluster algorithm start ===========
// ========= kmeans start ==========
// list<Point> pointlist;
// ========= kmeans end ==========
// std::clock_t start;
// double duration;
// Kmeans kmeans(5);
// kmeans.InitPoints(IR_DROP_POINT_FILE);
// start = std::clock();
// kmeans.InitCenters();
// // kmeans.InitSpecifiedCenters();
// kmeans.RunKmean();
// duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
// std::cout << "calculate time " << duration << std::endl;
// kmeans.SaveEPS("S1.eps");

// std::cout << "----------------- kmeans Plus Plus -----------------" << std::endl;
// KmeansPlusPlus kmeansPlusPlus(8);
// kmeansPlusPlus.InitPoints(IR_DROP_POINT_FILE);
// start = std::clock();
// kmeansPlusPlus.InitCenters();
// // kmeans.InitSpecifiedCenters();
// kmeansPlusPlus.RunKmean();
// duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
// std::cout << "calculate time " << duration << std::endl;
// kmeansPlusPlus.SaveEPS("S1_++.eps");
// kmeansPlusPlus.SaveSVG("S1_++.svg");
// system("pause");
// =========== Kmeans  cluster algorithm end ===========