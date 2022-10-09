#include <cstdlib>
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <cmath>
using namespace std;

float standardDeviation(vector<float> *num_vec)
{
    float sum = 0;
    float std1 = 0; // 標準差 平均值
    float mean = 0;
    for (int i = 0; i < (*num_vec).size(); i++)
    {
        sum+= (*num_vec)[i];
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

int main(int argc, char *argv[])
{
    vector<float> num_vec;
    num_vec.push_back(1.0);
    num_vec.push_back(3.0);
    num_vec.push_back(4.0);
    num_vec.push_back(4.0);
    num_vec.push_back(3.0);
    num_vec.push_back(9.0);
    float num = standardDeviation(&num_vec);

    cout << num << endl;

    // double table[6];
    // double sum = 0;        //總和
    // double std1 = 0, mean; // 標準差 平均值
    // for (int i = 0; i < 6; i++)
    // {
    //     cin >> table[i];
    //     sum += table[i];
    // }
    // mean = sum / 6;
    // cout << mean << endl;
    // for (int i = 0; i < 6; i++)
    // {
    //     std1 += (table[i] - mean) * (table[i] - mean);
    // }
    // std1 /= 6;
    // cout << std1 << endl;
    // std1 = sqrt(std1);
    // cout << std1 << endl;
    //        system("pause");
    return 0;
}