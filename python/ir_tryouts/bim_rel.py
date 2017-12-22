
# 100 docs in total
N = 100
# 25 docs out of 100 contain term i
ni = 25
# Relevant docs
R = 10

def bim_without_rel (N, ni):

    return float(N - ni) / float(ni);

def bim_with_rel (N, R, ni, ri):

    pi = float(ri) / float(R)
    si = float(ni - ri) / float(N - R)

    return float(pi * (1 - si)) / float(si * (1 - pi))

for ri in range(1, 10):

    print bim_without_rel(N, ni), " - ", bim_with_rel(N, R, ni, ri)
