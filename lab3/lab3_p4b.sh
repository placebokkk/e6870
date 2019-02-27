#!/bin/bash -e


if [[ -e ./src/lab3_lm ]] ; then
    binStr="./src/lab3_lm"
else
    echo "Couldn't find program to execute."
    exit 1
fi


echo
echo "======================================================================"
echo "3-gram model (WB smoothing; different size SWB corpus)"
echo "======================================================================"
echo

order=3
gzip -cd nbest/*.nbest.gz > all.nbest.orign

# nbest_stat keep the nbest number for each utt
python3 ./get_nbest_stat.py all.nbest.orign > nbest_stat

#remove comment, remove ~SIL in utt
cat all.nbest.orign|  grep -v '#'|sed 's/~SIL//g'|sed -E 's/( )+/ /g'|sed -E 's/^ //g'|sed -E 's/ $//g' > all.nbest
head all.nbest

for d in 2000 20000 200000 232838 ; do
    # get lm score
    $binStr --vocab lab3.syms --train swb.$d.txt --test all.nbest \
            --n $order --sent_log_probs all.nbest.$d.lmprob

    # put am and lm score together
    paste all.nbest all.nbest.$d.lmprob > all.nbest.$d.prob

    python3 rescoring.py all.nbest.$d.prob nbest_stat 4.0 > all.nbest.$d.rec

    # convert UH and UM to (%HESITATION)
    python3 convert_hesitation.py  all.nbest.$d.rec >  all.nbest.$d.rec2

    paste -d' ' all.nbest.$d.rec2 p4.id > p4b.$d.reco
    ./calc-wer.sh p4.ref p4b.$d.reco > p4b.$d.wer
done

#swb2000 44.09%
#swb20000 43.94 % 
#swb200000 44.09%
#swb232838 43.94 % 

exit 1

# You could try different lm weight
echo
echo "======================================================================"
echo "3-gram model (WB smoothing; full SWB corpus; different LM weights)"
echo "======================================================================"
echo

d=232838
if [[ ! -e all.nbest.$d.prob ]] ; then
    echo "Couldn't find all.nbest.$d.prob. Make sure you have run lab3_p4b.sh"
else
    for lmw in 2.0 3.0 4.0 ; do
        python3 rescoring.py all.nbest.$d.prob nbest_stat $lmw > all.nbest.$d.$lmw.rec

        # convert UH and UM to (%HESITATION)
        python3 convert_hesitation.py  all.nbest.$d.$lmw.rec >  all.nbest.$d.$lmw.rec2

        paste -d' ' all.nbest.$d.$lmw.rec2 p4.id > p4b.$d.$lmw.reco
        ./calc-wer.sh p4.ref p4b.$d.$lmw.reco > p4b.$d.$lmw.wer
    done
fi


# You could try different order
