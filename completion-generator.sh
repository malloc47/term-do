#!/bin/bash
while true; do 
    for i in `compgen -abck` ; do
	echo "%start%" > pipe-out
	read response < pipe-in
	echo $i > pipe-out
	read response < pipe-in
    done
    echo "%start%" > pipe-out
    read response < pipe-in
    echo "%quit%" > pipe-out
done