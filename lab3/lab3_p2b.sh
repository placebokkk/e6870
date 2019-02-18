#!/bin/bash -e


if [[ -e ./lab3_lm ]] ; then
    binStr="./lab3_lm"
elif [[ -e Lab3Lm.class ]] ; then
    binStr="java Lab3Lm"
else
    echo "Couldn't find program to execute."
    exit 1
fi


$binStr --vocab lab3.syms --train minitrain.txt --test test2.txt \
    --delta 0.0001 --n 3 --word_probs p2b.probs


