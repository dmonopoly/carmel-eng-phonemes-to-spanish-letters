#!/usr/bin/python
# Given a bunch of words in a text file, this wgets the CMU pronouncing
# dictionary and collects English phonemes.
# Example Output:
# T IY
# TH EY T AH
# HH UH D
# T UW
# V IY
# W IY
# Y IY L D
# Z IY

import sys
import subprocess as sub
from datetime import datetime

# Line 38 should have something like this (for 'cool'):
# <IMG SRC="http://www.speech.cs.cmu.edu/images/yellow-ball.gif"> <tt>K UW L .</tt>
def grab_phons(cmu_file):
  """Returns a list of phonemes for a single word.
  """
  lis = []
  fin = open(cmu_file, 'r')
  for i, line in enumerate(fin):
    if i == 38:
      parts = line.split()
      # <tt>phon1
      phon1 = parts[2][3:]
      lis.append(phon1)
      j = 3
      while j < len(parts):
        next_phon = parts[j]
        lis.append(next_phon)
  return lis

def handle_line(line, phons):
  parts = line.split()
  if len(parts) <= 1:
    return
  i = 0
  while i < len(parts):
    word = parts[i]
    cmd = 'wget http://www.speech.cs.cmu.edu/cgi-bin/cmudict?in='
    cmd += word
    cmd += '#lookup'
    sub.call(cmd)
    resulting_file = 'cmudict?in=%s' % word
    lis = grab_phons(resulting_file)
    phons.append(lis)
    i += 1

def main():
  file1 = sys.argv[1]
  file2 = sys.argv[2]
  fin = open(file1, 'r')
  fout = open(file2, 'w')

  print '-Reading file %s-' % file1
  # List of phoneme sequences for words, e.g. [0] = [T IY], [1] = [HH UH D]
  phons = []
  for i, line in enumerate(fin):
    line = line.rstrip()
    handle_line(line, phons)

  for x in phons:
    fout.write(x)
  print '-Finished writing to file %s-' % file2

if len(sys.argv) != 3:
  print 'Usage: python <exec> <input-file> <output-file>'
else:
  main()

