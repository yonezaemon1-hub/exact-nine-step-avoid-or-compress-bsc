#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#if defined(_MSC_VER)
#include <intrin.h>
#endif

using Transition = std::array<int, 11>;
static constexpr std::uint16_t INITIAL_S = (1u << 8) - 1u; // {0,...,7}

static inline std::uint16_t image_mask(std::uint16_t mask, const Transition& t) {
    std::uint16_t out = 0;
    while (mask) {
#if defined(_MSC_VER)
        unsigned long idx;
        _BitScanForward(&idx, static_cast<unsigned long>(mask));
        unsigned i = static_cast<unsigned>(idx);
#else
        unsigned i = static_cast<unsigned>(__builtin_ctz(static_cast<unsigned>(mask)));
#endif
        mask &= static_cast<std::uint16_t>(mask - 1u);
        out |= static_cast<std::uint16_t>(1u << t[i]);
    }
    return out;
}

static inline int popcount16(std::uint16_t x) {
#if defined(_MSC_VER)
    int n = 0;
    while (x) { x &= static_cast<std::uint16_t>(x - 1u); ++n; }
    return n;
#else
    return __builtin_popcount(static_cast<unsigned>(x));
#endif
}

static inline bool unary_9_safe(const Transition& t) {
    std::uint16_t mask = INITIAL_S;
    for (int j = 1; j <= 9; ++j) {
        mask = image_mask(mask, t);
        if (popcount16(mask) != 8 || (mask & 1u) == 0) return false;
    }
    return true;
}

static std::uint64_t count_p_permutations() {
    Transition t{};
    t[2] = 0; // P: c^{-1}(0)={2}
    const std::array<int, 10> positions = {0,1,3,4,5,6,7,8,9,10};
    std::array<int, 10> values = {1,2,3,4,5,6,7,8,9,10};
    std::uint64_t count = 0;
    do {
        for (int i = 0; i < 10; ++i) t[positions[i]] = values[i];
        if (unary_9_safe(t)) ++count;
    } while (std::next_permutation(values.begin(), values.end()));
    return count;
}

// Kernel blocks for singular letters are D={1,8} plus nine singletons.
// type: 0=P (preimage of 0 is {2}), 1=Q ({0}), 2=D ({1,8}).
static std::uint64_t count_singular(int type) {
    const std::array<std::array<int,2>,10> block = {{{1,8},{0,-1},{2,-1},{3,-1},{4,-1},
                                                     {5,-1},{6,-1},{7,-1},{9,-1},{10,-1}}};
    const std::array<int,10> block_size = {2,1,1,1,1,1,1,1,1,1};
    const int special = (type == 2 ? 0 : (type == 1 ? 1 : 2));
    std::uint64_t count = 0;

    // Target 0 must be present, so the omitted image target is one of 1,...,10.
    for (int omitted = 1; omitted <= 10; ++omitted) {
        std::array<int,9> values{};
        int z = 0;
        for (int v = 1; v <= 10; ++v) if (v != omitted) values[z++] = v;
        do {
            Transition t{};
            for (int k = 0; k < block_size[special]; ++k) t[block[special][k]] = 0;
            int q = 0;
            for (int b = 0; b < 10; ++b) {
                if (b == special) continue;
                const int v = values[q++];
                for (int k = 0; k < block_size[b]; ++k) t[block[b][k]] = v;
            }
            if (unary_9_safe(t)) ++count;
        } while (std::next_permutation(values.begin(), values.end()));
    }
    return count;
}

int main() {
    const std::uint64_t Pperm = count_p_permutations();
    const std::uint64_t Psing = count_singular(0);
    const std::uint64_t Qperm = 3628800ULL; // 10!, all fix 0 and are automatically unary-safe.
    const std::uint64_t Qsing = count_singular(1);
    const std::uint64_t Dsing = count_singular(2);

    std::cout << "P-permutation=" << Pperm << "\n";
    std::cout << "P-singular=" << Psing << "\n";
    std::cout << "Q-permutation=" << Qperm << "\n";
    std::cout << "Q-singular=" << Qsing << "\n";
    std::cout << "D-singular=" << Dsing << "\n";

    const std::uint64_t I_PP = (Pperm + Psing) * (Pperm + Psing) - Pperm * Pperm;
    const std::uint64_t I_PQ = (Pperm + Psing) * (Qperm + Qsing) - Pperm * Qperm;
    const std::uint64_t II_DD = Dsing * Dsing;
    const std::uint64_t II_DQ = Dsing * (Qperm + Qsing);
    const std::uint64_t total = I_PP + I_PQ + II_DD + II_DQ;

    std::cout << "I-PP=" << I_PP << "\n";
    std::cout << "I-PQ=" << I_PQ << "\n";
    std::cout << "II-DD=" << II_DD << "\n";
    std::cout << "II-DQ=" << II_DQ << "\n";
    std::cout << "family-total=" << total << "\n";

    const bool pass =
        Pperm == 907200ULL &&
        Psing == 426720ULL &&
        Qperm == 3628800ULL &&
        Qsing == 1074240ULL &&
        Dsing == 449280ULL &&
        total == 6252596812800ULL;

    if (!pass) {
        std::cerr << "FAIL: one or more counts disagree with the manuscript.\n";
        return 1;
    }
    std::cout << "PASS_NO19_UNARY_ENUMERATION\n";
    return 0;
}
