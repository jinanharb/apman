# Gnuplot script adapté pour bench_hash.csv

FILE = "bench/bench_hash.csv"

load "bench/common.plt" # Common plot options

# First plot: linear vs closed
set key top left
set output "bench/first.png"

plot FILE using 1:2 with lines ls 101 title "linear", \
     FILE using 1:3 with lines ls 102 title "closed"

# Second plot: ratio closed / linear
set key top right
set output "bench/second.png"

plot FILE using 1:($3/$2) with lines ls 103 title "closed/linear"

