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


const string DEF_FILE = "6t49_b19_routing_88_9_floorplan_transfer.txt";  
const string EFFR_FILE_VDD = "VDDX.effr";
const string ADD_STRIPE_FILE = "add_stripe_PD9_6t49_b19_routing_88_9_stripe_change_range.tcl";
const string NET_NAME_VDD = "VDDX";
const string NET_NAME_VSS = "VSSX";
const string STRIPE_METAL = "M3";
// the bottom loaction
struct Location
{
    string x_location;
    string y_location;
};
struct ResistLine{
    string x_location;
    string total_resistance;

};

struct Stripe
{
    string net_name;
    string start_x_location;
    string start_y_location;
    string end_x_location;
    string end_y_location;
    string move_range_x_left;
    string move_range_x_right;
    map<string,ResistLine> resist_line_map;
    ResistLine total_resist_line;
    string total_resist = "0";
    
    
};
struct Range{
    Stripe star_stripe;
    Stripe end_stripe;

};
struct ResistNode{
    string reff; 
    string instance_id;
    string pin;
    string cell_name;
    string x_location;
    string y_location;
    string layer;
};

struct CoreSite{

    string left_x_location; 
    string right_x_location;
    string up_y_location;
    string down_y_location;

};



vector<string> splitByPattern(string content, string pattern);
vector<string> splitByPatternEffrReport(string content, string pattern);
string &trim(string &str);
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector,vector<Range> *vdd_range_vector,vector<Range> *vss_range_vector);
void getResisNodeMap(string eff_file_name,map<string, ResistLine> *resist_line_vdd_map, map<string,vector<ResistNode> > *resist_node_vdd_map);
void getCoreSite(string def_file_name,CoreSite *core_site);
void getStripeMovingRange(vector<Stripe> *vdd_stripe_vector,vector<Stripe> *vss_stripe_vector,CoreSite *core_site);
void getLeastResistStripe(map<string,ResistLine > *resist_line_vdd_map,vector<Stripe> *vdd_stripe_vector);

int main()
{
  vector<Stripe> vdd_stripe_vector;
  vector<Stripe> vss_stripe_vector;
  vector<Range> vdd_range_vector;  
  vector<Range> vss_range_vector;  
  map<string,ResistLine > resist_line_vdd_map;
  map<string,vector<ResistNode> > resist_node_vdd_map;
  CoreSite core_site;

//   //算出middle line
//   //將它分成區段
        getStripeLocation(DEF_FILE,  &vdd_stripe_vector, &vss_stripe_vector,&vdd_range_vector,&vss_range_vector);
        getCoreSite( DEF_FILE, &core_site);
        getStripeMovingRange( &vdd_stripe_vector, &vss_stripe_vector,&core_site);
        getResisNodeMap(EFFR_FILE_VDD,&resist_line_vdd_map,&resist_node_vdd_map);
        getLeastResistStripe(&resist_line_vdd_map,&vdd_stripe_vector);
    
       
           //在range裡面 挑個電阻值最小的出來產生 addStripe
            ofstream myfile;
            myfile.open(ADD_STRIPE_FILE);
            set<string> add_stripe;
            for (int i = 0; i < vdd_stripe_vector.size(); i++)
            {
                Stripe stripe =  vdd_stripe_vector[i];
                ResistLine resist_line = stripe.total_resist_line;
                string start_x_location =  resist_line.x_location;
                string end_x_location =  to_string((stof(resist_line.x_location)+2));
                string star_y_location = stripe.start_y_location;
                string end_y_location = stripe.end_y_location;

                string stripe_location = start_x_location+ " " + star_y_location + " " + end_x_location + " " + end_y_location;

                 myfile << "addStripe -nets { VDDX } -layer "<< STRIPE_METAL << " -direction vertical -width 0.216 -set_to_set_distance 12.88 -number_of_sets 1  -area {" << stripe_location << "}" << endl;
            
            }
            myfile.close();




            
         
     
         


        





    return 0;
}

void getLeastResistStripe(map<string,ResistLine > *resist_line_vdd_map,vector<Stripe> *stripe_vector){

         int i = 0;
        //暫時寫兩個迴圈 放到range 裡面
        for (auto iter = (*resist_line_vdd_map).begin(); iter != (*resist_line_vdd_map).end(); ++iter){
            if(i%1000 == 0){
                cout << "put effective line in stripe : " << i << endl;
            }
            ResistLine resist_line = iter -> second;
            string x_location = iter -> first;
            double x_location_double = stof(x_location);
            for (int i = 0; i < (*stripe_vector).size(); i++)
            {
                Stripe stripe = (*stripe_vector)[i];
                double move_range_left = stof(stripe.move_range_x_left);
                double move_range_right = stof(stripe.move_range_x_right);
                if(move_range_left<= x_location_double && x_location_double <= move_range_right){
                     (*stripe_vector)[i].resist_line_map.insert(pair<string,ResistLine>(x_location,resist_line));
                    
                     if(stof((*stripe_vector)[i].total_resist) == 0){
                         (*stripe_vector)[i].total_resist_line = resist_line;
                         (*stripe_vector)[i].total_resist = resist_line.total_resistance;
                     }else if(stof((*stripe_vector)[i].total_resist) > stof(resist_line.total_resistance)){
                         (*stripe_vector)[i].total_resist_line = resist_line;
                         (*stripe_vector)[i].total_resist = resist_line.total_resistance;
                     }
                     break;;
                }                
            }
            i++;
        }

}





void getStripeMovingRange(vector<Stripe> *vdd_stripe_vector,vector<Stripe> *vss_stripe_vector,CoreSite *core_site){

    for (int i = 0; i < (*vdd_stripe_vector).size(); i++)
    {
        if( i == 0 ){
           double distance = (((stof((*vdd_stripe_vector)[i+1].start_x_location) - stof((*vdd_stripe_vector)[i].end_x_location))/2) + stof((*vdd_stripe_vector)[i].start_x_location));
           (*vdd_stripe_vector)[i].move_range_x_left = (*vdd_stripe_vector)[i].start_x_location;
           (*vdd_stripe_vector)[i].move_range_x_right = to_string(distance);

        }
        else if (i ==  ((*vdd_stripe_vector).size()-1) )
        {
            int front_index = i-1;
            (*vdd_stripe_vector)[i].move_range_x_right =  (*vdd_stripe_vector)[i].start_x_location;
            (*vdd_stripe_vector)[i].move_range_x_left = (*vdd_stripe_vector)[front_index].move_range_x_right;


        }
        else{
            double distance = (((stof((*vdd_stripe_vector)[i+1].start_x_location) - stof((*vdd_stripe_vector)[i].end_x_location))/2) + stof((*vdd_stripe_vector)[i].start_x_location));
            (*vdd_stripe_vector)[i].move_range_x_right = to_string(distance);
            (*vdd_stripe_vector)[i].move_range_x_left = (*vdd_stripe_vector)[i-1].move_range_x_right;

        }      
    }

    for (int i = 0; i < (*vss_stripe_vector).size(); i++)
    {
        if( i == 0 ){
           double distance = (((stof((*vss_stripe_vector)[i+1].start_x_location) - stof((*vss_stripe_vector)[i].end_x_location))/2) + stof((*vss_stripe_vector)[i].start_x_location));
           (*vss_stripe_vector)[i].move_range_x_left = (*core_site).left_x_location;
           (*vss_stripe_vector)[i].move_range_x_right = to_string(distance);

        }
        else if (i ==  ((*vss_stripe_vector).size()-1) )
        {
            int front_index = i-1;
            (*vss_stripe_vector)[i].move_range_x_right =  (*core_site).right_x_location;
            (*vss_stripe_vector)[i].move_range_x_left = (*vss_stripe_vector)[front_index].move_range_x_right;


        }
        else{
            double distance = (((stof((*vss_stripe_vector)[i+1].start_x_location) - stof((*vss_stripe_vector)[i].end_x_location))/2) + stof((*vss_stripe_vector)[i].start_x_location));
            (*vss_stripe_vector)[i].move_range_x_right = to_string(distance);
            (*vss_stripe_vector)[i].move_range_x_left = (*vss_stripe_vector)[i-1].move_range_x_right;

        }      
    }

}




void getCoreSite(string def_file_name,CoreSite *core_site){
    ifstream def_file(def_file_name);
    string def_content;

    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("FE_CORE_BOX_LL_X") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                (*core_site).left_x_location = def_content_array[3];

            }
            if (def_content.find("FE_CORE_BOX_UR_X") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                 (*core_site).right_x_location = def_content_array[3];

            }
            if (def_content.find("FE_CORE_BOX_LL_Y") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                 (*core_site).down_y_location = def_content_array[3];

            }
            if (def_content.find("FE_CORE_BOX_UR_Y") != string::npos)
            {
                vector<string> def_content_array = splitByPattern(def_content, " ");
                 (*core_site).up_y_location = def_content_array[3];

            }
            if (def_content.find("END PROPERTYDEFINITIONS") != string::npos)
            {
                break;
            }
        }

    }
}

void getResisNodeMap(string eff_file_name,map<string, ResistLine> *resist_line_vdd_map, map<string,vector<ResistNode> > *resist_node_vdd_map){

   ifstream eff_file(eff_file_name);
   string eff_content;
   int i = 0;

   if(eff_file){
       cout << "========= Notice : change file space otherwise program will get wrong  tab -> space =========" <<endl;
      while (getline(eff_file, eff_content))
    {
        if (eff_content.find("PASS/FAIL") != string::npos)
        {
            while (getline(eff_file, eff_content))
            {
                vector<string> eff_content_array = splitByPattern(eff_content, " ");
                ResistNode resist_node;

                resist_node.reff = eff_content_array[1];
                resist_node.instance_id = eff_content_array[2];
                resist_node.pin = eff_content_array[3];
                resist_node.x_location = eff_content_array[5];
                resist_node.y_location = eff_content_array[6];
                resist_node.layer =  eff_content_array[7];
            
    
               if((*resist_line_vdd_map).count(resist_node.x_location)){
                    (*resist_node_vdd_map)[resist_node.x_location].push_back(resist_node);
                    double total_resistance = stof((*resist_line_vdd_map)[resist_node.x_location].total_resistance);
                    total_resistance+= stof(resist_node.reff);
                    (*resist_line_vdd_map)[resist_node.x_location].total_resistance = to_string(total_resistance);
                    i++;
               }else{

                   ResistLine resist_line;
                   resist_line.x_location = resist_node.x_location;
                   resist_line.total_resistance = resist_node.reff;
                   vector<ResistNode> resist_node_vector;
                   resist_node_vector.push_back(resist_node);
                   (*resist_node_vdd_map).insert(pair<string,vector<ResistNode> >(resist_node.x_location,resist_node_vector));
                   (*resist_line_vdd_map).insert(pair<string,ResistLine>(resist_node.x_location,resist_line));
                   i++;
               }
                if(i%1000 == 0){
                    cout << "effective node :" << i << endl;
                }
                
            }            
        
        }
    }
    
   }else{
       cout << "eff file can't open" << endl;
   }

}



//只讀到Stripe 就 break
void getStripeLocation(string def_file_name, vector<Stripe> *vdd_stripe_vector, vector<Stripe> *vss_stripe_vector,vector<Range> *vdd_range_vector,vector<Range> *vss_range_vector){
    
    ifstream def_file(DEF_FILE);
    string def_content;

    if (def_file)
    {
        while (getline(def_file, def_content))
        {
            if (def_content.find("( * VDD )") != string::npos)
            {
                //VDDX stripe
                while (getline(def_file, def_content))
                {
                    if (def_content.find("( * VSS )") != string::npos)
                    {
                        break;
                    }
                    if (def_content.find("NEW") != string::npos)
                    {
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        if (def_content_array[4] == "SHAPE" && def_content_array[5] == "STRIPE" && def_content_array.size() == 14)
                        {
                            cout << "VDDX :" << def_content << " " << def_content_array.size() << endl;
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location =  def_content_array[12];
                            stripe.net_name = NET_NAME_VDD;

                            (*vdd_stripe_vector).push_back(stripe);
                            int index = (*vdd_stripe_vector).size();
                            if((*vdd_stripe_vector).size() >= 2){
                                Range stripe_range;
                                stripe_range.end_stripe = (*vdd_stripe_vector)[index-1];
                                stripe_range.star_stripe = (*vdd_stripe_vector)[index-2];
                                (*vdd_range_vector).push_back(stripe_range);
                            }

                        }
                    }
                }
                //VSSX stripe
                while (getline(def_file, def_content))
                {
                    if (def_content.find("NEW") != string::npos)
                    {
                        vector<string> def_content_array = splitByPattern(def_content, " ");
                        if (def_content_array[4] == "SHAPE" && def_content_array[5] == "STRIPE" && def_content_array.size() == 14)
                        {
                            cout << "VSSX :" << def_content << " " << def_content_array.size() << endl;
                            Stripe stripe;
                            stripe.start_x_location = def_content_array[7];
                            stripe.start_y_location = def_content_array[8];
                            stripe.end_x_location = def_content_array[7];
                            stripe.end_y_location =  def_content_array[12];
                            stripe.net_name = NET_NAME_VSS;
                            (*vss_stripe_vector).push_back(stripe);
                            int index = (*vss_stripe_vector).size();
                            if((*vss_stripe_vector).size() >= 2){
                                Range stripe_range;
                                stripe_range.end_stripe = (*vss_stripe_vector)[index-1];
                                stripe_range.star_stripe = (*vss_stripe_vector)[index-2];
                                (*vss_range_vector).push_back(stripe_range);
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        cout << "can't found file" << endl;
    }

    def_file.close();

}



//根據pattern切字串
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

    // for (auto item  : resist_node_vdd_map)
    // {
    //     int size = item.second.size();
    //         cout << "x location : " << item.first << " size : " << size << endl;
    // }



    // void getStripeMovingRange(vector<Range> *vdd_range_vector,vector<Range> *vss_range_vector,CoreSite *core_site){
//     vector<Stripe> stripe_vdd_vector;
//     //奇數條stripe
//     for (int i = 0; i < (*vdd_range_vector).size(); i++)
//     {
//         Range range = (*vdd_range_vector)[i];
//         Stripe left_stripe = range.star_stripe;
//         Stripe right_stripe = range.end_stripe;
//         //之後要改成邊界加上stripe width 
//         if(i == 0){
//           left_stripe.move_range_x_left = (*core_site).left_x_location;
//           double distance = ((stof(right_stripe.start_x_location) - stof(left_stripe.start_x_location))/2) + stof(left_stripe.start_x_location);
//           left_stripe.move_range_x_right = to_string(distance);
//           cout << "check in side : " <<  left_stripe.move_range_x_right << endl;
//           stripe_vdd_vector.push_back(left_stripe);
            
//         }
//         else if (i == ((*vdd_range_vector).size()-1))
//         {
//             //這邊做兩個 倒數第二條 倒數第一條
//             cout << "left : " << left_stripe.start_x_location << endl;
//             cout << "right : " << right_stripe.start_x_location << endl;
            
            
//         }
//         else{

//         }
        
        
//     }
    


// }