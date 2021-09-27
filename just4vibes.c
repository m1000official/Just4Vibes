/*
Author: Tshepang Raseobi
Date: 27 September 2021
Did it 4 the vibes man
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define SIZE 6

int drop_row;
int moves = 0;


bool checkmap(char map[][SIZE], int col, bool checkall);
void displaymap(char map[][SIZE]);
bool checkdiagonal(int row, int col, char map[SIZE][SIZE], int slope);
bool checkVertical(char map[][SIZE], int col);
bool checkHorizontal(char map[][SIZE], int row);
int main(){
    char map[SIZE][SIZE];
    for(int k = 0; k <= SIZE -1; k++){
        for(int m = 0; m <= SIZE -1; m++)
            map[k][m] = ' ';
    };
    int col;
    bool win = false;
    bool draw = false;
    displaymap(map);
    while ((win == false) && (draw == false)){
        if (moves % 2 == 0)
            printf("\nPlayer 1");
        else if (moves % 2 == 1)
            printf("\nPlayer 2");
        printf("\nAt which position do you wish to enter the ball: ");
        scanf("%d", &col);
        if (checkmap(map, col, false)){//To find the lowest row that the ball falls to simulate gravity 
            if (moves % 2 == 1)
                map[drop_row][col] = 'X';//Drops the ball. Player 2 is x
            else if (moves % 2 == 0)
                map[drop_row][col] = 'O';//Drops the ball. Player 1 is O
            moves++;//The moves are incremented is incremented
            displaymap(map);//Displays the changes in the map and show the moves that were made
        }
        /*This is for when the column is filled hence they can't drop the ball.
        It's more of a defensive programming technique*/
        else if (checkmap(map, col, false) == false){
            printf("\nOops this column is filled, try another column");
            continue;//continues to the beginning of the loop
        };

        if (checkdiagonal(drop_row, col, map, 1)){
            win = true;
        }else if (checkdiagonal(drop_row, col, map, -1)){
            win = true;
        }else if (checkVertical(map, col)){
            win = true;
        }else if (checkHorizontal(map, drop_row)){
            win = true;
        };

        if (checkmap(map, 0, true))
            continue;
        else draw = true;     
    };
    if (win){
        if ((moves -1)%2 == 0)
            printf("\nPlayer 1 wins");
        else if ((moves -1)%2 == 1)
            printf("\nPlayer 2 wins");
    }
    else if (draw)
        printf("Shmacks, it\'s a draw");
    
}

void displaymap(char map[][SIZE]){
    for(int i = 0; i <= (SIZE -1); i++){
        for(int k = 0; k <= (SIZE -1); k++){
            printf("%c ", map[i][k]);
        };
        printf("\n");
    }
}

/*The checkmap() function has two purposes:
1) To find the lowest row where the "ball" can be placed given that the user has 
picked in which column they wish to drop the ball. This value is assigned to the 
global variable drop_row

2) To find if there are any open positions on the map that the user can drop the 
ball to. If there are open positions then the game loop continues. If there aren't
then it can be assumed that the game has reached a draw
*/
bool checkmap(char map[][SIZE],int col, bool checkall){
    int i;
    bool stop;//This will be used to terminate the loop

    if (checkall == false){ //to find the lowest row/position
        i = SIZE -1;//Starting finding an open position from the lowest row to the highest row which is 0
        stop = false;
       while ((i >= 0) || (stop == false)){
            if (map[i][col] == ' '){ //If it is an open position...
                stop = true;//terminate the loop
                drop_row = i;//row where the ball will fall
                return true;//An open position was found hence return the function
            }
            else i--;//Counting upwards the array      
       }
       /*Given that i = 0 and stop is false this means that the highest row was reached
       and no open position was found. Hence the user can't drop the ball in that particular 
       column beacuse it's occupied and must enter a different column. Hence return a false value*/
       if ((i == 0) && (stop == false)){
           return false;
       }; 
    }
    else if (checkall == true){ //Find any open positions in the array/map       
        stop = false;
        int j = 0;
        /*A nested while loop checks the entire array/map until it finds an open position*/
        while ((j <= (SIZE -1)) || (stop == false)){
            i = 0;
            while ((i >= SIZE -1) || (stop == false)){
                    if (map[j][i] == ' '){
                        stop = true;
                        return true;
                    }
                    else i++;       
            };
            j++;            
        } 
        if (stop == false){//Means that there is no open position on the map and possibly a draw is reached
            return false;
        } 
    }
}
/*The checkdiagonal() is meant to check if the ball match in a diagonal pattern hence to find
out whether the user has won. xs go with xs and Os go with Os.

The function is based on the straight-line function: y = mx + c
Parameters:
row and col are the co-ordinates position of the ball that has been placed (col;row)
slope is how the program will find that matching ball in two diagonal ways. Kinda like a cross or x*/
bool checkdiagonal(int row, int col, char map[SIZE][SIZE], int slope){   
    /*min and max form a domain of x/column values between which the balls will be tested if they match*/
    int c, min, max, max_x;//c is the y-intercept

    /*The "row" increases downwards according the syntax rules of the array. Hence it's inverted such
    that it increases upward like the y-axis of the cartesian plane and allows for mathematical operations to
    occur according to the cartesian plane*/
    row = (-1 * row) + (SIZE -1);

    c = row - (slope * col);//Finding c, the y-intercept
    max_x = ((SIZE -1) -c)/slope;//This is the value of x for which y is equal to SIZE -1 or the higest row

    //The slope is positive 
    if (slope == 1){
        //When the y-intercept is between 0 and  SIZE -1 the minimum value is 0
        if ((c >= 0) && (c <= SIZE-1))
            min = 0;
        else min = -1 *c; //If this is not the case, then the x-intercept is the minium value

        if ((max_x >= 0) && (max_x <= SIZE -1))
            max = max_x;
        else max = SIZE -1;
    }
    else if (slope == -1){
        if ((max_x >= 0) && (max_x <= SIZE -1))
            min = max_x;
        else min = 0;

        /*If the x intercept is between the 0 and SIZE-1 (HINT: The array/map is a square and x-intercept = -c/slope)
        then the maximum value is the x intercept*/
        if ((c >= 0) && (c <= SIZE -1))
            max = c;
        else max = SIZE -1;//If the x-intercept is greater than the SIZE-1 then the maximum value will be SIZE-1
    };

    /*At this point the domain (x is an element of [min;max]) of the function is now set and the below section
    of the code tells the tests if the balls/points that are co-linear or form part of the function (mx + c) are identical
    or not*/
    int y, countballs = 0;//Number of matching balls
    for(int h = min; h <= max; h++){//Uses the range and the linear function to find and test the balls
        y = (slope * h) - c;
        y = (-1 * y) + (SIZE -1);//Inverted back to "array syntax" instead of "cartesian-plane synatax"
        if ((moves-1) % 2 == 1){//If player 2 was playing count the number of matching X balls
            if (map[y][h] == 'X')
                countballs++;
            else countballs = 0;
        }
        else if ((moves-1) % 2 == 0){//If player 2 was playing count the number of matching O balls
            if (map[y][h] == 'O')
                countballs++;
            else countballs = 0;
        }
    }
    if (countballs == SIZE-1){ 
        return true;
    }else return false;
}

bool checkVertical(char map[][SIZE], int col){
    int countballs = 0;
    for(int i; i <= SIZE-1; i++){
        if ((moves-1) % 2 == 1){
            if (map[i][col] == 'X'){
                countballs++;
            }
            else countballs = 0;
        }
        else if ((moves-1) % 2 == 0){
            if (map[i][col] == 'O')
                countballs++;
            else countballs = 0;
        }        
    }
    if (countballs == SIZE-1)
        return true;
    else return false;   

}

bool checkHorizontal(char map[][SIZE], int row){
    int countballs = 0;
    for(int i; i <= SIZE-1; i++){
        if (moves % 2 == 1){
            if (map[row][i] == 'X'){
                countballs++;
            }
            else countballs = 0;
        }
        else if (moves % 2 == 0){
            if (map[row][i] == 'O')
                countballs++;
            else countballs = 0;
        }        
    }
    if (countballs == SIZE-1)
        return true;
    else return false;   

}