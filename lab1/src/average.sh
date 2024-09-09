#!/bin/bash
s=0
count=0

# Проверяем, что данные переданы через стандартный ввод
if [ -t 0 ]; then
    for param in "$@"
    do
      s=$(( s + param ))
      count=$(( count + 1 ))
    done
else
    while read -r number; do
        s=$((s + number))
        count=$((count + 1))
    done
fi

echo "Length: $count"
if [ $count -eq 0 ]
then
echo "Average cannot be calculated"
else
echo "Average: $s $count" | awk '{printf "%.3f \n", $2/$3}'
fi
