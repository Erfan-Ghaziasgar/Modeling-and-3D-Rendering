#include <GL/glut.h>
#include <iostream>
#include <vector>    // For vector
using namespace std; // For std::vector

int windowWidth = 800;  // Window width in pixels
int windowHeight = 600; // Window height in pixels

// Store all white point positions as pairs of x and y coordinates
vector<pair<float, float>> whitePoints;

void drawWhitePoints()
{
    glColor3f(1.0f, 1.0f, 1.0f); // White color for points
    glPointSize(10.0f);          // Set point size to 10 pixels

    glBegin(GL_POINTS);                   // Start drawing points
    for (const auto &point : whitePoints) // Loop through all white points
    {
        glVertex2f(point.first, point.second); // Draw a white point at the current position
    }
    glEnd(); // End drawing points
}

// Function to draw a triangle around each point in the vector with blank points
void drawTriangle()
{
    glColor3f(0.0f, 1.0f, 0.0f); // Green color for triangles
    glLineWidth(2.0f);            // Set line width to 2 pixels

    for (const auto &point : whitePoints) // Loop through all white points
    {
        glBegin(GL_LINE_LOOP); // Start drawing a line loop
        glVertex2f(point.first, point.second + 0.1f); // Draw the first vertex of the triangle
        glVertex2f(point.first - 0.1f, point.second - 0.1f); // Draw the second vertex of the triangle
        glVertex2f(point.first + 0.1f, point.second - 0.1f); // Draw the third vertex of the triangle
        glEnd(); // End drawing the line loop
    }

}

void mydisplay()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Black background color
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer with the current glClearColor

    // Draw all white points
    drawWhitePoints();

    // Draw a triangle around each white point
    drawTriangle();

    glFlush(); // Flush the OpenGL pipeline to the viewport
}

void mouseCallback(int button, int state, int x, int y) // Mouse callback function
{
    // If the left mouse button is pressed and the mouse is in the down state
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        // Convert mouse coordinates to OpenGL coordinates
        // static_cast<float>(x) is used to convert x to a float
        // windowWidth is the width of the window in pixels
        // 2.0f is the width of the OpenGL coordinate system
        // -1.0f is the left edge of the OpenGL coordinate system
        float whitePointX = static_cast<float>(x) / windowWidth * 2.0f - 1.0f;
        float whitePointY = 1.0f - static_cast<float>(y) / windowHeight * 2.0f;

        // Add the new white point to the vector
        whitePoints.push_back(make_pair(whitePointX, whitePointY));

        glutPostRedisplay(); // Trigger a redraw
    }
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set up the window and display mode for the window
    // GLUT_SINGLE is used to create a single-buffered window
    // GLUT_RGB is used to set the initial display mode to RGBA mode
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // Set the window size and position
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("White Points on Click");

    // Set the display callback function
    glutDisplayFunc(mydisplay);
    glutMouseFunc(mouseCallback);

    // Enter the GLUT event processing loop
    glutMainLoop();

    // Return 0 to indicate normal termination
    return 0;
}
