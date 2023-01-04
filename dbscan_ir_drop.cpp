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
#include "D:\ir_drop_project\header\dbscan.h"

struct IrDropPoint
{
    string ir_drop;
    string layer;
    string x_location;
    string y_location;
};

#define MINIMUM_POINTS 5    // minimum number of cluster
#define EPSILON (1.5 * 1.5) // distance for clustering, metre^2

// Util
vector<string> splitByPattern(string content, string pattern);
string floatToString(const float value);
string &trim(string &str);
void printClusterReport(vector<Point> *points);
void printResults(vector<Point> &points, int num_points);

void setIrDropReport(vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
void printIrDropReport(unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map);
void readBenchmarkData(vector<Point> &points);
int main()
{
    // ir drop point map
    // ===========  ir drop map start ===========
    unordered_map<string, vector<IrDropPoint>> ir_drop_point_map;
    vector<string> ir_drop_file_vector;
    string ir_drop_file = "ir_report/M1_ir_drop_point_b19_31.report";
    ir_drop_file_vector.push_back(ir_drop_file);
    setIrDropReport(&ir_drop_file_vector, &ir_drop_point_map);
    // printIrDropReport(&ir_drop_point_map);
    // =========== ir drop map end ===========

    // =========== def file start ===========


    

    // =========== def file end ===========

    // ===========  cluster algorithm start ===========
    // vector<Point> points;
    // // read point data
    // readBenchmarkData(points);
    // // constructor
    // DBSCAN ds(MINIMUM_POINTS, EPSILON, points);
    // // main loop
    // ds.run();
    // // result of DBSCAN algorithm
    // printResults(ds.m_points, ds.getTotalPointSize());
    // printClusterReport(&(ds.m_points));
    // ===========  cluster algorithm end ===========

    return 0;
}
void printClusterReport(vector<Point> *points)
{
    unordered_map<string, vector<Point>> cluster_id_map;
    for (int i = 0; i < (*points).size(); i++)
    {
        string cluster_id = to_string((*points)[i].clusterID);
        // cout << "cluster_id :" << cluster_id << endl;
        if (cluster_id_map.count(cluster_id) == 0)
        {
            vector<Point> point_vector;
            point_vector.push_back((*points)[i]);
            cluster_id_map.insert(pair<string, vector<Point>>(cluster_id, point_vector));
        }
        else
        {
            cluster_id_map[cluster_id].push_back((*points)[i]);
        }
    }
    cout << "size : " << cluster_id_map.size() << endl;

    for (auto cluster_id_map_it = cluster_id_map.begin(); cluster_id_map_it != cluster_id_map.end(); ++cluster_id_map_it)
    {
        string cluster_id = cluster_id_map_it->first;
        ofstream myfile;
        string file_name = "cluster_report/cluster_id_report_" + cluster_id;
        myfile.open(file_name);

        for (int i = 0; i < cluster_id_map[cluster_id].size(); i++)
        {
            myfile << cluster_id_map[cluster_id][i].x << "," << cluster_id_map[cluster_id][i].y << endl;
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
void setIrDropReport(vector<string> *ir_drop_file_vector, unordered_map<string, vector<IrDropPoint>> *ir_drop_point_map)
{

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
        cout << "ir_drop_file_name : " << ir_drop_file_name << endl;
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

                    float ir_drop = stof(ir_drop_point.ir_drop);

                    // if (ir_drop > 71.0)
                    // {
                    ir_drop_point_vector.push_back(ir_drop_point);
                    // }

                    // ir_drop_point_vector.push_back(ir_drop_point);

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
        (*ir_drop_point_map).insert(pair<string, vector<IrDropPoint>>(layer, ir_drop_point_vector));
    }
}
void readBenchmarkData(vector<Point> &points)
{
    // load point cloud
    FILE *stream;
    stream = fopen("ir_drop_report", "ra");

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