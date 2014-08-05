#! /usr/bin/gnuplot

reset

set xrange [0.0:75.0]
set yrange [0.0:1.0]

set title "Solution space under EDF"

set terminal png
set output "edf.png"

unset key

plot "tmp.data" with lines
