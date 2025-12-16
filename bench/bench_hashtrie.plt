# bench/hashtrie_performance.plt
load "bench/common.plt"

set key top left

set output "bench/hashtrie_performance.png"
set title "Hashtrie Performance Benchmark"
set xlabel "Number of Elements (N)"
set ylabel "Time per Operation (ns)"

# Optional: logscale for better visualization
#set logscale y

# CSV columns:
# 1 = log2(N)
# 2 = N
# 3 = insert
# 4 = find
# 5 = remove

plot "bench/bench_hasht.csv" using 2:3 with lines ls 101 title "Insert", \
     "bench/bench_hasht.csv" using 2:4 with lines ls 102 title "Find", \
     "bench/bench_hasht.csv" using 2:5 with lines ls 103 title "Remove"
