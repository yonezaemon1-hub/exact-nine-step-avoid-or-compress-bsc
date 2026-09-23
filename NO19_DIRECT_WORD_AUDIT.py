#!/usr/bin/env python3
"""Independent direct word audit for the explicit No.19 exact-nine witness.

Enumerates every nonempty binary word through length 10 (2,046 words),
without subset-BFS deduplication, and verifies that no word of length <=8
succeeds and that the first successful length is exactly 9.
"""
from itertools import product

A = [4,7,3,8,0,6,5,1,9,2,10]
B = [1,6,7,3,4,0,5,8,7,10,9]
S0 = frozenset(range(8))
PROTECTED = 0


def step(X, t):
    return frozenset(t[x] for x in X)


def apply(word):
    X = S0
    for c in word:
        X = step(X, A if c == 'a' else B)
    return X


def successful(X):
    return PROTECTED not in X or len(X) < len(S0)


def main():
    first_len = None
    first_words = []
    counts = {}
    checked = 0
    for L in range(1, 11):
        success_here = []
        for bits in product('ab', repeat=L):
            w = ''.join(bits)
            checked += 1
            X = apply(w)
            if successful(X):
                success_here.append((w, sorted(X)))
        counts[L] = len(success_here)
        if success_here and first_len is None:
            first_len = L
            first_words = success_here

    print(f"words_checked={checked}")
    print(f"first_success_length={first_len}")
    print(f"successful_words_at_first_length={len(first_words)}")
    print(f"lexicographically_first_success={first_words[0][0] if first_words else None}")
    print(f"first_terminal={first_words[0][1] if first_words else None}")
    print("success_counts_by_length=" + repr(counts))

    assert checked == 2046
    assert first_len == 9
    assert any(w == 'aabaaabab' for w, _ in first_words)
    print("PASS_NO19_DIRECT_WORD_AUDIT")


if __name__ == '__main__':
    main()
