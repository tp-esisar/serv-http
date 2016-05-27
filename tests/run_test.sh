#!/bin/sh


for f in test_uri*; do 
	clear
	echo "----------- Processing $f ----------"
	cat $f
	cat $f | nc localhost 8080
	read message
done

