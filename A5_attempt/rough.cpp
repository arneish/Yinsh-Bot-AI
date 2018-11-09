#include<iostream>
#include<vector>
using namespace std;
vector<int>* function()
{
    vector<int>* temp_ptr = new vector<int>();
    *temp_ptr = {1,2,3,4};
    vector<int> &temp = *temp_ptr;
    temp.push_back(6);
    return temp_ptr;
}
int main(int argc, char** argv)
{
    vector<int> *ptr = function();
    cout<<ptr->at(0)<<"\n";
    for (auto it = ptr->begin(); it!=ptr->end(); ++it)
    {
        cout<<*it<<" ";
    }
    delete ptr;
    cout<<"\n";
    return 0;
}