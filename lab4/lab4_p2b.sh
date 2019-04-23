#!/bin/bash

# Created on 2018-05-12
# Author: Kaituo Xu (NPU, ASLP)
# NOTE:
# 1. Using OpenFst.
# 2. Add input/output symbols table for OpenFst.

# This script is almost the same with p2a.sh, but works on wd3

echo "*** Compile wd2.fst"
echo -n "<epsilon>" > wd.isyms; awk '{print $3}' wd2lx.fsm | sort -u | awk '{print $0 " " NR-1}' >> wd.isyms
fstcompile --isymbols=wd.isyms --osymbols=wd.isyms --keep_isymbols --keep_osymbols --acceptor wd3.fsm wd2.fst
fstprint wd2.fst
fstdraw wd2.fst | dot -Tps > wd2.ps

echo "*** Compile wd2lx.fst"
echo -n "<epsilon>" > wd2lx.osyms; awk '{print $4}' wd2lx.fsm | sort -u | awk '{print $0 " " NR-1}' >> wd2lx.osyms
fstcompile --isymbols=wd.isyms --osymbols=wd2lx.osyms --keep_isymbols --keep_osymbols wd2lx.fsm wd2lx.fst
fstprint wd2lx.fst
fstdraw wd2lx.fst | dot -Tps > wd2lx.ps

echo "*** Compose lx.fst by wd2.fst and wd2lx.fst"
fstcompose wd2.fst wd2lx.fst lx.fst
fstprint lx.fst
fstdraw lx.fst | dot -Tps > lx.ps

echo "*** Compile lx2pn.fst"
echo -n "<epsilon>" > lx2pn.osyms; awk '{print $4}' lx2pn.fsm | sort | uniq | awk '{print $0 " " NR-1}' >> lx2pn.osyms
fstcompile --isymbols=wd2lx.osyms --osymbols=lx2pn.osyms --keep_isymbols --keep_osymbols lx2pn.fsm lx2pn.fst
fstprint lx2pn.fst
fstdraw lx2pn.fst | dot -Tps > lx2pn.ps

echo "*** Compose pn.fst by lx.fst and lx2pn.fst"
fstcompose lx.fst lx2pn.fst pn.fst
fstprint pn.fst
fstdraw pn.fst | dot -Tps > pn.ps
