#!/bin/sh

echo -n 'set terminal svg size 1000,600
set output "time-wasted.svg"
set ylabel "Hours wasted watching anime"
set key outside right
set xdata time
set timefmt "%Y-%m-%d"
set format x "%y-%m-%d"
set xtics rotate by 270
set bmargin 5
plot ' > plot.gp

username=`./time-wasted.py < mylist.data`

echo -n "\"time-wasted-${username}.data\" " >> plot.gp
echo -n "using 1:2 with lines title \"${username}\"" >> plot.gp

gnuplot plot.gp

rm -f time-wasted-${username}.data plot.gp
