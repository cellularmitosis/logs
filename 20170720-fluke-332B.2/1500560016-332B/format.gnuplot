set key autotitle columnhead
set datafile separator ','
set terminal png size 1000,500
set xlabel 'Time'
set yrange [9999000:9999100]
set ytics ( \
    "9,999,000" 9999000, \
    "9,999,010" 9999010, \
    "9,999,020" 9999020, \
    "9,999,030" 9999030, \
    "9,999,040" 9999040, \
    "9,999,050" 9999050, \
    "9,999,060" 9999060, \
    "9,999,070" 9999070, \
    "9,999,080" 9999080, \
    "9,999,090" 9999090, \
    "9,999,100" 9999100, \
)
set format y "%.0f"
set format y "%'.0g"
plot 'data.csv' with lines
