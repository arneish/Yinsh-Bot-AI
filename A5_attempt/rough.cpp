// #include<iostream>
// #include<vector>
// using namespace std;
// vector<int>* function()
// {
//     vector<int>* temp_ptr = new vector<int>();
//     *temp_ptr = {1,2,3,4};
//     vector<int> &temp = *temp_ptr;
//     temp.push_back(6);
//     return temp_ptr;
// }
// int main(int argc, char** argv)
// {
//     vector<int> *ptr = function();
//     cout<<ptr->at(0)<<"\n";
//     for (auto it = ptr->begin(); it!=ptr->end(); ++it)
//     {
//         cout<<*it<<" ";
//     }
//     delete ptr;
//     cout<<"\n";
//     return 0;
// }
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

int f()
{
    std::this_thread::sleep_for(10s); //change value here to less than 1 second to see Success
    return 1;
}

int f_wrapper()
{
    std::mutex m;
    std::condition_variable cv;
    int retValue;

    std::thread t([&cv, &retValue]() 
    {
        retValue = f();
        cv.notify_one();
    });

    t.detach();

    {
        std::unique_lock<std::mutex> l(m);
        if(cv.wait_for(l, 1s) == std::cv_status::timeout) 
            throw std::runtime_error("Timeout");
    }

    return retValue;    
}

int main()
{
    bool timedout = false;
    try {
        f_wrapper();
    }
    catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        timedout = true;
    }

    if(!timedout)
        std::cout << "Success" << std::endl;

    return 0;
}