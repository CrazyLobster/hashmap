//
// Created by Shaneking on 2021/6/30.
//

#ifndef HASHMAP_HASHMAP_H
#define HASHMAP_HASHMAP_H

#include <x86intrin.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
template <typename T>
struct hashunit{
    unsigned int id;
    T unitvalue;
};
template <typename T>
struct hash_bucket{
    hashunit<T>*units;
    unsigned short unitnumber;
    unsigned short unitmax;
};

inline unsigned int remainder(unsigned int input,int mask){
    return(input-((input>>mask)<<mask));
}

template<typename T>
class hashmap{
public:
    hashmap();
    hashmap(int unitsstimate,bool keyconflict);
    bool remapbucket(unsigned int index);
    hashunit<T> getunit(unsigned int id);
    bool insertunit(unsigned int id,T unit);
    int getremapbucketcnt(){return remapbucketcnt;}
    void getunitnumberclass();
    hash_bucket<T>getbucket(unsigned int index);
    ~hashmap();

private:
    int bucketnumber;
    int mask;
    int remapbucketcnt=0;
    int insertcnt=0;
    hash_bucket<T>*hbuckets=NULL;
    bool keyconflict=false;
};

template<typename T>
hashmap<T>::hashmap(int unitsstimate,bool keyconflict){
    int quto=unitsstimate>>6;
    cout<<"quto="<<quto<<endl;
    int n=0;

    for(;(2<<n)<quto;n++){
        int x=(2<<n);
        cout<<"n="<<n<<",quto="<<quto<<",2^n="<<x<<endl;
    }
    mask=n;
    bucketnumber=(2<<mask);
    mask=n+1;
    cout<<mask<<","<<bucketnumber<<endl;
    hbuckets=new hash_bucket<T>[bucketnumber];
    for(int i;i<bucketnumber;i++){
        hbuckets[i].units=new hashunit<T>[64];
        hbuckets[i].unitnumber=0;
        hbuckets[i].unitmax=64;
    }
    this->keyconflict=keyconflict;
}

template<typename T>
bool hashmap<T>::remapbucket(unsigned int index)
{
    hash_bucket<T>tmpbucket=hbuckets[index];
    unsigned int oldmax=tmpbucket.unitmax;
    tmpbucket.unitmax=oldmax+10;
    hashunit<T>*newunits=new hashunit<T>[tmpbucket.unitmax];
    memcpy(newunits,tmpbucket.units,sizeof(hashunit<T>)*tmpbucket.unitnumber);
    delete[]tmpbucket.units;
    tmpbucket.units=newunits;
    hbuckets[index]=tmpbucket;
    remapbucketcnt++;
//cout<<"bucketindex="<<index<<",oldmax="<<oldmax<<",units="<<hbuckets[index].unitnumber<<",remapbucketcnt="<<remapbucketcnt<<",insertcnt="<<insertcnt<<endl;
    return true;
}
template<typename T>
hashunit<T>hashmap<T>::getunit(unsigned int id){
//cout<<"getunit";
    unsigned int index=remainder(id,mask);
    hash_bucket<T>hbucket=hbuckets[index];
    hashunit<T>res;
    int ires=0;
    for(int i=0;i<hbucket.unitnumber;i++){
        int tmp=_mm_popcnt_u32(_mm_popcnt_u32(_mm_popcnt_u32(_mm_popcnt_u32(_mm_popcnt_u32(id^hbucket.units[i].id)))));
        int size=!tmp;
        ires+=i*size;
//cout<<"ires="<<ires<<",i="<<i<<",id="<<hbucket.units[i].id<<",size="<<size<<endl;
    }
//cout<<endl;
//cout<<"unitindex="<<ires<<"|id="<<hbucket.units[ires].id<<"|value="<<hbucket.units[ires].unitvalue<<",";
    return hbucket.units[ires];
}
template<typename T>
bool hashmap<T>::insertunit(unsigned int id,T unit){
    unsigned int index=remainder(id,mask);
    hash_bucket<T>*hbucket=&hbuckets[index];
    if(hbucket->unitnumber==hbucket->unitmax){
        remapbucket(index);
    }
    hbucket=&hbuckets[index];
    hbucket->units[hbucket->unitnumber].id=id;
    hbucket->units[hbucket->unitnumber].unitvalue=unit;
    hbucket->unitnumber=hbucket->unitnumber+1;
    insertcnt++;
    return true;
}
template<typename T>
void hashmap<T>::getunitnumberclass(){
    int cnt0=0;
    int cnt10=0;
    int cnt20=0;
    int cnt30=0;
    int cnt40=0;
    int cntup40=0;
    for(int i=0;i<bucketnumber;i++){
        if(hbuckets[i].unitnumber==0){
            cnt0++;
        }elseif(hbuckets[i].unitnumber>0&&hbuckets[i].unitnumber<10){
            cnt10++;
        }elseif(hbuckets[i].unitnumber>=10&&hbuckets[i].unitnumber<20){
            cnt20++;
        }elseif(hbuckets[i].unitnumber>=20&&hbuckets[i].unitnumber<30){
            cnt30++;
        }elseif(hbuckets[i].unitnumber>=30&&hbuckets[i].unitnumber<40){
            cnt40++;
        }elseif(hbuckets[i].unitnumber>=40){
            cntup40++;
        }
    }
    cout<<"cnt0="<<cnt0<<"cnt10="<<cnt10<<",cnt20="<<cnt20<<",cnt30="<<cnt30<<",cnt40="<<cnt40<<",upcnt40="<<cntup40<<endl;
}
template<typename T>
hashmap<T>::~hashmap(){
    for(int i;i<bucketnumber;i++){
        delete[]hbuckets[i].units;
    }
    delete[]hbuckets;
}
template<typename T>
hash_bucket<T>hashmap<T>::getbucket(unsigned int index){
    return hbuckets[index];
}

#endif //HASHMAP_HASHMAP_H
