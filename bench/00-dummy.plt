HEAD="00-dummy"

load "bench/common.plt" # Common plot options

# First plot
set key top left
set output "bench/first.png"

plot "bench/" . HEAD . ".csv" using 1:2 with lines ls 101, '' using 1:3 with lines ls 102



# Second plot
set key right top
set output "bench/second.png"

plot "bench/" . HEAD . ".csv" using 1:4 with lines ls 102, '' using 1:5 with lines ls 103


