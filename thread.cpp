#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;
mutex mu;

void foo(int n)
{
    mu.lock();
    std::cout << "foo() " << n << std::endl;
    mu.unlock();
}

int main()
{
    std::vector<std::thread> threads;

    for (int i = 0; i < 100000; i++)
    {
        threads.push_back(std::thread(foo, i));
    }

    for (int i = 0; i < 100000; i++)
    {
        threads[i].join();
    }
    // for (int i = 0; i < 100000; i++)
    // {
    //     threads[i].detach();
    // }

    std::cout << "main() exit.\n";

    return 0;
}