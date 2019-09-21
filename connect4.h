#pragma once
#include <iostream>
#include "Board.h"
using namespace std;


int pick_AI_color(int user_color);
void init_game();
int AI_turn(Board board); // return the position that AI think it will not lose
int max_index_array(int* array, int len);
bool has_this_index(int a, int b, int c, int d, int pos);
bool has_line_two(int a, int b, int c, int d, int color);
bool has_line_three(int a, int b, int c, int d, int color);
bool has_line_four(int a, int b, int c, int d, int color);
int find_grade(int a, int b, int c, int d, int color);
int cacl_this_pos_grade(Board board, int pos, int color);