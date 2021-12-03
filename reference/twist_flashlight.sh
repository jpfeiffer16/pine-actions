#!/bin/bash

LAST_Z=1

while [ true ]; do
	X_ANGULAR_V=$(cat /sys/bus/iio/devices/iio\:device2/in_anglvel_x_raw)
	Y_ANGULAR_V=$(cat /sys/bus/iio/devices/iio\:device2/in_anglvel_y_raw)
	Z_ANGULAR_V=$(cat /sys/bus/iio/devices/iio\:device2/in_anglvel_z_raw)
#			  /sys/bus/iio/devices/iio\:device3/in_anglvel_x_raw
#	echo "$X_ANGULAR_V, $Y_ANGULAR_V, $Z_ANGULAR_V"
	if [ $LAST_Z -lt 0 ]; then
		if [ $Z_ANGULAR_V -gt 6000 ]; then
			ON=$(cat /sys/class/leds/white\:flash/brightness)
			if [ $ON -eq "1" ]; then
				echo 0 > /sys/class/leds/white\:flash/brightness
			else
				#echo 0 > /sys/class/leds/white\:flash/brightness
				echo 1 > /sys/class/leds/white\:flash/brightness
			fi
		fi
	fi
	LAST_Z=$Z_ANGULAR_V
	sleep .2s
done
