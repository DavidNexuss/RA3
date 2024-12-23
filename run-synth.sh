#!/bin/sh

mkdir -p result_synth/alpha
mkdir -p result_synth/N
mkdir -p result_synth/n


echo "Testing for alpha"
# Test if alpha makes any difference
./build/release/ra --generate 100000 0.1 10000 --estimate 100 > result_synth/alpha/0_5.csv
./build/release/ra --generate 100000 0.3 10000 --estimate 100 > result_synth/alpha/0_3.csv
./build/release/ra --generate 100000 0.5 10000 --estimate 100 > result_synth/alpha/0_5.csv
./build/release/ra --generate 100000 0.7 10000 --estimate 100 > result_synth/alpha/0_7.csv
./build/release/ra --generate 100000 0.9 10000 --estimate 100 > result_synth/alpha/0_9.csv
./build/release/ra --generate 100000 1.0 10000 --estimate 100 > result_synth/alpha/1_0.csv

echo "Testing for N"
# Test for different N
./build/release/ra --generate 100000 1.0 10000 --estimate 100 >   result_synth/N/100000.csv
./build/release/ra --generate 500000 1.0 10000 --estimate 100 >   result_synth/N/500000.csv
./build/release/ra --generate 1000000 1.0  10000 --estimate 100 > result_synth/N/1000000.csv
./build/release/ra --generate 1500000 1.0  10000 --estimate 100 > result_synth/N/1500000.csv
./build/release/ra --generate 2000000 1.0 10000 --estimate 100 >  result_synth/N/2000000.csv
./build/release/ra --generate 2500000 1.0 10000 --estimate 100 >  result_synth/N/2500000.csv

echo "Testing for n"
# Test for different n
./build/release/ra --generate 100000 1.0 10000 --estimate 100 > result_synth/n/10000.csv
./build/release/ra --generate 100000 1.0 30000 --estimate 100 > result_synth/n/30000.csv
./build/release/ra --generate 100000 1.0 50000 --estimate 100 > result_synth/n/50000.csv
./build/release/ra --generate 100000 1.0 70000 --estimate 100 > result_synth/n/70000.csv
./build/release/ra --generate 100000 1.0 90000 --estimate 100 > result_synth/n/90000.csv
