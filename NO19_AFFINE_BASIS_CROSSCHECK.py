#!/usr/bin/env python3
"""Independent cross-check of the exact affine-rank routine used by No.19 branch search.

Uses two implementations:
  (1) the integer fraction-free row-basis update mirrored from the C++ certifier;
  (2) an independent Fraction-based Gaussian elimination.
Checks all 120 singleton-protected safe 8-subsets individually, all pairs and triples,
and 10,000 deterministic pseudorandom sequences of lengths 4..20.
"""
from fractions import Fraction
from itertools import combinations
import math, random

S = sum(1 << i for i in range(8))
SAFE = []
for rest in combinations(range(1, 11), 7):
    m = 1
    for i in rest:
        m |= 1 << i
    SAFE.append(m)
assert len(SAFE) == 120

def diff(m):
    return [((m >> j) & 1) - ((S >> j) & 1) for j in range(11)]

def rank_integer(masks):
    rows, piv = [], []
    for m in masks:
        v = diff(m)
        for r, p in zip(rows, piv):
            if v[p] == 0:
                continue
            a, b = r[p], v[p]
            v = [v[j] * a - r[j] * b for j in range(11)]
            g = 0
            for x in v:
                g = math.gcd(g, abs(x))
            if g > 1:
                v = [x // g for x in v]
        p = next((j for j, x in enumerate(v) if x), None)
        if p is None:
            continue
        g = 0
        for x in v:
            g = math.gcd(g, abs(x))
        if g > 1:
            v = [x // g for x in v]
        if v[p] < 0:
            v = [-x for x in v]
        pos = len(piv)
        while pos and piv[pos - 1] > p:
            pos -= 1
        piv.insert(pos, p)
        rows.insert(pos, v)
    return len(rows)

def rank_fraction(masks):
    a = [[Fraction(x) for x in diff(m)] for m in masks]
    if not a:
        return 0
    r = 0
    for c in range(11):
        p = next((i for i in range(r, len(a)) if a[i][c]), None)
        if p is None:
            continue
        a[r], a[p] = a[p], a[r]
        pivot = a[r][c]
        a[r] = [x / pivot for x in a[r]]
        for i in range(len(a)):
            if i != r and a[i][c]:
                f = a[i][c]
                a[i] = [a[i][j] - f * a[r][j] for j in range(11)]
        r += 1
        if r == len(a):
            break
    return r

def check(ms):
    a, b = rank_integer(ms), rank_fraction(ms)
    if a != b:
        raise AssertionError((a, b, ms))

for m in SAFE:
    check([m])
for x, y in combinations(SAFE, 2):
    check([x, y])
for x, y, z in combinations(SAFE, 3):
    check([x, y, z])

rng = random.Random(190923)
for _ in range(10_000):
    k = rng.randint(4, 20)
    check(rng.sample(SAFE, k))

print('safe_masks=120')
print('all_pairs=7140')
print('all_triples=280840')
print('random_sequences=10000')
print('PASS_NO19_AFFINE_BASIS_CROSSCHECK')
