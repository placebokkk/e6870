import sys
def get_nbest_stat(fname):
    nbest = 0
    stat = []
    with open(fname) as f:
        for l in  f.readlines():
            if l.startswith('#'):
                stat.append(nbest)
                nbest = 0
            else:
                nbest = nbest + 1
        stat.append(nbest)
    for s in stat[1:]:
        print(s)


if __name__=='__main__':
    fname = sys.argv[1]
    get_nbest_stat(fname)
