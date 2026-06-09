#!/bin/bash
#shebang - executable

mkdir -p $folder 

for i in $(seq 1 1 $boarder)
do
    echo "processing $i"
    echo $i > ${folder}/file${i}.in
done