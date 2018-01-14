#!/bin/bash

set -e -o pipefail
set -x

dir=${!#}
plot_args=${@:1:$#-1}

./split-env-csv.sh "${dir}"

nice ./plot.py -y 2 "${dir}"/temp_c.csv
nice ./plot.py -y 2 "${dir}"/humidity.csv

for i in 1 10 100 1000
do
    nice ./plot.py -a "${i}" $plot_args "${dir}"/data.csv
    mv "${dir}"/data.png "${dir}/data-a${i}.png"
done

