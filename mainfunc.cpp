#include "clsfuncs.cpp"
using namespace std;
int main(void){
    ncdic=new floatVecDic();
    ncdic->setAll(vector<string>{"classroom","musicroom","practice","piano","chapel","office","corridor"},
                {25,25,25,25,20,25,30});
    cout<<"ncdic:";
    ncdic->show();
    ncvecdic=new float2dVecDic();
    ncvecdic->append("20",vector<float>{51,40,33,26,22,19,17});
    ncvecdic->append("25",vector<float>{54,44,37,31,27,24,22});
    ncvecdic->append("30",vector<float>{57,48,41,35,31,29,28});
    badSWdic=new float2dVecDic();
    badSWdic->setAll(vector<string>{"classroom","musicroom","practice","piano","chapel","office","corridor","board"},
                    vector<vector<float>>{{78,91,100,108,92,94,91},
                                    {78,91,100,108,92,94,91},
                                    {78,91,100,108,92,94,91},
                                    {72,85,93.5,102,85.5,87.5,85},
                                    {78,91,100,108,92,94,91},
                                    {0,70,75,80,75,68,63},
                                    {0,70,75,80,75,68,63},
                                    {7,77,82,87,82,75,70}});
    set_ncdic(ncdic);
    set_badSWdic(badSWdic);
    vector<float> wall1={36,46,62,78,89,91,90};
    vector<float> wall2={24,37,43,50,53,48,90};
    //create a pointer of type section* pointing to a room object
    section *s1=new room();
    s1->init("room1","s1type",{72,85,93.5,102,85.5,87.5,85},10,50);
    s1->configure(vector<float>{11.1,12.4,13.3},//numbers for denstration purpose only, not realistic
        vector<vector<float>>{{0.033333333, 0.02, 0.02, 0.03, 0.04, 0.04, 0.05},
                              {0.705, 0.24, 0.4, 0.78, 0.98, 0.96, 0.87},
                              {0.5485, 0.529, 0.582, 0.58, 0.569, 0.463, 0.516}});
    //create a pointer of type section* pointing to a section object
    //it will be demonstrated that a section and a room can link to each other
    section *s2=new section();
    s2->init("section2","practice");//note that nc and baddb are not given but automatically assigned by rmtype
    
    //create a pointer of type section* pointing to a section object
    section *s3=new section();
    s3->init("section3","corridor");
    
    //create a pointer of type section* pointing to a section object
    section *s4=new section();
    s4->init("section4","office");

    //create a pointer of type section* pointing to a section object
    section *s5=new section();
    s5->init("section5","piano");

    //create a pointer of type section* pointing to a room object
    section *s0=new room();
    s0->init("room0","classroom",70);
    s0->configure(vector<float>{29.5,12.8},
        vector<vector<float>>{{0.11,0.12,0.13,0.24,0.25,0.17,0.16},
                              {0.543333333, 0.3, 0.45, 0.65, 0.56, 0.59, 0.71}});
    
    s0->link(s1,wall1,1,1);
    cout<<"###s0 linked to s1###"<<endl;
    s2->link(s0,wall2,1,1);
    cout<<"###s2 linked to s0###"<<endl;
    s3->link(s0,wall1,1,1);
    cout<<"###s3 linked to s0###"<<endl;
    s0->link(s4,wall2,1,1);
    cout<<"###s0 linked to s4###"<<endl;
    s4->link(s5,wall1,1,1);
    cout<<"###s4 linked to s5###"<<endl;
    s3->link(s4,wall1,1,1);
    cout<<"###s3 linked to s4###"<<endl;
    cout<<"-----find s0 in s5's neighbor s3'neighbors and describe-----"<<endl;
    s5->getNeibors()->get("section4")->getNeibors()->get("room0")->describe();
    //notice that s0 can be found by name in s3's neighbors similar to a linked list 
    //s0->describe();//will give the same result

    s4->delink(s0);
    cout<<"###s4 delinked with s0###"<<endl;
    s0->delink(s2);
    cout<<"###s0 delinked with s2###"<<endl;
    cout<<"-----describe s0 after delinking with s1 and s2-----"<<endl;
    s0->describe(); //only s3 remaining
    cout<<"-----s0 reverberation time in seconds (7 octaves)-----"<<endl;
    s0->calc_rt();
    showvec(s0->getrt());
}