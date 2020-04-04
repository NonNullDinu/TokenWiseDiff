# TokenWiseDiff
A lightweight program that compares files(was explicitly build for source files) that outputs information on how similar they look "at a first glance".

The syntax of the files should be the same.

For now, there is only one syntax, for C/C++ sources, but other ones can be written easily.
If you would like to submit the syntax for a programming language that currently doesn't have a syntax file in the [syntax folder](syntax),
feel free to open a pull request.
## Synopsis
```
twdiff -s syntax_file [-i index:path/to/input/file] [-p index1:index2] [-f]
```
## Options
- The `-s` specifies the syntax file. Find more about it 
    [here](SYNTAX_FILE.md)
- The `-i` specifies a pair of one index and an input file.
    The index should be an unsigned, not null integer,
    and the input file is the file that is going to be diffed.
- The `-p` specifies a pair of indices(first element of `-i`)
    that we should diff. Note that files that are not part of
    any pair are going to be discarded.
- The `-f` stands for fancy output

## Examples
The [examples folder](examples) contains some examples that you can test with.

```shell script
twdiff -s syntax/c_cpp.syntax.twdiff -i 1:examples/t1.cpp -i 2:examples/t2.cpp -p 1:2 -f
```

## Building
Just run this inside your clone of this repo.
```shell script
./build.sh
```
