#!/bin/bash
source /etc/bash_completion

__completions() {
    for item in ${COMPREPLY[@]} ; do
	echo $item
    done
}

line=$1
# pipe=pipe-in
# while true; do
#     if read line <$pipe; then
#         # if [[ "$line" == "" ]]; then
#         #     break
#         # fi

	COMP_WORDS=($line)
	if [ -z "${COMP_WORDS[0]}" ] ; then
	    echo $(compgen -abck )
	    # continue;
	    exit
	fi
	USE_FUNC=true;
	FUNC=$(complete -p | grep "\b${COMP_WORDS[0]}\b" | grep -o "\-F [^ ]*" | awk '{print $2}')
# FUNC=_ssh
	if [ -z "$FUNC" -o -z "${COMP_WORDS[0]}" ] ; then
	    USE_FUNC=false;
	fi
	COMP_LINE="$line"
	COMP_COUNT=$(( ${#line} + 1))
	COMP_POINT=$COMP_COUNT
	COMP_CWORD=$(( ${#COMP_WORDS[@]} - 1))
	LAST=${COMP_WORDS[ $(( $COMP_CWORD -1 ))]}
	LAST_CHAR=${COMP_LINE: $(( ${#COMP_LINE} -1 )):1}

# add another "word" if we know we're done completing the last one
	if [ "$LAST_CHAR" == " " ] ; then
	    COMP_CWORD=$(( $COMP_CWORD + 1 ))
	    LAST=""
	fi

	if [ $USE_FUNC == true ] ; then
	    $FUNC
	    __completions
	else
	    echo $(compgen -abck \"$LAST\" )
	fi

#redo everything with a - at the end to complete on switches
	COMP_LINE="${line}-"
	COMP_WORDS=($COMP_LINE)
	COMP_COUNT=$(( ${#line} + 1))
	COMP_POINT=$COMP_COUNT
	COMP_CWORD=$(( ${#COMP_WORDS[@]} - 1))
	LAST=${COMP_WORDS[ $(( $COMP_CWORD -1 ))]}
	LAST_CHAR=${COMP_LINE: $(( ${#COMP_LINE} -1 )):1}

	if [ "$LAST_CHAR" == " " ] ; then
	    COMP_CWORD=$(( $COMP_CWORD + 1 ))
	    LAST=""
	fi

	if [ $USE_FUNC == true ] ; then
	    $FUNC
	    __completions
	else
	    echo $(compgen -abck \"$LAST\" )
	fi

#     fi
#     echo "%quit%"
# done > pipe-out
