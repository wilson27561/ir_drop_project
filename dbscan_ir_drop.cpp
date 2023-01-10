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
    string average_ir_drop;
};

struct CellInfo
{
    string cell_name;
    string cell_height;
    string cell_width;
};

#define MINIMUM_POINTS 6 // minimum number of cluster
// #define EPSILON (0.864 * 0.864) // distance for clustering, metre^2
#define EPSILON 1.728 // distance for clustering, metre^2
// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
bool floatIsEqualOrLess(float a, float b);
bool floatIsEqualOrMore(float a, float b);
bool floatIsEqual(float a, float b);

void printClusterReport(vector<Point> *points, unordered_map<string, Cluster> *cluster_map);
void printResults(vector<Point> &points, int num_points);
void readBenchmarkData(vector<Point> &points, string file_name);

void getLefCellImformation(string LEF_FILE, unordered_map<string, CellInfo> *cell_info_map);
void setPlacePosition(vector<string> *def_content_array, CellPlacedInfo *cell_placed_info);
void getDefPlacedImformation(string def_transfer_file_name, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellInfo> *cell_info_map);
void getCellLocation(CellPlacedInfo *cell_placed_info, unordered_map<string, CellInfo> *cell_info_map);
void setIrDropReport(vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map, string range_start, string range_end, string ir_drop_percent);
void printIrDropReport(unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
void getRowCore(unordered_map<string, Row> *row_map);
void setCellInRowCore(unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map);
void reviseCellPlacedPosition(CellPlacedInfo *cell_placed_info);
void setIrDropPointInRow(unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
string getIrDropMiddleLocation(string y_location);
void setIrDropCost(unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, CellPlacedInfo> *cell_location_map);
void printCellIrDropPointMap(unordered_map<string, CellPlacedInfo> *cell_placed_map);
void transferCellIrDropPointMapToVector(unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<CellPlacedInfo> *cell_placed_vector);
void setIrDropInCell(string middle_location, string ir_drop_x_location, IrDropPoint *ir_drop_point, unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
void getBoudaryClusterAndIrDrop(unordered_map<string, Cluster> *cluster_map, unordered_map<string, CellPlacedInfo> *cell_location_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<Cluster> *cluster_vector);
float innovusPointIntToFloat(int number);
int innovusPointFloatToInt(float location);
void getStripeLocation(string def_transfer_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector, CoreSite *core_site);
bool isPowerStripe(Stripe *stripe, CoreSite *core_site);
void getCoreSite(string DEF_TRANSFER_FILE_name, CoreSite *core_site);
void setIrDropInCluster(unordered_map<string, Cluster> *cluster_map, unordered_map<string, CellPlacedInfo> *cell_location_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<Cluster> *cluster_vector);
bool isOddLayer(string layer);
void setClusterBoundary(vector<Cluster> *cluster_vector);
void transferStripePositionFromTcl(string layer, Stripe *stripe, string start_x_location, string start_y_location, string end_x_location, string end_y_location);
void setStripeLength(Stripe *stripe);
void getAddStripeTclFirstMethod(vector<Stripe> *add_stripe_tcl, unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector);
void getStripeLocationFromStripeTcl(string stripe_tcl_file_name, unordered_map<string, vector<Stripe>> *vdd_stripe_map, unordered_map<string, vector<Stripe>> *vss_stripe_map, CoreSite *core_site, set<string> *stripe_width_set);
const string LEF_FILE = "tech_lef_file/characterization_6T_ALL_20200610area_4x.lef";
const string DEF_TRANSFER_FILE = "def_file/gpu_nerualNetwork_ricsv_aes_8pad/6t49_b19_routing_run0_31_transfer.def";
const string IR_DROP_POINT_FILE = "cell_ir_drop_point_report";
const string STRIPE_TCL = "stripe_tcl/moving_stripe_b19_pin_access_power_8pad.tcl";
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";

// add power stripe config
const string IR_DROP_RANGE_START = "0.649";
const string IR_DROP_RANGE_END = "0.656";
// 294.048(一條power stripe 長度)
const string POWER_STRIPE_RESOURCE_HEIGHT = "588.096";
const string POWER_STRIPE_RESOURCE_WIDTH = "0.224";
// 50%
const string IR_DROP_PERCENT = "0.5";
const int FIRSTMETHODNUMBEROFIRPOINT = 20;

int main()
{

    unordered_map<string, CellInfo> cell_info_map;
    unordered_map<string, CellPlacedInfo> cell_placed_map;
    // ir drop point map
    // ===========  ir drop map start ===========
    unordered_map<string, vector<IrDropPoint>> ir_drop_point_map;
    vector<Row> boundary_row;
    unordered_map<string, Row> row_map;
    vector<string> ir_drop_file_vector;
    vector<Cluster> cluster_vector;
    unordered_map<string, Cluster> cluster_map;
    unordered_map<string, CellPlacedInfo> cell_location_map;
    CoreSite core_site;
    vector<Stripe> vdd_stripe_vector;
    vector<Stripe> vss_stripe_vector;
    unordered_map<string, vector<Stripe>> vdd_stripe_map;
    unordered_map<string, vector<Stripe>> vss_stripe_map;
    vector<Stripe> add_stripe_vector;
    set<string> stripe_width_set;

    string ir_drop_file = "ir_report/M1_ir_drop_point_b19_31.report";
    // ========= kmeans start ==========
    // list<Point> pointlist;
    // ========= kmeans end ==========
    ir_drop_file_vector.push_back(ir_drop_file);
    setIrDropReport(&ir_drop_file_vector, &ir_drop_point_map, IR_DROP_RANGE_START, IR_DROP_RANGE_END, IR_DROP_PERCENT);
    // printIrDropReport(&ir_drop_point_map);
    // // =========== ir drop map end ===========

    // // =========== def file start ===========
    getLefCellImformation(LEF_FILE, &cell_info_map);
    getDefPlacedImformation(DEF_TRANSFER_FILE, &cell_placed_map, &cell_info_map);
    getRowCore(&row_map);
    setCellInRowCore(&row_map, &cell_placed_map);
    setIrDropPointInRow(&row_map, &cell_placed_map, &ir_drop_point_map);
    setIrDropCost(&cell_placed_map, &cell_location_map);
    printCellIrDropPointMap(&cell_placed_map);
    // // =========== def file end ===========

    // =========== dbscan  cluster algorithm start ===========
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

    // for (int i = 0; i < cluster_vector.size(); i++)
    // {
    //     cout << "point size : " << cluster_vector[i].point_vector.size() << endl;
    //     cout << "placed size : " << cluster_vector[i].cell_placed_info_vector.size() << endl;
    // }
    // =========== dbscan cluster algorithm end ===========

    // =========== get Add Power Stripe start ============

    getCoreSite(DEF_TRANSFER_FILE, &core_site);
    // get Stripe from def TODO Layer
    // getStripeLocation(DEF_TRANSFER_FILE, &vdd_stripe_vector, &vss_stripe_vector, &core_site);
    // get Stripe from tcl
    getStripeLocationFromStripeTcl(STRIPE_TCL, &vdd_stripe_map, &vss_stripe_map, &core_site, &stripe_width_set);

    // method 1 :
    getAddStripeTclFirstMethod(&add_stripe_vector, &cell_location_map, &cluster_vector);
    setClusterBoundary(&cluster_vector);
    // method 2 :

    // method 3 :

    // =========== get Add Power Stripe end   ============

    // =========== Kmeans  cluster algorithm start ===========
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

    return 0;
}
void setClusterBoundary(vector<Cluster> *cluster_vector)
{
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        (*cluster_vector)[i].cell_placed_info_vector;
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
    }
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        cout << "cluster id : " << (*cluster_vector)[i].cluster_id << endl;
        cout << "location   : " << (*cluster_vector)[i].left_x_location << " " << (*cluster_vector)[i].down_y_location << " " << (*cluster_vector)[i].right_x_location << " " << (*cluster_vector)[i].up_y_location << endl;
    }
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
void getAddStripeTclFirstMethod(vector<Stripe> *add_stripe_tcl, unordered_map<string, CellPlacedInfo> *cell_location_map, vector<Cluster> *cluster_vector)
{
    // sort(track_point.begin(), track_point.end(), sortTrackPointVector);

    // step 1 : 挑出最嚴重的 20 point
    for (int i = 0; i < (*cluster_vector).size(); i++)
    {
        if ((*cluster_vector)[i].point_vector.size() > FIRSTMETHODNUMBEROFIRPOINT)
        {
            cout << "before : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << (*cluster_vector)[i].cell_placed_info_vector.size() << endl;
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
            cout << "after : " << (*cluster_vector)[i].average_ir_drop << " cluster id : " << (*cluster_vector)[i].cluster_id << " cluster placed size : " << cell_placed_info_vector.size() << endl;
        };
    }
}

void setIrDropInCluster(unordered_map<string, Cluster> *cluster_map, unordered_map<string, CellPlacedInfo> *cell_location_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, vector<Cluster> *cluster_vector)
{
    // for (auto cluster_map_it = (*cluster_map).begin(); cluster_map_it != (*cluster_map).end(); ++cluster_map_it)
    // {
    //     string cluster_id = cluster_map_it->first;
    //     // 左 下 右 上
    //     vector<Point> point_vector = (*cluster_map)[cluster_id].point_vector;
    //     for (int i = 0; i < point_vector.size(); i++)
    //     {
    //         float point_x_location = point_vector[i].x;
    //         float point_y_location = point_vector[i].y;

    //         if ((*cluster_map)[cluster_id].left_x_location > point_x_location)
    //         {
    //             (*cluster_map)[cluster_id].left_x_location = point_x_location;
    //         }
    //         if ((*cluster_map)[cluster_id].right_x_location < point_x_location)
    //         {
    //             (*cluster_map)[cluster_id].right_x_location = point_x_location;
    //         }
    //         if ((*cluster_map)[cluster_id].up_y_location < point_y_location)
    //         {
    //             (*cluster_map)[cluster_id].up_y_location = point_y_location;
    //         }
    //         if ((*cluster_map)[cluster_id].down_y_location > point_y_location)
    //         {
    //             (*cluster_map)[cluster_id].down_y_location = point_y_location;
    //         }
    //     };
    // }

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
        (*cluster_vector).push_back(cluster_map_it->second);
        // cout << "cluster id : " << cluster_id << " ir_drop_count : " << cluster_map_it->second.average_ir_drop << endl;
        // cout << "location : " << cluster_map_it->second.left_x_location << " " << cluster_map_it->second.down_y_location << " " << cluster_map_it->second.right_x_location << " " << cluster_map_it->second.up_y_location << endl;
        // cout << "===================================================================" << endl;
    }

    cout << " getBoudaryClusterAndIrDrop end" << endl;
}

void printCellIrDropPointMap(unordered_map<string, CellPlacedInfo> *cell_placed_map)
{
    ofstream myfile;
    myfile.open(IR_DROP_POINT_FILE);
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
        // myfile << cell_placed_info_vector[i].middle_x_location << " " << cell_placed_info_vector[i].middle_y_location << endl;
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
    for (auto ir_drop_point_map_it = (*ir_drop_point_map).begin(); ir_drop_point_map_it != (*ir_drop_point_map).end(); ++ir_drop_point_map_it)
    {
        string layer = ir_drop_point_map_it->first;
        int log = 0;
        for (int i = 0; i < ir_drop_point_map_it->second.size(); i++)
        {
            log++;
            string down_middle_location = ir_drop_point_map_it->second[i].down_middle_location;
            string up_middle_location = ir_drop_point_map_it->second[i].up_middle_location;
            string ir_drop_x_location = ir_drop_point_map_it->second[i].x_location;
            if (log % 1000 == 0)
            {
                cout << "ir data setting: " << log << endl;
            }
            if ((*row_map).count(down_middle_location) != 0)
            {
                setIrDropInCell(down_middle_location, ir_drop_x_location, &ir_drop_point_map_it->second[i], &(*row_map), &(*cell_placed_map), &(*ir_drop_point_map));
            }
            else
            {
                // cout << "y_location      : " << ir_drop_point_map_it->second[i].y_location << endl;
                // cout << "middle_location : " << middle_location << endl;
                cout << "row map error down" << endl;
            }
            if ((*row_map).count(up_middle_location) != 0)
            {

                setIrDropInCell(up_middle_location, ir_drop_x_location, &ir_drop_point_map_it->second[i], &(*row_map), &(*cell_placed_map), &(*ir_drop_point_map));
            }
            else
            {
                // cout << "y_location      : " << ir_drop_point_map_it->second[i].y_location << endl;
                // cout << "middle_location : " << middle_location << endl;
                cout << "row map error up" << endl;
            }
        }
    }
    for (auto cell_placed_map_it = (*cell_placed_map).begin(); cell_placed_map_it != (*cell_placed_map).end(); ++cell_placed_map_it)
    {
        string cell_id = cell_placed_map_it->first;
        int cell_size = (*cell_placed_map)[cell_id].ir_drop_point_vector.size();
        // cout << " cell id   : " << cell_id << " cell size : " << cell_size << endl;
    }
}

void setIrDropInCell(string middle_location, string ir_drop_x_location, IrDropPoint *ir_drop_point, unordered_map<string, Row> *row_map, unordered_map<string, CellPlacedInfo> *cell_placed_map, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
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

void getRowCore(unordered_map<string, Row> *row_map)
{
    string step = "0.864";
    string down_boundary = "10.08";
    string up_boundary = "296.928";

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

void setIrDropReport(vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map, string range_start, string range_end, string ir_drop_percent)
{

    float range_start_float = stof(range_start);
    float range_end_float = stof(range_end);
    float ir_drop_percent_float = stof(ir_drop_percent);

    range_start_float = 0.7 - range_start_float;
    range_end_float = 0.7 - range_end_float;
    //    0.51 ~ 0.44
    float range = range_start_float - range_end_float;
    range = range * ir_drop_percent_float;
    range_end_float = range_end_float + range;

    range_start_float = range_start_float * 1000;
    range_end_float = range_end_float * 1000;

    for (int i = 0; i < (*ir_drop_file_vector).size(); i++)
    {

        string ir_drop_file_name = (*ir_drop_file_vector)[i];

        vector<string> ir_file_content_array = splitByPattern(ir_drop_file_name, "/");

        string file_name_detail = ir_file_content_array[1];
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

                    // if (ir_drop > 44.0)
                    // {
                    //     ir_drop_point_vector.push_back(ir_drop_point);
                    // }
                    if (ir_drop >= range_end_float && ir_drop <= range_start_float)
                    {
                        ir_drop_point_vector.push_back(ir_drop_point);
                    }
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
        (*ir_drop_point_map).insert(pair<string, vector<IrDropPoint>>(layer, ir_drop_point_vector));
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