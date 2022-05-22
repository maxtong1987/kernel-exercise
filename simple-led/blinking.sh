#!/bin/bash
while true
do
	echo 1 > /dev/led
	sleep 1
	echo 0 > /dev/led
done
