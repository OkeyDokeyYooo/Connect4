#pragma once
#include <iostream>
using namespace std;

#define DIG_LEN 12

class Board{
    public:
        Board(); // constructor 
        Board(const Board& bd); // deep copy constructor
        ~Board(); // destructor 
        // three function to check the any row, column, diagonal has connect 4 pieces
        bool check_row();
        bool check_column();
        bool check_diagonal();
        bool is_win(); // there is any row that has 4 consecutive pieces
        bool is_tie(); // if the board is full and not win, then this is a tie game
        bool all_same(int a, int b, int c, int d); // if all input are same, return true
        void make_column_list(int** chessboard);
        void make_diagonal_list(int** chessboard);
        void display_chess_board();
        void update_list();
        int chess_len(int* ptr);
        void display_valid_pos();
        void display_pos();
        bool is_valid_pos(int pos);
        void make_change(int pos, int color);
        int* valid_array();
        int num_valid_pos();
        int* height_of_each_row();
        int* find_exact_row_list(int height);
        int* find_exact_column_list(int pos);
        int* find_exact_diagonal_list(int pos);
    private:
        int width = 7;
        int height = 6;
        int** chessboard;
        int** row_list;
        int** column_list;
        int** diagonal_list;
        int* valid_position;
        void deepcopy(const Board & bd);
};
