// fragment shading of sphere model
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "math.h"
#include "vec2.h"
#include "mat2.h"

const int NumTimesToSubdivide = 6; // number of subdivisions
const int NumTriangles = 16384;	   // (4 faces)^(NumTimesToSubdivide + 1)
const int NumVertices = 3 * NumTriangles;

float dx = 0, dy = 0; 		// for light position change

vec4 points[NumVertices]; // vertices of the triangles
vec3 normals[NumVertices]; // normals of the triangles

// Model-view and projection matrices uniform location
GLuint ModelView, Projection;
GLuint InitShader(const char *vShaderFile, const char *fShaderFile);
static char *ReadShaderSource(const char *ShaderFile);
//----------------------------------------------------------------------------

int Index = 0;

void triangle(const vec4 &a, const vec4 &b, const vec4 &c)
{
	vec3 normal = normalize(cross(b - a, c - b)); // normal vector of the triangle and for each vertex

	// for each vertex of the triangle store the normal and the vertex in the points and normals array
	normals[Index] = normal;
	points[Index] = a;
	Index++;
	normals[Index] = normal;
	points[Index] = b;
	Index++;
	normals[Index] = normal;
	points[Index] = c;
	Index++;
}

//----------------------------------------------------------------------------
// normalize the vector and set the w component to 1
vec4 unit(const vec4 &p)
{
	float len = p.x * p.x + p.y * p.y + p.z * p.z;

	vec4 t;
	if (len > 0.0000001)
	{
		t = p / sqrt(len);
		t.w = 1.0;
	}

	return t;
}

// divide the triangle into 4 triangles and call the triangle function to store the vertices and normals
void divide_triangle(const vec4 &a, const vec4 &b,
					 const vec4 &c, int count)
{
	if (count > 0)
	{
		vec4 v1 = unit(a + b); // calculate the mid point of the edge and normalize it to project it on the sphere
		vec4 v2 = unit(a + c);
		vec4 v3 = unit(b + c);
		divide_triangle(a, v1, v2, count - 1); // divide the triangle into 4 triangles
		divide_triangle(c, v2, v3, count - 1);
		divide_triangle(b, v3, v1, count - 1);
		divide_triangle(v1, v3, v2, count - 1);
	}
	else
	{
		triangle(a, b, c);
	}
}

// create a tetrahedron and divide it into a sphere
void tetrahedron(int count)
{
	vec4 v[4];										   // vertices of the tetrahedron
	v[0] = vec4(0.0, 0.0, 1.0, 1.0);				   // top vertex
	v[1] = vec4(0.0, 0.942809, -0.333333, 1.0);		   // bottom vertex
	v[2] = vec4(-0.816497, -0.471405, -0.333333, 1.0); // left vertex
	v[3] = vec4(0.816497, -0.471405, -0.333333, 1.0);  // right vertex

	// divide each face of the tetrahedron into 4 triangles and again divide each triangle into 4 triangles
	// and project the vertices on the sphere
	divide_triangle(v[0], v[1], v[2], count);
	divide_triangle(v[3], v[2], v[1], count);
	divide_triangle(v[0], v[3], v[1], count);
	divide_triangle(v[0], v[2], v[3], count);
}

//----------------------------------------------------------------------------

// OpenGL initialization
mat4 model_view;
vec4 light_position(2.0 + dx, 2.0 + dy, 0.5, 0.0); // directional light source
bool isdirectional = true;						   // light type

// Initialize shader lighting parameters
vec4 light_ambient(0.2, 0.2, 0.2, 1.0);
vec4 light_diffuse(1.0, 1.0, 1.0, 1.0);
vec4 light_specular(1.0, 1.0, 1.0, 1.0);

vec4 material_ambient(0.2, 0.2, 0.2, 1.0);
vec4 material_diffuse(0.8, 0.8, 0.8, 1.0); // Neutral color
vec4 material_specular(1.0, 1.0, 1.0, 1.0);
float material_shininess = 15;

GLuint program;
void init()
{
	// Subdivide a tetrahedron into a sphere
	tetrahedron(NumTimesToSubdivide);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
				 NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
					sizeof(normals), normals);

	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
						  (const GLvoid *)sizeof(points));

	vec4 ambient_product = light_ambient * material_ambient;
	vec4 diffuse_product = light_diffuse * material_diffuse;
	vec4 specular_product = light_specular * material_specular;

	// Set up uniform variables
	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
				 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
				 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
				 1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
				 1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"),
				material_shininess);

	// Retrieve transformation uniform variable locations
	ModelView = glGetUniformLocation(program, "ModelView");
	Projection = glGetUniformLocation(program, "Projection");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 at(0.0, 0.0, 0.0, 1.0);		   // set up the object at the origin
	vec4 eye(0.0, 0.0, 2.0, 1.0);		   // set up the camera
	vec4 up(0.0, 1.0, 0.0, 0.0);		   // set up the up vector
	mat4 model_view = LookAt(eye, at, up); // set up the model-view matrix

	glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view); // set up the model-view matrix

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
				 1, light_position); // set up the light position in the shader if the light is changed

	glDrawArrays(GL_TRIANGLES, 0, NumVertices); // draw the sphere
	glutSwapBuffers();							// swap the buffers
}

//----------------------------------------------------------------------------

// light color function
void setLightColor(float r, float g, float b)
{
	light_diffuse = vec4(r, g, b, 1.0); // set up the diffuse light color and change the sphere color
	vec4 diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
				 1, diffuse_product); // set up the diffuse product in the shader

	glutPostRedisplay(); // redraw the sphere
}

//----------------------------------------------------------------------------
// color menu function
void colorMenu(int id)
{
	switch (id)
	{
	case 1:
		setLightColor(1.0, 0.0, 0.0); // Red
		break;
	case 2:
		setLightColor(0.0, 1.0, 0.0); // Green
		break;
	case 3:
		setLightColor(0.0, 0.0, 1.0); // Blue
		break;
	case 4:
		setLightColor(1.0, 1.0, 1.0); // White
		break;
	case 5:
		setLightColor(1.0, 1.0, 0.0); // Yellow
		break;
	case 6:
		setLightColor(1.0, 0.0, 1.0); // Magenta
		break;
	case 7:
		setLightColor(0.0, 1.0, 1.0); // Cyan
		break;
	}
}
// create the menu
void createMenu()
{
	glutCreateMenu(colorMenu);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("White", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Magenta", 6);
	glutAddMenuEntry("Cyan", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: // Escape Key
	case 'r':
		dx += 0.1; // move the light source in the x direction
		break;
	case 'l':
		dx -= 0.1; // move the light source in the -x direction
		break;
	case 'u':
		dy += 0.1; // move the light source in the y direction
		break;
	case 'd':
		dy -= 0.1; // move the light source in the -y direction
		break;
	case 'q':
	case 'Q':
		exit(EXIT_SUCCESS);
		break;
	// toggle light type(directional or point)
	case 't':
		isdirectional = !isdirectional; // change the light type
		break;
	}
	// set the light position and change the light type with the keyboard input
	light_position = vec4(2.0 + dx, 2.0 + dy, 2.0, isdirectional ? 0.0 : 1.0);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
// reshape function to set up the projection matrix and the viewport matrix when the window size is changed
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	GLfloat left = -2.0, right = 2.0;
	GLfloat top = 2.0, bottom = -2.0;
	GLfloat zNear = -2.0, zFar = 2.0;
	GLfloat aspect = GLfloat(width) / height; // set up the aspect ratio of the window size to keep the sphere in the correct shape

	if (aspect > 1.0)
	{
		left *= aspect;
		right *= aspect;
	}
	else
	{
		top /= aspect;
		bottom /= aspect;
	}
	// set up the projection matrix and send it to the shader
	mat4 projection = Ortho(left, right, bottom, top, zNear, zFar); // Orthographic projection
	glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);			// set up the projection matrix in the shader
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{

	glutInit(&argc, argv);									   // initialize the glut
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH); // set up the display mode
	glutInitWindowSize(512, 512);							   // set up the window size
	glutInitWindowPosition(0, 0);							   // set up the window position
	glutCreateWindow("Project_Erfan_GhaziAsgar");			   // create the window
	glewInit();												   // initialize the glew
	init();													   // initialize the program
	glutDisplayFunc(display);								   // set up the display function
	glutReshapeFunc(reshape);								   // set up the reshape function
	glutKeyboardFunc(keyboard);								   // set up the keyboard function
	createMenu();											   // create the menu
	glutMainLoop();											   // run the main loop
}

//----------------------------------------------------------------------------
// Shader
GLuint InitShader(const char *vShaderFile, const char *fShaderFile)
{
	char *svs, *sfs;
	GLuint program, VertexShader, FragmentShader;

	program = glCreateProgram();
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	svs = ReadShaderSource(vShaderFile);
	// printf("\n %s", svs);
	glShaderSource(VertexShader, 1, (const GLchar **)&svs, NULL);
	glCompileShader(VertexShader);
	glAttachShader(program, VertexShader);

	GLint compiled;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("/n failed to compile");
		GLint logSize;
		glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &logSize);
		char *logMsg = new char[logSize];
		glGetShaderInfoLog(VertexShader, logSize, NULL, logMsg);
		printf("\n  %s", logMsg);
		getchar();
		// delete[] logMsg;
		exit(EXIT_FAILURE);
	}

	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	sfs = ReadShaderSource(fShaderFile);
	// printf("\n  %s", sfs);
	glShaderSource(FragmentShader, 1, (const GLchar **)&sfs, NULL);
	glCompileShader(FragmentShader);

	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("\n failed to compile");
		GLint logSize2;
		glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &logSize2);
		char logMsg2[161]; // = new char[162];//logSize2];
		glGetShaderInfoLog(FragmentShader, logSize2, NULL, logMsg2);
		printf("\n  %s", logMsg2);
		getchar();
		// delete[] logMsg2;
		exit(EXIT_FAILURE);
	}

	glAttachShader(program, FragmentShader);
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		printf("/n failed to link");
		GLint logSize;
		glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &logSize);
		char *logMsg = new char[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		printf("\n  %s", logMsg);
		// delete[] logMsg;
		exit(EXIT_FAILURE);
	}

	glUseProgram(program);
	return program;
}
static char *ReadShaderSource(const char *ShaderFile)
{
	FILE *fp;
	fp = fopen(ShaderFile, "rt");
	if (!fp)
		return NULL;
	long size = 0;
	while (!feof(fp))
	{
		fgetc(fp);
		size++;
	}
	size--;
	fseek(fp, 0, SEEK_SET);
	char *buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = 0;
	fclose(fp);
	return buf;
}
