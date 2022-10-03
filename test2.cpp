
// C++ program to implement insertion sort using STL.
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct Point2
{
    int x;
    int y;
};

bool GreaterSort(Point2 a, Point2 b)
{
    return (a.x > b.x);
}

int main()
{
    vector<Point2> aaa;
    Point2 temp_a;
    temp_a.x = 10;
    temp_a.y = 20;

    Point2 temp_b;
    temp_b.x = 30;
    temp_b.y = 30;

    Point2 temp_c;
    temp_c.x = 15;
    temp_c.y = 15;
    aaa.push_back(temp_a);
    aaa.push_back(temp_b);
    aaa.push_back(temp_c);

    sort(aaa.begin(),aaa.end(),GreaterSort);

    for (int i = 0; i < aaa.size(); i++)
    {
       cout << aaa[i].x << endl;
    }
    
}