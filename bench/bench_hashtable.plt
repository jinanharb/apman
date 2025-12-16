load "bench/common.plt" # Common plot options

# Insert performance comparison
set key top left
set output "bench/hashtable_insert.png"
set title "Insert Performance: Linear vs Robin Hood vs Closed"

plot "bench/bench_linear.csv" using 1:3 with lines ls 101 title "Linear", \
     "bench/bench_robinhood.csv" using 1:3 with lines ls 102 title "Robin Hood", \
     "bench/bench_closed.csv" using 1:3 with lines ls 103 title "Closed"

# Find/Get performance comparison
set output "bench/hashtable_find.png"
set title "Find Performance: Linear vs Robin Hood vs Closed"

plot "bench/bench_linear.csv" using 1:4 with lines ls 101 title "Linear", \
     "bench/bench_robinhood.csv" using 1:4 with lines ls 102 title "Robin Hood", \
     "bench/bench_closed.csv" using 1:4 with lines ls 103 title "Closed"

# Remove performance comparison
set output "bench/hashtable_remove.png"
set title "Remove Performance: Linear vs Robin Hood vs Closed"

plot "bench/bench_linear.csv" using 1:5 with lines ls 101 title "Linear", \
     "bench/bench_robinhood.csv" using 1:5 with lines ls 102 title "Robin Hood", \
     "bench/bench_closed.csv" using 1:5 with lines ls 103 title "Closed"

# Combined view - all operations
set output "bench/hashtable_combined.png"
set title "All Operations: Linear vs Robin Hood vs Closed"
set key top left

plot "bench/bench_linear.csv" using 1:3 with lines ls 101 title "Linear Insert", \
     "bench/bench_linear.csv" using 1:4 with lines ls 101 dt 2 title "Linear Find", \
     "bench/bench_linear.csv" using 1:5 with lines ls 101 dt 3 title "Linear Remove", \
     "bench/bench_robinhood.csv" using 1:3 with lines ls 102 title "RH Insert", \
     "bench/bench_robinhood.csv" using 1:4 with lines ls 102 dt 2 title "RH Find", \
     "bench/bench_robinhood.csv" using 1:5 with lines ls 102 dt 3 title "RH Remove", \
     "bench/bench_closed.csv" using 1:3 with lines ls 103 title "Closed Insert", \
     "bench/bench_closed.csv" using 1:4 with lines ls 103 dt 2 title "Closed Find", \
     "bench/bench_closed.csv" using 1:5 with lines ls 103 dt 3 title "Closed Remove"