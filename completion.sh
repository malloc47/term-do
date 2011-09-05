#!/bin/bash
source /etc/bash_completion

__completions() {
    for item in ${COMPREPLY[@]} ; do
	echo $item
    done
}

line=$1
COMP_WORDS=($line)
if [ -z "${COMP_WORDS[0]}" ] ; then
    echo $(compgen -abc )
    exit
fi
USE_FUNC=true;
# FUNC=$(complete -p | grep "\b${COMP_WORDS[0]}\b" | grep -o "\-F [^ ]*" | awk '{print $2}')
FUNC=$2
if [ -z "$FUNC" -o -z "${COMP_WORDS[0]}" ] ; then
    USE_FUNC=false;
fi
COMP_LINE="$line "
COMP_COUNT=$(( ${#line}))
COMP_POINT=$COMP_COUNT
COMP_CWORD=${#COMP_WORDS[@]}

if [ $USE_FUNC == true ] ; then
    $FUNC
    __completions
else
    if [ "${#COMP_WORDS}" == "1"  ] ; then
	LAST=${COMP_WORDS[ $(( $COMP_CWORD -1 ))]}
	echo $(compgen -abc "$LAST" )
    fi
fi

#redo for switches
COMP_WORDS=(${line} -)
COMP_LINE="${line} -"
COMP_COUNT=$(( ${#line} + 3))
COMP_POINT=$COMP_COUNT
COMP_CWORD=$(( ${#COMP_WORDS[@]} -1))

if [ $USE_FUNC == true ] ; then
    $FUNC
    __completions
fi