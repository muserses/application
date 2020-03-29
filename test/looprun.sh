#! /bin/sh
#
# looprun.sh
# Copyright (C) 2017 muserses <muserses@vmachine>
#
# Distributed under terms of the MIT license.
#

for i in $(seq 1 3)
do
	echo "loop $i"
	qemu-arm -L /usr/arm-linux-gnueabihf test&
	ps | grep qemu-arm
	sleep 10
	PID=$!
	echo "PID = $PID"
	kill -INT $PID
done

