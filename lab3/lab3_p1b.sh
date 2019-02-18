#!/bin/bash -e


if [[ -e ./src/lab3_lm ]] ; then
    binStr="./src/lab3_lm"
else
    echo "Couldn't find program to execute."
    exit 1
fi


$binStr --vocab lab3.syms --train minitrain.txt --test test1.txt \
    --count_file p1b.counts


