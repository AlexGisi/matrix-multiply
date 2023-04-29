# matrix-multiply
Assessing shared-memory matrix multiplacation algorithms.

## Usage
Install the `Eigen` library. 

Change the compiler path in `build/Makefile` to the location of your cilk
compiller install.

Once compiled,

```./mm <directory with matrix examples> <matrix size> <algorithm>```

## Acknowledgment
This work was for a project in Prof. Hongyang Sun's EECS 690 class at KU, and I worked off his sample code for the block and divide-and-conquer algorithms.
