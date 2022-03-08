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

const string CONGESTAREA_FILE = "congestArea_1.25";
const string CONGESTAREA_TRANSFER_FILE = "congestAreaAll_1.25_transfer.txt";

struct Position
{
  string ori_position;
  string new_position;
};

vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
void trimAllBlanket(string &s);
string eraseBrackets(string content);
string erasePattern(string content,char pattern);
void transfer_Postion(unordered_map<string, Position> *position_map,vector<string> *congest_content_array);
void caculate(unordered_map<string, Position> *position_map,string congest_content);
string replacePosition(string congest_content,unordered_map<string, Position> *position_map);
void subreplace(string &resource_str, string &sub_str, string &new_str);

int main()
{
  ifstream congest_area_file(CONGESTAREA_FILE);
  string congest_content;
  ofstream myfile;
  myfile.open(CONGESTAREA_TRANSFER_FILE);
  if (congest_area_file)
  {
    while (getline(congest_area_file, congest_content))
    { 
       vector<string> congest_content_array = splitByPattern(congest_content," ");
       unordered_map<string,Position> position_map;
       if(congest_content.find("V")!= string::npos){
          transfer_Postion(&position_map, &congest_content_array);
          congest_content = replacePosition(congest_content, &position_map);
          myfile << congest_content <<endl;
       }else{
          myfile << congest_content <<endl;
       }
       
    }
  }
  congest_area_file.close();


  return 0;
}

string replacePosition(string def_content,unordered_map<string, Position> *position_map){

  for (unordered_map<string, Position>::iterator position_map_it = (*position_map).begin(); position_map_it != (*position_map).end(); ++position_map_it){
        string ori_position = position_map_it->first;
        string new_position = position_map_it->second.new_position;
        if(def_content.find(ori_position)!=string::npos){
          subreplace(def_content,ori_position,new_position);
        }
  }
  return def_content;
}


void transfer_Postion(unordered_map<string, Position> *position_map,vector<string> *congest_content_array)
{
        for (int i = 0; i < 4; i++)
        {
          string congest_content = (*congest_content_array)[i];
          caculate(&(*position_map),congest_content);
        }
}

void caculate(unordered_map<string, Position> *position_map,string congest_content){

        Position position;
        position.ori_position = congest_content;
        // cout << "congest_content : " <<congest_content << " ";
        trimAllBlanket(congest_content);
        string position_content = eraseBrackets(congest_content);
        position_content = erasePattern(position_content,',');
        // cout << "first : " << position_content;
        float number = atoi(position_content.c_str());
        number = number/4000;
        // cout << "number : " << number << endl;
        string new_position = to_string(number);
        position.new_position = new_position;
        (*position_map).insert(pair<string,Position>(position_content,position));
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

void subreplace(string &resource_str, string &sub_str, string &new_str)
{
  string::size_type pos = 0;
  while ((pos = resource_str.find(sub_str)) != string::npos) //替换所有指定子串
  {
    resource_str.replace(pos, sub_str.length(), new_str);
  }
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


// //去除括號
string erasePattern(string content,char pattern)
{
  content.erase(remove(content.begin(), content.end(), pattern), content.end());
  return content;
}

// //去除括號
string eraseBrackets(string content)
{
  content.erase(remove(content.begin(), content.end(), '('), content.end());
  content.erase(remove(content.begin(), content.end(), ')'), content.end());
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