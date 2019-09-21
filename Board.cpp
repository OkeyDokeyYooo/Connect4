#include "Board.h"
#include <iostream>
using namespace std;

const char* red  = "🔴";
const char* blue = "🔵";

Board::Board(){
    chessboard = new int*[height];
    for (int i = 0; i < height; i++){
        chessboard[i] = new int [width];
    }

    for(int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            chessboard[i][j] = 0;
        }
    }

    valid_position = new int[width];

    for(int i = 0; i < width; i++){
        valid_position[i] = 0;
    }


    row_list = chessboard;

    column_list = new int*[width];
    for (int i = 0; i < width; i++){
        column_list[i] = new int [height];
    }

    diagonal_list = new int*[DIG_LEN];
    int diagonal_list_len = 4;
    int start_i = 0;
    while (diagonal_list_len != 7){
        int end_i = start_i + 3;
        for(;start_i <= end_i; start_i++){
            diagonal_list[start_i] = new int[diagonal_list_len];
        }
        diagonal_list_len++;
    }
}

Board::Board(const Board& bd){
    deepcopy(bd);
}

void Board::deepcopy(const Board& bd){
    //init an 2D array
    chessboard = new int*[height];
    for (int i = 0; i < height; i++){
        chessboard[i] = new int [width];
    }
    //copy all element from chessboard
    for(int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            chessboard[i][j] = bd.chessboard[i][j];
        }
    }
    //init valid_position array
    valid_position = new int[width];
    //copy element from chessboard
    for(int i = 0; i < width; i++){
        valid_position[i] = bd.valid_position[i];
    }

    row_list = chessboard;
    column_list = new int*[width];
    for (int i = 0; i < width; i++){
        column_list[i] = new int [height];
    }
    diagonal_list = new int*[DIG_LEN];
    int diagonal_list_len = 4;
    int start_i = 0;
    while (diagonal_list_len != 7){
        int end_i = start_i + 3;
        for(;start_i <= end_i; start_i++){
            diagonal_list[start_i] = new int[diagonal_list_len];
        }
        diagonal_list_len++;
    }
    update_list();
}

Board::~Board(){
    for (int i = 0; i < height; i++){
        delete[] chessboard[i];
    }
    for (int i = 0; i < width; i++){
        delete[] column_list[i];
    }
    for (int i = 0; i < DIG_LEN; i++){
        delete[] diagonal_list[i];
    }
    delete[] chessboard;
    delete[] valid_position;
    delete[] column_list;
    delete[] diagonal_list;
}

bool Board::check_row(){
    update_list();
    for (int i = 0; i < height; i++){
        for (int j = 0; j <= width - 3; j++){
            if (all_same(row_list[i][j],row_list[i][j+1],row_list[i][j+2],row_list[i][j+3]))
                return true;
        }
    }
    return false;
}

bool Board::check_column(){
    update_list();
    for (int i = 0; i < width; i++){
        for (int j = 0; j <= height-3; j++){
            // cout << column_list[i][j] << column_list[i][j+1] << column_list[i][j+2] << column_list[i][j+3] << endl;
            if (all_same(column_list[i][j],column_list[i][j+1],column_list[i][j+2],column_list[i][j+3]))
                return true;
        }
    }
    return false;
}

bool Board::check_diagonal(){
    update_list();
    int array_size = 3;
    int count = 1;
    for (int i = 0; i < DIG_LEN; i++){
        for (int j = 0; j <= array_size-3; j++){
            // cout << diagonal_list[i][j] << diagonal_list[i][j+1] << diagonal_list[i][j+2] << diagonal_list[i][j+3] << endl;
            if (all_same(diagonal_list[i][j], diagonal_list[i][j+1],diagonal_list[i][j+2], diagonal_list[i][j+3]))
                return true;
        }
        if (count % 4 == 0){
            array_size++;
        }
        count++;
    }
    return false;
}

bool Board::is_win(){
    return (check_row() || check_column() || check_diagonal());
}

bool Board::is_tie(){
    int flag = 0; // there is no empty in board
    for (int i = 0; i < height; i++){
        if (flag == 1) break;
        for (int j = 0; j < width; j++){
            if (chessboard[i][j] == 0){
                flag = 1; // there is empty in board
                break;
            }
        }
    }
    if (flag == 1) return false;

    if ((flag == 0) && (!is_win())) return true;
    else return false;
}

void Board::make_column_list(int** chessboard){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            column_list[i][j] = chessboard[j][i];
        }
    }
}

bool Board::all_same(int a, int b, int c, int d){
    if ((a == 1) && (b == 1) && (c == 1) && (d == 1)) return true;
    else if ((a == 2) && (b == 2) && (c == 2) && (d == 2)) return true;
    else return false;
}


void Board::make_diagonal_list(int** chessboard){
    // length 4
    diagonal_list[0][0] = chessboard[0][3];
    diagonal_list[0][1] = chessboard[1][2];
    diagonal_list[0][2] = chessboard[2][1];
    diagonal_list[0][3] = chessboard[3][0];

    diagonal_list[1][0] = chessboard[2][6];
    diagonal_list[1][1] = chessboard[3][5];
    diagonal_list[1][2] = chessboard[4][4];
    diagonal_list[1][3] = chessboard[5][3];

    diagonal_list[2][0] = chessboard[0][3];
    diagonal_list[2][1] = chessboard[1][4];
    diagonal_list[2][2] = chessboard[2][5];
    diagonal_list[2][3] = chessboard[3][6];

    diagonal_list[3][0] = chessboard[2][0];
    diagonal_list[3][1] = chessboard[3][1];
    diagonal_list[3][2] = chessboard[4][2];
    diagonal_list[3][3] = chessboard[5][3];
    
    //legnth 5
    diagonal_list[4][0] = chessboard[0][4];
    diagonal_list[4][1] = chessboard[1][3];
    diagonal_list[4][2] = chessboard[2][2];
    diagonal_list[4][3] = chessboard[3][1];
    diagonal_list[4][4] = chessboard[4][0];

    diagonal_list[5][0] = chessboard[1][6];
    diagonal_list[5][1] = chessboard[2][5];
    diagonal_list[5][2] = chessboard[3][4];
    diagonal_list[5][3] = chessboard[4][3];
    diagonal_list[5][4] = chessboard[5][2];

    diagonal_list[6][0] = chessboard[0][2];
    diagonal_list[6][1] = chessboard[1][3];
    diagonal_list[6][2] = chessboard[2][4];
    diagonal_list[6][3] = chessboard[3][5];
    diagonal_list[6][4] = chessboard[4][6];

    diagonal_list[7][0] = chessboard[1][0];
    diagonal_list[7][1] = chessboard[2][1];
    diagonal_list[7][2] = chessboard[3][2];
    diagonal_list[7][3] = chessboard[4][3];
    diagonal_list[7][4] = chessboard[5][4];

    //length 6
    diagonal_list[8][0] = chessboard[0][5];
    diagonal_list[8][1] = chessboard[1][4];
    diagonal_list[8][2] = chessboard[2][3];
    diagonal_list[8][3] = chessboard[3][2];
    diagonal_list[8][4] = chessboard[4][1];
    diagonal_list[8][5] = chessboard[5][0];

    diagonal_list[9][0] = chessboard[0][6];
    diagonal_list[9][1] = chessboard[1][5];
    diagonal_list[9][2] = chessboard[2][4];
    diagonal_list[9][3] = chessboard[3][3];
    diagonal_list[9][4] = chessboard[4][2];
    diagonal_list[9][5] = chessboard[5][1];

    diagonal_list[10][0] = chessboard[0][1];
    diagonal_list[10][1] = chessboard[1][2];
    diagonal_list[10][2] = chessboard[2][3];
    diagonal_list[10][3] = chessboard[3][4];
    diagonal_list[10][4] = chessboard[4][5];
    diagonal_list[10][5] = chessboard[5][6];

    diagonal_list[11][0] = chessboard[0][0];
    diagonal_list[11][1] = chessboard[1][1];
    diagonal_list[11][2] = chessboard[2][2];
    diagonal_list[11][3] = chessboard[3][3];
    diagonal_list[11][4] = chessboard[4][4];
    diagonal_list[11][5] = chessboard[5][5];
}


void Board::display_chess_board(){
    display_valid_pos();

    cout << "\t\t⎹ –––⎹ –––⎹ –––⎹ –––⎹ –––⎹ –––⎹ –––⎹ \n";
    for (int i = 0; i < height; i++){
        cout << "\t\t⎹  ";
        for (int j = 0; j < width; j++){
            if (j == width - 1){
                if(chessboard[i][j] == 1){
                    cout << red << " ⎹  ";
                }
                else if(chessboard[i][j] == 2){
                    cout << blue << " ⎹  ";

                }else {
                    cout << "  " << "⎹  ";
                }
            }
            else{
                if(chessboard[i][j] == 1){
                    cout << red << " ⎹  ";
                }
                else if(chessboard[i][j] == 2){
                    cout << blue << " ⎹  ";

                }else {
                    cout << " " << " ⎹  ";
                }
            }
        }
        cout << "\n\t\t⎹ –––⎹ –––⎹ –––⎹ –––⎹ –––⎹ –––⎹ –––⎹ \n";
    }
}

void Board::display_valid_pos(){
    cout << "\n\t\t   ";
    for(int i = 0; i < 7; i++){
        if (valid_position[i] != 6)
            cout << i+1 << "    "; 
        else
            cout << "     ";
    }
    cout << endl;
}

void Board::display_pos(){
    for(int i = 0; i < 7; i++){
        if (valid_position[i] != 6)
            if (i != 6)
                cout << i+1 << ",";
            else
                cout << i+1;
        else
            cout << "";
    }
}

bool Board::is_valid_pos(int pos){
    pos--;
    // cout << "valid position = "<< valid_position[pos] << endl;
    if (valid_position[pos] >= 6)
        return false;
    else
        return true;
}

void Board::update_list(){
    row_list = chessboard;
    make_column_list(chessboard);
    make_diagonal_list(chessboard);
}


//the pos must be valid
void Board::make_change(int pos, int color){
    pos -= 1;
    int start_i = 5;
    while(1){
        if (chessboard[start_i][pos] == 0){
            chessboard[start_i][pos] = color;
            valid_position[pos]++;
            return;
        }
        else start_i--;
    }
    update_list();
}

int* Board::valid_array(){
    update_list();
    int num_element = num_valid_pos();

    int* array = new int[num_element];
    int index = 0;
    for (int i = 0; i < width; i++){
        if(valid_position[i] >= 6) continue;
        else{
            array[index] = i;
            index++;
        }
    }
    return array;
}

int Board::num_valid_pos(){
    int num_element = 0;
    for (int i = 0; i < width; i++){
        if(valid_position[i] >= 6) continue;
        else num_element++;
    }
    return num_element;
}

int* Board::height_of_each_row(){
    return valid_position;
}

int* Board::find_exact_row_list(int height){
    update_list();
    return row_list[height];
}

int* Board::find_exact_column_list(int pos){
    update_list();
    return column_list[pos];
}

int* Board::find_exact_diagonal_list(int pos){
    update_list();
    return diagonal_list[pos];
}

