using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

int main()
{

    // Build command to execute.  For example if the input
    // file name is a.cpp, then str holds "gcc -o a.out a.cpp"
    // Here -o is used to specify executable file name

    // string innovus_voltus_generate = "innovus -batch -file innovus_voltus_generate_pad_location.tcl";
    // const char *innovus_voltus_generate_str = innovus_voltus_generate.c_str();
    // system(innovus_voltus_generate_str);

    vector<string> config_vector;

    // string config_riscv = "config/config_riscv.txt";
    // string config_gpu = "config/config_gpu.txt";
    // string config_aes = "config/config_aes.txt";
    string config_neuralNetwork = "config/config_b19.txt";
    // string config_b19 = "config/config_b19.txt";
    // config_vector.push_back(config_riscv);
    // config_vector.push_back(config_gpu);
    // config_vector.push_back(config_aes);
    config_vector.push_back(config_neuralNetwork);
    //  config_vector.push_back(config_b19);

    string def_exe = "def_stripe_transform.exe ";
    string decrease_power_stripe_exe = "decrease_power_stripe.exe ";
    string pin_access_power_consume_exe = "pin_access_power_consume.exe ";

    for (int i = 0; i < config_vector.size(); i++)
    {
        // //產生def_transfer file
        string def_stripe_transform_command = "g++ def_stripe_transform.cpp -o def_stripe_transform";
        const char *command_def = def_stripe_transform_command.c_str();
        // cout << "Compiling file using " << command_def << endl;
        string command_exe_str = def_exe + config_vector[i];
        const char *command_exe = command_exe_str.c_str();
        system(command_def);
        system(command_exe);

        // //產生decrease tcl and wire tcl
        // string decrease_power_stripe_command = "g++ decrease_power_stripe.cpp -o decrease_power_stripe";
        // const char *decrease_power_stripe = decrease_power_stripe_command.c_str();
        // // cout << "Compiling file using " << command_def << endl;
        // string decrease_power_stripe_exe_str = decrease_power_stripe_exe + config_vector[i];
        // const char *decrease_power_stripe_exe = decrease_power_s  tripe_exe_str.c_str();
        // system(decrease_power_stripe);
        // system(decrease_power_stripe_exe);

        // 產生pin access algorithm tcl
        // string pin_access_power_consume_command = "g++ pin_access_power_consume.cpp -o pin_access_power_consume";
        // const char *pin_access_power = pin_access_power_consume_command.c_str();
        // cout << "Compiling file using " << pin_access_power << endl;
        // string pin_access_power_consume_exe_str = pin_access_power_consume_exe + config_vector[i];
        // const char *pin_access_power_consume_exe = pin_access_power_consume_exe_str.c_str();
        // system(pin_access_power);
        // system(pin_access_power_consume_exe);
    }

    // //產生def_transfer file
    // string def_stripe_transform_command = "g++ def_stripe_transform.cpp -o def_stripe_transform";
    // const char *command_def = def_stripe_transform_command.c_str();
    // // cout << "Compiling file using " << command_def << endl;
    // system(command_def);
    // system("def_stripe_transform.exe config/config_riscv.txt");

    // //產生decrease tcl and wire tcl
    // string decrease_power_stripe_command = "g++ decrease_power_stripe.cpp -o decrease_power_stripe";
    // const char *decrease_power_stripe = decrease_power_stripe_command.c_str();
    // // cout << "Compiling file using " << command_def << endl;
    // system(decrease_power_stripe);
    // system("decrease_power_stripe.exe config/config_riscv.txt");

    // // 產生pin access algorithm tcl
    // string pin_access_power_consume_command = "g++ pin_access_power_consume.cpp -o pin_access_power_consume";
    // const char *pin_access_power = pin_access_power_consume_command.c_str();
    // cout << "Compiling file using " << pin_access_power << endl;
    // system(pin_access_power);
    // system("pin_access_power_consume.exe config/config_riscv.txt");

    // 產生power pad tcl
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

    return 0;
}