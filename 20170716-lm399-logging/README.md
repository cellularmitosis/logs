Example plotting:

```
nice ./plot.py -s 800 -y 0 -a 100 1500270000-LM399-P7/data.csv
```

continuous plotting:

```
watch -n10 'nice ./plot.py -s 20 -y 0 1500351740-LM399_P6/data.csv'
```

Example of starting logs:

```
./start-logs.sh LM399_P5
```

Example of stopping logs:

```
./stop-logs.sh 1500270000/pids
```

