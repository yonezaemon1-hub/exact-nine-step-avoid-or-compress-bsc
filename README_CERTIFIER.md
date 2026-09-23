# No.19 complete exact-ten branch certifier

Prepared: 2026-09-23

Target slice: 11-state binary strongly connected synchronizing automata with
`S={0,...,7}`, protected state `q=0`, and `(d,k)=(3,7)` in the singleton-protected slice.

## Analytic input

Any hypothetical exact-ten candidate is assumed to have already passed the analytic reduction in the manuscript:

- maximal affine growth through depth 9;
- a unique global one-step merge pair `D`;
- every singular letter has rank 10 with kernel doubleton `D`;
- `q notin D`;
- one of six normal-form branches: O-PP, O-PQ, I-PP, I-PQ, II-DD, II-DQ.

The finite programs here exhaust exactly those normal forms. They do not search outside the proved reduction.

## What every case program does

1. Exhaustively enumerates all admissible unary depth-nine-safe letters of the required predecessor/rank type.
2. Quotients the first letter by the full relabeling group preserving the canonical normal form (`S`, `q`, `D`, and `p` when present).
3. Uses the forced depth-1/depth-2 affine conditions only as exact necessary prefilters.
4. For every retained ordered pair, performs exact subset-graph exploration through depth 9 and rejects if:
   - any reachable image avoids `q`, or
   - any reachable image has size < 8, or
   - cumulative affine rank at depth `i` differs from `i`.
5. Every local survivor is tested for strong connectivity and synchronizability by the complete unordered-pair criterion.

PP branches are covered by naming a singular letter first (permitted by swapping the two letter names). PQ branches are split into two disjoint cases: P-singular/Q-any and P-permutation/Q-singular.

## Frozen results

| Subcase | first-letter orbits | second letters | fully checked pairs | local survivors | BSC survivors |
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

For every subcase with nonzero local survivors, the classified rerun found both
`strongly_connected_local=0` and `synchronizing_local=0`.

## Independent anchors

- `NO19_EXACT9_WITNESS_CHECK.py`: exact subset-BFS distance 9, strong connectivity, synchronization.
- `NO19_DIRECT_WORD_AUDIT.py`: all 2,046 nonempty binary words through length 10.
- `NO19_UNARY_SAFE_ENUMERATOR.cpp`: independent mixed-branch unary counts.
- `NO19_AFFINE_BASIS_CROSSCHECK.py`: independent Fraction-based cross-check of the integer affine-rank routine on all 7,140 pairs, all 280,840 triples, and 10,000 deterministic random sequences of safe 8-subsets.
- `NO19_II_DD_ORBIT_AUDIT_INDEPENDENT.cpp`: independent generator-graph audit of the II mixed relabeling quotient; reproduces 822 first-letter orbits from 449,280 unary-safe D-singular maps.
- `NO19_LOCAL_SURVIVOR_CROSSBRANCH_SPOTCHECK.py`: exports one local survivor from each nonzero-survivor subcase and checks non-strong-connectivity by transitive closure and non-synchronization by full-subset BFS.

## Claim boundary

The result is a hybrid analytic-finite theorem. The code proves emptiness only after the symbolic branch reduction. The earlier unrecovered neighborhood / redirect / 20M exploratory searches are not used.

## Reviewer-driven independent count audit (2026-09-23)

A second unary-count implementation was added after external review. It does not reuse the
bitmask / `next_permutation` implementation in `NO19_UNARY_SAFE_ENUMERATOR.cpp`:

- `NO19_UNARY_COUNTS_INDEPENDENT_V2.cpp` derives the P-permutation count analytically from
  the protected cycle and uses recursive bijection assignment on kernel blocks for singular maps;
- unary safety is checked with explicit eight-state image lists rather than subset bitmasks.

Frozen output:

- P-permutation = 907,200;
- P-singular = 426,720;
- Q-permutation = 3,628,800;
- Q-singular = 1,074,240;
- D-singular = 449,280;
- `PASS_INDEPENDENT_UNARY_COUNTS`.

The P-permutation identity is now also proved in the manuscript:
`sum_{ell=2}^8 (6!/(8-ell)!)(11-ell)! = 907200 = 10!/4`.

The revised manuscript also makes `D not subset S` explicit as a corollary and removes the
unused rank-eight-word section, because that constraint is not used by the final certifier.

## Third singular-count brute-force audit (review R3)

A third implementation was added after a further independent review. It is deliberately simpler
than both prior unary enumerators:

- `NO19_SINGULAR_COUNTS_THIRD_BRUTE.cpp` fixes the relevant preimage block of protected state 0;
- for each singular type and each omitted nonzero target, it lexicographically enumerates all `9!`
  bijections from the remaining quotient blocks to the remaining nonzero targets;
- thus it checks exactly `10 * 9! = 3,628,800` rank-ten maps for each of P-singular, Q-singular,
  and D-singular;
- unary nine-safety is tested by a direct image-list simulation, with no recursive block assignment.

Frozen output:

- P-singular = 426,720;
- Q-singular = 1,074,240;
- D-singular = 449,280;
- `PASS_THIRD_BRUTE_SINGULAR_COUNTS`.

Review R3 also corrects the attribution sentence in the Structural Reduction theorem proof:
item (i) is Lemma 1; items (ii)-(iii) are Theorem 1; item (iv) is Theorem 2; and branch items
(v)-(vi) come from Theorem 3, whose proof uses Corollary 1.


## Independent II mixed orbit audit (review R4)

For II-DD / II-DQ the canonical labels are `q=0`, `S={0,...,7}`, and `D={1,8}`.
The full normal-form relabeling group fixes `0,1,8` pointwise and is
`Sym({2,...,7}) x Sym({9,10})`, of order `6!*2!=1440`. A generating set is
`(2 3),(3 4),(4 5),(5 6),(6 7),(9 10)`.

`NO19_II_DD_ORBIT_AUDIT_INDEPENDENT.cpp` does not erase candidates using all 1,440
group elements as the branch certifier does. Instead it enumerates all 449,280 unary-safe
D-singular maps, builds a hash index, and unions maps only along the six generator-conjugacy
edges. It checks 2,695,680 generator edges and obtains exactly 822 connected components.
The orbit-size histogram sums to 449,280 and the run ends with
`PASS_INDEPENDENT_II_DD_ORBIT_COUNT`.

## Cross-branch local-survivor spot audit (review R4)

One concrete local survivor was exported from each nonzero-survivor subcase:
II-DQ, I-PP, I-PQ(P-sing/Q-any), and I-PQ(P-perm/Q-sing).
`NO19_LOCAL_SURVIVOR_CROSSBRANCH_SPOTCHECK.py` checks these four maps independently:

- strong connectivity is tested by transitive closure on the 11-state directed graph;
- synchronizability is tested by BFS from the full 11-state subset over the complete subset automaton, not by the branch certifier's unordered-pair criterion.

All four samples fail both properties. The run ends with
`PASS_CROSSBRANCH_LOCAL_SURVIVOR_SPOTCHECK`.

## Review R5: all first-letter quotient audits

`NO19_ALL_ORBIT_QUOTIENT_AUDIT_INDEPENDENT.cpp` independently reconstructs all five distinct first-letter quotient universes using only generator conjugacy edges and Union-Find, rather than enumerating every group element as the branch certifier does.

The relabeling groups are:

- II: `Sym({2,3,4,5,6,7}) x Sym({9,10})`, generated by `(2 3),(3 4),(4 5),(5 6),(6 7),(9 10)`.
- I: `Sym({3,4,5,6,7}) x Sym({9,10})`, generated by `(3 4),(4 5),(5 6),(6 7),(9 10)`.
- O: `Sym({2,3,4,5,6,7}) x Sym({8,9})`, generated by `(2 3),(3 4),(4 5),(5 6),(6 7),(8 9)`.

The independent output reproduces every first-letter orbit count used in the eight branch subcases: 822, 3030, 5520, 569, and 1186. It reports no missing generator image and every component-size sum returns the full underlying unary-safe map count.
