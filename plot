set terminal jpeg size 2000,1000
set y2tics
set output 'fitness.jpg'
plot 'data' every :::::0 using 1:2 with lines title 'fitness',\
	'ils' every :::1 using 1:2 with lines title 'ILS',\
	'data' every :::::0 using 1:3 with lines title 'size' axes x1y2

set output 'nbEval.jpg'
plot 'data' every :::::0 using 1:2 with lines title 'fitness',\
	'ils'every :::1 using 1:2 with lines title 'ILS',\
	'data' every :::::0 using 1:4 with lines title 'nbEval' axes x1y2

set output 'ils.jpg'
plot 'ils' every :::::0 using 1:2 with lines title 'best',\
	'ils' every :::::0 using 1:3 with lines title 'nbeval' axes x1y2

set output 'correlation.jpg'
plot 'data' every :::1::1 using 1:2 with points title 'rand',\
	'data' every :::2::2 using 1:2 with points title 'f obj',\
	'data' every :::3::3 using 1:2 with points title 'ind',\
	'data' every :::4::4 using 1:2 with points title 'ils'
