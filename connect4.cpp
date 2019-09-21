#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "Board.h"
using namespace std;

#define N 1000
#define WIN 5
#define LOSS -2
#define TIE 0
#define HEIGHT 6
#define WIDTH 7
#define MINIMAX_DEPTH 5
#define MAX_NUM_GAME 20
#define RED 1
#define BLUE 2

int user_color, user_choice, user_order;
int AI_color;

int pick_AI_color(int input){
    if (input == 1)
        return 2;
    else if (input == 2)
        return 1;
    else return -1;
}

int max_index_array(int* array, int len){
    srand(time(NULL));
    int max = 0;
    for(int i = 1; i < len; i++){
        if (array[i] > array[max]) max = i;
        else if (array[i] == array[max]) {
            int coin = rand() % 2;
            if (coin == 1) max = i;
        }
    }
    return max;
}

// void display_array(int* array){
//     cout << "[";
// }

void init_game(){
    cout << "Please Pick Color -> 1: Red, 2: Blue => ";
    cin >> user_color;
    while(user_color > 2 || user_color < 1){
        cout << "Please Pick from 1 or 2 => ";
        cin >> user_color;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    AI_color = pick_AI_color(user_color);
    if (AI_color == -1) exit(-1);

    cout << "Order -> 1: Go First, 2: Go Second => ";
    cin >> user_order;
    while(user_order > 2 || user_order < 1){
        cout << "Please Pick from 1 or 2 => ";
        cin >> user_order;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int pMCT_turn(Board board){
    srand(time(NULL));
    int random_user_choice;
    int random_AI_choice;
    int* AI_choice_array = board.valid_array();
    int num_pos = board.num_valid_pos();
    int grade_list[num_pos];

    for (int i = 0; i < num_pos; i++){
        int AI_choice = AI_choice_array[i] + 1;
        int this_choice_grade = 0;
        for (int j = 0; j < N; j++){
            Board AI_Board(board);
            AI_Board.make_change(AI_choice, AI_color);
            if (AI_Board.is_win()){
                this_choice_grade += WIN;
                grade_list[i] = this_choice_grade;
                continue;
            } else if (AI_Board.is_tie()){
                this_choice_grade += TIE;
                grade_list[i] = this_choice_grade;
                continue;
            }
            while (1) {
                random_user_choice = AI_Board.valid_array()[rand() % AI_Board.num_valid_pos()] + 1;
                AI_Board.make_change(random_user_choice, user_color);
                if (AI_Board.is_win()){
                    this_choice_grade += LOSS;
                    grade_list[i] = this_choice_grade;
                    break;
                } else if (AI_Board.is_tie()){
                    this_choice_grade += TIE;
                    grade_list[i] = this_choice_grade;
                    break;
                }
                random_AI_choice = AI_Board.valid_array()[rand() % AI_Board.num_valid_pos()] + 1;
                AI_Board.make_change(random_AI_choice, AI_color);
                if (AI_Board.is_win()){
                    this_choice_grade += WIN;
                    grade_list[i] = this_choice_grade;
                    break;
                } else if (AI_Board.is_tie()){
                    this_choice_grade += TIE;
                    grade_list[i] = this_choice_grade;
                    break;
                }
            }
        }
    }

    cout << "[";
    for (int i = 0; i < num_pos; i ++){
        cout << grade_list[i] << " ";
    }
    cout << "]\n";

    int AI_final_choice = AI_choice_array[max_index_array(grade_list, num_pos)];
    cout << "AI final choice pos =>" << AI_final_choice + 1 << endl;
    return AI_final_choice + 1;
}

bool has_line_four(int a, int b, int c, int d, int color){
    if ((a == color) && (b == color) && (c == color) && (d == color)) return true;
    return false;
}

bool has_line_three(int a, int b, int c, int d, int color){
    int opp;
    if (color == RED) opp = BLUE;
    else if (color == BLUE) opp = RED;

    if (((a == color) && (b == color) && (c == color) && (d != opp)) || 
        ((a == color) && (b == color) && (d == color) && (c != opp)) ||
        ((b == color) && (c == color) && (d == color) && (a != opp)) ||
        ((a == color) && (c == color) && (d == color) && (b != opp)))
            return true;
    return false;
}


bool has_line_two(int a, int b, int c, int d, int color){
    // if (((a == color && b == color) && (c == 0 && d == 0)) ||
    //     ((b == color && c == color) && (a == 0 && d == 0)) ||
    //     ((c == color && d == color) && (a == 0 && b == 0)) ||
    //     ((a == color && d == color) && (b == 0 && c == 0)) ||
    //     ((b == color && d == color) && (a == 0 && c == 0)) ||
    //     ((a == color && c == color) && (b == 0 && d == 0)))

    if (((a == color && b == color) && (c == 0 && d == 0)) ||
        ((b == color && c == color) && (a == 0 && d == 0)) ||
        ((c == color && d == color) && (a == 0 && b == 0)))
            return true;
    return false;
}

bool has_this_index(int a, int b, int c, int d, int pos){
    if (a == pos || b == pos || c == pos || d == pos) return true;
    return false;
}

int find_grade(int a, int b, int c, int d, int color){
    if (has_line_four(a, b, c, d, color)) return 10000;
    else if (has_line_three(a,b,c,d,color)) return 5;
    else if (has_line_two(a,b,c,d,color)) return 2;
    else return 0;
}

bool has_left_diagonal_list(int x, int y){
    if(x == 0 && y <=2 ) return false;
    else if (x == 1 && y <= 1) return false;
    else if (x == 2 && y == 0) return false;
    else if (x == 3 && y == 6) return false;
    else if (x == 4 && y >= 5) return false;
    else if (x == 5 && y >= 4) return false;
    else return true;
}

bool has_right_diagonal_list(int x, int y){
    if(x == 0 && y >=4 ) return false;
    else if (x == 1 && y >= 5) return false;
    else if (x == 2 && y == 6) return false;
    else if (x == 3 && y == 0) return false;
    else if (x == 4 && y <= 1) return false;
    else if (x == 5 && y <= 2) return false;
    else return true;
}


int diagonal_left_list_index(int x, int y){
    if      ((x == 0 && y == 3) || (x == 1 && y == 2) || (x == 2 && y == 1) || (x == 3 && y == 0)) return 0;
    else if ((x == 2 && y == 6) || (x == 3 && y == 5) || (x == 4 && y == 4) || (x == 5 && y == 3)) return 1;

    else if ((x == 0 && y == 4) || (x == 1 && y == 3) || (x == 2 && y == 2) || (x == 3 && y == 1) || (x == 4 && y == 0)) return 4;
    else if ((x == 1 && y == 6) || (x == 2 && y == 5) || (x == 3 && y == 4) || (x == 4 && y == 3) || (x == 5 && y == 2)) return 5;

    else if ((x == 0 && y == 5) || (x == 1 && y == 4) || (x == 2 && y == 3) || (x == 3 && y == 2) || (x == 4 && y == 1) || (x == 5 && y == 0)) return 8;
    else if ((x == 0 && y == 6) || (x == 1 && y == 5) || (x == 2 && y == 4) || (x == 3 && y == 3) || (x == 4 && y == 2) || (x == 5 && y == 1)) return 9;

    return -1;
}

int diagonal_right_list_index(int x, int y){
    if ((x == 0 && y == 3) || (x == 1 && y == 4) || (x == 2 && y == 5) || (x == 3 && y == 6)) return 2;
    else if ((x == 2 && y == 0) || (x == 3 && y == 1) || (x == 4 && y == 2) || (x == 5 && y == 3)) return 3;
   
    else if ((x == 0 && y == 2) || (x == 1 && y == 3) || (x == 2 && y == 4) || (x == 3 && y == 5) || (x == 4 && y == 6)) return 6;
    else if ((x == 1 && y == 0) || (x == 2 && y == 1) || (x == 3 && y == 2) || (x == 4 && y == 3) || (x == 5 && y == 4)) return 7;

    else if ((x == 0 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 3) || (x == 3 && y == 4) || (x == 4 && y == 5) || (x == 5 && y == 6)) return 10;
    else if ((x == 0 && y == 0) || (x == 1 && y == 1) || (x == 2 && y == 2) || (x == 3 && y == 3) || (x == 4 && y == 4) || (x == 5 && y == 5)) return 11;

    return -1;
}

int diagonal_left_list_length(int x, int y){
    if      ((x == 0 && y == 3) || (x == 1 && y == 2) || (x == 2 && y == 1) || (x == 3 && y == 0)) return 4;
    else if ((x == 2 && y == 6) || (x == 3 && y == 5) || (x == 4 && y == 4) || (x == 5 && y == 3)) return 4;

    else if ((x == 0 && y == 4) || (x == 1 && y == 3) || (x == 2 && y == 2) || (x == 3 && y == 1) || (x == 4 && y == 0)) return 5;
    else if ((x == 1 && y == 6) || (x == 2 && y == 5) || (x == 3 && y == 4) || (x == 4 && y == 3) || (x == 5 && y == 2)) return 5;

    else if ((x == 0 && y == 5) || (x == 1 && y == 4) || (x == 2 && y == 3) || (x == 3 && y == 2) || (x == 4 && y == 1) || (x == 5 && y == 0)) return 6;
    else if ((x == 0 && y == 6) || (x == 1 && y == 5) || (x == 2 && y == 4) || (x == 3 && y == 3) || (x == 4 && y == 2) || (x == 5 && y == 1)) return 6;

    return -1;
}

int diagonal_right_list_length(int x, int y){
    if      ((x == 0 && y == 3) || (x == 1 && y == 4) || (x == 2 && y == 5) || (x == 3 && y == 6)) return 4;
    else if ((x == 2 && y == 0) || (x == 3 && y == 1) || (x == 4 && y == 2) || (x == 5 && y == 3)) return 4;

    else if ((x == 0 && y == 2) || (x == 1 && y == 3) || (x == 2 && y == 4) || (x == 3 && y == 5) || (x == 4 && y == 6)) return 5;
    else if ((x == 1 && y == 0) || (x == 2 && y == 1) || (x == 3 && y == 2) || (x == 4 && y == 3) || (x == 5 && y == 4)) return 5;

    else if ((x == 0 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 3) || (x == 3 && y == 4) || (x == 4 && y == 5) || (x == 5 && y == 6)) return 6;
    else if ((x == 0 && y == 0) || (x == 1 && y == 1) || (x == 2 && y == 2) || (x == 3 && y == 3) || (x == 4 && y == 4) || (x == 5 && y == 5)) return 6;

    return -1;
}

int convert_to_left_index(int x, int y){
    if      ((x == 0 && y == 3) || (x == 0 && y == 4) ||(x == 0 && y == 5) ||(x == 0 && y == 6) ||(x == 1 && y == 6) || (x == 2 && y == 6)) return 0;
    else if ((x == 1 && y == 2) || (x == 1 && y == 3) ||(x == 1 && y == 4) ||(x == 1 && y == 5) ||(x == 2 && y == 5) || (x == 3 && y == 5)) return 1;
    else if ((x == 2 && y == 1) || (x == 2 && y == 2) ||(x == 2 && y == 3) ||(x == 2 && y == 4) ||(x == 3 && y == 4) || (x == 4 && y == 4)) return 2;
    else if ((x == 3 && y == 0) || (x == 3 && y == 1) ||(x == 3 && y == 2) ||(x == 3 && y == 3) ||(x == 4 && y == 3) || (x == 5 && y == 3)) return 3;
    else if ((x == 4 && y == 0) || (x == 4 && y == 1) ||(x == 4 && y == 2) ||(x == 5 && y == 2)) return 4;
    else if ((x == 5 && y == 0) || (x == 5 && y == 1)) return 5;
    else return -1;
}

int convert_to_right_index(int x, int y){
    if      ((x == 0 && y == 3) || (x == 0 && y == 2) ||(x == 0 && y == 1) ||(x == 0 && y == 0) ||(x == 1 && y == 0) || (x == 2 && y == 0)) return 0;
    else if ((x == 1 && y == 4) || (x == 1 && y == 3) ||(x == 1 && y == 2) ||(x == 1 && y == 1) ||(x == 2 && y == 1) || (x == 3 && y == 1)) return 1;
    else if ((x == 2 && y == 5) || (x == 2 && y == 4) ||(x == 2 && y == 3) ||(x == 2 && y == 2) ||(x == 3 && y == 2) || (x == 4 && y == 2)) return 2;
    else if ((x == 3 && y == 6) || (x == 3 && y == 5) ||(x == 3 && y == 4) ||(x == 3 && y == 3) ||(x == 4 && y == 3) || (x == 5 && y == 3)) return 3;
    else if ((x == 4 && y == 6) || (x == 4 && y == 5) ||(x == 4 && y == 4) ||(x == 5 && y == 4)) return 4;
    else if ((x == 5 && y == 6) || (x == 5 && y == 5)) return 5;
    else return -1;
}

//pos = [1,7]
// int cacl_this_pos_grade(Board board, int pos, int color){
//     int opp_color;
//     int grade = 0;
//     int height_of_pos = board.height_of_each_row()[pos];
//     if (height_of_pos >= 6) return 0; // if this role is full, then return grade 0;
//     int x = 5-height_of_pos;
//     int y = pos;
//     if (pos == 3) grade += 4;

//     if (color == 1) opp_color = 2;
//     else if (color == 2) opp_color = 1;
//     // make change of the temp_board
//     Board temp_board (board);
//     temp_board.make_change(pos+1, color);


//     int temp = 0;
//     int max = 0;
//     int* row_list = temp_board.find_exact_row_list(x);
//     // thie row_list len must be 7;
//     for (int i = 0; i < WIDTH-3; i++){
//         if (has_this_index(i, i+1, i+2, i+3, pos)){
//             // cout << row_list[i] << row_list[i+1] << row_list[i+2] << row_list[i+3] << endl;
//             // grade += find_grade(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], color);
//             if (has_line_four(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], color)) return +10000; // if the oppoent win, then just return -100, bad move!!!
//             else if (has_line_three(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], color)) grade += 4;
//             else if (has_line_two(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], color)) grade += 2;

//         }
//     }  
//     // cout << "row grade = " << grade << endl;


//     temp = 0, max = 0;
//     int* column_list = temp_board.find_exact_column_list(pos);
//     for (int i = 0; i < HEIGHT-3; i++){
//         if(has_this_index(i, i+1, i+2, i+3, 5-height_of_pos)){
//             // cout << column_list[i] << column_list[i+1] << column_list[i+2] << column_list[i+3] << endl;

//             // temp = find_grade(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], color);
//             if (has_line_four(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], color)) return +10000; // if the oppoent win, then just return -100, bad move!!!
//             else if (has_line_three(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], color)) grade += 4;
//             else if (has_line_two(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], color)) grade += 2;
//         }
//         // if (temp > max) max = temp;
//     }
//     // grade += max;
//     // cout << "column grade = " << grade << endl;

//     temp = 0, max = 0;
//     if(has_left_diagonal_list(x, y)){
//         int* left_diagonal_list = temp_board.find_exact_diagonal_list(diagonal_left_list_index(x, y));
//         for (int i = 0; i < diagonal_left_list_length(x, y)-3; i++){
//             if(has_this_index(i, i+1, i+2, i+3, convert_to_left_index(x,y))){
//                 // temp = find_grade(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], color);
//                 if (has_line_four(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], color)) return +10000;
//                 else if (has_line_three(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], color)) grade += 4;
//                 else if (has_line_two(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], color)) grade +=2;

//             }
//             // if (temp > max) max = temp;
//         }
//     }
//     // grade += max;

//     // cout << "left diagonal grade = " << grade << endl;
//     temp = 0, max = 0;
//     if(has_right_diagonal_list(x, y)){
//         int* right_diagonal_list = temp_board.find_exact_diagonal_list(diagonal_right_list_index(x, y));
//         for (int i = 0; i < diagonal_right_list_length(x, y)-3; i++){
//             if(has_this_index(i, i+1, i+2, i+3, convert_to_right_index(x,y))){
//                 if (has_line_four(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], color)) return +10000;
//                 else if (has_line_three(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], color)) grade += 4;
//                 else if (has_line_two(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], color)) grade +=2;

//             //    temp = find_grade(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], color);
//             }
//             // if (temp > max) max = temp;
//         }
//     }
//     // grade += max;

//     // cout << "right diagonal grade = " << grade << endl;
//     // cout << "\n(" << x << "," << y  << ")" << " => get points = " << grade <<endl; 

//     // now checking if opponemnt choose the same row after the AI make the move
//     int opp_x, opp_y;

//     int* opp_valid_pos_arr = temp_board.valid_array();
//     int num_opp_pos = temp_board.num_valid_pos();

//     // cout << "opp_color = " << opp_color << endl;
//     for (int i = 0; i < num_opp_pos; i++){
//         opp_y = opp_valid_pos_arr[i];
//         opp_x = 5-temp_board.height_of_each_row()[opp_y];
//         // cout << "AI choose => (" << opp_x << ", " << opp_y << ")" << endl; 

//         Board test_board(temp_board);
//         test_board.make_change(opp_y+1, opp_color);

//         row_list = test_board.find_exact_row_list(opp_x);
//         for (int i = 0; i < WIDTH-3; i++){
//             if (has_this_index(i, i+1, i+2, i+3, opp_y)){
//                 // cout << row_list[i] << row_list[i+1] << row_list[i+2] << row_list[i+3] << endl;
//                 if (has_line_four(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], opp_color)) grade -= 100; // if the oppoent win, then just return -100, bad move!!!
//                 else if (has_line_three(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], opp_color)) grade -= 4;
//                 else if (has_line_two(row_list[i], row_list[i+1], row_list[i+2], row_list[i+3], opp_color)) grade -= 2;
//             }
//         } 

//         column_list = test_board.find_exact_column_list(opp_y);
//         for (int i = 0; i < HEIGHT-3; i++){
//             if (has_this_index(i, i+1, i+2, i+3, opp_x)){
//                 // cout << column_list[i] << column_list[i+1] << column_list[i+2] << column_list[i+3] << endl;
//                 if (has_line_four(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], opp_color)) grade -= 100; // if the oppoent win, then just return -100, bad move!!!
//                 else if (has_line_three(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], opp_color)) grade -= 4;
//                 else if (has_line_two(column_list[i], column_list[i+1], column_list[i+2], column_list[i+3], opp_color)) grade -= 2;
//             }
//         } 

//         if(has_left_diagonal_list(opp_x, opp_y)){
//             int* left_diagonal_list = test_board.find_exact_diagonal_list(diagonal_left_list_index(opp_x, opp_y));
//             // cout << " choose =>" << opp_x <<" " << opp_y << endl;
//             for (int i = 0; i < diagonal_left_list_length(opp_x, opp_y)-3; i++){
//                 if(has_this_index(i, i+1, i+2, i+3, convert_to_left_index(x,y))){
//                     // cout << left_diagonal_list[i] << left_diagonal_list[i+1] << left_diagonal_list[i+2] << left_diagonal_list[i+3] << endl;
//                     if (has_line_four(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], opp_color)) grade -= 100;
//                     else if (has_line_three(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], opp_color)) grade -= 4;
//                     else if (has_line_two(left_diagonal_list[i], left_diagonal_list[i+1], left_diagonal_list[i+2], left_diagonal_list[i+3], opp_color)) grade -=2;
//                 }
//             }
//         }

//         if(has_right_diagonal_list(opp_x, opp_y)){
//             int* right_diagonal_list = test_board.find_exact_diagonal_list(diagonal_right_list_index(opp_x, opp_y));
//             for (int i = 0; i < diagonal_right_list_length(opp_x, opp_y)-3; i++){
//                 if(has_this_index(i, i+1, i+2, i+3, convert_to_left_index(x,y))){
//                     if (has_line_four(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], opp_color)) grade -= 100;
//                     else if (has_line_three(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], opp_color)) grade -= 4;
//                     else if (has_line_two(right_diagonal_list[i], right_diagonal_list[i+1], right_diagonal_list[i+2], right_diagonal_list[i+3], opp_color)) grade -=2;
//                 }
//             }
//         }
//     }
//     return grade;
// } 


int find_max_value(int* list, int num_pos){
    int max_index = 0;
    for (int i = 0; i < num_pos; i++){
        if (list[i] > list[max_index]) max_index = i;
    }
    return list[max_index];
}

int find_max_index(int* list, int num_pos){
    int max_index = 0;
    for (int i = 0; i < num_pos; i++){
        if (list[i] > list[max_index]) max_index = i;
        else if (list[i] == list[max_index]) {
            int coin = rand()%2;
            if (coin == 1) max_index = i;
        }
    }
    return max_index;
}

int find_min_index(int* list, int num_pos){
    int min_index = 0;
    for (int i = 0; i < num_pos; i++){
        if (list[i] < list[min_index]) min_index = i;
    }
    return min_index;
}

int find_min_value(int* list, int num_pos){
    int min_index = 0;
    for (int i = 0; i < num_pos; i++){
        if (list[i] < list[min_index]) min_index = i;
    }
    return list[min_index];
}

// int herustirc_AI(Board board, int color){
//     int* valid_array = board.valid_array();
//     int num_pos = board.num_valid_pos();
//     int grade_list[num_pos];
//     // cout << "grade list =>  ";
//     for (int i = 0; i < num_pos; i++){
//         grade_list[i] = cacl_this_pos_grade(board,valid_array[i], color);
//         // grade_list[i] = min_max_AI(board, color, 1, i, MINIMAX_DEPTH);
//     } 
//     // cout << endl;
//     // cout << "pick pos => " << valid_array[find_max_index(grade_list, num_pos)] + 1<< endl;
//     return valid_array[find_max_index(grade_list, num_pos)];
// }
void print_list(int* lst, int len){
    for (int i = 0; i < len; i++){
        cout << lst[i] << " ";
    }
    cout << endl;
}

// check any connected 4, or 3 or 2, different situations add different points 
int check_any_continues(int* list, int color, int len, int pos){
    for (int i = 0; i < len - 3; i++){
        if((has_line_three(list[i], list[i+1],list[i+2],list[i+3], color)) && (has_this_index(i, i+1, i+2, i+3, pos))){
            return 10; // line of 3 make a +10 points, in a space of 4, is there is any connect 3 and the left one is empty
        }
        else if ((has_line_two(list[i], list[i+1],list[i+2],list[i+3], color)) && (has_this_index(i, i+1, i+2, i+3, pos))){
            return 2; // same as line of 3
        }
    }
    return 0; // if there is nothing, return points 0
}

int check_opp_any_continues(int* list, int color, int len, int pos){
    for (int i = 0; i < len - 3; i++){
        if((has_line_three(list[i], list[i+1],list[i+2],list[i+3], color)) && (has_this_index(i, i+1, i+2, i+3, pos))){
            return -100; // line of 3 make a +10 points, in a space of 4, is there is any connect 3 and the left one is empty
        }
        else if ((has_line_two(list[i], list[i+1],list[i+2],list[i+3], color)) && (has_this_index(i, i+1, i+2, i+3, pos))){
            return -2; // same as line of 3
        }
    }
    return 0; // if there is nothing, return points 0
}


int find_grade_of_this_pos(Board board, int pos, int color){
    int* height_of_each_column = board.height_of_each_row();
    int height_pos = height_of_each_column[pos];
    assert(height_pos < HEIGHT);        // the input pos must be valid pos  

    int grade = 0;                      // start to calculate the grade
    if (pos == 3) grade += 5;           // if the pos is the middle column, then get +5 treats.

    int x_pos = HEIGHT - height_of_each_column[pos] - 1;    // convert to (x,y) to the chess graph
    int y_pos = pos;

    // cout << "the input => (" << x_pos << ", " << y_pos << ") " << endl;
    // make a temp board and assume the play insert the chess to here
    Board temp_board (board);
    temp_board.make_change(pos+1, color); // make change range is [1,7]
    if (temp_board.is_win()) return 10000; // if the change make the game end, means that this is the prefect move


    //check the row of the input piece

    int* changed_row = temp_board.find_exact_row_list(x_pos);
    grade += check_any_continues(changed_row, color, WIDTH, pos);
    // cout << "row points => " << grade << endl; 

    int* changed_column = temp_board.find_exact_column_list(y_pos);    
    grade += check_any_continues(changed_column, color, HEIGHT, x_pos);
    // cout << "column points => " << grade << endl; 

    if (has_left_diagonal_list(x_pos, y_pos)){
        int* left_diagonal_list = temp_board.find_exact_diagonal_list(diagonal_left_list_index(x_pos, y_pos));
        int left_diagonal_len = diagonal_left_list_length(x_pos, y_pos);
        int convert_to_left_lst_pos = convert_to_left_index(x_pos, y_pos);
        grade += check_any_continues(left_diagonal_list, color, left_diagonal_len, convert_to_left_lst_pos);
    }
    // cout << "left diagonal points =>" << grade << endl;

    if (has_right_diagonal_list(x_pos, y_pos)){
        int* right_diagonal_list = temp_board.find_exact_diagonal_list(diagonal_right_list_index(x_pos, y_pos));
        int right_diagonal_len = diagonal_right_list_length(x_pos, y_pos);
        int convert_to_right_lst_pos = convert_to_right_index(x_pos, y_pos);
        grade += check_any_continues(right_diagonal_list, color, right_diagonal_len, convert_to_right_lst_pos);
    }
    // cout << "right diagonal points =>" << grade << endl;

    // now checking if the opponent move after I place this chess, if he place it on top of me, and make it win, then bad move

    int opp_color;
    if (color == RED) opp_color = BLUE;
    else if(color == BLUE) opp_color = RED;

    if (temp_board.height_of_each_row()[pos] < 6){ // it can be placed a chess on this column 
        temp_board.make_change(pos+1, opp_color);
        temp_board.display_chess_board();
        int opp_x = x_pos - 1;
        int opp_y = y_pos;


        int* changed_row = temp_board.find_exact_row_list(opp_x);
        grade += check_opp_any_continues(changed_row, opp_color, WIDTH, opp_x);

        int* changed_column = temp_board.find_exact_column_list(y_pos);    
        grade += check_opp_any_continues(changed_column, opp_color, HEIGHT, x_pos);

        if (has_left_diagonal_list(opp_x, opp_y)){
            int* left_diagonal_list = temp_board.find_exact_diagonal_list(diagonal_left_list_index(opp_x, opp_y));
            int left_diagonal_len = diagonal_left_list_length(opp_x, opp_y);
            int convert_to_left_lst_pos = convert_to_left_index(opp_x, opp_y);
            grade += check_any_continues(left_diagonal_list, opp_color, left_diagonal_len, convert_to_left_lst_pos);
        }


        if (has_right_diagonal_list(opp_x, opp_y)){
            int* right_diagonal_list = temp_board.find_exact_diagonal_list(diagonal_right_list_index(opp_x, opp_y));
            int right_diagonal_len = diagonal_right_list_length(opp_x, opp_y);
            int convert_to_right_lst_pos = convert_to_right_index(opp_x, opp_y);
            grade += check_any_continues(right_diagonal_list, opp_color, right_diagonal_len, convert_to_right_lst_pos);
        }

    }
    return grade;
}   

int heuristic_AI (Board board, int color) {
    int* valid_pos = board.valid_array();
    int num_valid_pos = board.num_valid_pos();
    int gradelist[num_valid_pos];

    for (int i = 0; i < num_valid_pos; i++){
        gradelist[i] = find_grade_of_this_pos(board, valid_pos[i], color);
    }
    print_list(gradelist, num_valid_pos);

    return valid_pos[find_max_index(gradelist, num_valid_pos)];
}

// find the max grade for each Depth
int MiniMax_alg(Board board, int color, int pos){
    int opp_color;
    if (color == RED) opp_color = BLUE;
    else if (color == BLUE) opp_color = RED;

}





int VSherustirc(){
    Board game_board;
    cout << "––––––––––––––––––––––––––––– STARTING Connect 4 –––––––––––––––––––––––––––––\n\nvalid Position:\n" << endl;
    game_board.display_chess_board();

    init_game();

    if (user_order == 1){
        while (1){
            // required user input
            cout << "Please Pick Position from the list below : ";
            game_board.display_pos();
            cout << " => ";
            cin >> user_choice;
            while((!game_board.is_valid_pos(user_choice)) || user_choice <=0 || user_choice > 7){
                cout << "Please Pick An valid position => ";
                cin >> user_choice;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            // change the state of the board
            game_board.make_change(user_choice, user_color);
            game_board.display_chess_board();
            if (game_board.is_win()){
                cout << "You Win !!!" << endl;
                break;
            } else if (game_board.is_tie()){
                cout << "Tie GAME !!!" << endl;
                break;
            }
            cout << "pMCT_turn : \n";
            int AI_move = heuristic_AI(game_board, AI_color) + 1;
            game_board.make_change(AI_move, AI_color);

            game_board.display_chess_board();
            if (game_board.is_win()){
                cout << "AI Win !!!" << endl;
                break;
            } else if (game_board.is_tie()){
                cout << "Tie Gmae !!!" << endl;
                break;
            }
        }
    } else if (user_order == 2){
        while(!game_board.is_win()){
            cout << "pMCT_turn : \n";
            game_board.make_change(heuristic_AI(game_board, AI_color)+1, AI_color);
            game_board.display_chess_board();
            if (game_board.is_win()){
                cout << "AI Win !!!" << endl;
                break;
            } else if (game_board.is_tie()){
                cout << "Tie Game !!!" << endl;
                break;
            }
            cout << "Please Pick Position from the list below : ";
            game_board.display_pos();
            cout << " => ";
            cin >> user_choice;
            while((!game_board.is_valid_pos(user_choice)) || user_choice <=0 || user_choice > 7){
                cout << "Please Pick An valid position => ";
                cin >> user_choice;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            // change the state of the board
            game_board.make_change(user_choice, user_color);
            game_board.display_chess_board();

            if (game_board.is_win()){
                cout << "You Win !!!" << endl;
                break;
            } else if (game_board.is_tie()){
                cout << "Tie GAME !!!" << endl;
                break;
            }
        }
    }

    return 0;

}

int main(void){
    // VSherustirc();
    // Board board;
    // board.make_change(4,RED);
    // find_grade_of_this_pos(board, 3, RED);
    return 0;
}

// //1 : red 2: blue
// int VSpMCT (){
//     Board game_board;
//     cout << "––––––––––––––––––––––––––––– STARTING Connect 4 –––––––––––––––––––––––––––––\n\nvalid Position:\n" << endl;
//     game_board.display_chess_board();

//     init_game();

//     if (user_order == 1){
//         while (1){
//             // required user input
//             cout << "Plese Pick Position from the list below : ";
//             game_board.display_pos();
//             cout << " => ";
//             cin >> user_choice;
//             while((!game_board.is_valid_pos(user_choice)) || user_choice <=0 || user_choice > 7){
//                 cout << "Plese Pick An valid position => ";
//                 cin >> user_choice;
//                 cin.clear();
//                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             }
//             // change the state of the board
//             game_board.make_change(user_choice, user_color);
//             game_board.display_chess_board();
//             if (game_board.is_win()){
//                 cout << "You Win !!!" << endl;
//                 break;
//             } else if (game_board.is_tie()){
//                 cout << "Tie Gmae !!!" << endl;
//                 break;
//             }
//             cout << "pMCT_turn : \n";
//             game_board.make_change(pMCT_turn(game_board), AI_color);

//             game_board.display_chess_board();
//             if (game_board.is_win()){
//                 cout << "AI Win !!!" << endl;
//                 break;
//             } else if (game_board.is_tie()){
//                 cout << "Tie Gmae !!!" << endl;
//                 break;
//             }
//         }
//     } else if (user_order == 2){
//         while(!game_board.is_win()){
//             cout << "pMCT_turn : \n";
//             game_board.make_change(pMCT_turn(game_board), AI_color);
//             game_board.display_chess_board();
//             if (game_board.is_win()){
//                 cout << "AI Win !!!" << endl;
//                 break;
//             } else if (game_board.is_tie()){
//                 cout << "Tie Gmae !!!" << endl;
//                 break;
//             }
//             cout << "Plese Pick Position from the list below : ";
//             game_board.display_pos();
//             cout << " => ";
//             cin >> user_choice;
//             while((!game_board.is_valid_pos(user_choice)) || user_choice <=0 || user_choice > 7){
//                 cout << "Plese Pick An valid position => ";
//                 cin >> user_choice;
//                 cin.clear();
//                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             }
//             // change the state of the board
//             game_board.make_change(user_choice, user_color);
//             game_board.display_chess_board();

//             if (game_board.is_win()){
//                 cout << "You Win !!!" << endl;
//                 break;
//             } else if (game_board.is_tie()){
//                 cout << "Tie Gmae !!!" << endl;
//                 break;
//             }
//         }
//     }

//     return 0;
// }    




// int AI_battle(){
//     int num_games = MAX_NUM_GAME;
//     int pMST_win = 0;
//     int herustric_win = 0;
//     int num_tie = 0;
//     AI_color = 1;
//     user_color = 2;

//     while(num_games > 0){
//         Board game_board;
//         while(1){
//             game_board.make_change(pMCT_turn(game_board), AI_color);
//             game_board.display_chess_board();
//             if (game_board.is_win()){
//                 pMST_win++;
//                 break;
//             } else if (game_board.is_tie()){
//                 num_tie++;
//                 break;
//             }
//             int heurstic_choice = herustirc_AI(game_board, user_color);
//             cout << "herustric choice => " << heurstic_choice + 1<< endl;
//             game_board.make_change(heurstic_choice+1, user_color);
//             game_board.display_chess_board();
//             if (game_board.is_win()){
//                 herustric_win++;
//                 break;
//             } else if (game_board.is_tie()){
//                 num_tie++;
//                 break;
//             }
//         }
//         num_games--;
//     }

//     num_games = MAX_NUM_GAME;
//     int PMST_WIN_AFTER = 0;
//     int HEUR_WIN_FIRST = 0;
//     int NUM_TIE_OMST_WIN_AFTER = 0;
//     while(num_games > 0){
//         Board game_board;
//         while(1){
//             int heurstic_choice = herustirc_AI(game_board, user_color);
//             game_board.make_change(heurstic_choice+1, user_color);
//             cout << "herustric choice => " << heurstic_choice + 1<< endl;
//             game_board.display_chess_board();
//             if (game_board.is_win()){
//                 HEUR_WIN_FIRST++;
//                 break;
//             } else if (game_board.is_tie()){
//                 NUM_TIE_OMST_WIN_AFTER++;
//                 break;
//             }
//             game_board.make_change(pMCT_turn(game_board), AI_color);
//             // game_board.display_chess_board();
//             if (game_board.is_win()){
//                 PMST_WIN_AFTER++;
//                 break;
//             } else if (game_board.is_tie()){
//                 NUM_TIE_OMST_WIN_AFTER++;
//                 break;
//             }
//         }
//         num_games--;
//     }

//     cout << "END BATTLE !!!" <<endl;
//     cout << "PMST play first : TOTAL GAME = " << MAX_NUM_GAME << " , pMST Win =" << pMST_win << ", herustric Win = " << herustric_win << " Tie game = " << num_tie << endl;
//     cout << "heuristics play first : TOTAL GAME = " << MAX_NUM_GAME << " , pMST Win =" << PMST_WIN_AFTER << ", herustric Win = " << HEUR_WIN_FIRST << " Tie game = " << NUM_TIE_OMST_WIN_AFTER << endl;
//     return 0;
// }



// int main(){
//     int game_code;
//     cout << "––––––––––––––––––––––––––––– LET' GO HAVE SOME FUN –––––––––––––––––––––––––––––\n\n" << endl;
//     cout << "Please Pick Game Mode -> 1: VS. pMST AI, 2: VS. heuristic, 3: pMST vs. heuristic => ";
//     cin >> game_code;
//     while(game_code > 3 || game_code < 1){
//         cout << "Please Pick from 1 or 2 or 3 => ";
//         cin >> game_code;
//         cin.clear();
//         cin.ignore(numeric_limits<streamsize>::max(), '\n');
//     }
//     if (game_code == 1){
//         VSpMCT();
//     } else if (game_code == 2){
//         VSherustirc();
//     } else if (game_code == 3){
//         AI_battle();
//     }
// }

