set term png size 700, 700

# load 'parula.pal'
# jet palette: 
set palette defined ( 0 "#000090", 1 "#000fff", 2 "#0090ff", 3 "#0fffee", 4 "#90ff70", 5 "#ffee00", 6 "#ff7000", 7 "#ee0000", 8 "#7f0000")
set cbrange [-0.1:0.1]
set xrange [-1:1]
set yrange [-1:1]
set zrange [-.2:.75]
set pm3d
set xyplane at -0.2

do for [i = 0:999:5] {
    set output sprintf("%03d.png", i)
    # print sprintf("%d.csv", i)
    splot sprintf("%d.csv", i) u 1:2:3 w pm3d title sprintf("step = %d", i) 
}