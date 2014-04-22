#!/bin/sh
#PBS -l walltime=1:00:00
cd $PBS_O_WORKDIR
cipher_file='Ciphers/spanish.written_one_third.txt'
# cipher_file='Ciphers/spanish.written.quoted.txt'

clang++ EnglishPhonemeTrigramModel.cc -o 1.out
./1.out
clang++ PhonemeToLetter.cc $cipher_file -o 2.out
./2.out

carmel --train-cascade -HJ -X .99999 -! 20 -: $cipher_file 1_eng_phoneme_trigram.wfsa 2_phoneme_to_letter.fst
awk 'NF>0' $cipher_file > cipher.data.noe 
carmel --project-right --project-identity-fsa -HJ 2_phoneme_to_letter.fst.trained > 2_phoneme_to_letter.fst.noe 
cat cipher.data.noe | carmel -qbsriWEIk 1 2_phoneme_to_letter.fst.noe
