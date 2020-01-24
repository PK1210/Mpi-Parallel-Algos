# Mpi-Parallel-Algos

*Coded by:*
**Prajwal Krishna**


About The Codes
-------------

1. This file contains 2 codes.
2. Paralized version of quick sort and paralized version of Bellmann Ford Algorithm.

----------

## Running the program

- First compile
	- `mpic++ 20171086_1.cpp -o 20171086_1`
- Running the program is easy
	- `mpirun -np <number of cores> 20171086_1 <input_file> <output_file>`

## Performance
#### For question 1
|Sequential|Paralized|
|---|---|
|0.73s| 0.255683s|
|0.230799s|0.115000s|
|0.84184s|0.202924s|
|0.343312s|0.218307s|

#### For question 2
|Sequential|Paralized|
|---|---|
|0.177132s|0.134284s|
|0.080154s|0.055699s|
|0.063927s|0.058860s|
_______________

#### Prajwal Krishna Maitin
