# apple_pie

I keep hearing claims of how fast Apple's new silicon CPUs are in every possible scenario.
But when it comes to scientific computations, I have serious doubts about some of the claims made.

Some benchmarks are becoming a joke and are now more a publicity stunt for companies with big bucks.

One thing programmers can do to cut through the BS is to write our own little benchmarks. 

Here I present a playful way to test floating point speed in CPUs. This is by no means an optimal, complete, 
or even good way to benchmark CPUs, but it is my way and I had fun making it.


## How to run

The tests can be run with the python script: `bake_pie.py`.

This python script will compile and execute the C program: `c/eat_pi/eat_pi.c`,
which is run to stress the CPU with floating point operations, in this case by 
computing the value of PI (the mathematical constant) in a ridiculously slow and inefficient way, based on 
the ancient method of computing the series: 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 .... 
Although, a fast way to compute 800 digits of PI is included for fun, and computed when you run `eat_pi.bin --help`.
Remember, the point is to stress the CPU with floating point operations.

To get help on how to run the command use `--help`:

```bash
$ python3 bake_pie.py --help
usage: bake_pie.py [-h] [-n N] [-p P]

Run stress tests on floating point operations.

options:
  -h, --help  show this help message and exit
  -n N        number of repeated stress tests. Default: 10.
  -p P        number of parallel stress tests. Default: 10.
```


## Basic single core execution

In order to stress only one core of the CPU pass the argument `-p 1` (p: number of parallel stress tests. Default: 10).

We also pass argument `-n 30` (n: number of repeated stress tests. Default: 10). With -n you control how many times 
the C program will repeat the PI calculation. A higher n number keeps the CPU busy for longer.

```bash
$ python3 bake_pie.py -n 30 -p 1
```

## Multiple core execution

The argument `-p` controls how many stress tests to run in parallel. 
If you don't specify a value for -p it defaults to 10 parallel executions.

In this example we run 20 parallel stress tests to saturate all CPU cores:

```bash
$ python3 bake_pie.py -n 30 -p 20
```

## Reports

To be defined... 

But the idea is to generate a yaml file with information about each stress tests ran, so that 
other folks can contribute their test results via git fork / pull request.

I have the vague idea to also generate a bars plot in matblotlib at the end of each execution comparing your 
results to older results, separated by CPU architecture / type ?

# Old stuff

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







