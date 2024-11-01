# apple_pie

## How to compile the pie

```sh

cd apple_pie/c/eat_pi


gcc eat_pi.c -o eat_pi.bin -O3

```


## How to start a single execution


```sh

./eat_pi.bin

```


## A crappy way to run multiple processes to saturate the CPU cores

```
for i in `seq 1 100`; do nohup ./eat_pi.bin & done
```







