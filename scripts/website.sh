#!/bin/bash
source ./scripts/config
if command -v xdg-open > /dev/null 2>&1
then
	xdg-open $URL
else
	prerequisite firefox
	nohup firefox $URL > /dev/null 2>&1 &
fi
