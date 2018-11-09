#include<bits/stdc++.h>
#include<iostream>
using namespace std;

int main(int argc, char**argv)
{
    vector<int> t = {1,2,3,4,5};
    vector<int> q = vector<int> (10, -1);
    q = t;
    for (auto&elem: q)
        std::cerr<<elem<<" ";


    return 0;
}