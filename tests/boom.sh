#!/bin/sh

i=0
while [ $i -ne 1000000 ] ; do 
	
	for f in test*; do 
		cat $f | nc localhost 8080 >> /dev/null &
		sleep 1
	done
	i=$(($i + 1))
	echo $i

done

