#!/bin/bash
#part 1部分的作业，请一行一行执行学习fst的基本使用
#可以fstprint和fstdraw查看fst内容

#echo -n "<epsilon>" > wd.isyms; awk '{print $3}' wd2lx.fsm | sort -u | awk '{print $0 " " NR-1}' >> wd.isyms

# two string
echo "*** Compile p1a.fst"
fstcompile --isymbols=p1a.isyms --osymbols=p1a.osyms --keep_isymbols --keep_osymbols --acceptor p1a.fsm p1a.fst
fstprint p1a.fst
fstdraw p1a.fst | dot -Tps > p1a.ps

# a fst convert like to love
echo "*** Compile p1b.fst"
fstcompile --isymbols=p1a.osyms --osymbols=p1b.osyms --keep_isymbols --keep_osymbols p1b.fsm p1b.fst
fstprint p1b.fst
fstdraw p1b.fst | dot -Tps > p1b.ps

# do the convert 
echo "*** compose p1a.fst and p1b.fst into p1c.fst"
fstcompose p1a.fst p1b.fst p1c.fst
fstprint p1c.fst
fstdraw p1c.fst | dot -Tps > p1c.ps

# all three words sentence excluding two sentences.
fstcompile --isymbols=p1a.isyms --osymbols=p1a.isyms --keep_isymbols --keep_osymbols  --acceptor p1d.fsm p1d.fst
fstdraw p1d.fst | dot -Tps > p1d.ps
fstconcat p1d.fst p1d.fst|fstconcat - p1d.fst |fstdeterminize|fstrmepsilon|fstdifference - p1a.fst p1e.fst
fstdraw p1e.fst| dot -Tps| > p1e.ps
