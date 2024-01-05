# `mywc`

Custom implementation of the `wc` utility. For more info, check `man wc`.

This version supports the following flags:

```
$ ./mywc -h
usage: mywc [-h] [-c] [-w] [-l] [-m] [filename]

print line, word, byte and chars count for data source

positional arguments:
  filename

options:
  -h, --help   show this help message and exit
  -c, --bytes  print the bytes count
  -w, --words  print the words count
  -l, --lines  print the lines count
  -m, --chars  print the chars count
```

Example:

```sh
$ wc -c data.txt
342190 data.txt
$ ./mywc -c data.txt
Bytes: 342190	File: data.txt

$ wc -clwm data.txt
  7145  58164 339292 342190 data.txt
$ ./mywc -clwm data.txt
Lines: 7145  Words: 58164  Characters: 339292  Bytes: 342190	File: data.txt
```

When no flag specified, the output combines `-c`, `-l` and `-w`.

`mywc` also supports input from stdin:

```sh
$ cat data.txt | wc
   7145   58164  342190
$ cat data.txt | ./mywc
Lines: 7145  Words: 58164  Bytes: 342190
```
