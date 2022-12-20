#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <string>

#include "Functions.h"
using namespace std;

bool exitGame;
bool gameOver;
bool isMoving;
bool bombExplode;
const int height = 20;
const int width = 19;
const int speed = 2;
string gameBoardArray[21];
string splashScreenText[21];
string gameOverScreen[23];
int score;
int myCarX;
int myCarY;
int highScore = 0;
int enemyX[4];
int enemyY[4] = { -8, -18, -28, -38 };
int enemyPositionX;
int enemyPositionY;
const int enemyNum = 4;

string bombParticle1[4] = { "o   o"," ooo "," ooo " ,"o   o" };

string bombParticle2[4] = { " ooo " ,"o   o","o   o"," ooo " };

string myCar[4] = { " # ","# #", " # ", "# #" };

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void rules_logic(bool &dead) {
    srand(time(NULL));

    if (isMoving) isMoving = false;
    else isMoving = true;

    for (int i = 0; i < enemyNum; i++) {
        enemyY[i]++;
    }

    for (int i = 0; i < enemyNum; i++) {
        if (enemyY[i] > 21) {
            enemyPositionX = rand() % 3;
            if (enemyPositionX == 0) enemyX[i] = 2;
            else if (enemyPositionX == 1) enemyX[i] = 8;
            else if (enemyPositionX == 2) enemyX[i] = 14;
            enemyY[i] = -8;
            score++;
        }
    }
    for (int i = 0; i < enemyNum; i++) {
        if ((myCarY <= enemyY[i] + 3 && myCarY >= enemyY[i] || enemyY[i] >= myCarY && enemyY[i] <= myCarY + 3) && myCarX == enemyX[i]) {
            dead = true;
        }
    }
}

void startUp(bool &dead) {
    srand(time(NULL));
    exitGame = false;
    isMoving = false;
    gameOver = false;
    dead = false;
    myCarX = 8;
    myCarY = 16;
    score = 0;
    for (int i = 0; i < enemyNum; i++) {
        enemyPositionX = rand() % 3;
        if (enemyPositionX == 0) enemyX[i] = 2;
        else if (enemyPositionX == 1) enemyX[i] = 8;
        else if (enemyPositionX == 2) enemyX[i] = 14;
    }
    enemyY[0] = -8;
    enemyY[1] = -18;
    enemyY[2] = -28;
    enemyY[3] = -38;

}

void layout(int life) {
    for (int i = 0; i < height; i++) {
        gotoxy(0, i); cout << "#                 #";
        if (i % 2 == 0 && isMoving) {
            gotoxy(6, i);
            cout << "|    |";
        }
        else if (i % 2 != 0 && !isMoving) {
            gotoxy(6, i);
            cout << "|    |";
        }
    }
    gotoxy(5, 21);
    cout << "Life : " << life;
    gotoxy(5, 22);
    cout << "Score: " << score;
}
void drawMyCar(bool &dead) {
    if (!dead) {
        for (int i = 0; i < 4; i++) {
            gotoxy(myCarX, myCarY + i);
            cout << myCar[i];
        }
    }
    else {
        for (int i = 0; i < 4; i++) {
            gotoxy(myCarX, myCarY + i);
            cout << "   ";
        }
    }
}

void drawEnemyCar() {
    for (int i = 0; i < enemyNum; i++) {
        if (enemyY[i] + 3 > 0) {
            gotoxy(enemyX[i], enemyY[i] + 3);
            cout << " # ";
        }if (enemyY[i] + 2 > 0) {
            gotoxy(enemyX[i], enemyY[i] + 2);
            cout << "# #";
        }if (enemyY[i] + 1 > 0) {
            gotoxy(enemyX[i], enemyY[i] + 1);
            cout << " # ";
        }if (enemyY[i] > 0) {
            gotoxy(enemyX[i], enemyY[i]);
            cout << "# #";
        }if (enemyY[i] + 3 >= 20) {
            gotoxy(enemyX[i], enemyY[i] + 3);
            cout << "   ";
        }if (enemyY[i] + 2 >= 20) {
            gotoxy(enemyX[i], enemyY[i] + 2);
            cout << "   ";
        }if (enemyY[i] + 1 >= 20) {
            gotoxy(enemyX[i], enemyY[i] + 1);
            cout << "   ";
        }if (enemyY[i] >= 20) {
            gotoxy(enemyX[i], enemyY[i]);
            cout << "   ";
        }
    }
}

void input(bool &dead) {
    if (_kbhit()) {
        switch (_getch()) {
        case 72:
            if (myCarY > 0 && !dead) myCarY = myCarY - speed;
            break;
        case 75:
            if (myCarX > 2 && !dead) myCarX = myCarX - 6;
            break;
        case 77:
            if (myCarX < 14 && !dead) myCarX = myCarX + 6;
            break;
        case 80:
            if (myCarY < 16 && !dead) myCarY = myCarY + speed;
            break;
        }
    }
}

void BombExplosion() {
    if (bombExplode) {
        for (int i = 0; i < 4; i++) {
            gotoxy(myCarX - 1, myCarY - 1 + i);
            cout << bombParticle1[i] << endl;

        }
        bombExplode = false;

    }
    else {
        for (int i = 0; i < 4; i++) {
            gotoxy(myCarX - 1, myCarY - 1 + i);
            cout << bombParticle2[i] << endl;

        }
        bombExplode = true;

    }
    Sleep(100);
}

void died(bool &dead, int highestScore[], int &life) {
    if (dead) {
        life--;
        int count = 0;
        while (count != 10) {
            input(dead);
            BombExplosion();
            gotoxy(2, 22);
            cout << "You got " << score << " Score!";
            count++;

        }
        gotoxy(2, 22);
        cout << "                   ";
        highestScore[life] = score;
        startUp(dead);
    }
}

void transition() {
    for (int i = 19; i >= 0; i--) {
        gotoxy(1, i);
        cout << "##################";
        Sleep(15);
    }
    for (int i = 1; i < 20; i++) {
        gotoxy(1, i);
        cout << "                 ";
        Sleep(15);
    }
}

void game_Over(int highestScore[], int life) {
    highScore = 0;
    for (int i = 0; i < 3; i++) {
        if (highestScore[i] > highScore) highScore = highestScore[i];
    }

    if (life == 0) {
        fstream myFile;
        myFile.open("gameOver.txt", ios::in);
        gameOver = true;
        do {
            for (int i = 0; i <= 22; i++) {
                if (i == 10) {}
                else getline(myFile, gameOverScreen[i]);
            }
            for (int i = 0; i <= 22; i++) {
                if (i == 10) { 
                    gotoxy(0, 10); cout << "#       " << highScore << "         #"; 
                }
                else {
                    string* gameOverTextLocation = &gameOverScreen[i];
                    gotoxy(0, i); cout << *gameOverTextLocation;
                }
            }
        } while (_getch() != 'x');
        exit(1);
        myFile.close();
    }
}

void spiralEffect() {
    int row = 1, col = 1;
    int last_row = height - 2;
    int last_col = width - 2;

    while (row <= last_row && col <= last_col) {
        for (int i = col; i <= last_col; i++) {
            gotoxy(i, row);
            cout << "#";
            Sleep(2);
        } row++;

        for (int i = row; i <= last_row; i++) {
            gotoxy(last_col, i);
            cout << "#";
        }last_col--;

        if (row <= last_row) {
            for (int i = last_col; i >= col; i--) {
                gotoxy(i, last_row);
                cout << "#";
                Sleep(1);
            }last_row--;
        }
        if (col <= last_col) {
            for (int i = last_row; i >= row; i--) {
                gotoxy(col, i);
                cout << "#";
            } col++;
        }
    }

    row = 1, col = 1;
    last_row = height - 2, last_col = width - 2;

    while (row <= last_row && col <= last_col) {
        for (int i = col; i <= last_col; i++) {
            gotoxy(i, row);
            cout << " ";
            Sleep(2);
        } row++;

        for (int i = row; i <= last_row; i++) {
            gotoxy(last_col, i);
            cout << " ";
        }last_col--;

        if (row <= last_row) {
            for (int i = last_col; i >= col; i--) {
                gotoxy(i, last_row);
                cout << " ";
                Sleep(1);
            }last_row--;
        }
        if (col <= last_col) {
            for (int i = last_row; i >= row; i--) {
                gotoxy(col, i);
                cout << " ";
            } col++;
        }
    }
}
void splashScreen() {
    fstream myFile;
    myFile.open("splashScreen.txt", ios::in);
    for (int i = 0; i <= 20; i++) {
        getline(myFile, splashScreenText[i]);
    }
    for (int i = 0; i <= 20; i++) {
        string* screeTextLocation = &splashScreenText[i];
        gotoxy(0, i); cout << *screeTextLocation;
    }
    myFile.close();
    Sleep(500);
}

void games() {
    fstream myFile;
    myFile.open("gameBoard.txt", ios::in);
    for (int i = 0; i <= 20; i++) {
        getline(myFile, gameBoardArray[i]);
    }

    do {
        for (int i = 0; i <= 20; i++) {
            string* gameBoard = &gameBoardArray[i];
            gotoxy(0, i); cout << *gameBoard;
        }
        
    } while (_getch() != 32);
    myFile.close();
}
