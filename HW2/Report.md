# Report on the PONG Game C++ OpenGL Program

## Introduction

The program is a simple implementation of the classic Pong game using OpenGL and GLUT (OpenGL Utility Toolkit). The game features basic elements such as rackets, a ball, collision detection, score tracking, and a timer to control the game duration.

## Time Calculation

Time calculation in this program is crucial for managing game duration and ensuring consistent updates. The `start_time` variable captures the start time of the game:

```cpp
start_time = glutGet(GLUT_ELAPSED_TIME) / 1000;
```

- `glutGet(GLUT_ELAPSED_TIME)` returns the elapsed time since the program started in milliseconds.
- Dividing by 1000 converts this time to seconds.

The game duration is calculated as the difference between the current time and the start time:

```cpp
game_duration = glutGet(GLUT_ELAPSED_TIME) / 1000 - start_time;
```

## Process and Execution Order

The execution of the program follows these steps:

1. **Initialization**:

   - `glutInit` initializes the GLUT library.
   - `glutInitDisplayMode` sets the initial display mode to double buffering and RGB color.
   - `glutInitWindowSize` sets the window size.
   - `glutCreateWindow` creates a window with the title "PONG Game".
   - `initOpenGL` initializes OpenGL settings such as the background color and projection mode.

2. **Setting Callbacks**:

   - `glutDisplayFunc` sets the display callback function (`display`), which is called whenever the window needs to be redrawn.
   - `glutKeyboardFunc` sets the keyboard callback function (`keyboard`), which handles keyboard input for moving the left racket.
   - `glutSpecialFunc` sets the special input callback function (`specialInput`), which handles special keys (arrow keys) for moving the right racket.
   - `glutTimerFunc` sets a timer callback (`update`), which is called every 16 milliseconds (approximately 60 times per second) to update the game state.

3. **Entering Main Loop**:
   - `glutMainLoop` enters the GLUT event processing loop, which handles rendering, input, and timer events.

## Swap Buffer

The program uses double buffering to prevent flickering and provide smooth animations. In the `display` function, the frame buffer is swapped using:

```cpp
glutSwapBuffers();
```

This swaps the back buffer (where rendering takes place) with the front buffer (displayed on the screen).

## `glutTimerFunc(16, update, 0)`

This function sets a timer to call the `update` function after 16 milliseconds. This achieves a frame rate of approximately 60 frames per second (FPS), which is a common standard for smooth animations in games.

### Update Function

The `update` function is responsible for:

- Updating the ball's position based on its direction.
- Handling collisions with the top and bottom of the window, reversing the ball's vertical direction.
- Handling collisions with the rackets, reversing the ball's horizontal direction.
- Checking if the ball goes out of bounds and updating the score accordingly.
- Redrawing the scene by calling `glutPostRedisplay`.
- Setting the timer to call `update` again after 16 milliseconds using `glutTimerFunc(16, update, 0)`.

### Main Function

The `main` function sets up the initial state of the program and starts the GLUT event loop. The key difference between `update` and `main` in the context of `glutTimerFunc` is:

- In `main`, `glutTimerFunc` is called once to start the loop.
- In `update`, `glutTimerFunc` is called repeatedly to keep the game updating at a consistent rate.

## Detailed Breakdown

### Initialization

The `main` function initializes the game environment:

```cpp
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize the GLUT library
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode to double buffering and RGB color
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // Set the window size
    glutCreateWindow("PONG Game"); // Create the window with a title

    initOpenGL(); // Initialize OpenGL settings
    start_time = glutGet(GLUT_ELAPSED_TIME) / 1000; // Record the start time

    // Set callback functions
    glutDisplayFunc(display); // Display callback
    glutKeyboardFunc(keyboard); // Keyboard input callback
    glutSpecialFunc(specialInput); // Special key input callback
    glutTimerFunc(16, update, 0); // Timer callback for game updates

    glutMainLoop(); // Enter the GLUT event processing loop
    return 0;
}
```

### OpenGL Initialization

The `initOpenGL` function sets up the initial OpenGL state:

```cpp
void initOpenGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set the background color to black
    glMatrixMode(GL_PROJECTION); // Set the projection matrix mode
    glLoadIdentity(); // Reset the projection matrix
    gluOrtho2D(-WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2); // Set the orthographic projection
}
```

### Drawing Functions

These functions handle rendering the game objects:

- `drawRacket` draws the rackets:

```cpp
void drawRacket(float x, float y)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y - racket_height / 2);
    glVertex2f(x + racket_width, y - racket_height / 2);
    glVertex2f(x + racket_width, y + racket_height / 2);
    glVertex2f(x, y + racket_height / 2);
    glEnd();
}
```

- `drawBall` draws the ball:

```cpp
void drawBall(float x, float y)
{
    glBegin(GL_QUADS);
    glVertex2f(x - ball_size / 2, y - ball_size / 2);
    glVertex2f(x + ball_size / 2, y - ball_size / 2);
    glVertex2f(x + ball_size / 2, y + ball_size / 2);
    glVertex2f(x - ball_size / 2, y + ball_size / 2);
    glEnd();
}
```

### Display Function

The `display` function handles the rendering of the game scene:

```cpp
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen

    // Draw rackets
    drawRacket(-WINDOW_WIDTH / 2 + 20, racket1_y);
    drawRacket(WINDOW_WIDTH / 2 - 30, racket2_y);

    // Draw ball
    drawBall(ball_x, ball_y);

    // Display scores and timer
    glColor3f(1.0, 1.0, 1.0);
    std::string score_text = "Score: " + std::to_string(score1) + " - " + std::to_string(score2);
    std::string time_text = "Time: " + std::to_string(game_duration - ((glutGet(GLUT_ELAPSED_TIME) / 1000) - start_time));

    // Render text (this part is commented out because the actual rendering code for text is not provided)
    // renderBitmapString(-50, WINDOW_HEIGHT / 2 - 20, GLUT_BITMAP_HELVETICA_18, score_text);
    // renderBitmapString(-50, WINDOW_HEIGHT / 2 - 40, GLUT_BITMAP_HELVETICA_18, time_text);

    glutSwapBuffers(); // Swap buffers for double buffering
}
```

### Keyboard Input Functions

The `keyboard` and `specialInput` functions handle user input:

- `keyboard` handles regular keys:

```cpp
void keyboard(unsigned char key, int x, int y)
{
    if (game_over)
        return; // Do not process input if the game is over

    switch (key)
    {
    case 'w':
        racket1_y += 10; // Move left racket up
        break;
    case 's':
        racket1_y -= 10; // Move left racket down
        break;
    }
    glutPostRedisplay(); // Redraw the scene
}
```

- `specialInput` handles special keys (arrow keys):

```cpp
void specialInput(int key, int x, int y)
{
    if (game_over)
        return; // Do not process input if the game is over

    switch (key)
    {
    case GLUT_KEY_UP:
        racket2_y += 10; // Move right racket up
        break;
    case GLUT_KEY_DOWN:
        racket2_y -= 10; // Move right racket down
        break;
    }
    glutPostRedisplay(); // Redraw the scene
}
```

### Update Function

The `update` function is the core of the game loop:

```cpp
void update(int value)
{
    if (game_over)
        return; // Stop updating if the game is over

    // Update ball position
    ball_x += ball_dir_x;
    ball_y += ball_dir_y;

    // Ball collision with top and bottom
    if (ball_y + ball_size / 2 > WINDOW_HEIGHT / 2 || ball_y - ball_size / 2 < -WINDOW_HEIGHT / 2)
    {
        ball_dir_y = -ball_dir_y; // Reverse vertical direction
    }

    // Ball collision with rackets
    if (ball_x - ball_size / 2 < -WINDOW_WIDTH / 2 + 30 &&
        ball_y > racket1_y - racket_height /

 2 &&
        ball_y < racket1_y + racket_height / 2)
    {
        ball_dir_x = -ball_dir_x; // Reverse horizontal direction
    }
    if (ball_x + ball_size / 2 > WINDOW_WIDTH / 2 - 30 &&
        ball_y > racket2_y - racket_height / 2 &&
        ball_y < racket2_y + racket_height / 2)
    {
        ball_dir_x = -ball_dir_x; // Reverse horizontal direction
    }

    // Ball out of bounds
    if (ball_x + ball_size / 2 > WINDOW_WIDTH / 2)
    {
        score1++; // Left player scores
        ball_x = ball_y = 0.0f; // Reset ball position
    }
    if (ball_x - ball_size / 2 < -WINDOW_WIDTH / 2)
    {
        score2++; // Right player scores
        ball_x = ball_y = 0.0f; // Reset ball position
    }

    glutPostRedisplay(); // Redraw the scene
    glutTimerFunc(16, update, 0); // Set the timer to call update again after 16 milliseconds
}
```

### Main Function

The `main` function brings everything together:

```cpp
int main(int argc, char **argv)
{
    glutInit(&argc, argv); // Initialize the GLUT library
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Set display mode to double buffering and RGB color
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // Set the window size
    glutCreateWindow("PONG Game"); // Create the window with a title

    initOpenGL(); // Initialize OpenGL settings
    start_time = glutGet(GLUT_ELAPSED_TIME) / 1000; // Record the start time

    // Set callback functions
    glutDisplayFunc(display); // Display callback
    glutKeyboardFunc(keyboard); // Keyboard input callback
    glutSpecialFunc(specialInput); // Special key input callback
    glutTimerFunc(16, update, 0); // Timer callback for game updates

    glutMainLoop(); // Enter the GLUT event processing loop
    return 0;
}
```

### Difference Between `update` and `main` Regarding `glutTimerFunc`

- **In `main`**: `glutTimerFunc(16, update, 0)` is called once to start the game update loop. This schedules the first call to `update` after 16 milliseconds.
- **In `update`**: `glutTimerFunc(16, update, 0)` is called repeatedly within the `update` function to continue scheduling subsequent updates every 16 milliseconds. This ensures the game logic and rendering happen approximately 60 times per second, providing a smooth gameplay experience.

## Summary

The program is a basic but functional implementation of the Pong game, showcasing fundamental concepts in game development using OpenGL and GLUT. It initializes the game environment, sets up necessary callbacks for rendering and input handling, and employs a timer mechanism to maintain a consistent frame rate for smooth animation and game logic execution.
