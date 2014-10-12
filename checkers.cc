/*******************
 * Michael Mansfield
 * Checkers Class
 * Game Project
 * mm815113@ohio.edu
*******************/

#include <iostream>
#include <cstdlib>
#include "checkers.h"
using namespace std;

void checkers::restart()
{
  for(int y = 0; y < 8; y++)
  {
    for(int x = 0; x < 8; x++)
    {
      if(y == 0 && x % 2 != 0)
      {
        board[x][y].occupy();
        board[x][y].black();
        board[x][y].unking();
      }
      else if(y == 1 && x % 2 == 0)
      {
        board[x][y].occupy();
        board[x][y].black();
        board[x][y].unking();
      }
      else if(y == 2 && x % 2 != 0)
      {
        board[x][y].occupy();
        board[x][y].black();
        board[x][y].unking();
      }
      else if(y == 5 && x % 2 == 0)
      {
        board[x][y].occupy();
        board[x][y].red();
        board[x][y].unking();
      }
      else if(y == 6 && x % 2 != 0)
      {
        board[x][y].occupy();
        board[x][y].red();
        board[x][y].unking();
      }
      else if(y == 7 && x % 2 == 0)
      {
        board[x][y].occupy();
        board[x][y].red();
        board[x][y].unking();
      }
      else
      {
        board[x][y].vacate();
      }
    }
  }
  game::restart();
}

void checkers::display_status() const
{
  system("clear");
  cout << endl << "     *------------------------*" << endl;
  for(int i = 0; i < 8; i++)
  {
    cout << "   " << 8 - i << " |";
    for(int j = 0; j < 8; j++)
    {
      if(!(board[j][i].is_occupied()))
      {
        if(i % 2 != 0 && j % 2 == 0) {cout << B_WHITE << "   " << RESET;}
        else if(i % 2 == 0 && j % 2 != 0) {cout << B_WHITE << "   " << RESET;}
        else {cout << B_BLUE << "   " << RESET;}
      }
      else if(!(board[j][i].is_king()))
      {
        if(board[j][i].what_team() == 'R')
        {
          cout << RED << B_WHITE << " R " << RESET;
        }
        else if(board[j][i].what_team() == 'B')
        {
          cout << BLACK << B_WHITE << " B " << RESET;
        }
      }
      else if(board[j][i].is_king())
      {
        if(board[j][i].what_team() == 'R')
        {
          cout << RED << B_WHITE << "*R*" << RESET;
        }
        else if(board[j][i].what_team() == 'B')
        {
          cout << BLACK << B_WHITE << "*B*" << RESET;
        }
      }
    }
    cout << "|" << endl;
  }
  cout << "     *------------------------*" << endl
       << "       a  b  c  d  e  f  g  h " << endl << endl
       << "(Enter your moves in this format: a3b4)" << endl
       << "(Note: enter only the first of multiple" << endl
       << " jumps and they will be autocompleted.)" << endl << endl;
}

void checkers::convert_move(const string &move, int coords[4]) const
{
  coords[0] = toupper(move[0]) - 'A';
  coords[1] = '8' - move[1];
  coords[2] = toupper(move[2]) - 'A';
  coords[3] = '8' - move[3];
}

const string checkers::reverse_convert(const int coords[4]) const
{
  char x, y, x2, y2;
  x = coords[0] + 'A';
  y = '8' - coords[1];
  x2 = coords[2] + 'A';
  y2 = '8' - coords[3];
  string tmp;
  tmp += x;
  tmp += y;
  tmp += x2;
  tmp += y2;
  return tmp;
}

bool checkers::is_legal(const string &move) //const
{
  /////////////////////////////
  //The is_legal function was//
  //altered in the game class//
  //to allow for calling of a//
  //function that assembles a//
  //stack of available jumps //
  /////////////////////////////
  int x, y, x2, y2, coords[4];
  convert_move(move, coords);
  x = coords[0];
  y = coords[1];
  x2 = coords[2];
  y2 = coords[3];
  //Convert the move string to coordinate integers
  if(!(board[x][y].is_occupied())) {return false;}
  //Illegal if there is no piece here
  if(board[x2][y2].is_occupied()) {return false;}
  //Illegal if there is a piece on the destination
  if(x % 2 == 0 && y % 2 == 0) {return false;}
  if(x % 2 != 0 && y % 2 != 0) {return false;}
  if(x2 % 2 == 0 && y2 % 2 == 0) {return false;}
  if(x2 % 2 != 0 && y2 % 2 != 0) {return false;}
  //Illegal if either piece or destination is a non-game space
  if(x > 7 || x < 0 || y > 7 || y < 0) {return false;}
  //Illegal if piece is off the board
  if(x2 > 7 || x2 < 0 || y2 > 7 || y2 < 0) {return false;}
  //Illegal if destination is off the board
  if(!(board[x][y].is_king()) && board[x][y].what_team() == 'R')
  {if(y < y2) return false;}
  //Illegal if a red piece tries to move backwards
  if(!(board[x][y].is_king()) && board[x][y].what_team() == 'B')
  {if(y2 < y) return false;}
  //Illegal if a black piece tries to move backwards
  if(last_mover() == COMPUTER && board[x][y].what_team() == 'B')
  {return false;}
  //Illegal if player tries to move computer piece
  if(last_mover() == HUMAN && board[x][y].what_team() == 'R')
  {return false;}
  //Illegal if computer tries to move player piece
  if(is_jump_available())
  {
    if(!(is_jump(move))) return false;
    else return true;
  }
  //Illegal if there is a jump available and
  //the entered move is not an available jump
  if(abs(x2 - x) > 1 || abs(y - y2) > 1) {return false;}
  if(abs(x - x2) > 1 || abs(y2 - y) > 1) {return false;}
  //Illegal if move is more than 1 space
  //away, checked after checking for jump
  return true;
}

void checkers::make_move(const string &move)
{
  int coords[4], temp[4], temp2[4], temp3[4], temp4[4];
  int xdiff, ydiff, x2, y2;
  bool was_jump = false;
  string tmp, tmp2, tmp3, tmp4;
  convert_move(move, coords);
  if(is_jump_available() && is_jump(move))
  {
    xdiff = (coords[2] - coords[0])/2;
    ydiff = (coords[3] - coords[1])/2;
    x2 = coords[0] + xdiff;
    y2 = coords[1] + ydiff;
    board[x2][y2].vacate();
    was_jump = true;
  }
  board[coords[2]][coords[3]] = board[coords[0]][coords[1]];
  board[coords[0]][coords[1]].vacate();
  if(coords[3] == 0 && board[coords[2]][coords[3]].what_team() == 'R' && !board[coords[2]][coords[3]].is_king())
  {
    board[coords[2]][coords[3]].make_king();
    game::make_move(move);
    return;
  }
  else if(coords[3] == 7 && board[coords[2]][coords[3]].what_team() == 'B' && !board[coords[2]][coords[3]].is_king())
  {
    board[coords[2]][coords[3]].make_king();
    game::make_move(move);
    return;
  }
  else if(is_jump_available() && was_jump)
  {
    temp[0] = coords[2];
    temp[1] = coords[3];
    temp[2] = coords[2] + 2;
    temp[3] = coords[3] + 2;
    temp2[0] = coords[2];
    temp2[1] = coords[3];
    temp2[2] = coords[2] + 2;
    temp2[3] = coords[3] - 2;
    temp3[0] = coords[2];
    temp3[1] = coords[3];
    temp3[2] = coords[2] - 2;
    temp3[3] = coords[3] + 2;
    temp4[0] = coords[2];
    temp4[1] = coords[3];
    temp4[2] = coords[2] - 2;
    temp4[3] = coords[3] - 2;
    tmp = reverse_convert(temp);
    tmp2 = reverse_convert(temp2);
    tmp3 = reverse_convert(temp3);
    tmp4 = reverse_convert(temp4);
    if(is_legal(tmp)) {make_move(tmp); return;}
    else if(is_legal(tmp2)) {make_move(tmp2); return;}
    else if(is_legal(tmp3)) {make_move(tmp3); return;}
    else if(is_legal(tmp4)) {make_move(tmp4); return;}
  }
  game::make_move(move);
  flush_jumps();
}

const bool checkers::is_jump_available()
{
	flush_jumps();
  string tmp;
  int coord[4], alt[4];
  for(int y = 0; y < 8; y++)
  {
    for(int x = 0; x < 8; x++)
    {
      if(!(board[x][y].is_king()))
      {
        if(board[x][y].what_team() == 'R' && last_mover() == COMPUTER)
        {
          if((!(board[x+2][y-2].is_occupied())) && (board[x+1][y-1].what_team() == 'B'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x + 2;
            coord[3] = y - 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] > 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x-2][y-2].is_occupied())) && (board[x-1][y-1].what_team() == 'B'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x - 2;
            coord[3] = y - 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] > 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
        }
        else if(board[x][y].what_team() == 'B' && last_mover() == HUMAN)
        {
          if((!(board[x+2][y+2].is_occupied())) && (board[x+1][y+1].what_team() == 'R'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x + 2;
            coord[3] = y + 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 7 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x-2][y+2].is_occupied())) && (board[x-1][y+1].what_team() == 'R'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x - 2;
            coord[3] = y + 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 7 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
        }
      }
      else if(board[x][y].is_king())
      {
        if(board[x][y].what_team() == 'R' && last_mover() == COMPUTER)
        {
          if((!(board[x+2][y+2].is_occupied())) && (board[x+1][y+1].what_team() == 'B'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x + 2;
            coord[3] = y + 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x+2][y-2].is_occupied())) && (board[x+1][y-1].what_team() == 'B'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x + 2;
            coord[3] = y - 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x-2][y+2].is_occupied())) && (board[x-1][y+1].what_team() == 'B'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x - 2;
            coord[3] = y + 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x-2][y-2].is_occupied())) && (board[x-1][y-1].what_team() == 'B'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x - 2;
            coord[3] = y - 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
        }
        else if(board[x][y].what_team() == 'B' && last_mover() == HUMAN)
        {
          if((!(board[x+2][y+2].is_occupied())) && (board[x+1][y+1].what_team() == 'R'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x + 2;
            coord[3] = y + 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x+2][y-2].is_occupied())) && (board[x+1][y-1].what_team() == 'R'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x + 2;
            coord[3] = y - 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x-2][y+2].is_occupied())) && (board[x-1][y+1].what_team() == 'R'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x - 2;
            coord[3] = y + 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
          if((!(board[x-2][y-2].is_occupied())) && (board[x-1][y-1].what_team() == 'R'))
          {
            coord[0] = x;
            coord[1] = y;
            coord[2] = x - 2;
            coord[3] = y - 2;
            if(coord[0] < 8 && coord[0] >= 0 && coord[1] < 8 && coord[1] >= 0
               && coord[2] < 8 && coord[2] >= 0 && coord[3] < 8 && coord[3] >= 0)
            {
              tmp = reverse_convert(coord);
              jumps.push(tmp);
            }
          }
        }
      }
    }
  }
  if(!(jumps.empty())) {return true;}
  return false;
}

const bool checkers::is_jump(const string &move)
{
  bool result =  false;
  while(!(jumps.empty()))
  {
    string tmp;
    tmp = move;
    tmp[0] = toupper(tmp[0]);
    tmp[2] = toupper(tmp[2]);
    if(jumps.top() == tmp) {result = true;}
    jumps.pop();
  }
  return result;
}

void checkers::flush_jumps()
{
  while(!(jumps.empty())) {jumps.pop();}
}

game* checkers::clone() const
{
  return new checkers(*this);
}

game::who checkers::winning() const
{
  int red_tally = 0, black_tally = 0;
  for(int y = 0; y < 8; y++)
  {
    for(int x = 0; x < 8; x++)
    {
      if(board[x][y].is_occupied())
      {
        if(board[x][y].what_team() == 'R') {red_tally++;}
        if(board[x][y].what_team() == 'B') {black_tally++;}
      }
    }
  }
  if(red_tally > black_tally) {return HUMAN;}
  else if(black_tally > red_tally) {return COMPUTER;}
  else {return NEUTRAL;}
}

bool checkers::is_game_over()
{
	////////////////////////////////////
	//Function modified in game class //
  //to allow for computing if there //
  //are any remaining legal moves.  //
  //His throwing const everywhere   //
  //is dumb and inconvenient.       //
  ////////////////////////////////////
  int tally = 0, red_tally = 0, black_tally = 0;
  queue<string> moves;
  for(int y = 0; y < 8; y++)
  {
    for(int x = 0; x < 8; x++)
    {
      if(board[x][y].is_occupied())
      {
        if(board[x][y].what_team() == 'R') {red_tally++;}
        if(board[x][y].what_team() == 'B') {black_tally++;}
        tally++;
      }
    }
  }
  if(tally == 1) {return true;}
  if(red_tally == tally) {return true;}
  if(black_tally == tally) {return true;}
  compute_moves(moves);
  if(moves.empty()) {return true;}
  return false;
}

void checkers::compute_moves(queue<string>& moves)
{
	/////////////////////////////
  //Function was altered in  //
  //game class because it    //
  //it refused to pass this  //
  //object as the arguement  //
  //with the const parameter.//
  /////////////////////////////
	int coords[4];
  string tmp;
  for(int x = 0; x < 8; x++)
  {
    for(int y = 0; y < 8; y++)
    {
    	coords[0] = x;
      coords[1] = y;
      coords[2] = x + 1;
      coords[3] = y + 1;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x + 1;
      coords[3] = y - 1;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x - 1;
      coords[3] = y + 1;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x - 1;
      coords[3] = y - 1;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x + 2;
      coords[3] = y + 2;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x + 2;
      coords[3] = y - 2;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x - 2;
      coords[3] = y + 2;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
      coords[2] = x - 2;
      coords[3] = y - 2;
      tmp = reverse_convert(coords);
      if(is_legal(tmp)) {moves.push(tmp);}
    }
  }
}

int checkers::evaluate() const
{
  int score = 0;
  for (int x = 0; x < 8; x++)
  {
  	for(int y = 0; y < 8; y++)
  	{
  	  if(board[x][y].is_occupied() && !(board[x][y].is_king()))
  	  {
  	    if(board[x][y].what_team() == 'R') {score--;}
        if(board[x][y].what_team() == 'B') {score++;}
  	  }
      else if(board[x][y].is_occupied() && board[x][y].is_king())
  	  {
  	    if(board[x][y].what_team() == 'R') {score -= 4;}
        if(board[x][y].what_team() == 'B') {score += 4;}
  	  }
  	}
  }
  return score;
}