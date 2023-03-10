// Benjamin Belandres, sb-analyze.cpp


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
    const Disjoint_Set ConstructDisjointsets(int mRow = 0, int mCol = 0);     // Custom function that returns a Disjoint Set structure which combines all similar colors together.
                                                                              // Since ConstructDisjointsets has default values, you don't need to input parameters when you call ConstructDisjointsets() 
    void FindScoringCells();       // Calls ConstructDisjointsets and also populates the scoringCells
    int r;                // The amount of rows
    int c;                // The amount of columns
    int mss;              // The minimum score size
    int empty;            // The number of empty cells on the board
    vector <int> board;   // The superball board stored in a 1 dimensional vector (indexed with board[i * c + j])
    vector <int> goals;   // The locations of all of the goal cells (indexed like board). Despite the name, it has bools, not ints, that say whether or not a cell is a goal
    vector <int> colors;  // The values of all of the colors (indexed with the letter char)
    vector <int> scoringCells;  // Location index of all the cells that can be used to score a set
    vector<pair<bool, bool>> takenPaths;  // When the program takes a pathway in constructdisjointsets, either right or down, the program marks this so that the path is never taken again
                                          // The first element of the pair is for right and the second element is for down.
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

  d.Initialize(r*c);      // Representing the whole board as a disjoint set
  takenPaths.resize(r*c); // Also representing the whole board
  
}

const Disjoint_Set Superball::ConstructDisjointsets(int mRow, int mCol) 
{
  int currentSpace = mRow * c + mCol;
  int nextSpace;
  char currentColor = board[currentSpace];
  char nextColor;

  if (mCol < c - 1 && takenPaths[currentSpace].first == false) { // Moving right on the board when not on the edge or the path hasn't been taken yet
    takenPaths[currentSpace].first = true;  // Making sure that we never take this path again

    nextSpace = mRow * c + (mCol + 1);
    nextColor = board[nextSpace];

    if (currentColor != '.' && currentColor != '*' && currentColor == nextColor) {

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
    ConstructDisjointsets(mRow, mCol + 1);
  }

  if (mRow < r - 1 && takenPaths[currentSpace].second == false) { // Moving down on the board
    takenPaths[currentSpace].second = true;  // Making sure that we never take this path again
    
    
    nextSpace = (mRow + 1) * c + mCol;
    nextColor = board[nextSpace];

    if (currentColor != '.' && currentColor != '*' && currentColor == nextColor) {
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
    ConstructDisjointsets(mRow + 1, mCol);
  }
  return d;
}

void Superball::FindScoringCells() {
  const vector<int> *sizes = d.Get_Sizes();
  const vector<int> *setIds = d.Get_Set_Ids();

  int row;
  int col;
  int currentCell;

  // Looking for a set that is big enough to be scored through the goal
  for (size_t i = 0; i < setIds->size(); i++) {
    currentCell = setIds->at(i);
    row = currentCell / c;
    col = currentCell % c;

    if (DEBUG) cout << "Searching (" << row << ", " << col << ") == " << sizes->at(currentCell) << " and comparing with " << mss << ". . . \n";
    if (board[currentCell] != '.' && board[currentCell] != '*' && sizes->at(currentCell) >= mss) {  // This means that the given set is able to be scored if one of the elements is in the goal
      if (DEBUG) cout << "  A scoring set was found at (" << row << ", " << col << ")\n";
      
      // Looking for a goal element
      if (DEBUG) cout << "    Looking for goal elements. . .\n";
      list<int> elements = d.Get_Elements()->at(currentCell);
      for (list<int>::iterator it = elements.begin(); it != elements.end(); it++) {
        if (DEBUG) cout << "      Searching " << *it << ".\n";
        if (goals[*it]) { // We have found an element that is at a goal cell
          if (DEBUG) cout << "      Found a goal cell. Pushing back " << *it << ".\n";
          scoringCells.push_back(*it);
          break;
        }
        // elements
      }
    }
  }


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

  
  s->ConstructDisjointsets();
  s->takenPaths.clear();  // Just to make sure that if I want to use ConstructDisjointsets() again, takenPaths is clean.
  s->FindScoringCells();

  // Calculating the ouput
  int currentSet;
  
  cout << "Scoring sets: " << endl;
  for (size_t i = 0; i < s->scoringCells.size(); i++) {
    // DEBUG cout << "scoringCell == " << s->scoringCells[i] << " ";
    
    currentSet = s->d.Find(s->scoringCells[i]);
    printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n", s->d.Get_Sizes()->at(currentSet), s->board[currentSet], s->scoringCells[i] / s->c, s->scoringCells[i] % s->c);

  }
  
  exit(0);
}
