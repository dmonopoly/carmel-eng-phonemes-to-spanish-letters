#!/usr/bin/python
# Input:
# AE T
# HH AH T
# Output:
# 1 HH AH T
# 1 _ HH AH
# 1 AH T _
# (ignores 2 sound words)
import sys
from datetime import datetime

def gather_probs(line, probs):
  parts = line.split()
  if len(parts) <= 2:
    return
  i = 0
  while True:
    if i + 2 >= len(parts):
      break
    p1 = parts[i]
    p2 = parts[i + 1]
    p3 = parts[i + 2]
    key = (p1, p2, p3)
    if key in probs:
      probs[key] += 1
    else:
      probs[key] = 1
    i += 1
  # Add space-initial and space-final trigrams.
  space = '_'
  p1 = parts[0]
  p2 = parts[1]
  key = (space, p1, p2)
  if key in probs:
    probs[key] += 1
  else:
    probs[key] = 1
  p1 = parts[-2]
  p2 = parts[-1]
  key = (p1, p2, space)
  if key in probs:
    probs[key] += 1
  else:
    probs[key] = 1

def main():
  file1 = sys.argv[1]
  file2 = sys.argv[2]
  fin = open(file1, 'r')
  fout = open(file2, 'w')

  print '-Reading file %s-' % file1
  # Map of (phon1, phon2, phon3) to count, e.g. (HH AH T) => 23
  probs = {}
  for i, line in enumerate(fin):
    line = line.rstrip()
    gather_probs(line, probs)

  for tup in probs.keys():
    p1 = tup[0]
    p2 = tup[1]
    p3 = tup[2]
    count = probs[(p1, p2, p3)]
    fout.write('%s %s %s %s\n' % (count, p1, p2, p3))
  print '-Finished writing to file %s-' % file2

if len(sys.argv) != 3:
  print 'Usage: python <exec> <target-file1> <target-file2>'
else:
  main()

