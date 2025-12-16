# Output type, may switch to PDF instead of PNG
set terminal pngcairo

# CSV input parameters
set datafile separator ','
set key autotitle columnhead

# Common ploy parameters
set grid
set ytics format "%.1s%c"
set xtics 1
set xlabel "log_2 N"
set ylabel "ns / element"
set key box width 1 height 1

# Line styles
set style line 101 lw 2 lt rgb "#f62aa0" 
set style line 102 lw 2 lt rgb "#26dfd0" 
set style line 103 lw 2 lt rgb "#b8ee30" 