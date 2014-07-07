#! /usr/bin/gnuplot

reset

set yrange [0.0:1.0]

set title "Solution space under EDF"

set terminal png
set output "csf.png"

unset key

plot "tmp.data" with lines
