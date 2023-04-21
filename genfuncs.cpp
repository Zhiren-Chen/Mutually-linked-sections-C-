#include "dblinker.h"
#include "dict.h"
using namespace std;
static floatVecDic* ncdic;
static float2dVecDic* badSWdic;
static float2dVecDic* ncvecdic;
void set_ncdic(floatVecDic* ancdic){
    ncdic=ancdic;
}
void set_badSWdic(float2dVecDic* abadSWdic){
    badSWdic=abadSWdic;
}
template <class T> void showvec(vector<T> a){
    for (int i=0;i<a.size();i++){
        cout<<a[i]<<",";
    }
    cout<<endl;
}
template <class T> void show2dvec(vector<vector<T>> a){
    for(int j=0;j<a.size();j++){
        cout<<"(";
        for (int i=0;i<a[j].size();i++){
            cout<<a[j][i]<<",";
        }
        cout<<");";
    }
    cout<<endl;
}
int len(float* d){
    return sizeof(d)/sizeof(*d);
}
float max(vector<float> a){
    float vmax=-inf;
    for(int i=0;i<size(a);i++){
        if (a[i]>vmax){
            vmax=a[i];
        }
    }
    return vmax;
}
float min(vector<float> a){
    float vmin=inf;
    for(int i=0;i<size(a);i++){
        if (a[i]<vmin){
            vmin=a[i];
        }
    }
    return vmin;
}
float maxid(vector<float> a){
    float vmax=-inf;
    int id=0;
    for(int i=0;i<size(a);i++){
        if (a[i]>vmax){
            vmax=a[i];
            id=i;
        }
    }
    return id;
}
float minid(vector<float> a){
    float vmin=inf;
    int id=0;
    for(int i=0;i<size(a);i++){
        if (a[i]<vmin){
            vmin=a[i];
            id=i;
        }
    }
    return id;
}
float sum(vector<float> a){
    float sum=0;
    for(int i=0;i<dbnum;i++){
        sum+=a[i];
    }
    return sum;
}
float max(float* a,int len=dbnum){
    float vmax=-inf;
    for(int i=0;i<dbnum;i++){
        if (a[i]>vmax){
            vmax=a[i];
        }
    }
    return vmax;
}
float min(float* a,int len=dbnum){
    float vmin=inf;
    for(int i=0;i<dbnum;i++){
        if (a[i]<vmin){
            vmin=a[i];
        }
    }
    return vmin;
}
float sum(float* a,int len=dbnum){
    float sum=0;
    for(int i=0;i<dbnum;i++){
        sum+=a[i];
    }
    return sum;
}
void showarr(float a[],int len=dbnum){
    for (int i=0;i<len;i++){
        cout<<a[i]<<",";
    }
    cout<<endl;
}
vector<float> operator+ (float a,vector<float> b){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a+b[i];
    }
    return c;
}
vector<float> operator+ (vector<float> b,float a){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a+b[i];
    }
    return c;
}
vector<float> operator+ (vector<float> a,vector<float> b){
    if (size(a)==1){
        cout<<"Warning: + casted single length vector to float\n";
        return a[0]+b;
    }else if(size(b)==1){
        cout<<"Warning: + casted single length vector to float\n";
        return a+b[0];
    }
    if (size(a)!=size(b)){
        throw invalid_argument("+: a and b must have same length to add");
    }
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a[i]+b[i];
    }
    return c;
}
vector<float> operator- (float a,vector<float> b){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a-b[i];
    }
    return c;
}
vector<float> operator- (vector<float> b,float a){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=-a+b[i];
    }
    return c;
}
vector<float> operator- (vector<float> a,vector<float> b){
    if (size(a)!=size(b)){
        throw invalid_argument("a and b must have same length to subtract");
    }
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a[i]-b[i];
    }
    return c;
}
vector<float> operator* (float a,vector<float> b){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a*b[i];
    }
    return c;
}
vector<float> operator* (vector<float> b,float a){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a*b[i];
    }
    return c;
}
vector<float> operator* (vector<float> a,vector<float> b){
    if (size(a)!=size(b)){
        throw invalid_argument("a and b must have same length to multiply");
    }
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a[i]*b[i];
    }
    return c;
}
vector<float> operator/ (float a,vector<float> b){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a/b[i];
    }
    return c;
}
vector<float> operator/ (vector<float> b,float a){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=b[i]/a;
    }
    return c;
}
vector<float> operator/ (vector<float> a,vector<float> b){
    if (size(a)!=size(b)){
        throw invalid_argument("a and b must have same length to divide");
    }
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=a[i]/b[i];
    }
    return c;
}
vector<float> pow(float a, vector<float> b){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=pow(a,b[i]);
    }
    return c;
}
vector<float> log10(vector<float> b){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=log10(b[i]);
    }
    return c;
}
float round(float b,int p){
    int mag=pow(10,p);
    float bigval=round(b*mag);
    float val=bigval/mag;
    return val;
}
vector<float> round(vector<float> b,int p){
    vector<float> c(size(b),0);
    for (int i=0;i<size(b);i++){
        c[i]=round(b[i],p);
    }
    return c;
}
vector<float> dbsum(vector<vector<float>> a){
    int leng=size(a[0]);
    int num=size(a);
    vector<float> sww(leng,0);
    for (int i=0;i<num;i++){
        if (size(a[i])!=leng){
            throw invalid_argument("dbsum(): all vectors in 2Dvector must have same length");
        }
        sww=sww+pow(10,(a[i]/10));
    }
    sww=round(10*log10(sww),1);
    return sww;
}
vector<float> dbw2dbp(vector<float> dbw,float r, float R, float Q=1){
    vector<float> dbp;
    dbp=dbw+(10*log10((Q/(4*3.14159*r*r))+(4/R)));
    return dbp;
}
vector<float> dbw2dbp(vector<float> dbw,float r, vector<float> R, float Q=1){
    if (size(R)!=size(dbw)){
        throw invalid_argument("dbw2dbp(): dbw and R must have same length");
    }
    vector<float> dbp;
    dbp=dbw+(10*log10((Q/(4*3.14159*r*r))+(4/R)));
    return dbp;
}
vector<float> getRsum(vector<float> Ss, vector<vector<float>> alphas){
    vector<float> Rsum=vector<float>(dbnum,0);
    for(int i=0;i<size(Ss);i++){
        if (max(alphas[i])<1){
            vector<float> adder=(Ss[i]*alphas[i]/(1-alphas[i]));
            Rsum=Rsum+adder;
        }else{
            Rsum=vector<float>(dbnum,0)+inf;
            cout<<"Warning: received alpha>1"<<endl;
            break;
        }
    }
    return Rsum;
}