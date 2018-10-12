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
plot 'data' every :::3::3 using 1:2 with points title 'rand' ,\
	'data' every :::4::4 using 1:2 with points title 'f obj' ,\
	'data' every :::5::5 using 1:2 with points title 'ind' ,\
	'data' every :::6::6 using 1:2 with points title 'ils'
