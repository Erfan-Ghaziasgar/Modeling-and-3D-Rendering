#include <GL/glut.h>
#include <iostream>
#include <string>

// Full report in Report.md

const int WINDOW_WIDTH = 800;  // Window width in pixels
const int WINDOW_HEIGHT = 600; // Window height in pixels

// Racket positions and sizes
float racket1_y = 0.0f;             // Racket 1 vertical position (left)
float racket2_y = 0.0f;             // Racket 2 vertical position (right)
const float racket_width = 10.0f;   // Racket width
const float racket_height = 100.0f; // Racket height

// Ball position and speed
float ball_x = 0.0f;           // Ball horizontal position
float ball_y = 0.0f;           // Ball vertical position
float ball_dir_x = 1.0f;       // Ball horizontal direction
float ball_dir_y = 1.0f;       // Ball vertical direction
const float ball_size = 10.0f; // Ball size

// Scores
int score1 = 0;
int score2 = 0;

// Timer
int game_duration = 20; // seconds
int start_time;

// Flag to check if the game is over
bool game_over = false;

void initOpenGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2);
}

void drawRacket(float x, float y)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y - racket_height / 2);
    glVertex2f(x + racket_width, y - racket_height / 2);
    glVertex2f(x + racket_width, y + racket_height / 2);
    glVertex2f(x, y + racket_height / 2);
    glEnd();
}

void drawBall(float x, float y)
{
    glBegin(GL_QUADS);
    glVertex2f(x - ball_size / 2, y - ball_size / 2);
    glVertex2f(x + ball_size / 2, y - ball_size / 2);
    glVertex2f(x + ball_size / 2, y + ball_size / 2);
    glVertex2f(x - ball_size / 2, y + ball_size / 2);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw rackets
    drawRacket(-WINDOW_WIDTH / 2 + 20, racket1_y);
    drawRacket(WINDOW_WIDTH / 2 - 30, racket2_y);

    // Draw ball
    drawBall(ball_x, ball_y);

    // Display scores and timer
    glColor3f(1.0, 1.0, 1.0);
    std::string score_text = std::to_string(score1) + " - " + std::to_string(score2);
    glRasterPos2f(0, WINDOW_HEIGHT / 2 - 30);
    for (char c : score_text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    int elapsed_time = glutGet(GLUT_ELAPSED_TIME) / 1000 - start_time;
    int remaining_time = game_duration - elapsed_time;
    if (remaining_time <= 0)
    {
        remaining_time = 0; // Ensure time does not go negative
        game_over = true;
    }
    std::string timer_text = std::to_string(remaining_time);
    glRasterPos2f(WINDOW_WIDTH / 2 - 50, -WINDOW_HEIGHT / 2 + 20);
    for (char c : timer_text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    if (game_over)
    {
        if (score1 > score2)
        {
            std::string win_text = "Player `Left` wins!";
            glRasterPos2f(0, 0);
            for (char c : win_text)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
        else if (score2 > score1)
        {
            std::string win_text = "Player `Right` wins!";
            glRasterPos2f(0, 0);
            for (char c : win_text)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
        else
        {
            std::string win_text = "It's a draw!";
            glRasterPos2f(0, 0);
            for (char c : win_text)
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (game_over)
        return; // Do not move rackets if game is over

    switch (key)
    {
    case 'w':
        racket1_y += 10;
        break;
    case 's':
        racket1_y -= 10;
        break;
    }
    glutPostRedisplay();
}

void specialInput(int key, int x, int y)
{
    if (game_over)
        return; // Do not move rackets if game is over

    switch (key)
    {
    case GLUT_KEY_UP:
        racket2_y += 10;
        break;
    case GLUT_KEY_DOWN:
        racket2_y -= 10;
        break;
    }
    glutPostRedisplay();
}

void update(int value)
{
    if (game_over)
        return; // Stop updating if game is over

    // Update ball position
    ball_x += ball_dir_x;
    ball_y += ball_dir_y;

    // Ball collision with top and bottom
    if (ball_y + ball_size / 2 > WINDOW_HEIGHT / 2 || ball_y - ball_size / 2 < -WINDOW_HEIGHT / 2)
    {
        ball_dir_y = -ball_dir_y;
    }

    // Ball collision with rackets
    if (ball_x - ball_size / 2 < -WINDOW_WIDTH / 2 + 30 &&
        ball_y > racket1_y - racket_height / 2 &&
        ball_y < racket1_y + racket_height / 2)
    {
        ball_dir_x = -ball_dir_x;
    }
    if (ball_x + ball_size / 2 > WINDOW_WIDTH / 2 - 30 &&
        ball_y > racket2_y - racket_height / 2 &&
        ball_y < racket2_y + racket_height / 2)
    {
        ball_dir_x = -ball_dir_x;
    }

    // Ball out of bounds
    if (ball_x + ball_size / 2 > WINDOW_WIDTH / 2)
    {
        score1++;
        ball_x = ball_y = 0.0f;
    }
    if (ball_x - ball_size / 2 < -WINDOW_WIDTH / 2)
    {
        score2++;
        ball_x = ball_y = 0.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("PONG Game");

    initOpenGL();
    start_time = glutGet(GLUT_ELAPSED_TIME) / 1000;

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialInput);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
