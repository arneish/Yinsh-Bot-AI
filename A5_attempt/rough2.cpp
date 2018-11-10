#include<bits/stdc++.h>
#include<iostream>
#include<bits/stdc++.h>
#include <algorithm>
using namespace std;

struct data{
    string word;
    int number;
    bool t = true;
};

inline bool compare(const data* a, const data* b)
{
    return a->word.size()>b->word.size();
}

int main(int argc, char**argv)
{
    data* ONE = new data;
    ONE->word="one";
    data* TWO = new data;
    TWO->word = "onetwo";
    data* THREE = new data;
    THREE->word="onetwothree";
    vector<data*> temp = {ONE, TWO, THREE};
    sort(temp.begin(), temp.end(), compare);
    for (auto&elem: temp)
    {
        cout<<elem->t<<" ";
    }
//     vector<int> t = {1,2,3,4,5};
//     vector<int> q = vector<int> (10, -1);
//     q = t;
//     for (auto&elem: q)
//         std::cerr<<elem<<" ";


    return 0;
}