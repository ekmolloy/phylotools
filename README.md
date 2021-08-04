
So far this github repository only contains a tool used in [Springer et al. 2020](https://doi.org/10.1093/jhered/esz076) to convert a matrix of 0/1 characters (in PHYLIP or FASTA format) into a file of newick strings (one per line).

To compile and run the code, use the following commands:
```
cd phylotools/src
make
./run_binary_character_matrix_to_newick [input file] > [output file]
```

If nothing is written to the output file, there may be having issues with the newline character used in your input file, as discussed [here](https://leemendelowitz.github.io/blog/remove-carriage-return-control-character.html). 
To summarize, the input file must use `\n` as the newline character.
If it was created on a Windows system, then `\r\n` may be used as the newline character.
In this case, you can remove the `\r` characters from the input file with the following command:
```
tr -d '\r' < [input file] > [output file]
```
Similarly, if the file was created on an older Mac, the `\r` may be used as the newline character. In this case, you can replace all of the `\r` characters with the `\n` character using the following command:
```
tr '\r' '\n' < [input file] > [output file]
```

