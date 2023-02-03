#include <iostream>
using namespace std;

struct Cluster
{
    int left_x_location; // 左上角x坐标
    int up_y_location; // 左上角Y坐标
    int height; // 矩形的高
    int width; // 矩形的宽
};

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
int main()
{
    Cluster rect1 = {5, 5, 3, 3};
    Cluster rect2 = {1, 4, 6, 6};
    if (isInCluster(&rect1, &rect2))
    {
        cout << "is in side" << endl;
    }
    else
    {
        cout << "is out side" << endl;
    }
    return 0;
}