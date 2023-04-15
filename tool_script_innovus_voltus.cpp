using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
vector<string> splitByPattern(string content, string pattern);
string &trim(string &str);
int main()
{

    // Build command to execute.  For example if the input
    // file name is a.cpp, then str holds "gcc -o a.out a.cpp"
    // Here -o is used to specify executable file name

    // string innovus_voltus_generate = "innovus -batch -file innovus_voltus_generate_pad_location.tcl";
    // const char *innovus_voltus_generate_str = innovus_voltus_generate.c_str();
    // system(innovus_voltus_generate_str);

    vector<string> config_vector;

    string config_riscv = "config/config_riscv.txt";
    string config_gpu = "config/config_gpu.txt";
    string config_aes = "config/config_aes.txt";
    string config_neuralNetwork = "config/config_neuralNetwork.txt";
    string config_b19 = "config/config_b19.txt";
    // string config_power_pad = "config/power_pad_config.txt";
    // string config_power_stripe = "config/power_stripe_config.txt";
    // string config_metal_resource = "config/caculate_metal_resource.txt";

    // string aes_placement_5track_ir_report_config = "config/ir_drop_report_config/aes_placement_5track_ir_report_config.txt";
    // string aes_placement_7track_ir_report_config = "config/ir_drop_report_config/aes_placement_7track_ir_report_config.txt";
    // string aes_routing_5track_ir_report_config = "config/ir_drop_report_config/aes_routing_5track_ir_report_config.txt";
    // string aes_routing_7track_ir_report_config = "config/ir_drop_report_config/aes_routing_7track_ir_report_config.txt";
    // string aes_clockTree_5track_ir_report_config = "config/ir_drop_report_config/aes_clockTree_5track_ir_report_config.txt";
    // string aes_clockTree_7track_ir_report_config = "config/ir_drop_report_config/aes_clockTree_7track_ir_report_config.txt";

    // string b19_placement_5track_ir_report_config = "config/ir_drop_report_config/b19_placement_5track_ir_report_config.txt";
    // string b19_placement_7track_ir_report_config = "config/ir_drop_report_config/b19_placement_7track_ir_report_config.txt";
    // string b19_routing_5track_ir_report_config = "config/ir_drop_report_config/b19_routing_5track_ir_report_config.txt";
    // string b19_routing_7track_ir_report_config = "config/ir_drop_report_config/b19_routing_7track_ir_report_config.txt";
    // string b19_clockTree_5track_ir_report_config = "config/ir_drop_report_config/b19_clockTree_5track_ir_report_config.txt";
    // string b19_clockTree_7track_ir_report_config = "config/ir_drop_report_config/b19_clockTree_7track_ir_report_config.txt";

    // string gpu_placement_5track_ir_report_config = "config/ir_drop_report_config/gpu_placement_5track_ir_report_config.txt";
    // string gpu_placement_7track_ir_report_config = "config/ir_drop_report_config/gpu_placement_7track_ir_report_config.txt";
    // string gpu_routing_5track_ir_report_config = "config/ir_drop_report_config/gpu_routing_5track_ir_report_config.txt";
    // string gpu_routing_7track_ir_report_config = "config/ir_drop_report_config/gpu_routing_7track_ir_report_config.txt";
    // string gpu_clockTree_5track_ir_report_config = "config/ir_drop_report_config/gpu_clockTree_5track_ir_report_config.txt";
    // string gpu_clockTree_7track_ir_report_config = "config/ir_drop_report_config/gpu_clockTree_7track_ir_report_config.txt";

    // string neuralNetwork_placement_5track_ir_report_config = "config/ir_drop_report_config/neuralNetwork_placement_5track_ir_report_config.txt";
    // string neuralNetwork_placement_7track_ir_report_config = "config/ir_drop_report_config/neuralNetwork_placement_7track_ir_report_config.txt";
    // string neuralNetwork_routing_5track_ir_report_config = "config/ir_drop_report_config/neuralNetwork_routing_5track_ir_report_config.txt";
    // string neuralNetwork_routing_7track_ir_report_config = "config/ir_drop_report_config/neuralNetwork_routing_7track_ir_report_config.txt";

    // string neuralNetwork_clockTree_5track_ir_report_config = "config/ir_drop_report_config/neuralNetwork_clockTree_5track_ir_report_config.txt";
    // string neuralNetwork_clockTree_7track_ir_report_config = "config/ir_drop_report_config/neuralNetwork_clockTree_7track_ir_report_config.txt";

    // string ricv_placement_5track_ir_report_config = "config/ir_drop_report_config/ricv_placement_5track_ir_report_config.txt";
    // string riscv_placement_7track_ir_report_config = "config/ir_drop_report_config/riscv_placement_7track_ir_report_config.txt";
    // string riscv_routing_5track_ir_report_config = "config/ir_drop_report_config/riscv_routing_5track_ir_report_config.txt";
    // string riscv_routing_7track_ir_report_config = "config/ir_drop_report_config/riscv_routing_7track_ir_report_config.txt";

    // string riscv_clockTree_5track_ir_report_config = "config/ir_drop_report_config/riscv_clockTree_5track_ir_report_config.txt";
    // string riscv_clockTree_7track_ir_report_config = "config/ir_drop_report_config/riscv_clockTree_7track_ir_report_config.txt";

    config_vector.push_back(config_riscv);
    config_vector.push_back(config_gpu);
    config_vector.push_back(config_aes);
    config_vector.push_back(config_neuralNetwork);
    config_vector.push_back(config_b19);
    // config_vector.push_back(config_power_stripe);
    // config_vector.push_back(aes_placement_5track_ir_report_config);
    // config_vector.push_back(aes_placement_7track_ir_report_config);
    // config_vector.push_back(aes_routing_5track_ir_report_config);
    // config_vector.push_back(aes_routing_7track_ir_report_config);

    // config_vector.push_back(aes_clockTree_5track_ir_report_config);
    // config_vector.push_back(aes_clockTree_7track_ir_report_config);

    // config_vector.push_back(b19_placement_5track_ir_report_config);
    // config_vector.push_back(b19_placement_7track_ir_report_config);
    // config_vector.push_back(b19_routing_5track_ir_report_config);
    // config_vector.push_back(b19_routing_7track_ir_report_config);3

    // config_vector.push_back(b19_clockTree_5track_ir_report_config);
    // config_vector.push_back(b19_clockTree_7track_ir_report_config);

    // config_vector.push_back(gpu_placement_5track_ir_report_config);
    // config_vector.push_back(gpu_placement_7track_ir_report_config);
    // config_vector.push_back(gpu_routing_5track_ir_report_config);
    // config_vector.push_back(gpu_routing_7track_ir_report_config);

    // config_vector.push_back(gpu_clockTree_5track_ir_report_config);
    // config_vector.push_back(gpu_clockTree_7track_ir_report_config);

    // config_vector.push_back(neuralNetwork_placement_5track_ir_report_config);
    // config_vector.push_back(neuralNetwork_placement_7track_ir_report_config);
    // config_vector.push_back(neuralNetwork_routing_5track_ir_report_config);
    // config_vector.push_back(neuralNetwork_routing_7track_ir_report_config);

    // config_vector.push_back(neuralNetwork_clockTree_5track_ir_report_config);
    // config_vector.push_back(neuralNetwork_clockTree_7track_ir_report_config);

    // config_vector.push_back(ricv_placement_5track_ir_report_config);
    // config_vector.push_back(riscv_placement_7track_ir_report_config);
    // config_vector.push_back(riscv_routing_5track_ir_report_config);
    // config_vector.push_back(riscv_routing_7track_ir_report_config);

    // config_vector.push_back(riscv_clockTree_5track_ir_report_config);
    // config_vector.push_back(riscv_clockTree_7track_ir_report_config);

    string def_exe = "def_stripe_transform.exe ";
    string decrease_power_stripe_exe = "decrease_power_stripe_revise.exe ";
    string pin_access_power_consume_exe = "pin_access_power_consume.exe ";
    string dbscan_ir_drop_exe = "dbscan.exe ";
    string compare_ir_report_exe = "compare_ir_report.exe ";
    string caculate_metal_resource_exe = "caculate_metal_resource.exe ";

    for (int i = 0; i < config_vector.size(); i++)
    {

        //產生def_transfer file
        // string def_stripe_transform_command = "g++ def_stripe_transform.cpp -o def_stripe_transform";
        // const char *command_def = def_stripe_transform_command.c_str();
        // cout << "Compiling file using " << command_def << endl;
        // string command_exe_str = def_exe + config_vector[i];
        // const char *command_exe = command_exe_str.c_str();
        // system(command_def);
        // system(command_exe);

        // // //產生decrease tcl and wire tcl
        // string decrease_power_stripe_command = "g++ decrease_power_stripe_revise.cpp -o decrease_power_stripe_revise";
        // const char *decrease_power_stripe = decrease_power_stripe_command.c_str();
        // // cout << "Compiling file using " << command_def << endl;
        // string decrease_power_stripe_exe_str = decrease_power_stripe_exe + config_vector[i];
        // const char *decrease_power_stripe_exe = decrease_power_stripe_exe_str.c_str();
        // system(decrease_power_stripe);
        // system(decrease_power_stripe_exe);

        // // // // // // // // 產生pin access algorithm tcl
        // string pin_access_power_consume_command = "g++ pin_access_power_consume.cpp -o pin_access_power_consume";
        // const char *pin_access_power = pin_access_power_consume_command.c_str();
        // cout << "Compiling file using " << pin_access_power << endl;
        // string pin_access_power_consume_exe_str = pin_access_power_consume_exe + config_vector[i];
        // const char *pin_access_power_consume_exe = pin_access_power_consume_exe_str.c_str();
        // system(pin_access_power);
        // system(pin_access_power_consume_exe);

        // string dbscan_ir_drop_command = "g++ dbscan_ir_drop.cpp cpp/dbscan.cpp -o dbscan";
        // const char *dbscan_ir_drop = dbscan_ir_drop_command.c_str();
        // cout << "Compiling file using " << dbscan_ir_drop << endl;
        // string dbscan_ir_drop_exe_str = dbscan_ir_drop_exe + config_vector[i];
        // const char *dbscan_ir_drop_exe = dbscan_ir_drop_exe_str.c_str();
        // system(dbscan_ir_drop);
        // system(dbscan_ir_drop_exe);

        // string compare_ir_drop_command = "g++ compare_ir_report.cpp -o compare_ir_report";
        // const char *compare_ir_drop = compare_ir_drop_command.c_str();
        // cout << "Compiling file using " << compare_ir_drop << endl;
        // string compare_ir_report_exe_str = compare_ir_report_exe + config_vector[i];
        // const char *compare_ir_report_exe = compare_ir_report_exe_str.c_str();
        // system(compare_ir_drop);
        // system(compare_ir_report_exe);
    }

    // 產生def_transfer file
    // string def_stripe_transform_command = "g++ def_stripe_transform.cpp -o def_stripe_transform";
    // const char *command_def = def_stripe_transform_command.c_str();
    // // cout << "Compiling file using " << command_def << endl;
    // system(command_def);
    // system("def_stripe_transform.exe config/power_pad_config.txt");

    // // //產生decrease tcl and wire tcl
    // // string decrease_power_stripe_command = "g++ decrease_power_stripe.cpp -o decrease_power_stripe";
    // // const char *decrease_power_stripe = decrease_power_stripe_command.c_str();
    // // // cout << "Compiling file using " << command_def << endl;
    // // system(decrease_power_stripe);
    // // system("decrease_power_stripe.exe config/config_riscv.txt");

    // // // 產生pin access algorithm tcl
    // // string pin_access_power_consume_command = "g++ pin_access_power_consume.cpp -o pin_access_power_consume";
    // // const char *pin_access_power = pin_access_power_consume_command.c_str();
    // // cout << "Compiling file using " << pin_access_power << endl;
    // // system(pin_access_power);
    // // system("pin_access_power_consume.exe config/config_riscv.txt");

    // // 產生power pad tcl
    // string generate_power_pad_tcl_command = "g++ generate_power_pad_tcl.cpp -o generate_power_pad_tcl";
    // const char *command_power_pad = generate_power_pad_tcl_command.c_str();
    // cout << "Compiling file using " << command_power_pad << endl;
    // system(command_power_pad);
    // system("generate_power_pad_tcl.exe");

    // 統計有多少條power stripe
    // string ir_drop_summary_command = "g++ ir_drop_summary.cpp -o ir_drop_summary";
    // const char *command_ir_drop_summary = ir_drop_summary_command.c_str();
    // cout << "Compiling file using " << command_ir_drop_summary << endl;
    // system(command_ir_drop_summary);
    // system("ir_drop_summary.exe");

    //  string innovus_voltus = "innovus -batch -file innovus_voltus.tcl";
    //  const char *innovus_voltus_str = innovus_voltus.c_str();
    //  system(innovus_voltus_str);

    // map<string, string> filen_name_map;
    // vector<string> def_transfer_file_vector;
    // string config_file = "config/config_caculate_metal_resource.txt";
    // ifstream config(config_file);
    // string config_content;
    // string DEF_TRANSFER_FILE = "";
    // string DEF_FILE = "";

    // if (config)
    // {
    //     while (getline(config, config_content))
    //     {
    //         vector<string> config_content_array = splitByPattern(config_content, " ");
    //         // std::cout << config_content_array[0] << " " << config_content_array[2] << std::endl;
    //         if (config_content_array[0] == "DEF_FILE")
    //         {
    //             DEF_TRANSFER_FILE = config_content_array[2];
    //             filen_name_map.insert(pair<string, string>(DEF_TRANSFER_FILE, DEF_TRANSFER_FILE));
    //         }
    //         if (config_content_array[0] == "DEF_TRANSFER_FILE")
    //         {
    //             DEF_TRANSFER_FILE = config_content_array[2];
    //             def_transfer_file_vector.push_back(DEF_TRANSFER_FILE);
    //         }
    //     }
    // }
    // for (int i = 0; i < def_transfer_file_vector.size(); i++)
    // {
    //     vector<string> file_name_transfer = splitByPattern(def_transfer_file_vector[i], "/");
    //     vector<string> file_name_transfer_detail_vector = splitByPattern(file_name_transfer[2], ".");
    //     string def_transfer_file = file_name_transfer_detail_vector[0];
    //     for (auto filen_name_map_it = filen_name_map.begin(); filen_name_map_it != filen_name_map.end(); ++filen_name_map_it)
    //     {
    //         string file_name = filen_name_map_it->first;
    //         vector<string> file_name_vector = splitByPattern(file_name, "/");
    //         vector<string> file_name_detail_vector = splitByPattern(file_name_vector[2], ".");
    //         string def_file = file_name_detail_vector[0];
    //         if (def_transfer_file.find(def_file) != string::npos)
    //         {
    //             filen_name_map[file_name] = def_transfer_file_vector[i];
    //             break;
    //         }
    //     }
    // }

    // // for (auto filen_name_map_it = filen_name_map.begin(); filen_name_map_it != filen_name_map.end(); ++filen_name_map_it)
    // // {

    // //     // cout << filen_name_map_it->first << "   " << filen_name_map_it->second << endl;
    // //     string def_stripe_transform_command = "g++ def_stripe_transform.cpp -o def_stripe_transform";
    // //     const char *command_def = def_stripe_transform_command.c_str();
    // //     cout << "Compiling file using " << command_def << endl;
    // //     string command_exe_str = def_exe + filen_name_map_it->first + " " + filen_name_map_it->second;
    // //     const char *command_exe = command_exe_str.c_str();
    // //      cout << "Compiling file using " << command_exe << endl;
    // //     system(command_def);
    // //     system(command_exe);
    // // }
    // for (auto filen_name_map_it = filen_name_map.begin(); filen_name_map_it != filen_name_map.end(); ++filen_name_map_it)
    // {
    //     string caculate_metal_resource_command = "g++ caculate_metal_resource.cpp -o caculate_metal_resource";
    //     const char *caculate_metal_resource = caculate_metal_resource_command.c_str();
    //     cout << "Compiling file using " << caculate_metal_resource << endl;
    //     string caculate_metal_resource_exe_str = caculate_metal_resource_exe + filen_name_map_it->first + " " + filen_name_map_it->second;
    //     const char *caculate_metal_resource_exe = caculate_metal_resource_exe_str.c_str();
    //     system(caculate_metal_resource);
    //     system(caculate_metal_resource_exe);
    // }

    return 0;
}

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
