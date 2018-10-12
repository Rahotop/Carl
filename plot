set terminal jpeg size 2000, 1000
set y2tics
set output 'fitness.jpg'
plot 'data' every :::::0 using 1:2 with lines title 'fitness' ,\
	'data' every :::::0 using 1:3 with lines title 'size' axes x1y2

set output 'nbEval.jpg'
plot 'data' every :::::0 using 1:2 with lines title 'fitness' ,\
	'data' every :::::0 using 1:4 with lines title 'nbEval' axes x1y2

set output 'propsat.jpg'
plot 'data' every :::::0 using 1:2 with lines title 'fitness' ,\
	'data' every :::::0 using 1:6 with lines title 'propsat' axes x1y2

set output 'correlation.jpg'
plot 'data' every :::1 using 1:2 with points title 'rand'
