/*******************
 * Michael Mansfield
 * Piece Class
 * Game Project
 * mm815113@ohio.edu
*******************/

#include <iostream>
#ifndef PIECE
#define PIECE

class piece
{
  public:
    piece() {king = false; occupied = false; team = 'E';}
    const bool is_king() const {return king;}
    const bool is_occupied() const {return occupied;}
    const char what_team() const {return team;}
    void make_king() {king = true;}
    void unking() {king = false;}
    void occupy() {occupied = true;}
    void vacate() {occupied = false; team = 'E'; king = false;}
    void red() {team = 'R';}
    void black() {team = 'B';}
  private:
    bool king;
    bool occupied;
    char team;
};

#endif