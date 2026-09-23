from collections import deque

samples = {
'II-DQ': ([1,0,7,6,3,5,2,4,0,10,9],[0,9,10,3,4,8,6,7,5,1,2]),
'I-PP': ([6,7,0,4,3,5,2,1,7,10,9],[2,9,0,10,4,8,7,6,5,1,3]),
'I-PQ(P-sing/Q-any)': ([6,7,0,4,3,5,2,1,7,10,9],[0,9,2,10,4,8,7,6,5,1,3]),
'I-PQ(P-perm/Q-sing)': ([7,10,0,9,8,6,2,5,4,3,1],[0,7,1,3,4,6,2,5,7,10,9]),
}
N=11

def transitive_closure(a,b):
    R=[[False]*N for _ in range(N)]
    for i in range(N): R[i][i]=R[i][a[i]]=R[i][b[i]]=True
    for k in range(N):
        for i in range(N):
            if R[i][k]:
                rowi=R[i]; rowk=R[k]
                for j in range(N): rowi[j]=rowi[j] or rowk[j]
    return R

def sccs(R):
    left=set(range(N)); comps=[]
    while left:
        x=min(left); c={y for y in left if R[x][y] and R[y][x]}; comps.append(sorted(c)); left-=c
    return sorted(comps,key=lambda c:(-len(c),c))

def image(mask,t):
    z=0
    for x in range(N):
        if mask>>x & 1: z |= 1<<t[x]
    return z

def sync_subset_bfs(a,b):
    start=(1<<N)-1; q=deque([start]); seen={start}; minrank=N; minm=start
    while q:
        m=q.popleft(); r=m.bit_count()
        if r<minrank: minrank,minm=r,m
        if r==1: return True,len(seen),minrank,minm
        for t in (a,b):
            z=image(m,t)
            if z not in seen: seen.add(z); q.append(z)
    return False,len(seen),minrank,minm

ok=True
for name,(a,b) in samples.items():
    R=transitive_closure(a,b); C=sccs(R); sc=len(C)==1
    sy,nreach,minrank,minm=sync_subset_bfs(a,b)
    ok &= (not sc and not sy)
    print(name)
    print('  strongly_connected =',sc)
    print('  SCCs =',C)
    print('  synchronizing =',sy)
    print('  reachable_subset_count =',nreach)
    print('  minimum_reachable_rank =',minrank)
    print('  one_minimum_image =',[i for i in range(N) if minm>>i&1])

print('PASS_CROSSBRANCH_LOCAL_SURVIVOR_SPOTCHECK' if ok else 'FAIL_CROSSBRANCH_LOCAL_SURVIVOR_SPOTCHECK')
