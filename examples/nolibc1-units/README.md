# No libc example 1 with unit tests

Comparing to [nolibc1](../nolibc1/), it's introduced `cmake` to build the sources.

## Compile and run
```shell
$ cmake -S . -B build

$ cmake --build build   

$ cmake --build build -t test
Running tests...
Test project /home/flores/pets/my_github/lowlevel/examples/nolibc1-units/build
    Start 1: Calculations are completed.spec.cpp
1/1 Test #1: Calculations are completed.spec.cpp ...   Passed    0.01 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec


$ cmake --build build -t run
[100%] Built target nolibc1-units
hello
[100%] Built target run
```

