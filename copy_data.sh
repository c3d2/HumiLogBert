#!/bin/sh

set -ex

sudo mount /dev/sdb1 /mnt/usb

grep T: /mnt/usb/kellerlog/* > testlog

# Only last logs
# grep T: /mnt/usb/kellerlog/* | tail -n 30000 > testlog

sudo umount /mnt/usb


gnuplot gnuplot.script

