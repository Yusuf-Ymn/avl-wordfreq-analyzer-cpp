# Word-Frequency Counter (C++)

A compact program that parses a text file, stores words in an **AVL tree**, and prints the top *K* most frequent words sorted by frequency (descending) and then alphabetically.  
The project consists of just three source files—`main.cpp`, `Solution.cpp`, `WordFreq.h`—plus a sample input file `1.txt`.

## Features
- **Balanced searches/inserts** via an AVL tree  
- Cleans non-alphabetic characters and normalises to lowercase  
- Ignores words shorter than 3 characters  
- Simple, tabular command-line output

### Example output (using `1.txt`)

```text
+----------------------+---------+
|          WORD        |   FREQ  |
+----------------------+---------+
|                 text |        4|
|                 and  |        3|
|                 file |        3|
|                 bold |        2|
|                 that |        2|
|              contains|        1|
|                 etc  |        1|
+----------------------+---------+
```

## File layout

├── main.cpp       # CLI front-end + table printing

├── Solution.cpp   # AVL tree + frequency logic

├── WordFreq.h     # Simple struct definition

├── 1.txt          # Sample text file

└── README.md      # (this file)
