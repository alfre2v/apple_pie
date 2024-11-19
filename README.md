# apple_pie

I keep hearing claims of how fast Apple's new silicon CPUs are in every possible scenario.
But when it comes to scientific computations, I have serious doubts about some of the claims made.

Some benchmarks are becoming a joke and are now more a publicity stunt for companies with big bucks.

One thing programmers can do to cut through the BS is to write our own little benchmarks. 
Here I present a playful way to test floating point speed in CPUs. This is by no means an optimal, complete, 
or even good way to benchmark CPUs, but it is my way and I had fun making it.


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







