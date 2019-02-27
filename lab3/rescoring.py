import sys
from itertools import accumulate

def resocring_one_utt(candidates, lm_weight):
    best_score = -1000000
    for c in candidates:
        utt, prob = c.strip().split("A=")
        am_prob, lm_prob = prob.strip().split('\t')
        score = float(am_prob) + lm_weight*float(lm_prob)
        if score > best_score:
            best_score = score
            best_utt =utt
    return best_utt

def rescoring(fname, nbest_stat, lm_weight):
    c = [0]
    #print(c)
    with open(nbest_stat) as f:
        c = c + [int(i.strip()) for i in f.readlines()]
        c = list(accumulate(c))
        #print(c)
    with open(fname) as f2:
        l = f2.readlines()
        if len(l) != c[len(c)-1]:
            print(len(l), c[len(c)-1])
        for u in range(len(c)-1):
            #print(u+1)
            print(resocring_one_utt(l[c[u]:c[u+1]], lm_weight))
    return

if __name__=='__main__':
    fname = sys.argv[1]
    nbest_stat = sys.argv[2]
    lm_weight = float(sys.argv[3])
    rescoring(fname, nbest_stat, lm_weight)
