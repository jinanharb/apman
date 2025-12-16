load "bench/common.plt"

set key top left

set output "bench/array_performance.png"
set title "Array Performance Benchmark"
set xlabel "Number of Elements"
set ylabel "Time (ns)"

# Optional: cleaner visualization
#set logscale y

# CSV columns:
# 1 = log2(N)
# 2 = N
# 3 = new
# 4 = push
# 5 = read
# 6 = write
# 7 = pop

plot "bench/bench_array.csv" using 2:3 with lines ls 101 title "Allocation", \
     "bench/bench_array.csv" using 2:4 with lines ls 102 title "Push", \
     "bench/bench_array.csv" using 2:7 with lines ls 103 title "Pop"
