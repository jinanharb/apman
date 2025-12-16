# bench/bench_stack.plt
# Uso típico:
#   gnuplot -c bench/bench_stack.plt

set terminal pngcairo size 800,600
set output 'bench/bench_stack.png'

set title 'Stack benchmark: array vs deque'
set xlabel 'N (number of elements)'
set ylabel 'Time (s)'
set grid

# normalmente faz sentido log em x, porque N cresce em potências de 2
set logscale x 2

set key left top

# arquivo gerado pelo bench/bench_stack.bench:
# coluna 1: N
# coluna 2: tempo da stack baseada em array
# coluna 3: tempo da stack baseada em deque
plot 'bench/bench_stack.csv' using 1:2 with linespoints title 'array stack', \
     'bench/bench_stack.csv' using 1:3 with linespoints title 'deque stack'
