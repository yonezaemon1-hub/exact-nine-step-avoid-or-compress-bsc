#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <numeric>
#include <string>
using namespace std;
struct T{uint8_t x[11];};
static constexpr uint16_t S=(1u<<8)-1u;
uint16_t img(uint16_t m,const T&t){uint16_t o=0;while(m){unsigned i=__builtin_ctz((unsigned)m);m&=m-1;o|=(1u<<t.x[i]);}return o;}
bool safe(uint16_t m){return __builtin_popcount((unsigned)m)==8 && (m&1u);}
bool unary9(const T&t){uint16_t m=S;for(int k=0;k<9;k++){m=img(m,t);if(!safe(m))return false;}return true;}
uint64_t enc(const T&t){uint64_t z=0;for(int i=0;i<11;i++)z|=(uint64_t)t.x[i]<<(4*i);return z;}
struct DSU{vector<int>p,s;DSU(int n):p(n),s(n,1){iota(p.begin(),p.end(),0);}int f(int x){return p[x]==x?x:p[x]=f(p[x]);}void u(int a,int b){a=f(a);b=f(b);if(a==b)return;if(s[a]<s[b])swap(a,b);p[b]=a;s[a]+=s[b];}};
array<int,11> transp(int a,int b){array<int,11>p{};for(int i=0;i<11;i++)p[i]=i;swap(p[a],p[b]);return p;}
T conj(const T&t,const array<int,11>&p){int inv[11];for(int i=0;i<11;i++)inv[p[i]]=i;T u{};for(int i=0;i<11;i++)u.x[i]=p[t.x[inv[i]]];return u;}

template<class F> void gen_I_Psing(F f){
 const int block[10][2]={{1,8},{0,-1},{2,-1},{3,-1},{4,-1},{5,-1},{6,-1},{7,-1},{9,-1},{10,-1}};const int bs[10]={2,1,1,1,1,1,1,1,1,1};int special=2;
 for(int omitted=1;omitted<=10;omitted++){int vals[9],z=0;for(int v=1;v<=10;v++)if(v!=omitted)vals[z++]=v;do{T t{};t.x[2]=0;int q=0;for(int bl=0;bl<10;bl++)if(bl!=special){int v=vals[q++];for(int k=0;k<bs[bl];k++)t.x[block[bl][k]]=v;}if(unary9(t))f(t);}while(next_permutation(vals,vals+9));}}
template<class F> void gen_I_Pperm(F f){T t{};t.x[2]=0;int pos[10]={0,1,3,4,5,6,7,8,9,10};int vals[10]={1,2,3,4,5,6,7,8,9,10};do{for(int i=0;i<10;i++)t.x[pos[i]]=vals[i];if(unary9(t))f(t);}while(next_permutation(vals,vals+10));}
template<class F> void gen_O_Psing(F f){
 const int block[10][2]={{8,9},{0,-1},{1,-1},{2,-1},{3,-1},{4,-1},{5,-1},{6,-1},{7,-1},{10,-1}};const int bs[10]={2,1,1,1,1,1,1,1,1,1};int special=2;
 for(int omitted=1;omitted<=10;omitted++){int vals[9],z=0;for(int v=1;v<=10;v++)if(v!=omitted)vals[z++]=v;do{T t{};t.x[1]=0;int q=0;for(int bl=0;bl<10;bl++)if(bl!=special){int v=vals[q++];for(int k=0;k<bs[bl];k++)t.x[block[bl][k]]=v;}if(unary9(t))f(t);}while(next_permutation(vals,vals+9));}}
template<class F> void gen_O_Pperm(F f){T t{};t.x[1]=0;int pos[10]={0,2,3,4,5,6,7,8,9,10};int vals[10]={1,2,3,4,5,6,7,8,9,10};do{for(int i=0;i<10;i++)t.x[pos[i]]=vals[i];if(unary9(t))f(t);}while(next_permutation(vals,vals+10));}

template<class Gen> void audit(string name,Gen gen,vector<array<int,11>> gs,long expected){
 vector<T>a; a.reserve(1000000); gen([&](const T&t){a.push_back(t);});
 unordered_map<uint64_t,int> idx; idx.reserve(a.size()*2);for(int i=0;i<(int)a.size();i++)idx.emplace(enc(a[i]),i);
 DSU d(a.size()); long long edges=0,missing=0;for(int i=0;i<(int)a.size();i++)for(auto&p:gs){auto u=conj(a[i],p);auto it=idx.find(enc(u));edges++;if(it==idx.end())missing++;else d.u(i,it->second);}long comps=0,sum=0;for(int i=0;i<(int)a.size();i++)if(d.f(i)==i){comps++;sum+=d.s[d.f(i)];}
 cout<<name<<" candidates="<<a.size()<<" generators="<<gs.size()<<" edges="<<edges<<" missing="<<missing<<" orbits="<<comps<<" sum="<<sum<<" expected="<<expected<<" PASS="<<(comps==expected&&missing==0&&sum==(long)a.size())<<"\n";
}
int main(){
 vector<array<int,11>> gI={transp(3,4),transp(4,5),transp(5,6),transp(6,7),transp(9,10)};
 vector<array<int,11>> gO={transp(2,3),transp(3,4),transp(4,5),transp(5,6),transp(6,7),transp(8,9)};
 audit("I_Psing",[](auto f){gen_I_Psing(f);},gI,3030);
 audit("I_Pperm",[](auto f){gen_I_Pperm(f);},gI,5520);
 audit("O_Psing",[](auto f){gen_O_Psing(f);},gO,569);
 audit("O_Pperm",[](auto f){gen_O_Pperm(f);},gO,1186);
}
