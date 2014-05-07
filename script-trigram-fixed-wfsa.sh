#!/bin/sh
#PBS -l walltime=6:00:00
# cd $PBS_O_WORKDIR
counts_file='LanguageModels/trigram_eng_phonemes.txt'
cipher_file='Ciphers/spanish.written_short.quoted.txt'
# cipher_file='Ciphers/spanish.written_one_third.txt '
# cipher_file='Ciphers/spanish.written.quoted.txt'

clang++ PhonemeToLetter.cc CypherReader.cc -o 2.out
./2.out $cipher_file
clang++ -g EnglishPhonemeTrigramModel.cc EMViterbiPackage/Notation.cc CypherReader.cc TagGrammarFinderSparse.cc -o 1.out
./1.out $counts_file

carmel --train-cascade -HJ -X .99999 $cipher_file 1_eng_phoneme_lm.wfsa 2_phoneme_to_letter.fst
awk 'NF>0' $cipher_file > cipher.data.noe
# carmel --project-right --project-identity-fsa -HJ 2_phoneme_to_letter.fst.trained > 2_phoneme_to_letter.fst.noe
cat cipher.data.noe | carmel -qbsriWEIk 1 2_phoneme_to_letter.fst.trained
