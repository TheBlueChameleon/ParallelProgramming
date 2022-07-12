set terminal gif animate delay 2
set output 'dummy.gif'

stats 'dummy.out' nooutput
set xrange [-1.5:1.5]
set yrange [-1.5:1.5]

do for [i=1:int(STATS_blocks)] {
    plot 'dummy.out' index (i-1) with points
}

# ============================================================================ #

reset
set terminal gif animate delay 0
set output 'coordinates.gif'
stats 'coordinates.log' nooutput
set xrange [-0.5:10.5]
set yrange [-0.5:10.5]

do for [i=1:int(STATS_blocks)] {
    plot 'coordinates.log' index (i-1) with points
}

# ============================================================================ #

reset
set terminal png
set output 'energies.png'

plot 'energies.log' with lines
