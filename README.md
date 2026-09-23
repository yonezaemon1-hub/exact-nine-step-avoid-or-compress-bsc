# An Exact Nine-Step Avoid-or-Compress Bound in Binary Strongly Connected Synchronizing Automata

Ryutaro Yonezu — Independent Researcher

This repository contains the paper source and complete finite certifier for the singleton-protected binary strongly connected synchronizing avoid-or-compress slice at `(d,k)=(3,7)`, where `n=11`, `|S|=8`, and `A={q}`.

## Main result

The exact value is

```
T_BSC(3,7) = 9.
```

An explicit 11-state binary strongly connected synchronizing automaton has exact avoid-or-compress distance 9. A deterministic exhaustive certifier excludes every hypothetical exact-10 candidate after an analytic reduction to six normal-form branches.

Across eight computational subcases, the certifier fully checks **432,128,504** symmetry-reduced, depth-two-compatible ordered pairs. It finds **1,872** local survivors and **0** binary strongly connected synchronizing survivors.

## Explicit exact-nine witness

With `S={0,...,7}` and protected state `q=0`:

```
a = [4,7,3,8,0,6,5,1,9,2,10]
b = [1,6,7,3,4,0,5,8,7,10,9]
```

One shortest successful word is

```
aabaaabab
```

The direct audit checks all 2,046 nonempty binary words through length 10 and finds no success through length 8.

## Exhaustive branch totals

| Subcase | First-letter orbits | Second letters | Fully checked pairs | Local survivors | BSC survivors |
|---|---:|---:|---:|---:|---:|
| II-DD | 822 | 449,280 | 9,615,680 | 0 | 0 |
| II-DQ | 822 | 4,703,040 | 44,971,752 | 88 | 0 |
| I-PP | 3,030 | 1,333,920 | 91,957,456 | 677 | 0 |
| I-PQ: P-sing/Q-any | 3,030 | 4,703,040 | 136,030,656 | 677 | 0 |
| I-PQ: P-perm/Q-sing | 5,520 | 1,074,240 | 70,824,720 | 430 | 0 |
| O-PP | 569 | 1,281,600 | 23,496,800 | 0 | 0 |
| O-PQ: P-sing/Q-any | 569 | 4,616,640 | 34,903,200 | 0 | 0 |
| O-PQ: P-perm/Q-sing | 1,186 | 987,840 | 20,328,240 | 0 | 0 |
| **Total** |  |  | **432,128,504** | **1,872** | **0** |

## Independent audit layers

The repository includes independent checks for:

- exact subset-BFS distance 9, strong connectivity, and synchronization of the witness;
- direct enumeration of all 2,046 nonempty binary words through length 10;
- affine-rank cross-check against exact rational Gaussian elimination;
- the five unary normal-form counts by multiple independent implementations;
- a third brute-force check of all `10*9! = 3,628,800` rank-ten maps for each singular type;
- independent generator-graph / Union-Find reconstruction of every first-letter symmetry quotient;
- independent spot checks of local survivors using transitive closure and full-subset BFS rather than the branch certifier's tests.

## Windows full rerun

On 23 September 2026 the complete R5 package was rerun on a Windows 10 machine with Strawberry `g++`.

The fresh run reported:

```
FINAL_PASS=true
TOTAL_FULL_PAIRS=432128504
TOTAL_LOCAL_SURVIVORS=1872
TOTAL_BSC_SURVIVORS=0
ELAPSED_MINUTES=3.31
```

The locally generated evidence archive was:

```
NO19_R5_FULL_WINDOWS_EVIDENCE_20260923_165536.zip
SHA-256 3999BA6A868B698C6A2B55E9B203F9E3805E8EAD371CC50F09AC00DC19B71937
```

## Proof/computation boundary

This is a hybrid analytic-finite theorem. The computation is used only after the manuscript proves the exact-ten structural reduction to the six normal-form branches. Earlier exploratory neighborhood / redirect / 20M searches are not part of the claim.

## Status

Review R5, 23 September 2026. Public-release preparation is in progress. No archival DOI has yet been assigned.
