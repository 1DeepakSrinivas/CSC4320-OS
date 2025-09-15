#!/bin/bash

# Checking if two arguments are given
if [ $# -lt 2 ]; then
    echo "Usage: $0 <source_dir> <dest_dir>"
    exit 1
fi

src=$1
dest=$2

timestamp=$(date +%Y%m%d_%H%M%S)

# Copying all .txt files with timestamp
for f in $src/*.txt
do
    cp "$f" "$dest/$(basename $f .txt)_$timestamp.txt"
done

echo "Successfully finished backing up files from $src to $dest"

