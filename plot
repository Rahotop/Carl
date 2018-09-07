set terminal jpeg size 2000,1000
set y2tics
set output 'fitness.jpg'
plot 'data' using 1:2 with lines title 'fitness',\
	'ils' every :::1 using 1:2 with lines title 'ILS',\
	'data' using 1:3 with lines title 'size' axes x1y2

set output 'nbEval.jpg'
plot 'data' using 1:2 with lines title 'fitness',\
	'ils'every :::1 using 1:2 with lines title 'ILS',\
	'data' using 1:4 with lines title 'nbEval' axes x1y2

set output 'ils.jpg'
plot 'ils' every :::::0 using 1:2 with lines title 'best'
