#include <array>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <numeric>

using Map = std::array<unsigned char,11>;
static constexpr std::array<int,8> S0 = {0,1,2,3,4,5,6,7};

// Deliberately different from the release enumerator: explicit 8-state image lists,
// recursive bijection assignment, no subset bitmasks and no next_permutation loop.
static bool safe_nine(const Map& f){
    std::array<unsigned char,8> cur{};
    for(int i=0;i<8;++i) cur[i]=static_cast<unsigned char>(S0[i]);
    for(int step=1; step<=9; ++step){
        bool seen[11] = {};
        std::array<unsigned char,8> nxt{};
        int k=0;
        for(int i=0;i<8;++i){
            unsigned char y=f[cur[i]];
            if(!seen[y]){ seen[y]=true; nxt[k++]=y; }
        }
        if(k!=8 || !seen[0]) return false;
        cur=nxt;
    }
    return true;
}

static std::uint64_t fact(int n){
    std::uint64_t x=1; for(int i=2;i<=n;++i) x*=i; return x;
}

// Analytic P-permutation count.  f^{-1}(0)={2}. Unary safety through 9
// forces the entire permutation cycle containing 0 to lie in S={0,...,7}.
// For cycle length ell, choose/order ell-2 states from S\{0,2}; permute rest freely.
static std::uint64_t pperm_formula(){
    std::uint64_t ans=0;
    for(int ell=2; ell<=8; ++ell){
        std::uint64_t falling=1;
        for(int j=0;j<ell-2;++j) falling*=static_cast<std::uint64_t>(6-j);
        ans += falling * fact(11-ell);
    }
    return ans;
}

static const std::array<std::vector<int>,10> blocks = {
    std::vector<int>{1,8}, std::vector<int>{0}, std::vector<int>{2},
    std::vector<int>{3}, std::vector<int>{4}, std::vector<int>{5},
    std::vector<int>{6}, std::vector<int>{7}, std::vector<int>{9},
    std::vector<int>{10}
};

static void assign_rec(int idx,
                       const std::array<int,9>& active_blocks,
                       const std::array<int,9>& targets,
                       std::array<bool,9>& used,
                       Map& f,
                       std::uint64_t& safe_count){
    if(idx==9){
        if(safe_nine(f)) ++safe_count;
        return;
    }
    int b=active_blocks[idx];
    for(int j=0;j<9;++j){
        if(used[j]) continue;
        used[j]=true;
        for(int x: blocks[b]) f[x]=static_cast<unsigned char>(targets[j]);
        assign_rec(idx+1, active_blocks, targets, used, f, safe_count);
        used[j]=false;
    }
}

// special block is exactly the kernel/preimage block sent to protected state 0.
// 0=D block {1,8}; 1=Q block {0}; 2=P block {2}.
static std::uint64_t singular_count(int special){
    std::uint64_t total=0;
    for(int omitted=1; omitted<=10; ++omitted){
        std::array<int,9> targets{};
        int t=0;
        for(int y=1;y<=10;++y) if(y!=omitted) targets[t++]=y;
        std::array<int,9> active{};
        int a=0;
        for(int b=0;b<10;++b) if(b!=special) active[a++]=b;
        Map f{};
        for(int x: blocks[special]) f[x]=0;
        std::array<bool,9> used{};
        assign_rec(0,active,targets,used,f,total);
    }
    return total;
}

int main(){
    auto pp = pperm_formula();
    auto ps = singular_count(2);
    auto qp = fact(10);
    auto qs = singular_count(1);
    auto ds = singular_count(0);
    std::cout << "P-permutation(formula)=" << pp << "\n";
    std::cout << "P-singular(recursive-independent)=" << ps << "\n";
    std::cout << "Q-permutation(formula)=" << qp << "\n";
    std::cout << "Q-singular(recursive-independent)=" << qs << "\n";
    std::cout << "D-singular(recursive-independent)=" << ds << "\n";
    bool ok = pp==907200ULL && ps==426720ULL && qp==3628800ULL && qs==1074240ULL && ds==449280ULL;
    std::cout << (ok ? "PASS_INDEPENDENT_UNARY_COUNTS\n" : "FAIL_INDEPENDENT_UNARY_COUNTS\n");
    return ok ? 0 : 1;
}
