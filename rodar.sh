#!/bin/bash
# Forte: N = 2^30 fixo | Fraca: N = t * 2^27 (maximo 6*2^27 < 2^30)
for v in critical reduction; do
  gcc -O2 -fopenmp soma_$v.c -o soma_$v
  for t in 1 2 3 4 5 6; do
    ./soma_$v 1073741824 $t | sed 's/^/FORTE  /'
    ./soma_$v $((t*134217728)) $t | sed 's/^/FRACA  /'
  done
done
