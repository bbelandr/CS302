// Benjamin Belandres, sb-analyze.cpp
// REMEMBER TO MAKE D PRIVATE AGAIN


#include "disjoint_set.hpp"
#include <iostream>
#include <cmath>
using namespace std;
using plank::Disjoint_Set;

class Superball {
  private:
    bool DEBUG = true;
  public:
    Disjoint_Set d;
    Superball(int argc, char **argv); // Populates the variables below with information from the commandline
    // ~Superball();                     // Deletes d
    const Disjoint_Set ConstructDisjointsets(Disjoint_Set &mySet, vector<int> mBoard, int mRow = 0, int mCol = 0);     // Custom function that returns a Disjoint Set structure which combines all similar colors together.
                                                                              // Since ConstructDisjointsets has default values, you don't need to input parameters when you call ConstructDisjointsets() 
    void FindScoringCells();                    // Calls ConstructDisjointsets and also populates the scoringCells
    double ClosestColor(int cell, char color);  // Takes the cell from the parameter and returns the distance to the closest matching color from the color argument
    double EvaluateBoard();                     // Returns a score for a given boardstate (assumes the normal board if nothing is passed to it)
    double EvaluateBoard(vector<int> mBoard);   // Overload so that you can pass whatever board you want to it
    void printBoard(vector<int> mBoard);        // Prints the current board 
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

// Superball::~Superball() {
//   delete d;
// }

const Disjoint_Set Superball::ConstructDisjointsets(Disjoint_Set &mySet, vector<int> mBoard, int mRow, int mCol) 
{
  bool mDEBUG = true;
  int currentSpace = mRow * c + mCol;
  int nextSpace;
  char currentColor = mBoard[currentSpace];
  char nextColor;

  // if (mDEBUG) cout << "ConstructDisjointsets just recieved this board: " << endl;
  // if (mDEBUG) printBoard(mBoard);

  if (mCol < c - 1 && takenPaths[currentSpace].first == false) { // Moving right on the mBoard when not on the edge or the path hasn't been taken yet
    // if (mDEBUG) cout << "chicken\n";
    takenPaths[currentSpace].first = true;  // Making sure that we never take this path again

    nextSpace = mRow * c + (mCol + 1);
    nextColor = mBoard[nextSpace];

    if (currentColor != '.' && currentColor != '*' && currentColor == nextColor) {

      int currentSet, nextSet;
      currentSet = mySet.Find(currentSpace);
      nextSet = mySet.Find(nextSpace);

      if (currentSet != nextSet) {  // Gotta make sure that these two are different sets, otherwise we would be unioning the same set with itself (don't do that)
        if (mDEBUG) cout << "  " << currentColor << " == " << nextColor << ", so Unioning (" << mRow << ", " << mCol << ") with (" << mRow << ", " << mCol + 1 << ")\n";
        mySet.Union(mySet.Find((currentSpace)), mySet.Find((nextSpace)));

        if (mDEBUG) mySet.Print();
      }
    }

    if (mDEBUG) cout << "(>)Analyzing (" << mRow << ", " << mCol + 1 << "). . .\n";  // Notice how the arrow is saying which way the program is going
    ConstructDisjointsets(mySet, mBoard, mRow, mCol + 1);
  }

  if (mRow < r - 1 && takenPaths[currentSpace].second == false) { // Moving down on the mBoard
    takenPaths[currentSpace].second = true;  // Making sure that we never take this path again
    
    
    nextSpace = (mRow + 1) * c + mCol;
    nextColor = mBoard[nextSpace];

    if (currentColor != '.' && currentColor != '*' && currentColor == nextColor) {
      int currentSet, nextSet;
      currentSet = mySet.Find(currentSpace);
      nextSet = mySet.Find(nextSpace);

      if (currentSet != nextSet) {  // Gotta make sure that these two are different sets, otherwise we would be unioning the same set with itself (don't do that)
        if (mDEBUG) cout << "  " << currentColor << " == " << nextColor << ", so Unioning (" << mRow << ", " << mCol << ") with (" << mRow << ", " << mCol + 1 << ")\n";
        mySet.Union(mySet.Find((currentSpace)), mySet.Find((nextSpace)));

        if (mDEBUG) mySet.Print();
      }
    }
    
    if (mDEBUG) cout << "(V)Analyzing (" << mRow + 1 << ", " << mCol << "). . .\n";
    ConstructDisjointsets(mySet, mBoard, mRow + 1, mCol);
  }
  return mySet;
}

void Superball::FindScoringCells() {
  bool mDEBUG = false;
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

    if (mDEBUG) cout << "Searching (" << row << ", " << col << ") == " << sizes->at(currentCell) << " and comparing with " << mss << ". . . \n";
    if (board[currentCell] != '.' && board[currentCell] != '*' && sizes->at(currentCell) >= mss) {  // This means that the given set is able to be scored if one of the elements is in the goal
      if (mDEBUG) cout << "  A scoring set was found at (" << row << ", " << col << ")\n";
      
      // Looking for a goal element
      if (mDEBUG) cout << "    Looking for goal elements. . .\n";
      list<int> elements = d.Get_Elements()->at(currentCell);
      for (list<int>::iterator it = elements.begin(); it != elements.end(); it++) {
        if (mDEBUG) cout << "      Searching " << *it << ".\n";
        if (goals[*it]) { // We have found an element that is at a goal cell
          if (mDEBUG) cout << "      Found a goal cell. Pushing back " << *it << ".\n";
          scoringCells.push_back(*it);
          break;
        }
        // elements
      }
    }
  }
}

double Superball::ClosestColor(int cell, char color) {
  int targetRow = cell / c; 
  int targetCol = cell % c;
  
  int currentRow, currentCol;
  int closestCell; 
  double smallestDistance = __DBL_MAX__;
  for (int currentCell = 0; currentCell < r * c; currentCell++) {
    currentRow = currentCell / c;
    currentCol = currentCell % c;

    if (board[currentCell] == color) {
      double currentDistance = sqrt(pow(currentRow - targetRow, 2) + pow(currentCol - targetCol, 2));
      if (smallestDistance > currentDistance) {
        if (DEBUG) printf("New smallest distance, %f, from (%d, %d) found at (%d, %d)\n", currentDistance, targetRow, targetCol, currentRow, currentCol);
        smallestDistance = currentDistance;
        closestCell = currentCell;
      }
    }


  }
  return smallestDistance;
}

double Superball::EvaluateBoard() {
  return EvaluateBoard(board);
}

double Superball::EvaluateBoard(vector<int> mBoard) {
  // Parameter List
  double WEIGHT_FOR_SET_IN_GOAL = 2;

  // Collecting information on the position


  if (1) {
    cout << "Here is the board:" << endl;
    printBoard(mBoard);
  }

  takenPaths.clear();       // Clearing and remaking takenPaths to be reused by ConstructDisjointsets()
  takenPaths.resize(r*c);
  Disjoint_Set newSet;      // Making a disjoint set to pass to ConstructDisjointsets() 
  newSet.Initialize(r*c);
  
  newSet = ConstructDisjointsets(newSet, mBoard); 

  if (DEBUG) cout << "Here's newSet in Evaluate: " << endl;
  if (DEBUG) newSet.Print();

  FindScoringCells();



  // // Evaluating
  // double score = 0;
  // const vector<int> *sizes = d->Get_Sizes();
  // int valOfColor;
  // const vector<int> *setIds = d->Get_Set_Ids();
  // int currentSet;

  // for (size_t i = 0; i < setIds->size(); i++) {
  //   currentSet = setIds->at(i);
  //   if (mBoard[currentSet] == '.' || mBoard[currentSet] == '*') { // Skipping any of the empty sets
  //     continue;
  //   }
  //   valOfColor = colors[mBoard[currentSet]];
    
  //   if (DEBUG) printf("Evaluating the %c (value %d) set ID %d-> . .\n", mBoard[currentSet], valOfColor, currentSet);
    
  //   double setEvaluation = pow( (sizes->at(currentSet)) * valOfColor, 2);  // Adding ((size-1) * colorVal)^1.1 to the score, making the program favor larger concentrations of high point colors
  //   if (DEBUG) printf("(%d * %d)^1.5 = %f\n", sizes->at(currentSet), valOfColor, setEvaluation);
    
  //   // Checking if the set has elements in the goal zone
  //   list<int> elements = d->Get_Elements()->at(currentSet);
  //   int elementsInGoal = 0;
    
  //   if (DEBUG) printf("   Looking for elements in the goal. . .\n");
  //   for (list<int>::iterator it = elements.begin(); it != elements.end(); it++) {
  //     if (goals[*it]) {
  //       if (DEBUG) printf("       Element %d is in the goal, adding to elementsInGoal.\n", *it);
  //       elementsInGoal++;
  //     }
  //   }

  //   if (DEBUG) printf("   Set %d had %d elements in the goal, weighting. . .\n", currentSet, elementsInGoal);
  //   setEvaluation = setEvaluation + elementsInGoal * valOfColor;


  //   // Checking if the set can be scored immediately
  //   for (size_t j = 0; j < scoringCells.size(); j++) {    
  //     if (d->Find(scoringCells[j]) == currentSet) {
  //       if (DEBUG) printf("   Set %d can be scored, weighting the evaluation.\n", currentSet);
  //       setEvaluation = setEvaluation * WEIGHT_FOR_SET_IN_GOAL;
  //       break;
  //     }
  //   }

  //   if (DEBUG) printf("   The set has evaluated to %f\n", setEvaluation);
  //   score += setEvaluation;
  // }

  // if (DEBUG) printf("This board's score is %f\n\n", score);

  // return score;
  
}

void Superball::printBoard(vector<int> mBoard) {
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      cout << (char)mBoard[i * c + j] << " ";
    }
    cout << endl;
  }
}

vector<int> swap(int cellA, int cellB, vector<int> board) {    // Returns the copy of a board after a swap
  int val = board[cellA];
  board[cellA] = board[cellB];
  board[cellB] = val;
  return board;
}

void printBoard(vector<int> mBoard, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      cout << (char)mBoard[i * cols + j] << " ";
    }
    cout << endl;
  }
}

int main(int argc, char **argv)
{
  bool DEBUG = true;
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
  // IF YOU HAVE LESS THAN 5 SQUARES LEFT ON THE BOARD, YOU WILL LOSE NEXT TURN
  
  vector<int> currentBoard = s->board;  // Putting this into memory so that when we start doing stuff with other
  double currentScore = s->EvaluateBoard(currentBoard);
  if (DEBUG) printf("\n\nThe current board has evaluated to %f\n", currentScore);

  // Finding the best swap
  vector<int> bestBoard = currentBoard;
  double maxScore = currentScore; // Used for storing the best evaluate score
  int cellA = 0;
  int cellB = 0;            // Used for storing the best swap

  for (i = 0; i < currentBoard.size(); i++) {
    for (j = i + 1; j < currentBoard.size(); j++) {
      if (currentBoard[i] == '.' || currentBoard[i] == '*' || currentBoard[j] == '.' || currentBoard[j] == '*') {
        continue;
      }

      // if (DEBUG) printf("Evaluating swap(%d, %d)\n", i, j);

      vector<int> newBoard = swap(i, j, currentBoard);
      double newEvaluation = s->EvaluateBoard(newBoard);
      if (newEvaluation > maxScore) {
        if (DEBUG) printf(" A better score of %f has been found by swapping (%d) with (%d)\n", newEvaluation, i, j);
        
        bestBoard = newBoard;
        maxScore = newEvaluation;
        cellA = i;
        cellB = j;
      }
    }
  }

  cout << endl;
  printBoard(bestBoard, 8, 10);

  cout << cellA << " " << cellB << endl;
  printf("SWAP %d %d %d %d\n", cellA / s->c, cellA % s->c, cellB / s->c, cellB % s->c);

  // s->ConstructDisjointsets();
  // s->takenPaths.clear();
  // s->takenPaths.resize(r*c);
  // s->FindScoringCells();

  

  // // Calculating the ouput
  // int currentSet;
  
  // cout << "Scoring sets: " << endl;
  // for (size_t i = 0; i < s->scoringCells.size(); i++) {
  //   // DEBUG cout << "scoringCell == " << s->scoringCells[i] << " ";
    
  //   currentSet = s->d->Find(s->scoringCells[i]);
  //   printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n", s->d->Get_Sizes()->at(currentSet), s->board[currentSet], s->scoringCells[i] / s->c, s->scoringCells[i] % s->c);

  // }
  
  // S T A R T   O F  S B - P L A Y
  // Swapping the colors that create the shortest distance between matching colors
  

  exit(0);
}
