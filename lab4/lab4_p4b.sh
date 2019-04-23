#!/bin/bash -e


if [[ -e ./src/lab4_vit ]] ; then
    binStr="./src/lab4_vit"
elif [[ -e Lab4Vit.class ]] ; then
    binStr="java Lab4Vit"
else
    echo "Couldn't find program to execute."
    exit 1
fi


echo "======================================================================"
echo "beam = 20"
echo "======================================================================"

$binStr --gmm isodigit.gmm --audio_file isodigit.10.dat \
    --graph_file isodigit.eps.fsm --word_syms isodigit.syms \
    --dcd_file /dev/null --ac_wgt 0.1 --log10_beam 20 --rank_beam 0

echo "======================================================================"
echo "beam = 10"
echo "======================================================================"

$binStr --gmm isodigit.gmm --audio_file isodigit.10.dat \
    --graph_file isodigit.eps.fsm --word_syms isodigit.syms \
    --dcd_file /dev/null --ac_wgt 0.1 --log10_beam 10 --rank_beam 0

echo "======================================================================"
echo "beam = 5"
echo "======================================================================"

$binStr --gmm isodigit.gmm --audio_file isodigit.10.dat \
    --graph_file isodigit.eps.fsm --word_syms isodigit.syms \
    --dcd_file /dev/null --ac_wgt 0.1 --log10_beam 5 --rank_beam 0


