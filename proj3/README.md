# This is the superballs

## About this Repository
All the code that was written for this assignment is within /src/. 
The important files are sb-read, sb-analyze, and sb-play.

### Sb-read
This code was written for me, but it is still very useful. Sb-read takes in a file 
that looks like this:

```
...yyryy.p
y.rg.yppyp
**gg.yrpPP
GGgbgybp**
R*bg.yrp*P
G*gygyypY*
yyybpby.pb
.pgg.yp.bb
```

and then prints out analytics on the board:

```
Empty cells:                    20
Non-Empty cells:                60
Number of pieces in goal cells:  8
Sum of their values:            33
```
#### Variables to Know
  Sb-read takes in information and stores these things into the Superball class:
  * the minimum score size (mss)
  * the number of empty cells in the board (empty)
  * the superball board stored in a 1 dimentional vector (board)
    * In order to access an element in location (i, j), you must index 
```
board[i * c + j]
```
  * the locations of all of the goal cells (goal)
    * These are indexed in the same way that board is
  * the values of all the colors (colors)
    * Indexed with the ascii value of the letter
    * The value is the value of the letter in points
```
In a standard pbyrg format,
colors['p'] == 2
```
### Sb-analyze
Sb-analyze is made with sb-read as a base.

While sb-read would print out the general facts of the input, sb-analyze would print out the 
sets that could be scored on the board. The sets that could be scored are only represented once
when printed, but the actual board could have multiple scoring tiles that would be able to also
be used.

#### Example:
Given this input from standard in:
```
yyggyryybp
ggrgpyppyp
RBgggyrpPP
GGgggybpPP
RGygryrpBP
YGyygyypYB
yyybpbyppb
ppggyypbbb

```

Here is its output:
```
Scoring sets:
  Size: 14  Char: g  Scoring Cell: 5,1
  Size: 15  Char: p  Scoring Cell: 4,9
  Size:  7  Char: y  Scoring Cell: 5,0
  Size:  5  Char: b  Scoring Cell: 5,9
```

Note how g had multiple scoring tiles but was only referenced once. The tile that is referenced
is the head of the disjoint set.

### Sb-play

## How SuperBall works:
### A typical game of SuperBall has 5 colors, all worth different amounts of points:
  - Purple = 2
  - Blue = 3
  - Yellow = 4
  - Red = 5
  - Green = 6
  
A command to run a game of superball would use the colors pbyrg, which represents the colors in increasing order by points.
### The actual run command looks like this:
  - 8 10 5 pbyrg
  #### Where:
    - 8 is the number of rows
    - 10 is the number of columns
    - 5 is the minmum amount of connections to score a group
    - pbyrg is the colors in increasing order

