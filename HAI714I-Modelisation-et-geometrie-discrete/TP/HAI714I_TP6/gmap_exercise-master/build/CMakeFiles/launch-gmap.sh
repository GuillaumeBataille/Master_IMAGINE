#!/bin/sh
bindir=$(pwd)
cd /home/guira/Bureau/master1_s1/HAI714I-Modelisation-et-geometrie-discrete/TP/HAI714I_TP6/gmap_exercise-master/src/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/guira/Bureau/master1_s1/HAI714I-Modelisation-et-geometrie-discrete/TP/HAI714I_TP6/gmap_exercise-master/build/gmap 
	else
		"/home/guira/Bureau/master1_s1/HAI714I-Modelisation-et-geometrie-discrete/TP/HAI714I_TP6/gmap_exercise-master/build/gmap"  
	fi
else
	"/home/guira/Bureau/master1_s1/HAI714I-Modelisation-et-geometrie-discrete/TP/HAI714I_TP6/gmap_exercise-master/build/gmap"  
fi
