#!/usr/bin/env python3

import sys

def usage() :
    print("convert_fmt.py : convert UH and UM to (%HESITATION)")
    print("         usage : python convert_hesitation.py reco")

if __name__ == '__main__':
    if len(sys.argv) == 1 :
        usage()
        sys.exit(0)
    reco_file = sys.argv[1]
    with open(reco_file) as f: 
        for l in f.readlines():
            fields = l.strip().split(' ')
            print(' '.join([ '(%HESITATION)' if x =='UH' or x == 'UM' else x for x in fields ]))