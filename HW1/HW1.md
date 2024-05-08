# Detailed Explanation of White Points Drawing Application

## Libraries

The program includes the necessary libraries:

- `GL/glut.h`: This library provides OpenGL and GLUT functionalities for creating graphical applications.
- `iostream`: Standard C++ library for input/output operations.
- `vector`: Standard C++ library for dynamically-sized arrays, used here for storing white point positions.

## Global Variables

- `windowWidth` and `windowHeight`: These variables define the dimensions of the window in pixels.
- `whitePoints`: This vector stores the positions of the white points as pairs of x and y coordinates.

## Drawing Functions

1. **`drawWhitePoints()`**: This function draws white points at the positions stored in the `whitePoints` vector. It sets the color to white and the point size to 10 pixels using OpenGL commands.

2. **`drawTriangle()`**: This function draws a green triangle around each white point. It iterates through each point in the `whitePoints` vector and draws a triangle around it using OpenGL commands.

## Display Function

- **`mydisplay()`**: This function is called to render the scene. It first clears the color buffer with a black background, then calls `drawWhitePoints()` to draw the white points and `drawTriangle()` to draw triangles around them.
- Finally, it flushes the OpenGL pipeline to the viewport to display the rendered scene.

## Mouse Callback Function

- **`mouseCallback()`**: This function is called when the user interacts with the mouse. It responds to mouse clicks by converting the mouse coordinates to OpenGL coordinates and adding a new white point to the `whitePoints` vector.
- **It then triggers a redraw of the window to update the display.**

## Main Function

- Initializes GLUT and sets up the window properties such as size and position.
- Registers the display function (`mydisplay()`) and mouse callback function (`mouseCallback()`).
- Enters the GLUT event processing loop (`glutMainLoop()`), where GLUT manages user input and rendering until the program exits.

## Conclusion

This code demonstrates the usage of OpenGL and GLUT for creating a simple interactive graphical application in C++. It provides a framework for drawing white points and triangles around them based on user input through mouse clicks.
