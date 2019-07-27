#!/bin/bash

if [ $# = 0 ] || [ $# = 1 ]
then
	echo "./run [debug/release] [build/pack/install] [channel_id if pack or install]"
	echo "channel_id is mm, 360, bdog, bdmg, yyb, wdj, xiaomi, 9you, lenovo"
else
	if [ $2 = "pack" ] || [ $2 = "install" ]
	then
		./script/$1_$2.sh $3 
	else
		./script/$1_$2.sh
	fi
fi
