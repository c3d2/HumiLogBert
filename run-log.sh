#!/bin/sh

cd $(dirname $0)

(while true; do
	sh log.sh
	sleep 1
done) &
