#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
using namespace std;
class section;
class floatVecDic{
    public:
    vector<string> keys=vector<string>{};
    vector<float> vals=vector<float>{};
    void setAll(vector<string> mykeys,vector<float> myvals);
    void append(string key, float val);
    void set(string key, float val);
    float get(string key);
    float idget(int idx);
    void remove(string key);
    void show();
};
class float2dVecDic{
    public:
    vector<string> keys=vector<string>{};
    vector<vector<float>> vals=vector<vector<float>>{};
    void setAll(vector<string> keys,vector<vector<float>> vals);
    void append(string key, vector<float> val);
    void set(string key, vector<float> val);
    vector<float> get(string key);
    vector<float> idget(int idx);
    void remove(string key);
    void show();
};
class secVecDic{
    public:
    //section *t;
    vector<string> keys=vector<string>{};
    vector<section*> vals=vector<section*>{};
    void setAll(vector<string> keys,vector<section*> vals);
    void append(string key, section* val);
    void set(string key, section* val);
    section* get(string key);
    section* idget(int idx);
    void remove(string key);
    void show();
};
