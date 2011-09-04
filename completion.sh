#!/bin/bash
source /etc/bash_completion
__print_completions() {
	for ((i=0;i<${#COMPREPLY[*]};i++));  do
		echo ${COMPREPLY[i]}
	done
}
COMP_WORDS=($1)
COMP_LINE="$1"
COMP_COUNT=$(( ${#1} + 1))
COMP_POINT=$COMP_COUNT
COMP_CWORD=1
#_${COMP_WORDS[0]}
#_git
$2
__print_completions

