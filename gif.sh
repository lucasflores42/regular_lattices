set terminal gif animate delay 10
set output 'gif_r4_g011.gif'				#compiling: gnuplot gif.sh
set key bottom
stats 'data_snap_gnuplot.txt' nooutput

set autoscale keepfix
set palette model RGB
unset colorbox
unset border
unset xtics
unset ytics
set palette defined ( 0 "red", 1 "blue",  2 "green")
set cbrange[0:2]
set size square
set xrange [0:100]
set yrange [0:100]

do for [i=0:99999] {
	
    plot 'data_snap_gnuplot.txt'  index (i) matrix with image;

}	



  


