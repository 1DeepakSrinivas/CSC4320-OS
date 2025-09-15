#!/bin/bash

echo "Current date and time:"
date

echo "Current working directory:"
pwd

echo "Files in current directory:"
ls -l

# Check if filename argument is provided
if [ $# -eq 0 ]; then
    echo "No filename provided."
else
    filename=$1
    if [ -f "$filename" ]; then
        echo "Number of lines in $filename:"
        wc -l < "$filename"
    else
        echo "File '$filename' does not exist."
    fi
fi

