#!/bin/bash

# Проверка существования файла: неверно указан путь или название файла
file_path=$1

if [ ! -e "$file_path" ]; then
    echo "неверно указан путь или название файла"
    exit 1
fi

count_str=$(grep .txt $file_path -c)
uniq_file=$(cat $file_path | grep -Eo '\b\w+\.\w+' | sort | uniq | wc -l)
hash_ch=$(($(awk -F ' - ' '{print $4}' $file_path | uniq | wc -l)-$uniq_file))
echo $count_str $uniq_file $hash_ch 
exit 1
