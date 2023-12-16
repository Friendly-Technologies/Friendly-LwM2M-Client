#! /bin/bash

cd ./coverage
awk '{gsub(/Missed/, ""); print}' index.txt | grep -E 'Filename|TOTAL' | sed 's/\x1b\[[0-9;]*m//g' > index_edit.txt
cat index_edit.txt | awk '{print $1, $2, $4, $5, $7, $8, $10, $11, $13}' | column -t
