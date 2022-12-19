#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string>
using namespace std;

bool exitGame;
bool gameOver;
bool isMoving;
bool bombExplode;
bool dead;
int height;
int width;
int life = 3;
int speed;
int score;
int myCarX;
int myCarY;
int highScore;
int highestScore[5];
int enemyX[4];
int enemyY[4] = { -8, -18, -28, -38 };
int enemyPositionX;
int enemyPositionY;
int enemyNum;

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

void startUp() {
    srand(time(NULL));
    exitGame = false;
    isMoving = false;
    gameOver = false;
    dead = false;
    height = 20;
    width = 19;
    myCarX = 8;
    myCarY = 16;
    speed = 1;
    enemyNum = 4;
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

void layout() {
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
void drawMyCar() {
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

void input() {
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

void rules_logic() {
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

void died() {
    if (dead) {
        life--;
        int count = 0;
        while (count != 10) {
            input();
            BombExplosion();
            gotoxy(2, 22); 
            cout << "You got " << score << " Score!";
            count++;

        }
        gotoxy(2, 22);
        cout << "                   ";
        highestScore[life] = score;
        startUp();
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

void game_Over() {
    for (int i = 0; i < life; i++) {
        if (highestScore[i] >= highestScore[i - 1]) highScore = highestScore[i];
        else if (highestScore[i] <= highestScore[i - 1]) highScore = highestScore[i - 1];
    }

    if (life == 0) {
        gameOver = true;
        do {
            gotoxy(0, 0); cout <<  "###################";
            gotoxy(0, 1); cout <<  "#                 #";
            gotoxy(0, 2); cout <<  "#                 #";
            gotoxy(0, 3); cout <<  "#                 #";
            gotoxy(0, 4); cout <<  "#                 #";
            gotoxy(0, 5); cout <<  "#   GAME OVER!!   #";
            gotoxy(0, 6); cout <<  "#                 #";
            gotoxy(0, 7); cout <<  "#    HIGHSCORE    #";
            gotoxy(0, 8); cout <<  "#                 #";
            gotoxy(0, 9); cout <<  "#                 #";
            gotoxy(0, 10); cout << "#       " << highScore << "          #";
            gotoxy(0, 11); cout << "#                 #";
            gotoxy(0, 12); cout << "#                 #";
            gotoxy(0, 13); cout << "#                 #";
            gotoxy(0, 14); cout << "#                 #";
            gotoxy(0, 15); cout << "#    Press  'X'   #";
            gotoxy(0, 16); cout << "#     TO EXIT     #";
            gotoxy(0, 17); cout << "#                 #";
            gotoxy(0, 18); cout << "#                 #";
            gotoxy(0, 19); cout << "###################";
            gotoxy(0, 20); cout << "                   ";
            gotoxy(0, 21); cout << "                   ";
        } while (_getch() != 'x');
        exit(1);
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
            gotoxy(0, 0); cout <<  "###################";
            gotoxy(0, 1); cout <<  "#                 # ";
            gotoxy(0, 2); cout <<  "#                 #  ";
            gotoxy(0, 3); cout <<  "#                 #  ";
            gotoxy(0, 4); cout <<  "#                 #  ";
            gotoxy(0, 5); cout <<  "#                 #  ";
            gotoxy(0, 6); cout <<  "#                 #  ";
            gotoxy(0, 7); cout <<  "#                 #  ";
            gotoxy(0, 8); cout <<  "#                 #  ";
            gotoxy(0, 9); cout <<  "#                 #  ";
            gotoxy(0, 10); cout << "#                 #  ";
            gotoxy(0, 11); cout << "#                 #  ";
            gotoxy(0, 12); cout << "#                 #  ";
            gotoxy(0, 13); cout << "#                 #  ";
            gotoxy(0, 14); cout << "#                 #  ";
            gotoxy(0, 15); cout << "#                 #  ";
            gotoxy(0, 16); cout << "#                 #  ";
            gotoxy(0, 17); cout << "#                 #  ";
            gotoxy(0, 18); cout << "#                 #  ";
            gotoxy(0, 19); cout << "###################  ";
            Sleep(500);
    }

void games() {
    do {
        gotoxy(0, 0); cout <<  "###################";
        gotoxy(0, 1); cout <<  "#                 #";
        gotoxy(0, 2); cout <<  "#                 #";
        gotoxy(0, 3); cout <<  "#                 #";
        gotoxy(0, 4); cout <<  "#   RACE CAR!!!   #";
        gotoxy(0, 5); cout <<  "#                 #";
        gotoxy(0, 6); cout <<  "#                 #";
        gotoxy(0, 7); cout <<  "#                 #";
        gotoxy(0, 8); cout <<  "#                 #";
        gotoxy(0, 9); cout <<  "#                 #";
        gotoxy(0, 10); cout << "#                 #";
        gotoxy(0, 11); cout << "#                 #";
        gotoxy(0, 12); cout << "#                 #";
        gotoxy(0, 13); cout << "#                 #";
        gotoxy(0, 14); cout << "#                 #";
        gotoxy(0, 15); cout << "#  PRESS 'SPACE'  #";
        gotoxy(0, 16); cout << "#     TO START    #";
        gotoxy(0, 17); cout << "#                 #";
        gotoxy(0, 18); cout << "#                 #";
        gotoxy(0, 19); cout << "###################";
    } while (_getch() != 32);
}

int main() {
    hideCursor();
    startUp();
    splashScreen();
    spiralEffect();
    transition();
    games();

    while (!dead) {
        layout();
        input();
        rules_logic();
        drawMyCar();
        drawEnemyCar();
        died();
        game_Over();
        Sleep(500);
    }
}

