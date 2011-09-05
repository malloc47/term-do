#!/bin/bash
source /etc/bash_completion
rm comp-in pipe-in pipe-out
mkfifo comp-in pipe-in pipe-out

__completions() {
    for item in ${COMPREPLY[@]} ; do
	echo "%start%" > pipe-out
	read response < pipe-in
	echo $item > pipe-out
	read response < pipe-in
    done
}

# line=$1
# pipe=pipe-in
while true; do
#     if read line <$pipe; then
#         # if [[ "$line" == "" ]]; then
#         #     break
#         # fi

    read line < comp-in
    # echo "${line}<-here"
	COMP_WORDS=($line)
	if [ -z "${COMP_WORDS[0]}" ] ; then
	    COMPREPLY=($(compgen -abck))
	    __completions
	    echo "%start%" > pipe-out
	    read response < pipe-in
	    echo "%quit%" > pipe-out
	    continue;
	    # exit;
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
	else
	    COMPREPLY=($(compgen -abck \"$LAST\" ))
	fi
	__completions

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
	else
	    COMPREPLY=($(compgen -abck \"$LAST\" ))
	fi
	__completions

	echo "%start%" > pipe-out
	read response < pipe-in
	echo "%quit%" > pipe-out

#     fi
done
