# No.19 review R4 response - 2026-09-23

This review pass targets the last practical reproducibility risks in the finite branch certifier.

## Symmetry quotient

For the II mixed normal form, the manuscript now states the exact relabeling group and generators.
A separate union-find implementation enumerates the 449,280 unary-safe D-singular first letters and
uses only the six generator conjugations. It obtains 822 orbits, matching the frozen II-DD/II-DQ count.

## Local survivor classification

One local survivor from each of the four nonzero-survivor computational subcases is included in an
independent Python spot checker. Strong connectivity is checked by transitive closure and synchronization
by full-subset BFS. All four examples independently fail both properties.

## Unary counts

The prior R3 audit remains unchanged: five unary counts have multiple independent implementations,
including a third full brute-force audit of all 10*9! singular maps for each singular type.

## Main branch certifier

The 432,128,504-pair frozen branch outputs and branch source are unchanged from R3. R4 adds independent
checks around the two most review-sensitive auxiliary mechanisms: the symmetry quotient and the final
BSC classification of local survivors.
