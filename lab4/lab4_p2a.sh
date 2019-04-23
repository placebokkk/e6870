#!/bin/bash

# part 2,word/lexicon/pron/cdphone/hmm构建图

# Created on 2018-05-12
# Author: Kaituo Xu (NPU, ASLP)
# NOTE:
# 1. Using OpenFst.
# 2. Add input/output symbols table for OpenFst.

# word序列-> word序列
echo "*** Compile wd2.fst"
echo -n "<epsilon>" > wd.isyms; awk '{print $3}' wd2lx.fsm | sort -u | awk '{print $0 " " NR-1}' >> wd.isyms
fstcompile --isymbols=wd.isyms --osymbols=wd.isyms --keep_isymbols --keep_osymbols --acceptor wd.fsm wd2.fst
fstprint wd2.fst
fstdraw wd2.fst | dot -Tps > wd2.ps

# word ->  多音字
echo "*** Compile wd2lx.fst"
echo -n "<epsilon>" > wd2lx.osyms; awk '{print $4}' wd2lx.fsm | sort -u | awk '{print $0 " " NR-1}' >> wd2lx.osyms
fstcompile --isymbols=wd.isyms --osymbols=wd2lx.osyms --keep_isymbols --keep_osymbols wd2lx.fsm wd2lx.fst
fstprint wd2lx.fst
fstdraw wd2lx.fst | dot -Tps > wd2lx.ps

# word序列 -> 多音字序列
echo "*** Compose lx.fst by wd2.fst and wd2lx.fst"
fstcompose wd2.fst wd2lx.fst lx.fst
fstprint lx.fst
fstdraw lx.fst | dot -Tps > lx.ps

# 多音字 -> 音素
echo "*** Compile lx2pn.fst"
echo -n "<epsilon>" > lx2pn.osyms; awk '{print $4}' lx2pn.fsm | sort | uniq | awk '{print $0 " " NR-1}' >> lx2pn.osyms
fstcompile --isymbols=wd2lx.osyms --osymbols=lx2pn.osyms --keep_isymbols --keep_osymbols lx2pn.fsm lx2pn.fst
fstprint lx2pn.fst
fstdraw lx2pn.fst | dot -Tps > lx2pn.ps


# word序列 -> 上下文无关音素序列
echo "*** Compose pn.fst by lx.fst and lx2pn.fst"
fstcompose lx.fst lx2pn.fst pn.fst
fstprint pn.fst
fstdraw pn.fst | dot -Tps > pn.ps

# 上下文无关音素 -> 上下文相关音素
echo "*** Compile pn2md.fst"
awk '{print $4}' pn2md.fsm | sort | uniq | awk '{print $0 " " NR-2}' | tail -n +2 > pn2md.osyms
fstcompile --isymbols=lx2pn.osyms --osymbols=pn2md.osyms --keep_isymbols --keep_osymbols pn2md.fsm pn2md.fst
fstprint pn2md.fst
fstdraw pn2md.fst | dot -Tps > pn2md.ps

# word序列 -> 上下文相关音素序列
echo "*** Compose md.fst by pn.fst and pn2md.fst"
fstcompose pn.fst pn2md.fst md.fst
fstprint md.fst
fstdraw md.fst | dot -Tps > md.ps

# 上下文相关音素 -> hmm状态
echo "*** Compile md2hmm.fst"
awk '{print $4}' md2hmm.fsm | sort -n | uniq | awk '{print $0 " " NR-2}' | tail -n +2 > md2hmm.osyms
fstcompile --isymbols=pn2md.osyms --osymbols=md2hmm.osyms --keep_isymbols --keep_osymbols md2hmm.fsm md2hmm.fst
fstprint md2hmm.fst
fstdraw md2hmm.fst | dot -Tps > md2hmm.ps

# word序列 -> hmm状态序列
echo "*** Compose hmm.fst by md.fst and md2hmm.fst"
fstcompose md.fst md2hmm.fst | fstinvert - hmm.fst
fstprint hmm.fst
fstdraw hmm.fst | dot -Tps > hmm.ps

