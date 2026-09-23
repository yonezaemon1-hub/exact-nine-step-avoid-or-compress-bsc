#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>
#include <map>

using Map = std::array<uint8_t,11>;
static constexpr std::array<int,8> S = {0,1,2,3,4,5,6,7};
static constexpr int blocks[10][2] = {{1,8},{0,-1},{2,-1},{3,-1},{4,-1},{5,-1},{6,-1},{7,-1},{9,-1},{10,-1}};
static constexpr int bsz[10] = {2,1,1,1,1,1,1,1,1,1};

static bool unary_safe_9(const Map& f){
    std::array<int,8> cur=S, nxt{};
    for(int step=0;step<9;++step){
        bool seen[11]{}; int n=0;
        for(int x:cur){ int y=f[x]; if(!seen[y]){seen[y]=true; nxt[n++]=y;} }
        if(n!=8 || !seen[0]) return false;
        for(int i=0;i<8;++i) cur[i]=nxt[i];
    }
    return true;
}
static uint64_t enc(const Map& f){ uint64_t z=0; for(int i=0;i<11;++i) z|=(uint64_t)f[i]<<(4*i); return z; }

static std::vector<Map> enumerate_D_singular(){
    std::vector<Map> out; out.reserve(449280);
    // D={1,8} is the preimage block of q=0. The remaining 9 quotient blocks map
    // bijectively to 9 of the 10 nonzero targets.
    for(int omitted=1; omitted<=10; ++omitted){
        std::array<int,9> vals{}; int k=0;
        for(int y=1;y<=10;++y) if(y!=omitted) vals[k++]=y;
        do{
            Map f{}; f.fill(255); f[1]=0; f[8]=0;
            int j=0;
            for(int b=1;b<10;++b){ int y=vals[j++]; for(int t=0;t<bsz[b];++t) f[blocks[b][t]]=y; }
            if(unary_safe_9(f)) out.push_back(f);
        } while(std::next_permutation(vals.begin(), vals.end()));
    }
    return out;
}

struct DSU{
    std::vector<int> p,sz;
    explicit DSU(int n):p(n),sz(n,1){std::iota(p.begin(),p.end(),0);} 
    int find(int x){while(p[x]!=x){p[x]=p[p[x]];x=p[x];}return x;}
    void unite(int a,int b){a=find(a);b=find(b);if(a==b)return;if(sz[a]<sz[b])std::swap(a,b);p[b]=a;sz[a]+=sz[b];}
};

static Map conjugate_transposition(const Map& f,int a,int b){
    // g = p o f o p^{-1}; p is the transposition (a b), hence p^{-1}=p.
    auto P=[&](int x){return x==a?b:(x==b?a:x);};
    Map g{};
    for(int x=0;x<11;++x) g[x]=(uint8_t)P(f[P(x)]);
    return g;
}

int main(){
    auto maps=enumerate_D_singular();
    std::cout << "D_singular_unary9=" << maps.size() << "\n";
    if(maps.size()!=449280){ std::cerr<<"unexpected unary count\n"; return 2; }

    std::unordered_map<uint64_t,int> idx; idx.reserve(maps.size()*2);
    for(int i=0;i<(int)maps.size();++i) idx.emplace(enc(maps[i]),i);

    // Full relabeling group preserving the II mixed normal form:
    // q=0 fixed, D={1,8} fixed pointwise (one endpoint is in S, one outside S),
    // S={0,...,7} fixed setwise. Therefore G = Sym({2,...,7}) x Sym({9,10}).
    // Adjacent transpositions below generate G; |G|=6!*2!=1440.
    const std::array<std::pair<int,int>,6> gens={{{2,3},{3,4},{4,5},{5,6},{6,7},{9,10}}};
    DSU dsu((int)maps.size());
    uint64_t edges=0;
    for(int i=0;i<(int)maps.size();++i){
        for(auto [a,b]:gens){
            Map g=conjugate_transposition(maps[i],a,b);
            auto it=idx.find(enc(g));
            if(it==idx.end()){
                std::cerr << "generator left the candidate set: i="<<i<<" swap="<<a<<","<<b<<"\n";
                return 3;
            }
            dsu.unite(i,it->second); ++edges;
        }
    }
    std::map<int,int> hist;
    int comps=0;
    for(int i=0;i<(int)maps.size();++i) if(dsu.find(i)==i){++comps; hist[dsu.sz[i]]++;}
    std::cout << "group_order=1440\n";
    std::cout << "generators=6\n";
    std::cout << "generator_edges_checked=" << edges << "\n";
    std::cout << "orbit_count=" << comps << "\n";
    std::cout << "orbit_size_histogram=";
    bool first=true; for(auto [sz,n]:hist){if(!first)std::cout<<",";first=false;std::cout<<sz<<":"<<n;} std::cout<<"\n";
    long long sum=0; for(auto [sz,n]:hist) sum += 1LL*sz*n;
    std::cout << "orbit_size_sum=" << sum << "\n";
    const bool ok = (comps==822 && sum==(long long)maps.size());
    std::cout << (ok?"PASS_INDEPENDENT_II_DD_ORBIT_COUNT\n":"FAIL_INDEPENDENT_II_DD_ORBIT_COUNT\n");
    return ok?0:1;
}
