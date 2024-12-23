# Running the program

The program is written in C++ to compile execute:

```c++
./generate.sh
./build.sh
```

Then to execute just execute

```c++
./build/release/ra
```

The program expects the following arguments: 


```bash
Usage:
  ./build/release/ra (Loader) (Tester)*
Loader:
  --generate <N> <alpha> <n>  Generate a Zipfian sequence of size N with alpha and range n.
  --loaddat filename.dat      Loads a dataset from disk.
  --loadtxt filename.txt      Loads a dataset from disk.
  --help                      Show this help message.
Tester:
  --test                      Test zipfian property (assming alpha = 1.0 for non generated data).
  --dump                      Writes values and frequency.
  --estimate <I>              Estimate cardinality with I number of iterations.
  --writetxt                  Writes the values generated/loaded as in a *.txt format
```

## Executing the tests

For executing the tests just run the scripts

```bash
run.sh
run-synth.sh
```

Which generate a table for each data set of the sampled result of 100 iterations with increased number of K or records for both methods REC and HLL and run some experiments on synthetic data for alpha n and N.
