#!/bin/bash

while getopts "b:f:" flag
do
    case ${flag} in
        b) border=${OPTARG};;
        f) folder=${OPTARG};;
        \?) echo "invalid argument"
    esac
done

echo "folder is $folder"
echo "boarder is $border"