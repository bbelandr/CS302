# This is the superballs

## About this Repository
All the code that was written for this assignment is within /src/. 

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
  Sb-read takes in information and stores the minimum score size (mss), the number of empty
  cells in the board (empty), a board 

### Sb-analyze
Sb-analyze is made with sb-read as a base.

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

