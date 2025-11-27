# Gnuplot script for find benchmark
FILE = "bench/bench_hash_find.csv"
load "bench/common.plt"

# First plot: linear vs closed find
set key top left
set output "bench/find_comparison.png"
set title "Hash Table Find Performance"
set ylabel "Time per operation (ns)"
plot FILE using 1:2 with lines ls 101 title "linear", \
     FILE using 1:3 with lines ls 102 title "closed"

# Second plot: ratio closed / linear
set key top right
set output "bench/find_ratio.png"
set title "Hash Table Find: Closed/Linear Ratio"
set ylabel "Ratio (closed/linear)"
plot FILE using 1:($3/$2) with lines ls 103 title "closed/linear"