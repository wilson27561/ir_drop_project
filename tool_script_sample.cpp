using namespace std;
#include <iostream>
#include <stdio.h>
#include <stdlib.h>


int main()
{
    cout << "hello world !" << endl;
  
    cout << "Enter file name to compile "; 
    
  
    // Build command to execute.  For example if the input 
    // file name is a.cpp, then str holds "gcc -o a.out a.cpp"  
    // Here -o is used to specify executable file name 
    string str = "g++ "; 
    str = str + "test_too.cpp -o test_too "; 

//   cd "d:\ir_drop_project\" ; if ($?) { g++ def_stripe_transform.cpp -o def_stripe_transform } ; if ($?) { .\def_stripe_transform }

    // Convert string to const char * as system requires 
    // parameter of type const char * 
    const char *command = str.c_str(); 
  
    cout << "Compiling file using " << command << endl; 
    system(command); 
  
    string compile = "./test_too";
    const char *compile_str = compile.c_str(); 
    // cout << "\nRunning file "; 
    system(compile_str); 
  
    

     string tcl_test = "voltus -batch -file voltus_file.tcl";
     const char *tcl_test_str = tcl_test.c_str(); 
     system(tcl_test_str); 


 return 0;
}