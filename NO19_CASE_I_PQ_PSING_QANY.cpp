#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <numeric>
#include <chrono>

struct T { uint8_t x[11]; };
static constexpr uint16_t S=(1u<<8)-1u;
inline uint16_t img(uint16_t m,const T&t){uint16_t o=0;while(m){unsigned i=__builtin_ctz((unsigned)m);m&=m-1;o|=(uint16_t)(1u<<t.x[i]);}return o;}
inline bool safe(uint16_t m){return __builtin_popcount((unsigned)m)==8&&(m&1u);} 
inline bool unary9(const T&t){uint16_t m=S;for(int j=0;j<9;j++){m=img(m,t);if(!safe(m))return false;}return true;}
uint64_t enc(const T&t){uint64_t z=0;for(int i=0;i<11;i++)z|=(uint64_t)t.x[i]<<(4*i);return z;}
T dec(uint64_t z){T t{};for(int i=0;i<11;i++)t.x[i]=(z>>(4*i))&15;return t;}

struct Basis{
  long long r[10][11]{}; int piv[10]{}; int n=0;
  static long long gcdrow(long long *v){long long g=0;for(int j=0;j<11;j++)g=std::gcd(g,std::llabs(v[j]));return g;}
  bool addMask(uint16_t m){
    long long v[11];for(int j=0;j<11;j++)v[j]=((m>>j)&1)-((S>>j)&1);
    for(int k=0;k<n;k++){int p=piv[k]; if(v[p]==0)continue; long long a=r[k][p], b=v[p]; for(int j=p;j<11;j++)v[j]=v[j]*a-r[k][j]*b; long long g=gcdrow(v); if(g>1)for(int j=p;j<11;j++)v[j]/=g;}
    int p=-1;for(int j=0;j<11;j++)if(v[j]){p=j;break;} if(p<0)return false;
    long long g=gcdrow(v);if(g>1)for(int j=p;j<11;j++)v[j]/=g;if(v[p]<0)for(int j=p;j<11;j++)v[j]=-v[j];
    // insert sorted by pivot (new pivot should be after existing if reduced against all, but may be between only if rows aren't fully sorted; handle)
    int pos=n;while(pos>0&&piv[pos-1]>p){for(int j=0;j<11;j++)r[pos][j]=r[pos-1][j];piv[pos]=piv[pos-1];pos--;}
    for(int j=0;j<11;j++)r[pos][j]=v[j];piv[pos]=p;n++;return true;
  }
};

inline int affrank4(uint16_t X,uint16_t Y,uint16_t Z){Basis b;b.addMask(X);b.addMask(Y);b.addMask(Z);return b.n;}

bool strong_conn(const T&a,const T&b){
  for(int start=0;start<11;start++){bool vis[11]{};int q[11],h=0,t=0;q[t++]=start;vis[start]=1;while(h<t){int u=q[h++];int vs[2]={a.x[u],b.x[u]};for(int z:vs)if(!vis[z]){vis[z]=1;q[t++]=z;}}if(t<11)return false;}return true;
}
bool synchronizing(const T&a,const T&b){
  for(int x0=0;x0<11;x0++)for(int y0=x0+1;y0<11;y0++){
    bool vis[11][11]{};std::pair<int,int> q[128];int h=0,t=0;q[t++]={x0,y0};vis[x0][y0]=1;bool ok=false;
    while(h<t&&!ok){auto [x,y]=q[h++];for(const T*tt:{&a,&b}){int u=tt->x[x],v=tt->x[y];if(u==v){ok=true;break;}if(u>v)std::swap(u,v);if(!vis[u][v]){vis[u][v]=1;q[t++]={u,v};}}}
    if(!ok)return false;
  }return true;
}
bool pair_ok(const T&a,const T&b){
  bool seen[2048]{}; uint16_t front[128], next[128]; int nf=1;front[0]=S;seen[S]=true; Basis bas;
  for(int depth=1;depth<=9;depth++){
    int nn=0;
    for(int i=0;i<nf;i++){
      uint16_t u=front[i]; uint16_t va=img(u,a), vb=img(u,b);
      if(!safe(va)||!safe(vb))return false;
      if(!seen[va]){seen[va]=true;next[nn++]=va;if(bas.addMask(va)&&bas.n>depth)return false;}
      if(!seen[vb]){seen[vb]=true;next[nn++]=vb;if(bas.addMask(vb)&&bas.n>depth)return false;}
    }
    if(bas.n!=depth)return false;
    nf=nn;for(int i=0;i<nn;i++)front[i]=next[i];
  }
  return true;
}

template<class F> void eachSing(int type,F f){ // type 1 Q, 2 D
 const int block[10][2]={{1,8},{0,-1},{2,-1},{3,-1},{4,-1},{5,-1},{6,-1},{7,-1},{9,-1},{10,-1}}; const int bs[10]={2,1,1,1,1,1,1,1,1,1}; int special=(type==2?0:(type==1?1:2));
 for(int omitted=1;omitted<=10;omitted++){int vals[9],z=0;for(int v=1;v<=10;v++)if(v!=omitted)vals[z++]=v;do{T t{};for(int k=0;k<bs[special];k++)t.x[block[special][k]]=0;int q=0;for(int bl=0;bl<10;bl++)if(bl!=special){int v=vals[q++];for(int k=0;k<bs[bl];k++)t.x[block[bl][k]]=v;}if(unary9(t))f(t);}while(std::next_permutation(vals,vals+9));}
}
template<class F> void eachPperm(F f){T t{};t.x[2]=0;int pos[10]={0,1,3,4,5,6,7,8,9,10};int vals[10]={1,2,3,4,5,6,7,8,9,10};do{for(int i=0;i<10;i++)t.x[pos[i]]=vals[i];if(unary9(t))f(t);}while(std::next_permutation(vals,vals+10));}
template<class F> void eachQperm(F f){T t{};t.x[0]=0;int vals[10]={1,2,3,4,5,6,7,8,9,10};do{for(int i=0;i<10;i++)t.x[i+1]=vals[i];f(t);}while(std::next_permutation(vals,vals+10));}

std::vector<T> makeAreps(){
 std::unordered_set<uint64_t> pool;pool.reserve(600000);eachSing(0,[&](const T&t){pool.insert(enc(t));});
 std::vector<std::array<int,11>> gs;int in[5]={3,4,5,6,7};do{int out[2]={9,10};do{std::array<int,11>p{};for(int i=0;i<11;i++)p[i]=i;for(int i=0;i<5;i++)p[3+i]=in[i];p[9]=out[0];p[10]=out[1];gs.push_back(p);}while(std::next_permutation(out,out+2));}while(std::next_permutation(in,in+5));
 std::vector<T> reps;while(!pool.empty()){T t=dec(*pool.begin());for(auto&p:gs){int inv[11];for(int i=0;i<11;i++)inv[p[i]]=i;T u{};for(int i=0;i<11;i++)u.x[i]=p[t.x[inv[i]]];pool.erase(enc(u));}reps.push_back(t);}return reps;
}

int main(){auto start=std::chrono::steady_clock::now();
 auto A=makeAreps();std::cerr<<"A_orbit_reps="<<A.size()<<"\n";
 std::vector<T>B;B.reserve(4703040);eachQperm([&](const T&t){B.push_back(t);});eachSing(1,[&](const T&t){B.push_back(t);});std::cerr<<"B_candidates="<<B.size()<<"\n";
 // own buckets (s1,s2) -> indices
 std::unordered_map<uint32_t,std::vector<uint32_t>> own;own.reserve(20000);std::vector<uint32_t> BS;BS.reserve(70000);
 for(uint32_t i=0;i<B.size();i++){uint16_t x=img(S,B[i]),z=img(x,B[i]);own[((uint32_t)x<<11)|z].push_back(i);if(x==S)BS.push_back(i);} std::cerr<<"BS="<<BS.size()<<" ownkeys="<<own.size()<<"\n";
 // Cross buckets for B with first image S, indexed by X then z.
 std::map<uint16_t,std::unordered_map<uint16_t,std::vector<uint32_t>>> crossBS;
 std::unordered_set<uint16_t> Xneed;for(auto&a:A){auto x=img(S,a);if(x!=S)Xneed.insert(x);} for(auto X:Xneed){auto &M=crossBS[X];for(auto idx:BS){auto z=img(X,B[idx]);M[z].push_back(idx);}}
 std::cerr<<"Xneed="<<Xneed.size()<<"\n";
 uint64_t tested=0,depth2c=0,surv=0,sc_surv=0,sync_surv=0,bsc=0; T survA{},survB{}; int repno=0;
 for(auto&a:A){repno++; uint16_t x=img(S,a);
   if(x==S){
     // B first image X != S. y=a(X), z=b(X)=b^2(S)
     for(auto &kv:own){uint16_t bx=kv.first>>11, z=kv.first&2047; if(bx==S)continue; uint16_t y=img(bx,a); if(!safe(y)||!safe(z))continue; if(affrank4(bx,y,z)!=2)continue; depth2c+=kv.second.size(); for(auto idx:kv.second){tested++;if(pair_ok(a,B[idx])){surv++; bool sc=strong_conn(a,B[idx]); bool sy=synchronizing(a,B[idx]); if(sc)sc_surv++; if(sy)sync_surv++; if(sc&&sy){bsc++;survA=a;survB=B[idx]; std::cout<<"FOUND_BSC rep="<<repno<<" tested="<<tested<<"\n"; goto done;}}}}
   }else{
     uint16_t y=img(x,a); if(!safe(y))continue;
     // B first image x
     for(auto &kv:own){uint16_t bx=kv.first>>11;if(bx!=x)continue;uint16_t z=kv.first&2047;if(!safe(z)||affrank4(x,y,z)!=2)continue;depth2c+=kv.second.size();for(auto idx:kv.second){tested++;if(pair_ok(a,B[idx])){surv++; bool sc=strong_conn(a,B[idx]); bool sy=synchronizing(a,B[idx]); if(sc)sc_surv++; if(sy)sync_surv++; if(sc&&sy){bsc++;survA=a;survB=B[idx]; std::cout<<"FOUND_BSC rep="<<repno<<" tested="<<tested<<"\n"; goto done;}}}}
     // B first image S, cross image z=b(x)
     auto it=crossBS.find(x);if(it!=crossBS.end())for(auto &zz:it->second){uint16_t z=zz.first;if(!safe(z)||affrank4(x,y,z)!=2)continue;depth2c+=zz.second.size();for(auto idx:zz.second){tested++;if(pair_ok(a,B[idx])){surv++; bool sc=strong_conn(a,B[idx]); bool sy=synchronizing(a,B[idx]); if(sc)sc_surv++; if(sy)sync_surv++; if(sc&&sy){bsc++;survA=a;survB=B[idx]; std::cout<<"FOUND_BSC rep="<<repno<<" tested="<<tested<<"\n"; goto done;}}}}
   }
   if(repno%100==0){auto now=std::chrono::steady_clock::now();double sec=std::chrono::duration<double>(now-start).count();std::cerr<<"rep="<<repno<<" tested="<<tested<<" sec="<<sec<<"\n";}
 }
done:
 std::cout<<"A_orbit_reps="<<A.size()<<"\nB_unary_candidates="<<B.size()<<"\ndepth2_candidates_tested="<<depth2c<<"\npairs_full_checked="<<tested<<"\nlocal_survivors="<<surv<<"\nstrongly_connected_local="<<sc_surv<<"\nsynchronizing_local="<<sync_surv<<"\nbsc_survivors="<<bsc<<"\n";
 if(bsc){std::cout<<"A=";for(int i=0;i<11;i++){if(i)std::cout<<',';std::cout<<(int)survA.x[i];}std::cout<<"\nB=";for(int i=0;i<11;i++){if(i)std::cout<<',';std::cout<<(int)survB.x[i];}std::cout<<"\n";}
 auto end=std::chrono::steady_clock::now();std::cout<<"seconds="<<std::chrono::duration<double>(end-start).count()<<"\n";
}
