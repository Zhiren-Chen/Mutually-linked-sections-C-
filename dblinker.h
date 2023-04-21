//#ifndef DBLINKER.H
//#define DBLINKER.H
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <cmath>
#include "dict.h"
using namespace std;
const int dbnum=7;
const float inf=numeric_limits<float>::infinity();
const float minf=0;
class floatVecDic;
class float2dVecDic;
class secVecDic;
class infoDic;
class section;
class room;
class section {
protected:
    vector<float> baddb; //the sound power level in dB at 7 octaves (63-4000Hz) emitted by the section itself  
    secVecDic* neighbors=new secVecDic(); //the linked neighbor section objects of this section
    float2dVecDic* walltls=new float2dVecDic(); //transmission losses (7 octaves) of each wall to each neighbor
    floatVecDic* wallrs=new floatVecDic(); 
    //distance from the concerned point in this section to each wall to each neighbor, usually default to 1m
    float volume;//volume in m^3
    vector<float> totspl; //total sound pressure level (7 octaves) in dB
    vector<float> Rtot; //total R value (7 octaves)
    vector<float> Ss; //area of each type of wall surface in m^2
    vector<vector<float>> alphas{{0}}; //wall material absorptivity in 7 octaves of each type of wall surface
    vector<vector<float>> sourceswl{{0}}; //emitted sound power level (7 octaves) of each neighbor
    vector<vector<float>> transswl{{0}}; //transmitted sound power level (7 octaves) of each neighbor
    vector<vector<float>> transspl{{0}}; //sound pressure level (7 octaves) resulted from each neighbor
public:
    string name; //name of this section, used as an index
    float nc; //NC noise criterion of 7 octaves
    string rmtype; //type of section, can be used to access default parameters
    //section(int v);
    void init(string myname,string myrmtype,vector<float> mybaddb,float mync,float V);
    void init(string myname,string myrmtype,float mync,float V);
    void init(string myname,string myrmtype,float V);
    void configure(vector<float> mySs, vector<vector<float>> myalphas);
    void emptyDics();
    void refresh();
    secVecDic* getNeibors();
    floatVecDic* getWallrs();
    float2dVecDic* getWalltls();
    void link(section* aroom,vector<float> walldb,float ri, float ro);
    void sublink(section* aroom,vector<float> walldb,float ro);
    void delink(section* aroom);
    void subdelink(section* aroom);
    void setV(float V);
    float getV();
    vector<float> getBaddb();
    void setBaddbs(vector<float> baddbs);
    void setVolume(float v);
    virtual void describe();
    virtual vector<float> calc_rt();//interface to be overrided
    virtual vector<float> getrt();//interface to be overrided
};
class room:public section{
    private:
    vector<float> rt; //reverberation time in second
    public:
    vector<float> calc_rt();
    vector<float> getrt();
    void describe();
};
//#endif