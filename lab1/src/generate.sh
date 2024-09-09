#!/bin/bash

output_file="numbers.txt"

count=150

> "$output_file"

for ((i=0; i<count; i++))
do
    # Генерируем случайное число из 4 байт (32 бита)
    random_number=$(od -An -N4 -tu4 < /dev/random)
    
    echo -e "$random_number" >> "$output_file"
done
