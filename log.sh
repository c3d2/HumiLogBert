LOGDIR=/home/kellerlog

mkdir -p $LOGDIR

cu -l /dev/cuaU0 -s 9600 | \
	awk '{cmd="(date +%s)"; cmd | getline d; print d, $0; close(cmd)}' \
	>> "$LOGDIR/$(date +%Y-%m-%d)"

