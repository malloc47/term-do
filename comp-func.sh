#!/bin/bash
source /etc/bash_completion
complete -p | grep -o "\-F [A-Za-z0-9_ ]*$" | awk '{print $3","$2}'