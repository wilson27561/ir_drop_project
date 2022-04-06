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

const string DEF_FILE = "6t49_b19_routing_88_9_floorplan.def";
const string DEF_TRANSFER_FILE = "6t49_b19_routing_88_9_floorplan_transfer.def";

struct Position
{
    string ori_position;
    string new_position;
};
vector<string> splitByPattern(string content, string pattern);
void transfer_Postion(unordered_map<string, Position> *position_map, vector<string> *congest_content_array);
string replacePosition(string congest_content, unordered_map<string, Position> *position_map);
string &trim(string &str);
void caculate(unordered_map<string, Position> *position_map, string congest_content);
void trimAllBlanket(string &s);
string eraseBrackets(string content);
string erasePattern(string content, char pattern);
bool AllisNum(string str); 
void subreplace(string &resource_str, string &sub_str, string &new_str);

int main()
{
    ifstream def_file(DEF_FILE);
    string def_content;
    ofstream myfile;
    myfile.open(DEF_TRANSFER_FILE);
    int i;
    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            unordered_map<string, Position> position_map;
            if (def_content.find("SPECIALNETS") != string::npos)
            {
                myfile << def_content << endl;
                while (getline(def_file, def_content))
                {
                    vector<string> def_content_array = splitByPattern(def_content, " ");
                    transfer_Postion(&position_map, &def_content_array);
                    
                    def_content = replacePosition(def_content, &position_map);
                    myfile << def_content << endl;
                     i++;
                       if(i%1000 == 0){
                            cout << "data : " << i << endl;
                        }
                }
            }
             else if (def_content.find("COMPONENT") != string::npos and ((def_content.find("COMPONENTPIN") != string::npos) == false))
            { 
               
                 myfile << def_content << endl;
                 while (getline(def_file, def_content))
                {
                    vector<string> def_content_array = splitByPattern(def_content, " ");
                    transfer_Postion(&position_map, &def_content_array);
                    def_content = replacePosition(def_content, &position_map);
                    myfile << def_content << endl;
                    if(def_content.find("END COMPONENTS") != string::npos){
                        break;
                    }
                     i++;
                       if(i%1000 == 0){
                            cout << "data : " << i << endl;
                        }
                }
            }
            else
            {
                myfile << def_content << endl;
                i++;
                if(i%1000 == 0){
                cout << "data : " << i << endl;
                }
            }
        }
    }else{
        cout << "can't found file" << endl;
    }
    def_file.close();
    myfile.close();

    return 0;
}

void transfer_Postion(unordered_map<string, Position> *position_map, vector<string> *def_content_array)
{
    for (int i = 0; i < (*def_content_array).size(); i++)
    {
        string def_content = (*def_content_array)[i];
        if (AllisNum(def_content) && def_content.length()>0 && (def_content != "0") && (def_content != "1"))
        {
          caculate(&(*position_map), def_content);
        }
    }
}

string replacePosition(string def_content,unordered_map<string, Position> *position_map){

  for (unordered_map<string, Position>::iterator position_map_it = (*position_map).begin(); position_map_it != (*position_map).end(); ++position_map_it){
        string ori_position = position_map_it->first;
        string new_position = position_map_it->second.new_position;
        // cout << "ori_position : " << ori_position << "new_position : " << new_position << endl;
        // if(def_content.find(ori_position)!=string::npos){
        //   subreplace(def_content,ori_position,new_position);
        // }
        if(def_content.find(ori_position)!=string::npos){
          subreplace(def_content,ori_position,new_position);
        }
  }

  (*position_map).erase((*position_map).begin(),(*position_map).end());

  return def_content;
}


void caculate(unordered_map<string, Position> *position_map, string def_content)
{

    Position position;
    position.ori_position = def_content;
    // cout << "congest_content : " <<congest_content << " ";
    trimAllBlanket(def_content);
    string position_content = eraseBrackets(def_content);
    position_content = erasePattern(position_content, ',');
    // cout << "first : " << position_content;
    float number = atoi(position_content.c_str());
    number = number / 4000;
    // cout << "number : " << number << endl;
    string new_position = to_string(number);
    position.new_position = new_position;

    // cout << "ori_position: " << position.ori_position << endl;
    // cout << "new_position: " << position.new_position << endl;
    
    (*position_map).insert(pair<string, Position>(position_content, position));
}

void subreplace(string &resource_str, string &sub_str, string &new_str)
{
  string::size_type pos = 0;
  while ((pos = resource_str.find(sub_str)) != string::npos) //替换所有指定子串
  {
    resource_str.replace(pos, sub_str.length(), new_str);
  }
}

//去掉所有的空白
void trimAllBlanket(string &s)
{
    int index = 0;
    if (!s.empty())
    {
        while ((index = s.find(' ', index)) != string::npos)
        {
            s.erase(index, 1);
        }
    }
}

// //去除括號
string eraseBrackets(string content)
{
    content.erase(remove(content.begin(), content.end(), '('), content.end());
    content.erase(remove(content.begin(), content.end(), ')'), content.end());
    return content;
}

// //去除括號
string erasePattern(string content, char pattern)
{
    content.erase(remove(content.begin(), content.end(), pattern), content.end());
    return content;
}

//根據pattern切字串
vector<string> splitByPattern(string content, string pattern)
{
    vector<string> words;
    size_t pos = 0;
    while ((pos = content.find(pattern)) != string::npos)
    {
        string word = content.substr(0, pos);
        words.push_back(trim(word));
        content.erase(0, pos + pattern.length());
    }
    words.push_back(trim(content));
    return words;
}

//去掉首尾全部空格
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

bool AllisNum(string str)  
{  
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    } 
    return true;
}