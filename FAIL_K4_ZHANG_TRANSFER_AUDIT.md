# FAIL — A Computational Audit of Direct Zhang-Type Transfer to Four-Set Synchronization

**Ryutaro Yonezu — Independent Researcher**  
**Date:** 26 September 2026  
**Status:** Negative research note / failed candidate. **Not counted as a successful paper.**

## Abstract

This note records a failed attempt to transfer the 2026 triple-rendezvous construction of Enkai Zhang to the four-set synchronization problem.

The motivating gap is real: for synchronizing automata the known four-set rendezvous lower bound is linear, with the classical Černý family giving

[
\operatorname{rdv}(4,n) \ge 2n+1,
]

while Behague and Johnson proved a quadratic general upper bound

[
\operatorname{rdv}(4,n)
\le
\frac{11-\sqrt5-4\sqrt3}{4}n^2+\frac72n-1
\approx 0.4589n^2+O(n).
]

Szykuła's 2026 open-problems survey still treats fixed-(k) synchronization time as open and records the conjectural linear regime (O(kn)).

Zhang's 2026 construction raises the triple rendezvous lower bound to

[
T_3=\left\lfloor\frac{4n}{3}\right\rfloor
]

for an explicit binary strongly connected synchronizing family. The present audit asked whether the same construction principle could be lifted directly to (k=4).

It could not. Exact finite checks on the Zhang family produced the persistent pattern

[
T_4=T_3+1
=
\left\lfloor\frac{4n}{3}\right\rfloor+1
]

through the tested range (9\le n\le60). This is asymptotically and numerically below the already-known Černý lower bound (2n+1). The direct-transfer candidate is therefore rejected.

The failure mechanism is structural: Zhang's (k=3) argument delays synchronization by making the post-collision **pair** hard. For (k=4), the first rank drop leaves a **triple**, so a one-level pair potential is insufficient. The direct construction does not keep the relevant triple frontier far from synchronization.

This note intentionally does **not** promote later restricted observations or proof-route experiments into a replacement success claim. The original candidate was "direct Zhang-type transfer improves the (k=4) lower bound"; that candidate failed.

---

## 1. Prior-art map checked before the experiment

The following points were checked against the cited sources before the transfer experiment.

### Behague–Johnson

For synchronizing (n)-state automata, Behague and Johnson proved

[
\operatorname{rdv}(4,n)
\le
\frac{11-\sqrt5-4\sqrt3}{4}n^2+\frac72n-1,
]

and

[
\operatorname{rdv}(5,n)
\le
\frac{15-\sqrt5-4\sqrt3-\sqrt7}{4}n^2+5n-2.
]

Their table records the classical Černý-family lower bounds

[
\operatorname{rdv}(4,n)\ge2n+1,
\qquad
\operatorname{rdv}(5,n)\ge3n+1.
]

They also show that for the non-synchronizing analogue the tight fixed-(k) order is (\Theta(n^{k-1})).

Reference:

- L. Behague and P. Johnson, *Synchronizing Times for k-sets in Automata*, Electronic Journal of Combinatorics 29(3), P3.41, 2022.
- arXiv:2008.12166
- https://arxiv.org/abs/2008.12166

### Szykuła 2026 survey

The 2026 survey records the fixed-(k) synchronization-time problem as open, states the (O(kn)) conjectural regime for synchronizing automata, and explicitly discusses the relation among (k)-subset synchronization, compress-with-another, and avoiding-word problems.

Reference:

- M. Szykuła, *Synchronizing Automata: Open Problems*, 2026.
- arXiv:2608.24245
- https://arxiv.org/abs/2608.24245

### Zhang 2026

Zhang gives an explicit binary strongly connected synchronizing family with triple rendezvous time

[
T_3=
\left\lfloor\frac{4n}{3}\right\rfloor.
]

Reference:

- E. Zhang, *The triple rendezvous time of a synchronizing automaton can be floor(4n/3)*, 2026.
- arXiv:2609.19173
- https://arxiv.org/abs/2609.19173

---

## 2. Candidate tested

The preregistered-style candidate was:

> Reuse the structural idea behind Zhang's hard triple construction to obtain a four-set family whose rendezvous time improves the known lower bound (2n+1).

The test was deliberately concrete. Before inventing a new family, first evaluate the **same Zhang automata** at (k=4).

A direct success would have required at minimum

[
T_4>2n+1
]

for some (n), followed by a general construction/proof.

Anything below the existing Černý benchmark does not count as a lower-bound improvement.

---

## 3. Exact finite observation

Exact subset reverse-BFS on the Zhang family was used to compute the shortest synchronization time of the best four-set.

For every tested size

[
9\le n\le60,
]

the result was

[
T_4=T_3+1.
]

Equivalently,

[
T_4=
\left\lfloor\frac{4n}{3}\right\rfloor+1.
]

Examples:

| (n) | (T_3) | (T_4) | Černý benchmark (2n+1) |
|---:|---:|---:|---:|
| 9 | 12 | 13 | 19 |
| 12 | 16 | 17 | 25 |
| 24 | 32 | 33 | 49 |
| 60 | 80 | 81 | 121 |

Thus the direct Zhang family is not merely short of a new record; asymptotically it has slope (4/3), compared with the already-known slope (2) from Černý.

### Audit limitation

This repository note records the exact finite result obtained during the exploratory audit. It is **not** presented here as an independently packaged release certificate, and no DOI or archival claim is attached to it. A future reuse of the numerical statement should first rerun the checker from a frozen implementation and store the logs/hashes.

---

## 4. Why the transfer fails

Let (A) be a binary automaton with

- one permutation letter (b), and
- one rank-((n-1)) letter (a).

Let ({c,c'}) be the unique kernel pair of (a), with

[
ca=c'a=z.
]

For a 3-set, the first effective collision leaves a pair. Zhang's construction can therefore make the remaining problem hard by controlling distances of pairs.

For a 4-set, the first effective collision leaves a triple. The analogous object is not a pair potential (\lambda(P)), but a triple-level quantity.

A useful diagnostic identity in this restricted architecture is the following.

Define the collision frontier

[
F_a=
\bigl\{
\{z,y_1,y_2\}:
y_1\ne y_2, 
y_1,y_2\in\operatorname{Im}(a)\setminus\{z\}
\bigr\},
]

and

[
\Phi_4(A)
=
\min_{T\in F_a} d_3(T),
]

where (d_3(T)) is the shortest length of a word synchronizing the triple (T).

Then

[
T_4(A)=1+\Phi_4(A).
]

### Proof sketch

For the upper bound, choose a frontier triple (T=\{z,y_1,y_2\}) attaining (\Phi_4). Since (z) has two preimages under (a), while (y_1,y_2) each have one, (a^{-1}(T)) has four states. Starting from that four-set, one application of (a) produces (T), followed by an optimal synchronizing word for (T).

For the lower bound, the permutation letter cannot change set cardinality, and the rank-((n-1)) letter can reduce cardinality only when the current set contains both members of its unique kernel pair. Therefore the first (4\to3) drop must occur under (a), and its image is necessarily an element of (F_a). From that point at least (\Phi_4) further steps are required.

This identity explains the failed lift. A pair-level obstruction is one layer too low: a hard four-set construction must control the **post-collision triple frontier**.

The identity is used here only as a diagnostic lemma. This note makes no novelty claim for the identity without a separate prior-art audit.

---

## 5. Rejected follow-on routes

Several observations appeared after the direct transfer had already failed. They are recorded here specifically to prevent retroactive success relabeling.

### Restricted conjecture

Finite exploration suggested the possible restricted statement

[
T_4(A)\le2n+1
]

for binary synchronizing automata consisting of one permutation letter and one rank-((n-1)) letter.

This was **not proved** and is **not** counted as the result of the original candidate.

### Special-pair proof route

A proposed proof route attempted to rotate the unique kernel pair by the permutation letter and reduce the frontier-triple problem to a family of special pairs.

This route fails even in a small synchronizing example because the proposed admissible index set can be empty. The route is therefore rejected rather than repaired by adding favorable hypotheses.

### Circular/permutation-one-cycle case

A later attempt to isolate the case in which the permutation letter is a single (n)-cycle runs into existing circular-automata extension theory. Such restricted cases must not be presented as a new theorem without a dedicated prior-art audit.

---

## 6. Verdict

### Candidate

> Directly lift Zhang's (k=3) construction mechanism to obtain a new (k=4) rendezvous lower bound.

### Verdict

[
\boxed{\text{FAIL}}
]

Reason:

[
T_4=
\left\lfloor\frac{4n}{3}\right\rfloor+1
]

through the audited Zhang-family range (9\le n\le60), which is strictly weaker than the existing Černý-family lower bound

[
2n+1.
]

### What the failure teaches

The useful diagnosis is not a new bound. It is the mechanism:

[
k=3:quad 3\to2\to1
]

can be delayed by a pair-level obstruction, while

[
k=4:quad 4\to3\to2\to1
]

requires control of an additional subset layer.

That observation may inform future work, but it does not convert this failed candidate into a successful paper.

---

## 7. Claim boundary

This note does **not** claim:

- a new lower bound for (\operatorname{rdv}(4,n));
- a new upper bound for (\operatorname{rdv}(4,n));
- that (2n+1) is the exact value;
- that the restricted permutation + rank-((n-1)) conjecture is true;
- that the diagnostic frontier identity is novel;
- that Zhang's construction is suboptimal for (k=3);
- or that no different Zhang-inspired (k=4) construction can work.

It claims only that the **direct reuse of the tested Zhang family** does not improve the known (k=4) lower bound and records the structural reason observed in the audit.

---

## 8. Research-accounting status

- Main successful-paper count: **unchanged**
- Candidate status: **rejected**
- Failure category: **negative computational / structural transfer audit**
- Goalpost movement: **forbidden**
- Side observations: preserved, but not counted as replacement success
