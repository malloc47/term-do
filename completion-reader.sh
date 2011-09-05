#!/bin/bash
while true; do
    read go < pipe-out

    if [ "$go" != "%start%" ] ; then
	continue;
    fi

    echo "read" > pipe-in

    if read line <pipe-out ; then
	if [ "$line" == "%quit%" ] ; then
	    exit;
	fi
	echo $line
    fi
    
    echo "next" > pipe-in
done
