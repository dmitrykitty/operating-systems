#!/bin/bash

i=1; #when we should write semicolon or colon? 
j=$#;

while [ $i -le $j ] #or ((i <= j))
do
    echo "Param $i: $1";
    i=$((i + 1));
    shift 1; 
done 
