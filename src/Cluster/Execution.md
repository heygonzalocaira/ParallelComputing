# Execution

```
 $ mpicc cluester.c -o test
```

```
$ mpirun.mpich2 -np 10 ./test

master: hello world from process 9 of 10
master: hello world from process 7 of 10
master: hello world from process 8 of 10
master: hello world from process 6 of 10
master: hello world from process 2 of 10
master: hello world from process 3 of 10
master: hello world from process 4 of 10
master: hello world from process 1 of 10
master: hello world from process 5 of 10
master: hello world from process 0 of 10
```

```
mpirun.mpich2 -np 10 -f hosts ./test 
```

# Windows


```
mpiexec mpitest.exe

mpiexec -np 10 mpitest.exe
```

- [Tutorial link](http://www.science.smith.edu/dftwiki/index.php/Tutorial:_Create_an_MPI_Cluster_on_the_Amazon_Elastic_Cloud_(EC2))