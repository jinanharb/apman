# Gnuplot script for insert benchmark
FILE = "bench/bench_hash_insert.csv"
load "bench/common.plt"

# First plot: linear vs closed insert
set key top left
set output "bench/insert_comparison.png"
set title "Hash Table Insert Performance"
set ylabel "Time per operation (ns)"
plot FILE using 1:2 with lines ls 101 title "linear", \
     FILE using 1:3 with lines ls 102 title "closed"

# Second plot: ratio closed / linear
set key top right
set output "bench/insert_ratio.png"
set title "Hash Table Insert: Closed/Linear Ratio"
set ylabel "Ratio (closed/linear)"
plot FILE using 1:($3/$2) with lines ls 103 title "closed/linear"