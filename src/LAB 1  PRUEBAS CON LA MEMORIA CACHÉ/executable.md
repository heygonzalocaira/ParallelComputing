# Executable

> g++ -o test -std=c++11 1.cpp

Classic Multiplication

> g++ -std=c++11 -o test .\2.cpp 
- Valgrind
    > valgrind --leak-check=full --log-file="logfile.out" -v .\2.cpp
    >valgrind --tool=callgrind .\2.cpp

Block Multiplication
> g++ -std=c++11 -o test .\3.cpp 
- Valgrind
    > valgrind --leak-check=full --log-file="logfile.out" -v .\3.cpp
    >valgrind --tool=callgrind .\3.cpp
