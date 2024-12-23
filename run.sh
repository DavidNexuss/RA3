#!/bin/sh

# Run the program for each data set
for i in dat/*.txt; do
  fbname=$(basename "$i" .txt)
  ./build/release/ra --loadtxt $i --estimate 100 > result/$fbname.csv
done
