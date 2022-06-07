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
#include <cmath>
#include <iomanip>
#include <math.h>

void InsertionSort(vector<int> *num_vec)
{

    for (int i = 1; i < (*num_vec).size(); i++)
    {
        int key = (*num_vec)[i];
        cout << "key : "<< key << endl;
        int j = i - 1;
        while (key < (*num_vec)[j] && j >= 0)
        {
            (*num_vec)[j + 1] = (*num_vec)[j];
            j--;
        }
        (*num_vec)[j + 1] = key;
    }
}

int main()
{

    int array[6] = {5, 3, 1, 2, 6, 4};

    vector<int> num_vec;
    num_vec.push_back(5);
    num_vec.push_back(3);
    num_vec.push_back(1);
    num_vec.push_back(2);
    num_vec.push_back(6);



    InsertionSort(&num_vec);
    for (int i = 0; i < num_vec.size(); i++)
    {
        cout << num_vec[i] << endl;
    }
    

    std::cout << "sorted:\n";

    return 0;
}