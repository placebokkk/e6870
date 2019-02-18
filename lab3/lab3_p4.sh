#!/bin/bash -e
# Can't find the p018p1.rescore.py, please use lab3.p4b.sh

if [[ -e ./src/lab3_lm ]] ; then
    binStr="./src/lab3_lm"
else
    echo "Couldn't find program to execute."
    exit 1
fi


echo
echo "======================================================================"
echo "1-gram model (WB smoothing; full SWB corpus)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.txt --test %szCorpus \
        --n 1 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp

echo
echo "======================================================================"
echo "2-gram model (WB smoothing; full SWB corpus)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.txt --test %szCorpus \
        --n 2 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp

echo
echo "======================================================================"
echo "3-gram model (WB smoothing; full SWB corpus)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp


echo
echo "======================================================================"
echo "No smoothing, i.e., MLE (3-gram model; full SWB corpus)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile --delta 0.0
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp

echo
echo "======================================================================"
echo "Plus-delta smoothing (3-gram model; full SWB corpus)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile --delta 0.0001
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp

echo
echo "======================================================================"
echo "Witten-Bell smoothing (3-gram model; full SWB corpus)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp


echo
echo "======================================================================"
echo "2000 sentences of training data (3-gram model; WB smoothing)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.2000.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp

echo
echo "======================================================================"
echo "20000 sentences of training data (3-gram model; WB smoothing)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.20000.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp

echo
echo "======================================================================"
echo "200000 sentences of training data (3-gram model; WB smoothing)"
echo "======================================================================"
echo

p018p1.rescore.py -lw 4 -transp ~SIL 0.1 \
    p4.ctl nbest:.nbest.gz p4.tmp p4.tmp.trn \
    $binStr --vocab lab3.syms --train swb.200000.txt --test %szCorpus \
        --n 3 --sent_log_probs %szProbFile
p018h1.calc-wer.sh -filter filter.fsm p4.tmp.trn p4.ref p4.tmp


rm p4.tmp.trn


