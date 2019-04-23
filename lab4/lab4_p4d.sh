#!/bin/bash -e


dbgStr=""
while [[ ( $# -gt 0 ) && ( "$1" == -?* ) ]] ; do
    strFlag="$1" ; shift
    case "$strFlag" in
        -dbg) dbgStr="dbg.sh" ;;
        *) echo "Unrecognized flag: $strFlag" 1>&2 ; exit 1 ;;
    esac
done

if [[ -e ./src/lab4_vit ]] ; then
    binStr="./src/lab4_vit"
else
    echo "Couldn't find program to execute."
    exit 1
fi


$dbgStr $binStr --attila_gmm wsj --audio_file wsj.10.feats \
    --frontend.null true --graph_file small_graph.fsm --word_syms wsj.syms \
    --dcd_file p4d.dcd --ac_wgt 0.07 --log10_beam 100 --rank_beam 2000


