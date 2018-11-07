#include<iostream>
#include<vector>
using namespace std;
int main(int argc, char** argv)
{
    int *ptr;
    int *ptr2;
    int *ptr3;
    ptr = new int(3);
    ptr2 = new int (4);
    ptr3 = new int (5);
    vector<int*> vec = {ptr, ptr2, ptr3};
    for (auto&elem: vec)
    {
        cout<<*elem<<"\n";
    }
    cout<<"\n";
    *ptr3 = 6;
    for (auto&elem: vec)
    {
        cout<<*elem<<"\n";
    }
    return 0;
}