#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

using namespace std;
struct cell {
    bool is_fixed=false;
    int number=0;
};
//prints soduko board
void print_board (vector< vector<cell> > &board);
//fills number in struct cell
void fill_number(vector< vector<cell> > &board, int pos_row, int pos_column, int number_to_fill, bool is_fixed);
//removes number from cell
void unfill_number(vector< vector<cell> > &board, int pos_row, int pos_column);
//takes board initial values from user
void user_input(vector< vector<cell> > &board);
//checks whether a number is in 3*3 box
void user_input1(vector< vector<cell> > &board);

bool verify_number_box(vector< vector<cell> > &board, int pos_row, int pos_column, int number_to_verify);
//checks whether a number can be put in a cell (rows, columns and box)
bool verify_number(vector< vector<cell> > &board, int pos_row, int pos_column, int number_to_verify);
//checks whether valid board
bool verify_board(vector< vector<cell> > &board);

bool brute_force(vector< vector<cell> > &board, int pos_row=0, int pos_column=0, int value_to_set=1);
//performs brute_force on the next cell in 2d vector
bool next_brute_force(vector< vector<cell> > &board, int pos_row, int pos_column, int value_to_set);
//creates a file containing the empty board
void main_file_write(vector< vector<cell> > &board);
//reads the filled board
void main_file_read(vector< vector<cell> > &board);

int main()
{
    const int number_of_rows=9;
    vector< vector<struct cell> > soduko_board(number_of_rows, vector<cell>(number_of_rows));

    main_file_write(soduko_board);
    cout << "A file called soduko_board.txt has been created in your folder.\nFill the board and save it. \nPress enter to continue.\n";
    cin.get();
    cin.clear();
    main_file_read(soduko_board);

    cout << "This is your board:" << endl;
    print_board(soduko_board);

    if (verify_board(soduko_board) && brute_force(soduko_board))
    {
        cout << "Press enter to solve." << endl;
        cin.get();
        cout << "Here is your solved board:\n";
        print_board(soduko_board);
    }


    else cout << "Your board appears to be invalid." << endl;
    return 0;
}

bool brute_force(vector< vector<cell> > &board, int pos_row, int pos_column, int value_to_set) //parameters are the 2D soduko array and position in board
{
    if (board[pos_row][pos_column].is_fixed)
    {
        //performs brute force on next cell
        return next_brute_force(board, pos_row, pos_column, value_to_set);
    }
    else
    {
        for (int i=1; i<=9; i++)
        {
            if (verify_number(board, pos_row, pos_column, i))
            {
                fill_number(board, pos_row, pos_column, i, 0);
                int result = next_brute_force(board, pos_row, pos_column, value_to_set);
                if (result)
                {
                    return result;
                }
            }
        }
        unfill_number(board, pos_row, pos_column);
    }
    return false;
}

bool next_brute_force(vector< vector<cell> > &board, int pos_row, int pos_column, int value_to_set)
{
    if (pos_column<(board.size()-1))
    {
        int result = brute_force(board, pos_row, pos_column+1, value_to_set);
        if (result) return result;
    }
    else if (pos_row<board.size()-1)
    {
        pos_column=0;
        int result = brute_force(board, pos_row+1, pos_column, value_to_set);
        if (result) return result;
    }
    else
    {
        //this condition will apply after reaching the last element on sodoko board, so returns true
        return true;
    }
}

void fill_number(vector< vector<cell> > &board, int pos_row, int pos_column, int number_to_fill, bool is_fixed)
{
    if (is_fixed) board[pos_row][pos_column].is_fixed=true;
    board[pos_row][pos_column].number=number_to_fill;
}
void unfill_number(vector< vector<cell> > &board, int pos_row, int pos_column)
{
    //if number isn't fixed, than unfill it
    if (!board[pos_row][pos_column].is_fixed) board[pos_row][pos_column].number=0;
}

bool verify_number(vector< vector<cell> > &board, int pos_row, int pos_column, int number_to_verify)
{
    //to check whether contradiction in row or column
    for (int j=0; j<board.size(); j++)
    {   //found contradiction in row or column
        if ((board[j][pos_column].number==number_to_verify) && j!=pos_row) return false;
        if ((board[pos_row][j].number==number_to_verify) && j!=pos_column) return false;
    }

    if (!verify_number_box(board, pos_row, pos_column, number_to_verify)) return false; //found contradiction in box
    else return true; //no contradiction
}

bool verify_number_box(vector< vector<cell> > &board, int pos_row, int pos_column, int number_to_verify)
{
    int box_first_row, box_first_column; //the row number and column number for the first element in a box

    if (pos_row<3)
    {
        if (pos_column<3) box_first_row=0, box_first_column=0;
        else if (pos_column<6) box_first_row=0, box_first_column=3;
        else if (pos_column<9) box_first_row=0, box_first_column=6;
    }
    else if (pos_row<6)
    {
        if (pos_column<3) box_first_row=3, box_first_column=0;
        else if (pos_column<6)box_first_row=3, box_first_column=3;
        else if (pos_column<9) box_first_row=3, box_first_column=6;
    }
    else if (pos_row<9)
    {
        if (pos_column<3) box_first_row=6, box_first_column=0;
        else if (pos_column<6) box_first_row=6, box_first_column=3;
        else if (pos_column<9) box_first_row=6, box_first_column=6;
    }

    for (int row=box_first_row; row<(box_first_row+3); row++)
    {
        for (int column=box_first_column; column<(box_first_column+3); column++)
        {
            //if contradiction found
            if (row!=pos_row && column!=pos_column && board[row][column].number==number_to_verify) return false;
        }
    }
    return true; //no contradiction found
}

bool verify_board(vector< vector<cell> > &board)
{
    for (int row=0;row<board.size();row++)
    {
        for (int column=0;column<board.size();column++)
        {
            if (board[row][column].is_fixed)
            {
                if (verify_number(board, row, column, board[row][column].number) &&
                    verify_number_box(board, row, column, board[row][column].number)) continue;
                else return false;
            }
        }
    }
    return true;
}
void main_file_write(vector< vector<cell> > &board)
{
    int number_of_rows=board.size();
    ofstream soduko_file("soduko_board.txt");
    soduko_file << "  ";
    for (int i=0;i<(number_of_rows/3);i++)
    {
        for (int j=0; j<3; j++)
            {soduko_file<< " " << (3*i)+j+1;
            }
        soduko_file << "   ";
    }
    soduko_file << "\n";
    soduko_file << "  +";
    for (int i=0; i<(number_of_rows-1); i++)
    {soduko_file << "--";}
    soduko_file << "-";
    for (int i=0;i<(number_of_rows/4);i++)
    {soduko_file << "---";}
    soduko_file << "+";
    soduko_file << "\n";
    for (int row=0; row<number_of_rows; row++)
    {
        soduko_file << row+1 << " ";
        for (int column=0; column<number_of_rows; column++)
        {
            if (board[row][column].number>0 && board[row][column].number<=9)
            soduko_file << "|" << board[row][column].number;
            else soduko_file << "|" << ".";
            if ((column+1)%3==0)
                soduko_file << "|  ";
        }
        if ((row+1)%3==0 && ((row+1)!=number_of_rows))
        {
            soduko_file << "\n";
            soduko_file << "  |";
            for (int row=0; row<(number_of_rows-1); row++)
            {soduko_file << "--";}
            soduko_file << "-";
            for (int row=0;row<(number_of_rows/4);row++)
            {soduko_file << "---";}
            soduko_file << "|";
        }
        soduko_file << "\n";
    }
    soduko_file << "  +";
    for (int i=0; i<(number_of_rows-1); i++)
    {soduko_file << "--";}
    soduko_file << "-";
    for (int i=0;i<(number_of_rows/4);i++)
    {soduko_file << "---";}
    soduko_file << "+";
    soduko_file << "\n";
    soduko_file.close();
}

void main_file_read(vector< vector<cell> > &board)
{
    string line;
    ifstream soduko_file_read("soduko_board.txt");
    int row=0;
    while (getline(soduko_file_read, line))
    {
        if (isdigit(line[0]) && line[1]==' ' && line[2]=='|')
        {
            int column=0;
            for (int i=1; i<line.length(); i++)
            {
                if (line[i]=='.')
                    column++;
                if (isdigit(line[i]))
                {
                    fill_number(board, row, column, (int) line[i]-48, true);
                    column++;
                }
            }
            row++;
        }
    }

    soduko_file_read.close();
}

void print_board (vector< vector<cell> > &board)
{
    int number_of_rows=board.size();
    cout << "  ";
    for (int i=0;i<(number_of_rows/3);i++)
    {
        for (int j=0; j<3; j++)
            {cout << " " << (3*i)+j+1;
            }
        cout << "   ";
    }
    cout << "\n";
    cout << "  +";
    for (int i=0; i<(number_of_rows-1); i++)
    {cout << "--";}
    cout << "-";
    for (int i=0;i<(number_of_rows/4);i++)
    {cout << "---";}
    cout << "+";
    cout << "\n";
    for (int row=0; row<number_of_rows; row++)
    {
        cout << row+1 << " ";
        for (int column=0; column<number_of_rows; column++)
        {
            if (board[row][column].number>0)
            cout << "|" << board[row][column].number;
            else cout << "|" << " ";
            if ((column+1)%3==0)
                cout << "|  ";
        }
        if ((row+1)%3==0 && ((row+1)!=number_of_rows))
        {
            cout << "\n";
            cout << "  |";
            for (int row=0; row<(number_of_rows-1); row++)
            {cout << "--";}
            cout << "-";
            for (int row=0;row<(number_of_rows/4);row++)
            {cout << "---";}
            cout << "|";
        }
        cout << "\n";
    }
    cout << "  +";
    for (int i=0; i<(number_of_rows-1); i++)
    {cout << "--";}
    cout << "-";
    for (int i=0;i<(number_of_rows/4);i++)
    {cout << "---";}
    cout << "+";
    cout << "\n";
}
