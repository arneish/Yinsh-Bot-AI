#include<bits/stdc++.h>
#include<iostream>
#include<bits/stdc++.h>
#include <algorithm>
using namespace std;

struct data{
    string word;
    int number;
    int score;
    bool t = true;
};
inline bool OrderForMaxNode(const data *a, const data *b)
{
	return a->score > b->score;
}
inline bool OrderForMinNode(const data *a, const data *b)
{
	return a->score < b->score;
}

inline bool compare(const data* a, const data* b)
{
    return a->word.size()>b->word.size();
}

int main(int argc, char**argv)
{
    data* ONE = new data;
    ONE->score = 100;
    data* TWO = new data;
    TWO->score = 200;
    data* THREE = new data;
    THREE->score = 50;
    vector<data*> temp = {ONE, TWO, THREE};
    sort(temp.begin(), temp.end(), OrderForMaxNode);
    for (auto&elem: temp)
    {
        cout<<elem->score<<" ";
    }
    sort(temp.begin(), temp.end(), OrderForMinNode);
    for (auto&elem: temp)
    {
        cout<<elem->score<<" ";
    }
//     vector<int> t = {1,2,3,4,5};
//     vector<int> q = vector<int> (10, -1);
//     q = t;
//     for (auto&elem: q)
//         std::cerr<<elem<<" ";


    return 0;
}