#! /bin/bash

cd ./coverage
awk '{gsub(/Missed/, ""); print}' index.txt > index_edit.txt
cat index_edit.txt | awk '{print $1, $2, $4, $5, $7, $8, $10}' | grep -E 'Filename|TOTAL' | column -t
