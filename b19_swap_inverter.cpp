#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

struct Inverter
{
  string name;
  string id;
  string input_pin_name;
  string input_net_name;
  string output_pin_name;
  string output_net_name;
};
struct Input
{
  string ori_net_name;
  string revise_net_name;
};

struct LogicGate
{
  string name;
  string id;
  // key : A , value : key-> .A1 value-> \P1/P4/n1698
  map<string, map<string, Input>> input_map;
  map<string, string> output_map;
};

vector<string> splitByPattern(string content, string pattern);
string eraseBrackets(string content);
void getLogicGate(ifstream &fstream_b19_file, string &b19_content, unordered_map<string, LogicGate> *logic_gate_map);
void getInverter(ifstream &fstream_b19_file, string b19_content, unordered_map<string, Inverter> *inverter_map);
string getPinName(ifstream &fstream_b19_file, string b19_content);
string input_name_type(string input_pin_name);
string substrPinName(string content, string &start, string &end);
void subreplace(string &resource_str, string &sub_str, string &new_str);
string replaceLogic(string content, string logic_gate_id, ofstream &myfile, unordered_map<string, LogicGate> *logic_gate_map, unordered_map<string, string> *revise_logic_gate_name_map);
void getInverterContent(ifstream &fstream_b19_file, ofstream &myfile, string b19_content_revise, unordered_map<string, string> *invert_file_map);
void getlogicGateContent(ifstream &fstream_b19_file, ofstream &myfile, string b19_content, unordered_map<string, LogicGate> *logic_gate_map, unordered_map<string, string> *revise_logic_gate_name_map);
string replaceLogicName(string content, string logic_gate_id, string logic_gate_name, unordered_map<string, LogicGate> *logic_gate_map, unordered_map<string, string> *revise_logic_gate_name_map);
void transferLogicName(string &logic_gate_name, string &inv_gate);
string substr(string content, string &start, string &end);
void transferInverterFile(unordered_map<string, Inverter> *inverter_map, unordered_map<string, string> *invert_file_map);
void trimAllBlanket(string &s);
string &trim(string &str);
bool isContentReplaceNetName(string b_19_content, unordered_map<string, string> *invert_file_map);
void transferX1(unordered_map<string, LogicGate> *logic_gate_map);
regex input_name_regx("^.[A-D]$");
regex input_name_number_regx("^.[A-D][0-9]$");
const string INVERTER_FILE = "7_test.txt";
const string B19FILE = "b19_run2.v";
const string REVISE_FILE = "b19_swap_logic_gate.txt";
const string REVISE_LIST = "45list.txt";
int main()
{

  // Q: 是指說7_test.txt 裡面 inverter的 output 會對上b19_run2.v裡面 cell的input example Inverter net -> NOR2_xp33_75t .A1 .B ...
  //  找到後將此inverter的input換成.A1 or B的input.
  // Q: 7_test.txt 裡面INV_xp33_75t (n3317) 這個是input嗎？
  // Q: b19_run2.v的input作用為何？

  // ----- read inverter data from 7_test start -----
  ifstream fstream_7test_file(INVERTER_FILE);
  string test7_content;
  unordered_map<string, string> invert_file_map;
  if (fstream_7test_file)
  {
    while (getline(fstream_7test_file, test7_content))
    {
      if (test7_content.find("INV_xp33_75t") != string::npos)
      {
        string net_name_start = "(";
        string net_name_end = ")";
        string inverter = substrPinName(test7_content, net_name_start, net_name_end);
        //can use set check same net name exception
        invert_file_map.insert(pair<string, string>(inverter, inverter));
      }
    }
  }
  fstream_7test_file.close();
  // ----- read inverter data from 7_test end -----

  // ----- read logic gate and inverter data from b19_run2.v start -----
  // OAI21_xp33_75t \P1/P4/U673  ( .A1(\P1/P4/n944 ), .A2(\P1/P4/n557 ), .B(
  //     \P1/P4/n287 ), .Y(\P1/P4/n1909 ) );
  //key : netName
  //output_pin_name
  unordered_map<string, Inverter> inverter_map;
  unordered_map<string, LogicGate> logic_gate_map;
  ifstream fstream_b19_file(B19FILE);
  string b19_content;
  int logicsize = 0;
  int invertersize = 0;
  if (fstream_b19_file)
  {
    while (getline(fstream_b19_file, b19_content))
    {
      if (b19_content.find("_75t") != string::npos)
      {
        if (b19_content.find("INV") != string::npos)
        {
          getInverter(fstream_b19_file, b19_content, &inverter_map);
        }
        else
        {
          getLogicGate(fstream_b19_file, b19_content, &logic_gate_map);
        }
      }
    }
  };
  fstream_b19_file.close();

  // ----- read logic gate and inverter data from b19_run2.v end -----
  cout << "logic gate size : " << logic_gate_map.size() << endl;
  cout << "inverter size : " << inverter_map.size() << endl;

  transferInverterFile(&inverter_map, &invert_file_map);

  //  ----- change inverter from logic data start-----
  for (unordered_map<string, LogicGate>::iterator logic_gate_map_id_it = logic_gate_map.begin(); logic_gate_map_id_it != logic_gate_map.end(); ++logic_gate_map_id_it)
  {
    LogicGate logic_gate = logic_gate_map_id_it->second;
    string logic_gate_id = logic_gate_map_id_it->first;
    // cout << "logic_gate : " << logic_gate.name << " " << logic_gate.id << " " << endl;
    for (map<string, map<string, Input>>::iterator input_map_type_it = logic_gate.input_map.begin(); input_map_type_it != logic_gate.input_map.end(); ++input_map_type_it)
    {

      string type = input_map_type_it->first;
      map<string, Input> input_pin_map = input_map_type_it->second;
      if (input_pin_map.size() == 1)
      {
        for (map<string, Input>::iterator input_map_it = input_pin_map.begin(); input_map_it != input_pin_map.end(); ++input_map_it)
        {
          string logic_gate_input_net = input_map_it->second.ori_net_name;
          //if b19_run2 and 7_test.v has inverter than revise
          if (invert_file_map.count(logic_gate_input_net) and inverter_map.count(logic_gate_input_net))
          {
            string inverter_input_net = inverter_map[logic_gate_input_net].input_net_name;
            string logic_gate_input_pin = input_map_it->first;
            input_pin_map[logic_gate_input_pin].revise_net_name = inverter_input_net;
            logic_gate.input_map[type] = input_pin_map;
            transferLogicName(logic_gate.name, logic_gate_input_pin);
          }
          else
          {
            //if b19_run2 and 7_test.v hasn't inverter than revise
            string logic_gate_input_pin = input_map_it->first;
            input_pin_map[logic_gate_input_pin].revise_net_name = input_pin_map[logic_gate_input_pin].ori_net_name;
          }
        }
        logic_gate.input_map[type] = input_pin_map;
        logic_gate_map[logic_gate_id] = logic_gate;
      }
      else
      {
        // key : .A  value : \P1/P1/P3/n3580 ->這個其實不用map 應該要用vector
        vector<string> change_vector;
        vector<string> ori_vector;
        // 有排序過的map .A1 .A2 .B1 .B2
        for (map<string, Input>::iterator input_map_it = input_pin_map.begin(); input_map_it != input_pin_map.end(); ++input_map_it)
        {
          string logic_gate_input_net = input_map_it->second.ori_net_name;
          //logic gate 需要做替換 需要判斷兩個file是否有相同的inverter
          if (invert_file_map.count(logic_gate_input_net))
          {
            if (inverter_map.count(logic_gate_input_net))
            {
              string inverter_input_net = inverter_map[logic_gate_input_net].input_net_name;
              change_vector.push_back(inverter_input_net);
            }
            else
            {
              cout << " has some inveter exception that the inverter isn't in " << B19FILE << " " << logic_gate_input_net << endl;
            }
            //logic gate 不需要做替換
          }
          else
          {
            ori_vector.push_back(logic_gate_input_net);
          }
        }
        //將兩個vetor合併
        int changeIndex = change_vector.size() - 1;
        change_vector.insert(change_vector.end(), ori_vector.begin(), ori_vector.end());

        int index = 0;
        for (map<string, Input>::iterator input_map_it = input_pin_map.begin(); input_map_it != input_pin_map.end(); ++input_map_it)
        {
          string logic_gate_input_pin = input_map_it->first;
          input_pin_map[logic_gate_input_pin].revise_net_name = change_vector[index];
          if (index <= changeIndex)
          {
            transferLogicName(logic_gate.name, logic_gate_input_pin);
          }
          index += 1;
        }
        if (index == input_pin_map.size() - 1)
        {
          cout << " has some inverter change name exception " << endl;
        }

        logic_gate.input_map[type] = input_pin_map;
        logic_gate_map[logic_gate_id] = logic_gate;
      };
    };
  };
  // ----- change inverter from logic data end -----

  // ----- transfer name to x1 start -----
  transferX1(&logic_gate_map);
  // ----- transfer name to x1 end -----

  // ----- revise list start -----
  ifstream revise_file(REVISE_LIST);
  unordered_map<string, string> revise_logic_gate_name_map;
  string revise_file_content;
  if (revise_file)
  {
    while (getline(revise_file, revise_file_content))
    {
      revise_logic_gate_name_map.insert(pair<string, string>(revise_file_content, revise_file_content));
    }
  }

  // ----- revise list end -----

  // ----- output file start-----
  ofstream myfile;
  myfile.open(REVISE_FILE);
  ifstream b_19file;
  b_19file.open(B19FILE);
  string b19_content_revise;
  while (getline(b_19file, b19_content_revise))
  {
    if (b19_content_revise.find("_75t") != string::npos)
    {
      if (b19_content_revise.find("INV") != string::npos)
      {
        getInverterContent(b_19file, myfile, b19_content_revise, &invert_file_map);
      }
      else
      {
        getlogicGateContent(b_19file, myfile, b19_content_revise, &logic_gate_map, &revise_logic_gate_name_map);
      }
    }
    else
    {
      myfile << b19_content_revise << endl;
    }
  }

  myfile.close();
  b_19file.close();
  // ----- output file end-----

  return 0;
}





void transferX1(unordered_map<string, LogicGate> *logic_gate_map)
{
  for (unordered_map<string, LogicGate>::iterator logic_map_it = (*logic_gate_map).begin(); logic_map_it != (*logic_gate_map).end(); ++logic_map_it)
  {
    string logic_gate_id = logic_map_it->first;
    string logic_gate_name = logic_map_it->second.name;
    if (logic_gate_name.find("MAJ_x2") != string::npos)
    {
      (*logic_gate_map)[logic_gate_id].name = logic_gate_name + "_x1";
    }
    if (logic_gate_name.find("MAJI_xp5") != string::npos)
    {
      (*logic_gate_map)[logic_gate_id].name = logic_gate_name + "_x1";
    }
    if (logic_gate_name.find("HA_xp5") != string::npos)
    {
      (*logic_gate_map)[logic_gate_id].name = logic_gate_name + "_x1";
    }
    if (logic_gate_name.find("HA_x1") != string::npos)
    {
      (*logic_gate_map)[logic_gate_id].name = logic_gate_name + "_x2";
    }
    if (logic_gate_name.find("XOR2_xp5") != string::npos)
    {
      (*logic_gate_map)[logic_gate_id].name = logic_gate_name + "_x1";
    }
    if (logic_gate_name.find("XNOR2_xp5") != string::npos)
    {
      (*logic_gate_map)[logic_gate_id].name = logic_gate_name + "_x1";
    }
  }
}

void getlogicGateContent(ifstream &fstream_b19_file, ofstream &myfile, string b19_content_revise, unordered_map<string, LogicGate> *logic_gate_map, unordered_map<string, string> *revise_logic_gate_name_map)
{
  string b19_content_id = b19_content_revise;
  string logic_gate_id = splitByPattern(trim(b19_content_id), " ")[1];
  string logic_gate_name = splitByPattern(trim(b19_content_id), " ")[0];
  if (b19_content_revise.find(");") != string::npos)
  {
    b19_content_revise = replaceLogic(b19_content_revise, logic_gate_id, myfile, &(*logic_gate_map), &(*revise_logic_gate_name_map));
    b19_content_revise = replaceLogicName(b19_content_revise, logic_gate_id, logic_gate_name, logic_gate_map, &(*revise_logic_gate_name_map));
    myfile << b19_content_revise << endl;
    // cout << "after : " << b19_content_revise  << endl;
    // cout << "===================================" << endl;
  }
  else
  {
    b19_content_revise = replaceLogic(b19_content_revise, logic_gate_id, myfile, &(*logic_gate_map), &(*revise_logic_gate_name_map));
    b19_content_revise = replaceLogicName(b19_content_revise, logic_gate_id, logic_gate_name, logic_gate_map, &(*revise_logic_gate_name_map));
    myfile << b19_content_revise << endl;
    while (getline(fstream_b19_file, b19_content_revise))
    {
      b19_content_revise = replaceLogic(b19_content_revise, logic_gate_id, myfile, &(*logic_gate_map), &(*revise_logic_gate_name_map));
      myfile << b19_content_revise << endl;
      if (b19_content_revise.find(");") != string::npos)
      {
        break;
      }
    }
  }
}

void transferInverterFile(unordered_map<string, Inverter> *inverter_map, unordered_map<string, string> *invert_file_map)
{
  for (unordered_map<string, Inverter>::iterator input_map_it = (*inverter_map).begin(); input_map_it != (*inverter_map).end(); ++input_map_it)
  {
    string temp_inverter_id = input_map_it->first;
    trimAllBlanket(temp_inverter_id);
    if ((*invert_file_map).count(temp_inverter_id))
    {
      (*invert_file_map).erase(temp_inverter_id);
      (*invert_file_map).insert(pair<string, string>(input_map_it->first, input_map_it->first));
    }
  }
}

void getInverterContent(ifstream &fstream_b19_file, ofstream &myfile, string b19_content_revise, unordered_map<string, string> *invert_file_map)
{
  if (b19_content_revise.find(");") != string::npos)
  {
    if (isContentReplaceNetName(b19_content_revise, &(*invert_file_map)) == false)
    {
      myfile << b19_content_revise << endl;
    }
  }
  else
  {
    vector<string> content_vector;
    string content = b19_content_revise;
    content_vector.push_back(b19_content_revise);
    // myfile << b19_content_revise << endl;
    while (getline(fstream_b19_file, b19_content_revise))
    {
      content = content + b19_content_revise;
      content_vector.push_back(b19_content_revise);
      // myfile << b19_content_revise << endl;
      if (b19_content_revise.find(");") != string::npos)
      {
        if (isContentReplaceNetName(content, &(*invert_file_map)) == false)
        {
          for (int i = 0; i < content_vector.size(); i++)
          {
            myfile << content_vector[i] << endl;
          };
        }

        break;
      }
    }
  }
}

bool isContentReplaceNetName(string b_19_content, unordered_map<string, string> *invert_file_map)
{
  string left = "(";
  string right = ");";
  string pin_name = substr(b_19_content, left, right);
  vector<string> pin_vector = splitByPattern(pin_name, ",");
  string pin_name_start = ".";
  string pin_name_end = "(";
  string net_name_start = "(";
  string net_name_end = ")";
  string output_net_name = substrPinName(pin_vector[1], net_name_start, net_name_end);
  if ((*invert_file_map).count(output_net_name))
  {
    return true;
  }
  else
  {
    return false;
  }
}

string replaceLogicName(string content, string logic_gate_id, string logic_gate_name, unordered_map<string, LogicGate> *logic_gate_map, unordered_map<string, string> *revise_logic_gate_name_map)
{
  if ((*logic_gate_map).count(logic_gate_id))
  {
    string logic_name = trim((*logic_gate_map)[logic_gate_id].name);
    if ((*revise_logic_gate_name_map).count(logic_name))
    {
      LogicGate logic_gate = (*logic_gate_map)[logic_gate_id];
      subreplace(content, logic_gate_name, logic_gate.name);
    }
  }
  return content;
}

string replaceLogic(string content, string logic_gate_id, ofstream &myfile, unordered_map<string, LogicGate> *logic_gate_map, unordered_map<string, string> *revise_logic_gate_name_map)
{
  //有exception處理 所以寫在下一段
  if ((*logic_gate_map).count(logic_gate_id))
  {
    string logic_name = trim((*logic_gate_map)[logic_gate_id].name);

    if ((*revise_logic_gate_name_map).count(logic_name))
    {
      map<string, map<string, Input>> input_type_map = (*logic_gate_map)[logic_gate_id].input_map;
      for (map<string, map<string, Input>>::iterator input_map_type_it = input_type_map.begin(); input_map_type_it != input_type_map.end(); ++input_map_type_it)
      {
        map<string, Input> input_map = input_map_type_it->second;
        for (map<string, Input>::iterator input_map_it = input_map.begin(); input_map_it != input_map.end(); ++input_map_it)
        {
          Input input = input_map_it->second;
          subreplace(content, input.ori_net_name, input.revise_net_name);
        }
      }
    }
  }
  else
  {
    cout << "logic has some exception in replace , logic id : " << logic_gate_id << endl;
  }
  return content;
}

//確認 inverter 只有兩個input 跟 output
void getInverter(ifstream &fstream_b19_file, string b19_content, unordered_map<string, Inverter> *inverter_map)
{
  Inverter inverter;
  string b19_content_trim = trim(b19_content);
  vector<string> b19_content_vec = splitByPattern(b19_content_trim, " ");
  inverter.name = b19_content_vec[0];
  inverter.id = b19_content_vec[1];

  string pin_name = getPinName(fstream_b19_file, b19_content);
  vector<string> pin_vector = splitByPattern(pin_name, ",");

  string pin_name_start = ".";
  string pin_name_end = "(";
  string net_name_start = "(";
  string net_name_end = ")";

  string input_pin_name = substrPinName(pin_vector[0], pin_name_start, pin_name_end);
  string input_net_name = substrPinName(pin_vector[0], net_name_start, net_name_end);
  string output_pin_name = substrPinName(pin_vector[1], pin_name_start, pin_name_end);
  string output_net_name = substrPinName(pin_vector[1], net_name_start, net_name_end);
  inverter.input_pin_name = input_pin_name;
  inverter.input_net_name = input_net_name;
  inverter.output_pin_name = output_pin_name;
  inverter.output_net_name = output_net_name;

  // cout <<"input_net_name :" <<input_net_name <<input_net_name.length() << endl;

  if ((*inverter_map).count(output_net_name))
  {
    cout << "has some exception on read inverter" << endl;
    cout << "repeat inverter id " << (*inverter_map)[output_net_name].id << endl;
  }
  else
  {
    (*inverter_map).insert(pair<string, Inverter>(output_net_name, inverter));
  }
};

// AOI21_xp33_75t \P2/P4/U1573  ( .A1(\P2/P4/n1657 ), .A2(\P2/P4/n1969 ), .B(\P2/P4/n1488 ), .Y(\P2/P4/n1489 ) );
void getLogicGate(ifstream &fstream_b19_file, string &b19_content, unordered_map<string, LogicGate> *logic_gate_map)
{
  string b19_content_trim = trim(b19_content);
  vector<string> b19_content_vec = splitByPattern(b19_content_trim, " ");
  LogicGate logic_gate;
  logic_gate.name = b19_content_vec[0];
  logic_gate.id = b19_content_vec[1];

  string pin_name = getPinName(fstream_b19_file, b19_content);

  //.A1(\P2/P4/n1657 ), .A2(\P2/P4/n1969 ), .B(\P2/P4/n1488 ), .Y(\P2/P4/n1489 )
  vector<string> pin_vector = splitByPattern(pin_name, ",");
  // .A1(\P2/P4/n1657 )
  string pin_name_start = ".";
  string pin_name_end = "(";
  string net_name_start = "(";
  string net_name_end = ")";

  //將 input pin name 及 output pin name 裝入map 中
  map<string, map<string, Input>> input_map;
  map<string, string> output_map;
  int pin_index = pin_vector.size() - 1;

  for (int i = 0; i < pin_index; i++)
  {
    string input_pin_name = substrPinName(pin_vector[i], pin_name_start, pin_name_end);
    string input_net_name = substrPinName(pin_vector[i], net_name_start, net_name_end);

    if (input_pin_name.length() <= 2)
    {
      // check is .A
      if (regex_match(input_pin_name, input_name_regx))
      {
        string type = input_name_type(input_pin_name);
        if (input_map.count(type))
        {
          Input input;
          input.ori_net_name = input_net_name;
          input_map[type].insert(pair<string, Input>(input_pin_name, input));
          // .A('net_name') has repeat net name
          cout << "logic gate tyep has some exception for read b_19 file :" << type << endl;
        }
        else
        {
          map<string, Input> pin_net_map;
          Input input;
          input.ori_net_name = input_net_name;
          pin_net_map.insert(pair<string, Input>(input_pin_name, input));
          input_map.insert(pair<string, map<string, Input>>(type, pin_net_map));
        }
      }
      // check is not .A pattern .S input .Z->output
      else
      {
        cout << "need check input name" << pin_vector[i] << input_net_name << " " << input_pin_name << endl;
        Input input;
        map<string, Input> pin_net_map;
        input.ori_net_name = input_net_name;
        pin_net_map.insert(pair<string, Input>(input_pin_name, input));
        input_map.insert(pair<string, map<string, Input>>(input_pin_name, pin_net_map));
      }
    }
    else
    {
      // .A1 .A2
      if (regex_match(input_pin_name, input_name_number_regx))
      {
        string type = input_name_type(input_pin_name);
        if (input_map.count(type))
        {
          Input input;
          input.ori_net_name = input_net_name;
          input_map[type].insert(pair<string, Input>(input_pin_name, input));
        }
        else
        {
          map<string, Input> pin_net_map;
          Input input;
          input.ori_net_name = input_net_name;
          pin_net_map.insert(pair<string, Input>(input_pin_name, input));
          input_map.insert(pair<string, map<string, Input>>(type, pin_net_map));
        }
        // check is not .A pattern
      }
      else
      {
        map<string, Input> pin_net_map;
        Input input;
        input.ori_net_name = input_net_name;
        pin_net_map.insert(pair<string, Input>(input_pin_name, input));
        input_map.insert(pair<string, map<string, Input>>(input_pin_name, pin_net_map));
      }
    }
  }
  // output_pin
  string output_pin_name = substrPinName(pin_vector[pin_index], pin_name_start, pin_name_end);
  string output_net_name = substrPinName(pin_vector[pin_index], net_name_start, net_name_end);

  output_map.insert(pair<string, string>(output_pin_name, output_net_name));

  logic_gate.input_map = input_map;
  logic_gate.output_map = output_map;
  (*logic_gate_map).insert(pair<string, LogicGate>(logic_gate.id, logic_gate));
}

// get input name type A1 -> A
string input_name_type(string input_pin_name)
{
  if (input_pin_name.find("A") != string::npos)
  {
    return "A";
  }
  else if (input_pin_name.find("B") != string::npos)
  {
    return "B";
  }
  else if (input_pin_name.find("C") != string::npos)
  {
    return "C";
  }
  else if (input_pin_name.find("D") != string::npos)
  {
    return "D";
  }
  else
  {
    cout << "input_name_type has some exception " << endl;
    return "otherType";
  }
}

string getPinName(ifstream &fstream_b19_file, string b19_content)
{
  string left = "(";
  string right = ");";
  if (b19_content.find(");") != string::npos)
  {
    b19_content = substr(b19_content, left, right);
    return b19_content;
  }
  else
  {
    string pinContent = b19_content;
    while (getline(fstream_b19_file, b19_content))
    {
      pinContent = pinContent + b19_content;
      if (b19_content.find(");") != string::npos)
      {
        break;
      }
    }
    pinContent = substr(pinContent, left, right);
    return pinContent;
  }
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
// 去掉首尾空白
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

//去掉括號
string eraseBrackets(string content)
{
  content.erase(remove(content.begin(), content.end(), '('), content.end());
  content.erase(remove(content.begin(), content.end(), ')'), content.end());
  return content;
}
//pinName substr 並且去掉字串內的空白
string substrPinName(string content, string &start, string &end)
{
  string substr_content;
  if (content.find(start) == 0)
  {
    substr_content = content.substr(content.find(start), content.find(end));
  }
  else
  {
    substr_content = content.substr(content.find(start) + 1, content.find(end) - (content.find(start) + 1));
  }
  // substr_content.erase(std::remove(substr_content.begin(), substr_content.end(), ' '), substr_content.end());
  // return trim(substr_content);
  return substr_content;
}

string substr(string content, string &start, string &end)
{
  string substr_content;
  if (content.find(start) == 0)
  {
    substr_content = content.substr(content.find(start), content.find(end));
  }
  else
  {
    substr_content = content.substr(content.find(start) + 1, content.find(end) - (content.find(start) + 1));
  }
  return substr_content;
}

void transferLogicName(string &logic_gate_name, string &inv_gate)
{
  string command = ".";
  string empty = "";
  subreplace(inv_gate, command, empty);
  if (logic_gate_name.find("_inv") != string::npos)
  {
    logic_gate_name = logic_gate_name + "inv" + inv_gate;
  }
  else
  {
    logic_gate_name = logic_gate_name + "_inv" + inv_gate;
  }
}
void subreplace(string &resource_str, string &sub_str, string &new_str)
{
  string::size_type pos = 0;
  while ((pos = resource_str.find(sub_str)) != string::npos) //替换所有指定子串
  {
    resource_str.replace(pos, sub_str.length(), new_str);
    break;
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

// 測試data structure logic_gate_vector
//    cout << logic_gate_map.size() << endl;
//   for (auto const &log : logic_gate_map)
// {
//   LogicGate logic = log.second;
//   cout << logic.name << " " << logic.id << " ";
//   for (auto const &log_map : logic.input_map)
//   {
//     // cout << " " << log_map.first << " ";
//     for (auto const &gate : log_map.second)
//     {
//      cout << gate.first << gate.second.revise_net_name << " ";
//     }
//   }
//    for (auto const &out : logic.output_map)
//   {
//     // cout << " " << log_map.first << " ";

//       cout << out.first << " " << out.second << " ";

//   }
//   cout << "" << endl;
//   cout << "" << endl;
// }

// // ----- remove inverter from logic data start-----
// for(int i =0; i<logic_gate_vector.size();i++){
//   for (map<string, map<string, string> >::iterator input_map_type_it = logic_gate_vector[i].input_map.begin(); input_map_type_it != logic_gate_vector[i].input_map.end(); ++input_map_type_it)
//   {
//     string type = input_map_type_it -> first;
//     map<string,string> input_pin_map = input_map_type_it -> second;
//     if(input_pin_map.size() == 1){
//        for (map<string, string >::iterator input_map_it = input_pin_map.begin(); input_map_it != input_pin_map.end(); ++input_map_it){
//             string logic_gate_input_net = input_map_it -> second;
//               if(invert_file_map.count(logic_gate_input_net)){
//                  if(inverter_map.count(logic_gate_input_net)){
//                     string inverter_input_net = inverter_map[logic_gate_input_net].input_net_name;
//                     string logic_gate_input_pin = input_map_it -> first;
//                     input_pin_map[logic_gate_input_pin] = inverter_input_net;
//                     logic_gate_vector[i].input_map[type] = input_pin_map;

//                  }
//               }
//        }
//     }else{
//       // key : .A  value : \P1/P1/P3/n3580 ->這個其實不用map 應該要用vector
//       vector<string> change_vector;
//       vector<string> ori_vector;
//       // 有排序過的map .A1 .A2 .B1 .B2
//       for (map<string, string >::iterator input_map_it = input_pin_map.begin(); input_map_it != input_pin_map.end(); ++input_map_it){
//          string logic_gate_input_net = input_map_it -> second;
//          //logic gate 需要做替換
//          if(invert_file_map.count(logic_gate_input_net)){
//             if(inverter_map.count(logic_gate_input_net)){
//               string inverter_input_net =  inverter_map[logic_gate_input_net].input_net_name;
//               string inverter_output_net = inverter_map[logic_gate_input_net].output_net_name;
//               change_vector.push_back(inverter_input_net);
//             }else{
//               cout << " has some inveter exception " << endl;
//             }
//           //logic gate 不需要做替換
//          }else{
//            ori_vector.push_back(logic_gate_input_net);
//          }
//       }
//       //將兩個vetor合併
//       change_vector.insert(change_vector.end(),ori_vector.begin(),ori_vector.end());
//       int index = 0;
//       for (map<string, string >::iterator input_map_it = input_pin_map.begin(); input_map_it != input_pin_map.end(); ++input_map_it){
//           string logic_gate_input_pin = input_map_it -> first;
//           input_pin_map[logic_gate_input_pin] = change_vector[index];
//           index+=1;
//        }
//       if(index == input_pin_map.size()-1){
//         cout << " has some change name exception " << endl;
//       }
//       logic_gate_vector[i].input_map[type] = input_pin_map;

//     }
//   }
// }
// //  // ----- remove inverter from logic data end -----

//測試 logic_gate
//     cout << logic_gate_map.size() << endl;
//   for (auto const &log : logic_gate_map)
// {
//   LogicGate logic = log.second;
//   cout << logic.name << " " << logic.id << " ";
//   for (auto const &log_map : logic.input_map)
//   {
//     // cout << " " << log_map.first << " ";
//     for (auto const &gate : log_map.second)
//     {
//      cout << gate.first << gate.second.revise_net_name << " ";
//     }
//   }
//    for (auto const &out : logic.output_map)
//   {
//     // cout << " " << log_map.first << " ";

//       cout << out.first << " " << out.second << " ";

//   }
//   cout << "" << endl;
//   cout << "" << endl;
// }

// 測試data structure inverter_vector
//  for(auto const inver:inverter_map){
//     cout << inver.second.input_pin_name << " " << inver.second.input_net_name << " " <<  inver.second.output_pin_name << " " <<  inver.second.output_net_name <<endl;
//      }

// input 換行符號
// string getlogicGateContent(ifstream &fstream_b19_file, string b19_content)
// {
//   string logic_gate_content = "  "+b19_content;
//   bool start = false;
//   while (getline(fstream_b19_file,b19_content))
//   {
//      logic_gate_content =  logic_gate_content + b19_content+'\n';
//       if (b19_content.find(");")!=string::npos)
//       {
//         break;
//       }
//   }
//   return logic_gate_content;
// }
// string getPinName(ifstream &fstream_b19_file, string b19_content)
// {
//   string pinName;
//   bool start = false;

//   while (fstream_b19_file >> b19_content)
//   {
//     if (b19_content.find(".") != string::npos)
//     {
//       start = true;
//     }
//     if (start and (b19_content.empty() == false))
//     {
//       if (b19_content == ");")
//       {
//         break;
//       }
//       pinName = pinName + b19_content;
//     }
//   }
//   cout << "pinName : " << pinName << endl;
//   return pinName;
// }

// string getPinName(ifstream &fstream_b19_file, string b19_content)
// {
//   if(b19_content.find(");")){
//     return b19_content;
//   }
//     string pinContent = b19_content;
//   // getline(fstream_b19_file,b19_content);
//   // pinContent = pinContent + b19_content ;
//   while (getline(fstream_b19_file,b19_content))
//   {
//    pinContent = pinContent + b19_content ;
//     if (b19_content.find(");")!=string::npos)
//     {
//       break;
//     }
//   }
//   string start = "(";
//   string end = ");";
//   pinContent = substr( pinContent, start, end);
//   return pinContent;
// }

//     for (auto const &log : logic_gate_map)
// {
//   LogicGate logic = log.second;
//   cout << logic.name << " " << logic.id << " ";
//   for (auto const &log_map : logic.input_map)
//   {
//     // cout << " " << log_map.first << " ";
//     for (auto const &gate : log_map.second)
//     {
//      cout << gate.first << gate.second.ori_net_name<< gate.second.ori_net_name.length() << " " << gate.second.revise_net_name <<  gate.second.revise_net_name.length();
//     }
//   }
//    for (auto const &out : logic.output_map)
//   {
//     // cout << " " << log_map.first << " ";

//       cout << out.first << " " << out.second << " ";

//   }
//   cout << "" << endl;
//   cout << "" << endl;
// }
// void getlogicGateContent(ifstream &fstream_b19_file,ofstream &myfile, string b19_content_revise, unordered_map<string, LogicGate > *logic_gate_map)
// {
//   string b19_content_id = b19_content_revise;
//   string logic_gate_id = splitByPattern(trim(b19_content_id), " ")[1];
//   string logic_gate_name =  splitByPattern(trim(b19_content_id), " ")[0];

//   b19_content_revise = replaceLogic(b19_content_revise, logic_gate_id, myfile,  &(*logic_gate_map));
//   b19_content_revise = replaceLogicName(b19_content_revise, logic_gate_id , logic_gate_name , logic_gate_map);

//   myfile <<b19_content_revise << endl;
//   while (getline(fstream_b19_file,b19_content_revise))
//   {
//       b19_content_revise =replaceLogic(b19_content_revise, logic_gate_id, myfile,  &(*logic_gate_map));
//        myfile << b19_content_revise << endl;
//       if (b19_content_revise.find(");")!=string::npos)
//       {
//         break;
//       }
//   }
// }
