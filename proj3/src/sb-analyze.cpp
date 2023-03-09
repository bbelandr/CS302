// Benjamin Belandres, sb-analyze.cpp
// REMEMBER TO MAKE D PRIVATE AGAIN


#include "disjoint_set.hpp"
#include <iostream>
using namespace std;
using plank::Disjoint_Set;

class Superball {
  private:
    bool DEBUG = false;
  public:
    Disjoint_Set d;
    Superball(int argc, char **argv); // Populates the variables below with information from the commandline
    const Disjoint_Set Analyze(int mRow = 0, int mCol = 0);     // Custom function that returns a Disjoint Set structure which combines all similar colors together.
                                                          // Since Analyze has default values, you don't need to input parameters when you call Analyze() 
    int r;                // The amount of rows
    int c;                // The amount of columns
    int mss;              // The minimum score size
    int empty;            // The number of empty cells on the board
    vector <int> board;   // The superball board stored in a 1 dimensional vector (indexed with board[i * c + j])
    vector <int> goals;   // The locations of all of the goal cells (indexed like board)
    vector <int> colors;  // The values of all of the colors (indexed with the letter char)
};
void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }

  d.Initialize(r*c);    // Representing the whole board as a disjoint set
}

const Disjoint_Set Superball::Analyze(int mRow, int mCol) 
{
  int currentSpace = mRow * c + mCol;
  int nextSpace;
  char currentColor = board[currentSpace];
  char nextColor;
  

  if (mCol < c - 1) { // Moving right on the board
    nextSpace = mRow * c + (mCol + 1);
    nextColor = board[nextSpace];

    if (currentColor != '.' && currentColor == nextColor) {

      int currentSet, nextSet;
      currentSet = d.Find(currentSpace);
      nextSet = d.Find(nextSpace);

      if (currentSet != nextSet) {  // Gotta make sure that these two are different sets, otherwise we would be unioning the same set with itself (don't do that)
        if (DEBUG) cout << "  " << currentColor << " == " << nextColor << ", so Unioning (" << mRow << ", " << mCol << ") with (" << mRow << ", " << mCol + 1 << ")\n";
        if (DEBUG) d.Print();

        d.Union(d.Find((currentSpace)), d.Find((nextSpace)));
      }
    }

    if (DEBUG) cout << "(>)Analyzing (" << mRow << ", " << mCol + 1 << "). . .\n";  // Notice how the arrow is saying which way the program is going
    Analyze(mRow, mCol + 1);
  }

  if (mRow < r - 1) { // Moving down on the board
    nextSpace = (mRow + 1) * c + mCol;
    nextColor = board[nextSpace];

    if (currentColor != '.' && currentColor == nextColor) {
      int currentSet, nextSet;
      currentSet = d.Find(currentSpace);
      nextSet = d.Find(nextSpace);

      if (currentSet != nextSet) {  // Gotta make sure that these two are different sets, otherwise we would be unioning the same set with itself (don't do that)
        if (DEBUG) cout << "  " << currentColor << " == " << nextColor << ", so Unioning (" << mRow << ", " << mCol << ") with (" << mRow << ", " << mCol + 1 << ")\n";
        if (DEBUG) d.Print();

        d.Union(d.Find((currentSpace)), d.Find((nextSpace)));
      }
    }
    
    if (DEBUG) cout << "(V)Analyzing (" << mRow + 1 << ", " << mCol << "). . .\n";
    Analyze(mRow + 1, mCol);
  }
  return d;
}


int main(int argc, char **argv)
{
  Superball *s;
  int i, j;
  int ngoal;  // The number of pieces in goal cells
  int tgoal;  // The sum of the values
 
  s = new Superball(argc, argv);

  tgoal = 0;
  ngoal = 0;
  for (i = 0; i < s->r*s->c; i++) {
    if (s->goals[i] && s->board[i] != '*') {
      tgoal += s->colors[s->board[i]];
      ngoal++;
    }
  }

  // cout << "This program doesn't do anything yet.\n";
  
  s->Analyze().Print_Equiv();

  exit(0);
}
