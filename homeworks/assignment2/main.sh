#!/bin/bash

if [ ! -f "processes.txt" ]; then
    echo "Error: processes.txt not found!"
    exit 1
fi

gcc -o round_robin round_robin.c
if [ $? -ne 0 ]; then
    echo "Round Robin compilation failed"
    exit 1
fi

gcc -o mlfq mlfq.c
if [ $? -ne 0 ]; then
    echo "MLFQ compilation failed"
    exit 1
fi

./round_robin
echo
./mlfq
echo

echo "Comparison:"
rr_wait=$(./round_robin 2>/dev/null | grep "Waiting Time:" | awk '{print $4}')
rr_turn=$(./round_robin 2>/dev/null | grep "Turnaround Time:" | awk '{print $4}')
mlfq_wait=$(./mlfq 2>/dev/null | grep "Waiting Time:" | awk '{print $4}')
mlfq_turn=$(./mlfq 2>/dev/null | grep "Turnaround Time:" | awk '{print $4}')

echo "Round Robin: Wait time=$rr_wait, Turnaround time=$rr_turn"
echo "MLFQ: Wait time=$mlfq_wait, Turnaround time=$mlfq_turn"

if [ -f "plots.py" ]; then
    python3 plots.py
fi
