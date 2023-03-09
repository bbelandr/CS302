# This is the superballs

## About this Repository
All the code that was written for this assignment is within /src/. 
The important files are sb-read, sb-analyze, and sb-play.

### Sb-read
This code was pre-written, but it is still very useful to understand. Sb-read takes in a file 
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
    * Returns the value of the letter in points
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
Sb-play outputs an actual move that it reccommends the player to do in the next turn.

It's important to note that in order for the game to end, sb-play must make a final swap.
It's like a mandatory final (and bad) move that ends the game.

Here's what sb-play should look like:
```

```

#### Testing sb-play
In order to test sb-play on an actual game, you have to run sb-player. sb-player will provide
sb-play with a board and ask it for the next move to make. 

##### Arguments for sb-play
Sb-play takes in arguments that are slightly different from the normal sb-player. All you have to know
is that you need to pass it a program to play the game with. Here's the code that runs sb-player with sb-play:

```
./sb-player 8 10 5 pbyrg ./bin/sb-play n y -
```
The final four arguments for sb-player are these:
* the program to play the game with
* whether or not the program can be interacted with by the user
  * If this is set to true, the user and the program can take turns making moves on the game board
* whether or not the program should print the board
  * If this is set to false, the program will only print the final score and nothing else.
* a seed to make the board from

## How SuperBall works:
### The Board
Here is what the typical board looks like:

```
  0123456789
0 .r........
1 ..........
2 **g....b**
3 **....b.**
4 **.g....*Y
5 **......*P
6 .....rr...
7 ..b.......

```
If I were to index (0,1), I would get the r at the top left. Notice how the indexing works; the first value controls the row, and the second value controls the column.

DO NOT THINK OF THE ORDERED PAIR AS (X, Y). The pairs are formatted as (row, colunmn).
### A typical game of SuperBall has 5 colors, all worth different amounts of points:
  - Purple = 2
  - Blue = 3
  - Yellow = 4
  - Red = 5
  - Green = 6
  
A command to run a game of superball would use the colors pbyrg, which represents the colors 
in increasing order by points.
### The actual run command looks like this:
  - 8 10 5 pbyrg
  #### Where:
    - 8 is the number of rows
    - 10 is the number of columns
    - 5 is the minmum amount of connections to score a group
    - pbyrg is the colors in increasing order

# This is the Disjoint Sets
While Disjoint Sets already has documentation, I'm just going to summarize 
how Plank's disjoint sets work here.

## The structure of Plank's Disjoint Sets
When you create a new disjoint set, 

## How the Methods work
This is just a brief summary based off of Plank's disjoint_set.hpp file

### void Initialize(int nelements)
Initialize takes in the amount of new elements to create in a set and then clears everything else. 
To be clear, Initialize clears every single set from the data structure before adding the new elements and sets.

### int Union(int s1, int s2)
Union takes two set id's and puts them together through union by rank with path compression.
* The set id's are integers that represent different disjoint sets in the program.

Once finished, Union() returns the set id of the new union.

### int Find(int element)
Returns the set id of the set that contains the element.

Considering this arrangement of sets:
```
{4,2}{3,0,1}
```
The last value in both of these sets is considered the "leader" of the disjoint set, which holds the set ID. 

* Calling Find(3) will return 1, since 3 is part of the disjoint set that has 1 as the leader.
* Calling Find(0) will also return 1 for the same reasons as Find(3).
* Calling Find(2) will return 2, since 2 is the leader of its own set.

### Print()
Prints all the information in this form:

```
Node:    0  1  2  3  4
Links:  -1 -1 -1 -1 -1
Ranks:   1  1  1  1  1
Sizes:   1  1  1  1  1
Set IDs: {0,1,2,3,4}
Sets: {0}{1}{2}{3}{4}
```

This is an example of a disjoint set datastructure that has been initialized with 5 sets.

After calling Union(0, 1), you get this:

```
Node:    0  1  2  3  4
Links:   1 -1 -1 -1 -1
Ranks:      2  1  1  1
Sizes:      2  1  1  1
Set IDs: {4,1,2,3}
Sets: {4}{0,1}{2}{3}
```

### Print_Equiv()
Plank's description says "Print the equicvalence classes." All that you need to know is that this function prints all of the present sets in a sorted order based on the smallest element of each set.

As per the previous example where I called union on set 0 and 1, this is what it would look like if I then called the Print_Equiv() function:

```
{0,1},{2},{3},{4}
```

After calling Union(1, 3) and Union(2, 4), this is what the output would look like:

```
{0,1,3},{2,4}
```

### The const vector<int> * methods
There isn't much to say about what these functions do because they all are straightforward. Just be careful that when you index any vector that you get from these pointers, you index with vector::at(), and not the subscript operator [].

You apparently cannot index something that is a const pointer to an array with the subscript operator. Just deal with it.


## Run times
* Initialize is O(n)
* Union is O(1)
* Find is O(alpha(n)), which is basically O(1)
* Print is O(n)
* Print_Equiv is O(n * log(n))
