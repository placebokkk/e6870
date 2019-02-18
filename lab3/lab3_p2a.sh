#!/bin/bash -e


if [[ -e ./src/lab3_lm ]] ; then
    binStr="./src/lab3_lm"
else
    echo "Couldn't find program to execute."
    exit 1
fi


for n in 1 2 3 ; do
$binStr --vocab lab3.syms --train minitrain.txt --test test1.txt \
    --delta 0.0001 --n $n --word_probs p2a.$n.probs
done


