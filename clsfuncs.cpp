#include "dblinker.h"
#include "dict.h"
#include "genfuncs.cpp"
using namespace std;
class section;
void section::init(string myname,string myrmtype,vector<float> mybaddb,
                        float mync, float V=0){
    name=myname;
    rmtype=myrmtype;
    baddb=mybaddb;
    nc=mync;
    volume=V;
    emptyDics();
}
void section::init(string myname,string myrmtype,float mync, float V){
    name=myname;
    rmtype=myrmtype;
    nc=mync;
    volume=V;
    emptyDics();
}
void section::init(string myname,string myrmtype,float V=0){
    float mync;
    try{
        mync=ncdic->get(myrmtype);
    }catch(...){
        mync=100;
    }
    vector<float> mybaddb;
    try{
        mybaddb=badSWdic->get(myrmtype);
    }catch(...){
        mybaddb={0,0,0,0,0,0,0};
    }
    name=myname;
    rmtype=myrmtype;
    baddb=mybaddb;
    nc=mync;
    volume=V;
    emptyDics();
}
void section::configure(vector<float> mySs, vector<vector<float>> myalphas){
    Ss=mySs;
    alphas=myalphas;
    Rtot=getRsum(mySs,myalphas);
}
void section::emptyDics(){
    neighbors=new secVecDic();
    walltls=new float2dVecDic();
    wallrs=new floatVecDic();
}
void section::refresh(){ //major function to compute sound levels in this section
    vector<string> nbnums=neighbors->keys;
    if(size(nbnums)==0){
        cout<<"refresh() zero nbnum return void"<<endl;
        return void();
    }
    vector<vector<float>> tldbs=walltls->vals;
    vector<vector<float>> nbworstdbs;
    vector<vector<float>> transmitted;
    vector<vector<float>> spls;
    vector<vector<float>> alltraned;
    for (int i=0;i<size(nbnums);i++){
        string nbnum=nbnums[i];
        nbworstdbs.push_back(neighbors->get(nbnum)->getBaddb());
        vector<float> mytraned=neighbors->get(nbnum)->getBaddb()-walltls->get(nbnum);
        transmitted.push_back(mytraned);
        vector<float> myspls;
        myspls=(dbw2dbp(mytraned,wallrs->get(nbnum),Rtot));
        spls.push_back(myspls);
    }
    totspl=dbsum(spls);
    sourceswl=nbworstdbs;
    transswl=transmitted;
    transspl=spls;
    if (size(nbnums)!=size(sourceswl)||size(nbnums)!=size(tldbs)||size(nbnums)!=size(wallrs->keys)||
        size(nbnums)!=size(transswl)||size(nbnums)!=size(transspl)){
        cout<<"sizes of nbnum, walltls, wallrs sourceswl, transswl,transspl:"
        <<size(nbnums)<<","<<size(tldbs)<<","<<size(wallrs->keys)<<","<<size(sourceswl)<<","
        <<size(transswl)<<","<<size(transspl)<<endl;
        show2dvec(tldbs);
        throw invalid_argument("refresh(): inconsistent lengths");
    }
    //cout<<"refresh done"<<endl;
}
void section::sublink(section* aroom,vector<float> walldb,float ro){
    try{
        neighbors->append(aroom->name,aroom);
        walltls->append(aroom->name,walldb);
        wallrs->append(aroom->name,ro);
    }catch(invalid_argument const& e){
        neighbors->set(aroom->name,aroom);
        walltls->set(aroom->name,walldb);
        wallrs->set(aroom->name,ro);
    }
}
void section::link(section* aroom,vector<float> walldb,float ri=inf, float ro=inf){
    try{
        neighbors->append(aroom->name,aroom);
        walltls->append(aroom->name,walldb);
        wallrs->append(aroom->name,ri);
    }catch(invalid_argument const& e){
        neighbors->set(aroom->name,aroom);
        walltls->set(aroom->name,walldb);
        wallrs->set(aroom->name,ri);
    }
    aroom->sublink(this,walldb,ro);
}
void section::subdelink(section* aroom){
    try{
        neighbors->remove(aroom->name);
        walltls->remove(aroom->name);
        wallrs->remove(aroom->name);
    }catch(invalid_argument const& e){
        cout<<"section::subdelink() WARNING: room not in neighbor, cannot be removed";
    }
}
void section::delink(section* aroom){
    try{
        neighbors->remove(aroom->name);
        walltls->remove(aroom->name);
        wallrs->remove(aroom->name);
    }catch(invalid_argument const& e){
        cout<<"section::delink() WARNING: room not in neighbor, cannot be removed";
    }
    aroom->subdelink(this);
}
vector<float> section::getBaddb(){
    return baddb;
}
secVecDic* section::getNeibors(){
    return neighbors;
}
floatVecDic* section::getWallrs(){
    return wallrs;
}
float2dVecDic* section::getWalltls(){
    return walltls;
}
void section::setV(float V){
    volume=V;
}
float section::getV(){
    return volume;
}
void section::setBaddbs(vector<float> baddbs){
    baddb=baddbs;
}
void section::describe(){
    //cout<<"section::describe()"<<endl;
    refresh();
    cout<<"name: "<<name<<endl;
    cout<<"section type: "<<rmtype<<endl;
    cout<<"nc: "<<nc<<endl;
    cout<<"baddbs: ";
    showvec(baddb);
    cout<<"volume: "<<volume<<endl;
    cout<<"total noise from neighbors:";
    showvec(totspl);
    cout<<"neighbors: ";
    showvec(neighbors->keys);
    cout<<"--------neighbors info--------"<<endl;
    for (int i=0;i<size(neighbors->keys);i++){
        cout<<"neighbor:"<<neighbors->keys[i]<<endl;
        cout<<"walltls: ";
        showvec(walltls->vals[i]);
        cout<<"source SWL: ";
        showvec(sourceswl[i]);
        cout<<"transmitted SWL: ";
        showvec(transswl[i]);
        cout<<"transmitted SPL: ";
        showvec(round(transspl[i],1));
        cout<<"------------------"<<endl;
    }
}
void section::setVolume(float v){
    volume=v;
}
vector<float> section::calc_rt(){
    throw invalid_argument("section::calc_rt(): this virtual function should not be called");
    return {0};
}
vector<float> section::getrt(){
    throw invalid_argument("section::getrt(): this virtual function should not be called");
    return {0};
}
vector<float> room::calc_rt(){
    if (size(Ss)!=size(alphas)){
        throw invalid_argument("room::calc_rt(): Ss and alphas must have same length");
    }
    vector<float> Asum(size(alphas[0]),0);
    for (int i=0;i<size(Ss);i++){
        Asum=Asum+(Ss[i]*alphas[i]);
    }
    rt=0.161*volume/Asum;
    return rt;
}
vector<float> room::getrt(){
    return rt;
}
void room::describe(){
    //cout<<"room::describe()"<<endl;
    refresh();
    cout<<"name: "<<name<<endl;
    cout<<"section type: "<<rmtype<<endl;
    cout<<"NC: "<<nc<<endl;
    cout<<"noise emissions (dB): ";
    showvec(baddb);
    cout<<"volume (m^3): "<<volume<<endl;
    cout<<"wall areas (m^2): ";
    showvec(Ss);
    cout<<"absorptivities: ";
    show2dvec(alphas);
    cout<<"total noise from neighbors (dB):";
    showvec(totspl);
    cout<<"compare to NC (negative is good): ";
    showvec(totspl-ncvecdic->get(to_string((int)nc)));
    cout<<"neighbors: ";
    showvec(neighbors->keys);
    cout<<"--------neighbors info--------"<<endl;
    for (int i=0;i<size(neighbors->keys);i++){
        cout<<"neighbor:"<<neighbors->keys[i]<<endl;
        cout<<"wall transition losses (dB): ";
        showvec(walltls->vals[i]);
        cout<<"source SWL (dB): ";
        showvec(sourceswl[i]);
        cout<<"transmitted SWL (dB): ";
        showvec(transswl[i]);
        cout<<"transmitted SPL (dB): ";
        showvec(round(transspl[i],1));
        cout<<"------------------"<<endl;
    }
}
//---------------floatVecDict-------------
void floatVecDic::setAll(vector<string> mykeys,vector<float> myvals){
    if (size(mykeys)!=size(myvals)){
        throw invalid_argument("Keys and vals must have same length");
    }
    keys=mykeys;
    vals=myvals;
} 
void floatVecDic::append(string key, float val){
    for (int i=0;i<size(keys);i++){
        if (key==keys[i]){
            throw invalid_argument("floatVecDic::append(): key already exists");
            break;
        }
    }
    keys.push_back(key);
    vals.push_back(val);
}
void floatVecDic::set(string key, float val){
    int cnt=0;
    for (int i=0;i<size(keys);i++){
        if (key==keys[i]){
            vals[i]=val;
            cnt++;
        }
    }
    if (cnt!=1){
        throw invalid_argument("floatVecDic::set(): key not unique");
    }
}
float floatVecDic::get(string key){
    int i=0;
    int x=0;
    int got=0;
    for (i=0;i<size(keys);i++){
        if (keys[i]==key) {
            x=i;
            got++;
        }
    }
    if (got!=1){
        cout<<"#####EXCEPTION#####"<<endl;
        cout<<"keys and vals available:"<<endl;
        showvec(keys);
        showvec(vals);
        cout<<"key entered: "<<key<<endl;
        cout<<"####################"<<endl;
        throw invalid_argument("floatVecDic::get(): key not unique, which is not allowed");
    }
    return vals[x];
}
float floatVecDic::idget(int idx){
    return vals[idx];
}
void floatVecDic::remove(string key){
    int idx=-1;
    int found=0;
    for (int i=0;i<size(keys);i++){
        if (keys[i]==key){
            idx=i;
            found++;
        }
    }
    if (found==1){
        keys.erase(keys.begin()+idx);
        vals.erase(vals.begin()+idx);
    }else{
        throw invalid_argument("floatVecDic::remove(): key not unique");
    }
}
void floatVecDic::show(){
    int len=size(keys);
    for (int i=0;i<len;i++){
        cout<<"("<<keys[i]<<":"<<vals[i]<<");";
    }
    cout<<endl;
}
//---------------float2dVecDict-------------
void float2dVecDic::setAll(vector<string> mykeys,vector<vector<float>> myvals){
    if (size(mykeys)!=size(myvals)){
        throw invalid_argument("Keys and vals must have same length");
    }
    keys=mykeys;
    vals=myvals;
} 
void float2dVecDic::append(string key, vector<float> val){
    for (int i=0;i<size(keys);i++){
        if (key==keys[i]){
            throw invalid_argument("float2dVecDic::append(): key already exists");
            break;
        }
    }
    keys.push_back(key);
    vals.push_back(val);
}
void float2dVecDic::set(string key, vector<float> val){
    int cnt=0;
    for (int i=0;i<size(keys);i++){
        if (key==keys[i]){
            vals[i]=val;
            cnt++;
        }
    }
    if (cnt!=1){
        throw invalid_argument("float2dVecDic::set(): key not unique");
    }
}
vector<float> float2dVecDic::get(string key){
    int i=0;
    int x=0;
    int got=0;
    for (i=0;i<size(keys);i++){
        if (keys[i]==key) {
            x=i;
            got++;
        }
    }
    if (got!=1){
        cout<<"#####EXCEPTION#####"<<endl;
        cout<<"keys and vals available:"<<endl;
        showvec(keys);
        show2dvec(vals);
        cout<<"key entered: "<<key<<endl;
        cout<<"####################"<<endl;
        throw invalid_argument("float2dVecDic::get(): key not unique, which is not allowed");
    }
    return vals[x];
}
vector<float> float2dVecDic::idget(int idx){
    return vals[idx];
}
void float2dVecDic::remove(string key){
    int idx=-1;
    int found=0;
    for (int i=0;i<size(keys);i++){
        if (keys[i]==key){
            idx=i;
            found++;
        }
    }
    if (found==1){
        keys.erase(keys.begin()+idx);
        vals.erase(vals.begin()+idx);
    }else{
        throw invalid_argument("float2dVecDic::remove(): key not unique");
    }
}
void float2dVecDic::show(){
    int len=size(keys);
    for (int i=0;i<len;i++){
        cout<<keys[i]<<":";
        showvec(vals[i]);
    }
    cout<<endl;
}
//---------------SecVecDict-------------
void secVecDic::append(string key, section* val){
    for (int i=0;i<size(keys);i++){
        if (key==keys[i]){
            throw invalid_argument("secVecDic::append(): key already exists");
            break;
        }
    }
    keys.push_back(key);
    vals.push_back(val);
}
void secVecDic::set(string key, section* val){
    int cnt=0;
    for (int i=0;i<size(keys);i++){
        if (key==keys[i]){
            vals[i]=val;
            cnt++;
        }
    }
    if (cnt!=1){
        throw invalid_argument("secVecDic::set(): key not unique");
    }
}
section* secVecDic::get(string key){
    int i=0;
    int x=0;
    int got=0;
    for (i=0;i<size(keys);i++){
        if (keys[i]==key) {
            x=i;
            got++;
        }
    }
    if (got!=1){
        throw invalid_argument("section* secVecDic::get():key not unique, which is not allowed");
    }
    return vals[x];
}
section* secVecDic::idget(int idx){
    return vals[idx];
}
void secVecDic::remove(string key){
    int idx=-1;
    int found=0;
    for (int i=0;i<size(keys);i++){
        if (keys[i]==key){
            idx=i;
            found++;
        }
    }
    if (found==1){
        keys.erase(keys.begin()+idx);
        vals.erase(vals.begin()+idx);
    }else{
        throw invalid_argument("secVecDic::remove(): key not unique");
    }
}