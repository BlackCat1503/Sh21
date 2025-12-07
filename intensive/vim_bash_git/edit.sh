#!/bin/bash

# Проверка наличия аргументов
if [ $# -ne 3 ]; then
    echo "Использование: $0 <путь_до_файла> <строка_для_замены> <замена>"
    exit 1
fi

file_path=$1
search_string=$2
replace_string=$3

# Проверка существования файла: неверно указан путь или название файла
if [ ! -e "$file_path" ]; then
    echo "неверно указан путь или название файла"
    exit 1
fi

if grep -q "$search_string" $file_path;
then
    sed -i '' "s/$search_string/$replace_string/g" "$file_path"
    date_time="$(date +"%Y-%m-%d %H:%M")"
    sha_sum=$( shasum -a 256 "$file_path" | awk '{print $1}')
    file_size=$(ls -l "$file_path" | awk '{print $5}')
    echo "$file_path - $file_size - $date_time - $sha_sum - sha256" >> files.log
    echo "done!"
else 
    echo "строка не найдена"

exit 0
fi