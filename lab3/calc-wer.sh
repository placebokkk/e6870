ref=$1
reco=$2
#please use python2.7
python convert_fmt.py $ref ${ref}.fmt
python convert_fmt.py $reco ${reco}.fmt

python compute-wer.pyc --v=1 ${ref}.fmt ${reco}.fmt
