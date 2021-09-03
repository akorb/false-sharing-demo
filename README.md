Demonstrates [False sharing](https://en.wikipedia.org/wiki/False_sharing).

The `Makefile` creates two programs. `false-sharing-padded` and `false-sharing-unpadded`. The unpadded version should cause a lot of false sharing and therefore, be slower than the padded version which avoids false sharing.

Commands to see nice performance statistics:

```bash
make
perf stat -e instructions,cache-misses ./false-sharing-padded
perf stat -e instructions,cache-misses ./false-sharing-unpadded
```

On my machine (AMD Ryzen 5 5600X), I get the following output:

Padded version:
```            
    12.885.371.519      instructions:u            #    5,99  insn per cycle         
             8.874      cache-misses:u                                              

       0,233519166 seconds time elapsed
```

Unpadded version:
```
    12.887.015.757      instructions:u            #    0,08  insn per cycle         
     1.029.208.011      cache-misses:u                                              

      18,396965116 seconds time elapsed
```

Note how the count of instructions is similar. `perf` also derives the instructions per cycle, which is significantly higher for the padded version.
The count of `cache-misses` is way higher for the unpadded version, caused by false sharing and resulting in the lower instructions per cycle.
