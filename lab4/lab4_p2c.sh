#!/bin/bash

# Created on 2018-05-12
# Author: Kaituo Xu (NPU, ASLP)
# NOTE:
# 1. Using OpenFst.
# 2. Add input/output symbols table for OpenFst.

echo "*** Compile wd3.fst"
awk '{print $3}' addsil.fsm | sort -u | grep -v '^$' | awk '{print $0 " " NR-1}' > addsil.isyms
awk '{print $4}' addsil.fsm | sort -u | grep -v '^$' | awk '{print $0 " " NR-1}' > addsil.osyms
fstcompile --isymbols=addsil.isyms --osymbols=addsil.isyms --keep_isymbols --keep_osymbols --acceptor wd3.fsm wd3.fst
fstprint wd3.fst
fstdraw --acceptor wd3.fst | dot -Tps > wd3.ps

echo "*** Compile addsil.fst"
fstcompile --isymbols=addsil.isyms --osymbols=addsil.osyms --keep_isymbols --keep_osymbols addsil.fsm addsil.fst
fstprint addsil.fst
fstdraw addsil.fst | dot -Tps > addsil.ps

echo "*** Compose wd3sil.fst by wd3.fst and addsil.fst"
fstcompose wd3.fst addsil.fst wd3sil.fst
fstprint wd3sil.fst
fstdraw --acceptor wd3sil.fst | dot -Tps > wd3sil.ps

