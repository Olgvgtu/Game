#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string>

#include "Functions.h"
bool dead;
int highestScore[3];
using namespace std;

int main() {
    hideCursor();
    startUp(dead);
    splashScreen();
    spiralEffect();
    transition();
    games();

    while (!dead) {
        layout();
        input(dead);
        rules_logic(dead);
        drawMyCar(dead);
        drawEnemyCar();
        died(dead, highestScore);
        game_Over(highestScore);
        Sleep(500);
    }
}

