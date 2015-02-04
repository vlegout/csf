#! /usr/bin/gnuplot

reset

set xrange [0.0:1.0]
set yrange [0.0:1.0]

set title "Utilization bound under RM"

set terminal png
set output "u_rm.png"

set key left top

plot for [col=2:5] "tmp.data" using 1:col with lines title columnheader
