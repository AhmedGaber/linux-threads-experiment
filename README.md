Linux-threads Experiment
===================
A simple Matrix multiplication C program to test how threads work in Linux using the Pthread library. Read more about Pthread [here](https://computing.llnl.gov/tutorials/pthreads/).

###Compiling the code
First, you need to clone the code into your local (or VM) machine as follows: 
```git clone https://github.com/AhmedGaber/linux-threads-experiment.git```

Then compile the code:
```
gcc ­c main.c ­o main.o 
gcc ­c parser.c ­o parser.o
gcc -­pthread parser.o main.o ­o matMultp
```

Then, run the program:
```
./matMultp PATH a.txt b.txt c.txt
```
where:
`PATH`: is the files path
`a.txt`: is the file containing the first matrix
`b.txt`: is the file containing the second matrix
`c.txt`: is the output file to write the result in.
