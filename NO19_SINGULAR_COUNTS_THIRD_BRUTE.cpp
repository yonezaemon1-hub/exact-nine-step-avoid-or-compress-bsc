#include <array>
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using Map = std::array<int,11>;
static constexpr std::array<int,8> S = {0,1,2,3,4,5,6,7};
// Quotient blocks for the fixed mixed kernel D={1,8}.
static const std::array<std::vector<int>,10> B = {
    std::vector<int>{1,8}, std::vector<int>{0}, std::vector<int>{2},
    std::vector<int>{3}, std::vector<int>{4}, std::vector<int>{5},
    std::vector<int>{6}, std::vector<int>{7}, std::vector<int>{9},
    std::vector<int>{10}
};

static bool unary_safe_9(const Map& f){
    std::array<int,8> cur=S, nxt{};
    int ncur=8;
    for(int step=0; step<9; ++step){
        bool seen[11]{};
        int nnext=0;
        for(int i=0;i<ncur;++i){
            const int y=f[cur[i]];
            if(!seen[y]){ seen[y]=true; nxt[nnext++]=y; }
        }
        if(nnext!=8 || !seen[0]) return false;
        for(int i=0;i<8;++i) cur[i]=nxt[i];
        ncur=8;
    }
    return true;
}

// special_block is the quotient block whose whole preimage is mapped to q=0.
// 0 => D-singular, 1 => Q-singular, 2 => P-singular.
static std::uint64_t brute_count(int special_block){
    std::uint64_t safe=0, raw=0;
    Map f{};
    for(int omitted=1; omitted<=10; ++omitted){
        std::array<int,9> vals{};
        int k=0;
        for(int y=1;y<=10;++y) if(y!=omitted) vals[k++]=y;
        std::sort(vals.begin(), vals.end());
        do {
            ++raw;
            int j=0;
            for(int b=0;b<10;++b){
                const int target = (b==special_block) ? 0 : vals[j++];
                for(int x: B[b]) f[x]=target;
            }
            if(unary_safe_9(f)) ++safe;
        } while(std::next_permutation(vals.begin(), vals.end()));
    }
    std::cerr << "raw=" << raw << "\n";
    return safe;
}

int main(){
    const auto ps=brute_count(2);
    const auto qs=brute_count(1);
    const auto ds=brute_count(0);
    std::cout << "P-singular(third-brute)=" << ps << "\n";
    std::cout << "Q-singular(third-brute)=" << qs << "\n";
    std::cout << "D-singular(third-brute)=" << ds << "\n";
    const bool ok=(ps==426720ULL && qs==1074240ULL && ds==449280ULL);
    std::cout << (ok?"PASS_THIRD_BRUTE_SINGULAR_COUNTS\n":"FAIL_THIRD_BRUTE_SINGULAR_COUNTS\n");
    return ok?0:1;
}
