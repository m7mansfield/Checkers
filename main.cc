/*******************
 * Michael Mansfield
 * Main Application
 * Game Project
 * mm815113@ohio.edu
*******************/

#include <iostream>
#include "checkers.h"
using namespace std;

int main()
{
  checkers current_game;
  game::who winner;
  winner = current_game.play();
  if(winner == game::HUMAN)
  {cout << "RED WINS!" << endl;}
  else if(winner == game::COMPUTER)
  {cout << "BLACK WINS!" << endl;}
  else
  {cout << "DRAW!" << endl;}
}