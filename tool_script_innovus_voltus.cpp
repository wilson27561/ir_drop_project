using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{

    // Build command to execute.  For example if the input
    // file name is a.cpp, then str holds "gcc -o a.out a.cpp"
    // Here -o is used to specify executable file name
    
    // string innovus_voltus_generate = "innovus -batch -file innovus_voltus_generate_pad_location.tcl";
    // const char *innovus_voltus_generate_str = innovus_voltus_generate.c_str();
    // system(innovus_voltus_generate_str);


    string def_stripe_transform_command = "g++ def_stripe_transform.cpp -o def_stripe_transform";
    const char *command_def = def_stripe_transform_command.c_str();
    cout << "Compiling file using " << command_def << endl;
    system(command_def);
    system("def_stripe_transform.exe");

    // 產生power pad tcl
    // string generate_power_pad_tcl_command = "g++ generate_power_pad_tcl.cpp -o generate_power_pad_tcl";
    // const char *command_power_pad = generate_power_pad_tcl_command.c_str();
    // cout << "Compiling file using " << command_power_pad << endl;
    // system(command_power_pad);
    // system("generate_power_pad_tcl.exe");

    // string generate_power_pad_tcl_command = "g++ estimate_number_of_power_stripe_by_partition_power_pad.cpp -o estimate_number_of_power_stripe_by_partition_power_pad";
    // const char *command_power_pad = generate_power_pad_tcl_command.c_str();
    // cout << "Compiling file using " << command_power_pad << endl;
    // system(command_power_pad);
    // system("estimate_number_of_power_stripe_by_partition_power_pad.exe");

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