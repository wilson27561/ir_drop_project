#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int getdir(string dir, vector<string> &files);

string floatToString(const float value)
{
    std::ostringstream out;
    out.precision(6);
    out << value;
    return out.str();
}

string check(float a){
    std::stringstream buf;
    buf.precision(3);
    buf.setf(std::ios::fixed);
    buf<<a;
    string str = buf.str();
    buf.clear();
    return str;
}

int main(){

    
    // cout << floatToString(0.0359802) << endl;
    check(0.0359802); 
 cout <<  check(0.0359802) << endl;
    return 0;
}
int getdir(string dir, vector<string> &files){
    DIR *dp;//創立資料夾指標
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL){
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    while((dirp = readdir(dp)) != NULL){//如果dirent指標非空
        files.push_back(string(dirp->d_name));//將資料夾和檔案名放入vector
    }
    closedir(dp);//關閉資料夾指標
    return 0;
}