#!/bin/bash -e


if [[ -e ./src/lab4_vit ]] ; then
    binStr="./src/lab4_vit"
else
    echo "Couldn't find program to execute."
    exit 1
fi


echo
echo "======================================================================"
echo "WSJ, 3kw voc, small LM, beam = 10"
echo "======================================================================"
echo

$binStr --attila_gmm wsj --audio_file wsj.10.feats --frontend.null true \
    --graph_file small_graph.fsm --word_syms wsj.syms \
    --dcd_file p5.dcd --ac_wgt 0.07 --log10_beam 10 --rank_beam 0
p018h1.calc-wer.sh -filter wsj.lx2wd.fsm.gz p5.dcd wsj.ref p5.tmp


echo
echo "======================================================================"
echo "WSJ, 3kw voc, small LM, beam = 5"
echo "======================================================================"
echo

$binStr --attila_gmm wsj --audio_file wsj.10.feats --frontend.null true \
    --graph_file small_graph.fsm --word_syms wsj.syms \
    --dcd_file p5.dcd --ac_wgt 0.07 --log10_beam 5 --rank_beam 0
p018h1.calc-wer.sh -filter wsj.lx2wd.fsm.gz p5.dcd wsj.ref p5.tmp


echo
echo "======================================================================"
echo "WSJ, 3kw voc, small LM, beam = 3"
echo "======================================================================"
echo

$binStr --attila_gmm wsj --audio_file wsj.10.feats --frontend.null true \
    --graph_file small_graph.fsm --word_syms wsj.syms \
    --dcd_file p5.dcd --ac_wgt 0.07 --log10_beam 3 --rank_beam 0
p018h1.calc-wer.sh -filter wsj.lx2wd.fsm.gz p5.dcd wsj.ref p5.tmp


echo
echo "======================================================================"
echo "WSJ, 3kw voc, small LM, beam = 7"
echo "======================================================================"
echo

$binStr --attila_gmm wsj --audio_file wsj.10.feats --frontend.null true \
    --graph_file small_graph.fsm --word_syms wsj.syms \
    --dcd_file p5.dcd --ac_wgt 0.07 --log10_beam 7 --rank_beam 0
p018h1.calc-wer.sh -filter wsj.lx2wd.fsm.gz p5.dcd wsj.ref p5.tmp


echo
echo "======================================================================"
echo "WSJ, 21kw voc, small LM, beam = 7"
echo "======================================================================"
echo

$binStr --attila_gmm wsj --audio_file wsj.10.feats --frontend.null true \
    --graph_file medium_graph.fsm --word_syms wsj.syms \
    --dcd_file p5.dcd --ac_wgt 0.07 --log10_beam 7 --rank_beam 0
p018h1.calc-wer.sh -filter wsj.lx2wd.fsm.gz p5.dcd wsj.ref p5.tmp


echo
echo "======================================================================"
echo "WSJ, 21kw voc, large LM, beam = 7"
echo "======================================================================"
echo

$binStr --attila_gmm wsj --audio_file wsj.10.feats --frontend.null true \
    --graph_file large_graph.fsm --word_syms wsj.syms \
    --dcd_file p5.dcd --ac_wgt 0.07 --log10_beam 7 --rank_beam 0
p018h1.calc-wer.sh -filter wsj.lx2wd.fsm.gz p5.dcd wsj.ref p5.tmp


echo
echo "======================================================================"
echo "Isolated digit recognition."
echo "======================================================================"
echo

$binStr --gmm isodigit.gmm --audio_file isodigit.10.dat \
    --graph_file isodigit.fsm --word_syms isodigit.syms \
    --dcd_file /dev/null --ac_wgt 0.1 --log10_beam 999 --rank_beam 0


