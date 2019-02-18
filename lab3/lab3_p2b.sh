#!/bin/bash -e


if [[ -e ./src/lab3_lm ]] ; then
    binStr="./src/lab3_lm"
else
    echo "Couldn't find program to execute."
    exit 1
fi


$binStr --vocab lab3.syms --train minitrain.txt --test test2.txt \
    --delta 0.0001 --n 3 --word_probs p2b.probs


