/*******************
 * Michael Mansfield
 * Piece Header
 * Game Project
 * mm815113@ohio.edu
*******************/

#ifndef CHECKERS
#define CHECKERS
#include "piece.h"
#include "game.h"
#include "colors.h"
#include <stack>

class checkers:public game
{
  public:
    void restart();
    void display_status() const;
    void convert_move(const std::string &move, int coords[4]) const;
    const std::string reverse_convert(const int coords[4]) const;
    bool is_legal(const std::string &move);
    void make_move(const std::string &move);
    const bool is_jump_available();
    const bool is_jump(const std::string &move);
    void flush_jumps();
    game* clone() const;
    who winning() const;
    bool is_game_over();
    void compute_moves(std::queue<std::string>& moves);
    int evaluate() const;
  private:
    piece board[8][8];
    std::stack<std::string> jumps;
};

#endif