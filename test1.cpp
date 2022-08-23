#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>


using namespace std;

int getdir(string dir, vector<string> &files);
int main(){
    string dir = string("D:/ir_drop_project/congest_area_file");//資料夾路徑(絕對位址or相對位址)
    vector<string> files = vector<string>();
    getdir(dir, files);
	//輸出資料夾和檔案名稱於螢幕
    for(int i=0; i<files.size(); i++){
        cout << files[i] << endl;
    }
    system("pause");
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