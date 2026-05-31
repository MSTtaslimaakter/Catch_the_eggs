#include <GL/glut.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
using namespace std;

int windowWidth = 1000, windowHeight = 700;

float basketX = 440, basketY = 45;
float basketWidth = 120, basketHeight = 35;
float normalBasketWidth = 120;

float chickenX = 500;
float chickenSpeed = 3.0;

float objX, objY;
float normalObjSpeed = 4.0;
float slowObjSpeed = 1.2;
float objSpeed = 4.0;

int objType;

int score = 0;
int highScore = 0;
int timeLeft = 120;

bool paused = false;
bool gameOver = false;
bool gameStarted = false;
bool showHighScore = false;

int bigBasketTime = 0;
int slowTime = 0;

int extraTimeDropCount = 0;
int maxExtraTimeDrop = 4;

int slowDropCount = 0;
int maxSlowDrop = 6;

string formatTime(int totalSeconds)
{
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    string secText;
    if (seconds < 10)
        secText = "0" + to_string(seconds);
    else
        secText = to_string(seconds);

    return to_string(minutes) + ":" + secText;
}

void drawText(float x, float y, string text)
{
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
}

void drawBigText(float x, float y, string text)
{
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void drawBoldText(float x, float y, string text)
{
    drawBigText(x, y, text);
    drawBigText(x + 1, y, text);
    drawBigText(x, y + 1, text);
}

void drawCircle(float cx, float cy, float rx, float ry)
{
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float rad = i * 3.1416 / 180;
        glVertex2f(cx + rx * cos(rad), cy + ry * sin(rad));
    }
    glEnd();
}

void drawBackground()
{
    glColor3f(0.50, 0.80, 1.0);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(windowWidth, 0);
        glVertex2f(windowWidth, windowHeight);
        glVertex2f(0, windowHeight);
    glEnd();

    glColor3f(0.20, 0.75, 0.20);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(windowWidth, 0);
        glVertex2f(windowWidth, 55);
        glVertex2f(0, 55);
    glEnd();
}

void drawBambooStick()
{
    glColor3f(0.40, 0.70, 0.20);
    glBegin(GL_QUADS);
        glVertex2f(0, 510);
        glVertex2f(windowWidth, 510);
        glVertex2f(windowWidth, 525);
        glVertex2f(0, 525);
    glEnd();
}

void drawChicken()
{
    glColor3f(0.90, 0.45, 0.10);
    drawCircle(chickenX, 545, 35, 25);

    glColor3f(0.80, 0.30, 0.05);
    drawCircle(chickenX - 20, 550, 20, 15);

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
        glVertex2f(chickenX + 35, 548);
        glVertex2f(chickenX + 55, 555);
        glVertex2f(chickenX + 35, 540);
    glEnd();

    glColor3f(0, 0, 0);
    drawCircle(chickenX + 18, 555, 3, 3);
}

void drawBasket()
{
    glColor3f(0.60, 0.30, 0.10);
    glBegin(GL_QUADS);
        glVertex2f(basketX, basketY);
        glVertex2f(basketX + basketWidth, basketY);
        glVertex2f(basketX + basketWidth - 20, basketY + basketHeight);
        glVertex2f(basketX + 20, basketY + basketHeight);
    glEnd();

    glColor3f(0.25, 0.10, 0.03);
    glBegin(GL_LINE_LOOP);
        glVertex2f(basketX, basketY);
        glVertex2f(basketX + basketWidth, basketY);
        glVertex2f(basketX + basketWidth - 20, basketY + basketHeight);
        glVertex2f(basketX + 20, basketY + basketHeight);
    glEnd();
}

void drawObject()
{
    if (objType == 0)
    {
        glColor3f(1.0, 1.0, 0.85);
        drawCircle(objX, objY, 15, 22);
    }
    else if (objType == 1)
    {
        glColor3f(0.10, 0.30, 1.0);
        drawCircle(objX, objY, 15, 22);
    }
    else if (objType == 2)
    {
        glColor3f(1.0, 0.75, 0.0);
        drawCircle(objX, objY, 15, 22);
    }
    else if (objType == 3)
    {
        glColor3f(0.25, 0.12, 0.02);
        drawCircle(objX, objY, 18, 14);
    }
    else
    {
        if (objType == 4) glColor3f(0.80, 0.0, 1.0);
        else if (objType == 5) glColor3f(0.0, 1.0, 1.0);
        else glColor3f(0.0, 1.0, 0.0);

        glBegin(GL_QUADS);
            glVertex2f(objX - 18, objY - 18);
            glVertex2f(objX + 18, objY - 18);
            glVertex2f(objX + 18, objY + 18);
            glVertex2f(objX - 18, objY + 18);
        glEnd();

        glColor3f(0, 0, 0);
        if (objType == 4) drawText(objX - 4, objY - 4, "B");
        if (objType == 5) drawText(objX - 4, objY - 4, "S");
        if (objType == 6) drawText(objX - 4, objY - 4, "T");
    }
}

void resetObject()
{
    objX = chickenX;
    objY = 500;

    int r = rand() % 100;

    if (r < 45) objType = 0;
    else if (r < 65) objType = 1;
    else if (r < 78) objType = 2;
    else if (r < 90) objType = 3;
    else if (r < 94) objType = 4;
    else if (r < 98)
    {
        if (slowDropCount < maxSlowDrop)
        {
            objType = 5;
            slowDropCount++;
        }
        else objType = 0;
    }
    else
    {
        if (extraTimeDropCount < maxExtraTimeDrop)
        {
            objType = 6;
            extraTimeDropCount++;
        }
        else objType = 0;
    }
}

void resetGame()
{
    score = 0;
    timeLeft = 120;

    basketX = 440;
    basketWidth = normalBasketWidth;

    chickenX = 500;
    chickenSpeed = 3.0;

    objSpeed = normalObjSpeed;

    paused = false;
    gameOver = false;
    gameStarted = true;
    showHighScore = false;

    bigBasketTime = 0;
    slowTime = 0;

    extraTimeDropCount = 0;
    slowDropCount = 0;

    resetObject();
}

void checkCatch()
{
    float objLeft = objX - 15;
    float objRight = objX + 15;
    float objBottom = objY - 22;

    if (objBottom <= basketY + basketHeight &&
        objRight >= basketX &&
        objLeft <= basketX + basketWidth)
    {
        if (objType == 0)
            score += 1;

        else if (objType == 1)
            score += 5;

        else if (objType == 2)
            score += 10;

        else if (objType == 3)
            score -= 10;

        else if (objType == 4)
        {
            basketWidth = 180;
            bigBasketTime = 8;
        }

        else if (objType == 5)
        {
            slowTime = 12;
            objSpeed = slowObjSpeed;
        }

        else if (objType == 6)
        {
            timeLeft += 15;
        }

        resetObject();
    }
}
void drawMenu()
{
    drawBackground();

    glColor3f(1, 1, 1);
    drawBoldText(395, 560, "CATCH THE EGGS");

    glColor3f(0, 0, 0);
    drawBoldText(390, 480, "Press S : Start Game");
    drawBoldText(390, 440, "Press H : High Score");
    drawBoldText(390, 400, "Press ESC : Exit");

    drawText(350, 330, "Controls: A/D, Arrow Keys, or Mouse Move");
    drawText(395, 310, "P = Pause | R = Resume | M = Menu");
}

void drawHighScorePage()
{
    drawBackground();

    glColor3f(1, 1, 1);
    drawBoldText(410, 520, "HIGH SCORE");

    glColor3f(0, 0, 0);
    drawBoldText(430, 460, "Best Score: " + to_string(highScore));
    drawText(410, 400, "Press M to go back to Menu");
    drawText(410, 375, "Press S to Start Game");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (!gameStarted && !showHighScore)
    {
        drawMenu();
        glFlush();
        return;
    }

    if (showHighScore)
    {
        drawHighScorePage();
        glFlush();
        return;
    }

    drawBackground();
    drawBambooStick();
    drawChicken();
    drawBasket();
    drawObject();

    glColor3f(1, 1, 1);
    drawBoldText(20, 660, "Score: " + to_string(score));
    drawBoldText(850, 660, "Time: " + formatTime(timeLeft));

    glColor3f(0, 0, 0);
    drawText(20, 630, "Normal Egg +1 | Blue +5 | Golden +10 | Poop -10");
    drawText(20, 610, "B = Big Basket | S = Slow Egg | T = Extra Time");

    if (slowTime > 0)
    {
        glColor3f(0, 0, 1);
        drawBoldText(430, 660, "SLOW: " + to_string(slowTime));
    }

    if (bigBasketTime > 0)
    {
        glColor3f(0.5, 0, 0.7);
        drawBoldText(430, 635, "BIG: " + to_string(bigBasketTime));
    }

    if (paused)
    {
        glColor3f(1, 0, 0);
        drawBoldText(455, 360, "PAUSED");
        drawText(410, 330, "Press R to Resume | M for Menu");
    }

    if (gameOver)
    {
        glColor3f(1, 0, 0);
        drawBoldText(430, 390, "GAME OVER");
        drawBoldText(405, 360, "Final Score: " + to_string(score));
        drawBoldText(405, 330, "High Score: " + to_string(highScore));

        glColor3f(0, 0, 0);
        drawText(390, 295, "Press S to Restart | H for High Score | ESC to Exit");
    }

    glFlush();
}

void update(int value)
{
    if (gameStarted && !showHighScore && !paused && !gameOver)
    {
        chickenX += chickenSpeed;

        if (chickenX > windowWidth - 60 || chickenX < 60)
            chickenSpeed = -chickenSpeed;

        objY -= objSpeed;

        checkCatch();

        if (objY < 0)
            resetObject();
    }

    glutPostRedisplay();
    glutTimerFunc(20, update, 0);
}
