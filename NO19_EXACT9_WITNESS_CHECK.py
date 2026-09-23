#!/usr/bin/env python3
from collections import deque

A_LETTER=[4,7,3,8,0,6,5,1,9,2,10]
B_LETTER=[1,6,7,3,4,0,5,8,7,10,9]
S=frozenset(range(8))
PROTECTED=0


def image(X,t):
    return frozenset(t[x] for x in X)


def shortest_aoc():
    q=deque([(S,"")]); seen={S}
    while q:
        X,w=q.popleft()
        if w and (PROTECTED not in X or len(X)<len(S)):
            return len(w),w,X
        for ch,t in (("a",A_LETTER),("b",B_LETTER)):
            Y=image(X,t)
            if Y not in seen:
                seen.add(Y); q.append((Y,w+ch))
    raise RuntimeError


def strongly_connected():
    n=11
    for s in range(n):
        seen={s}; q=deque([s])
        while q:
            x=q.popleft()
            for t in (A_LETTER,B_LETTER):
                y=t[x]
                if y not in seen:
                    seen.add(y); q.append(y)
        if len(seen)!=n: return False
    return True


def synchronizing():
    n=11
    pairs=[(i,j) for i in range(n) for j in range(i,n)]
    idx={p:k for k,p in enumerate(pairs)}
    rev=[[] for _ in pairs]
    good=[False]*len(pairs); q=deque()
    for i in range(n):
        good[idx[(i,i)]]=True; q.append(idx[(i,i)])
    for p,(i,j) in enumerate(pairs):
        for t in (A_LETTER,B_LETTER):
            x,y=t[i],t[j]
            if x>y: x,y=y,x
            rev[idx[(x,y)]].append(p)
    while q:
        v=q.popleft()
        for u in rev[v]:
            if not good[u]: good[u]=True; q.append(u)
    return all(good)


def count_arithmetic():
    Pp,Ps,Qp,Qs,Ds=907_200,426_720,3_628_800,1_074_240,449_280
    return {
        "I_PP":(Pp+Ps)**2-Pp**2,
        "I_PQ":(Pp+Ps)*(Qp+Qs)-Pp*Qp,
        "II_DD":Ds**2,
        "II_DQ":Ds*(Qp+Qs),
    }

if __name__=="__main__":
    d,w,X=shortest_aoc()
    counts=count_arithmetic()
    print("distance=",d)
    print("first_word=",w)
    print("terminal=",sorted(X))
    print("strongly_connected=",strongly_connected())
    print("synchronizing=",synchronizing())
    print("family_counts=",counts)
    print("family_total=",sum(counts.values()))
    assert d==9 and w=="aabaaabab"
    assert strongly_connected() and synchronizing()
    assert sum(counts.values())==6_252_596_812_800
    print("PASS")
