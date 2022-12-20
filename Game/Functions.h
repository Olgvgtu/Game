#pragma once

void gotoxy(int x, int y);
void hideCursor();
void startUp(bool &dead);
void input(bool &dead);
void layout();
void rules_logic(bool &dead);
void drawMyCar(bool &dead);
void drawEnemyCar();
void BombExplosion();
void died(bool &dead, int highestScore[3]);
void transition();
void spiralEffect();
void splashScreen();
void games();
void game_Over(int highestScore[3]);