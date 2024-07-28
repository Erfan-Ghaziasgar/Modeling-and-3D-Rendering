# Shading of Sphere Approximated by Tetrahedrons

## Overview

This project involves rendering a sphere model using OpenGL and GLSL (OpenGL Shading Language). The sphere is constructed using recursive subdivision of a tetrahedron, and the resulting model is shaded using a custom vertex and fragment shader. The application allows for interactive manipulation of light position and color through keyboard and menu controls.

## Project Structure

The project is divided into several key components:

1. **Source Code (`Source.cpp`)**
2. **Vertex Shader (`vshader56.glsl`)**
3. **Fragment Shader (`fshader56.glsl`)**

Each component plays a specific role in achieving the final rendered output.

### 1. Source Code (`Source.cpp`)

The source code is responsible for initializing the OpenGL environment, setting up shaders, and handling user interactions. It includes functions for sphere generation, shader compilation, and rendering. Key functionalities include:

- **Sphere Generation:** The sphere is generated using recursive subdivision of a tetrahedron. This method involves dividing the faces of the tetrahedron into smaller triangles, which are then projected onto a sphere.
- **OpenGL Initialization:** The code initializes OpenGL, sets up buffer objects, and loads shaders. It configures vertex attributes and sets up lighting parameters.
- **Rendering:** The `display` function clears the screen, sets up the model-view matrix, and draws the sphere using the shader programs.
- **User Interaction:** The application provides controls for changing the light color and position. Users can interact through keyboard inputs and a right-click menu.

### 2. Vertex Shader (`vshader56.glsl`)

The vertex shader transforms vertex data and prepares it for lighting calculations.

#### Inputs and Outputs

- **Input Attributes:**

  - `in vec4 vPosition`: The position of the vertex in object space.
  - `in vec3 vNormal`: The normal vector at the vertex.

- **Output Variables:**

  - `out vec3 fN`: Transformed normal vector for the fragment shader.
  - `out vec3 fE`: View vector, representing the direction from the vertex to the camera.
  - `out vec3 fL`: Light vector, representing the direction from the vertex to the light source.

- **Uniform Variables:**
  - `uniform mat4 ModelView`: The combined model-view matrix.
  - `uniform vec4 LightPosition`: The position of the light source.
  - `uniform mat4 Projection`: The projection matrix.

#### Functionality

1. **Normal Transformation:** The vertex normal is transformed from object space to eye space and normalized.
2. **Vertex Transformation:** The vertex position is transformed from object space to eye space.
3. **View Vector Calculation:** Computes the direction from the vertex to the camera.
4. **Light Vector Calculation:** Computes the direction from the vertex to the light source, adjusting for directional or point light.
5. **Vertex Position Projection:** Transforms the vertex position to clip coordinates using the projection matrix.

### 3. Fragment Shader (`fshader56.glsl`)

The fragment shader calculates the final color of each pixel based on the lighting model.

#### Inputs and Outputs

- **Input Variables:**

  - `in vec3 fN`: Normal vector passed from the vertex shader.
  - `in vec3 fL`: Light vector passed from the vertex shader.
  - `in vec3 fE`: View vector passed from the vertex shader.

- **Output Variable:**

  - `out vec4 fColor`: The final color of the fragment.

- **Uniform Variables:**
  - `uniform vec4 AmbientProduct`: The ambient lighting component.
  - `uniform vec4 DiffuseProduct`: The diffuse lighting component.
  - `uniform vec4 SpecularProduct`: The specular lighting component.
  - `uniform vec4 LightPosition`: The position of the light source.
  - `uniform float Shininess`: The shininess factor for specular highlights.

#### Functionality

1. **Normalization:** Normalize input lighting vectors.
2. **Ambient Lighting:** Computes the ambient component of the lighting.
3. **Diffuse Lighting:** Calculates the diffuse component based on the angle between the light vector and the normal vector.
4. **Specular Lighting:** Computes the specular highlight using the shininess factor.
5. **Color Calculation:** Sums the ambient, diffuse, and specular components to determine the final color of the fragment.

------------------------------------------------------------------------------------------

### Keyboard Controls

r: Move the light source right.

l: Move the light source left.

u: Move the light source up.

d: Move the light source down.

t: Toggle between directional and point light sources.

q or ESC: Exit the application.

### Right-Click Menu

The right-click menu provides options for changing the light color and position. Users can select from the following options.

------------------------------------------------------------------------------------------

# Phong Reflection Model

The Phong Reflection Model calculates the intensity of reflected light using the following formula:

\[ I = K_a L_a + K_d L_d (\mathbf{l} \cdot \mathbf{n}) + K_s L_s (\mathbf{r} \cdot \mathbf{v})^n \]

where:

- \( I \) = Total light intensity reflected from the surface.
- \( K_a \) = Ambient reflectivity of the object.
- \( L_a \) = Ambient light intensity.
- \( K_d \) = Diffuse reflectivity of the object.
- \( L_d \) = Diffuse light intensity.
- \( \mathbf{l} \) = Light direction vector.
- \( \mathbf{n} \) = Surface normal vector.
- \( K_s \) = Specular reflectivity of the object.
- \( L_s \) = Specular light intensity.
- \( \mathbf{r} \) = Reflection vector.
- \( \mathbf{v} \) = View direction vector.
- \( n \) = Shininess factor of the material.

### Components:

1. **Ambient Reflection:**
   \[ K_a L_a \]

2. **Diffuse Reflection:**
   \[ K_d L_d (\mathbf{l} \cdot \mathbf{n}) \]

3. **Specular Reflection:**
   \[ K_s L_s (\mathbf{r} \cdot \mathbf{v})^n \]


------------------------------------------------------------------------------------------

### Summary

This project demonstrates the process of rendering a shaded sphere model using OpenGL and GLSL. It involves generating a sphere through recursive subdivision, applying vertex and fragment shaders to achieve realistic lighting, and providing interactive controls for adjusting light properties. The combination of vertex and fragment shaders ensures accurate rendering and dynamic visualization, enhancing the understanding of 3D graphics and shader programming.

The source code manages OpenGL initialization, shader setup, and user interactions, while the shaders handle the transformation of vertex data and the calculation of lighting effects. This approach illustrates the application of fundamental graphics techniques in creating interactive and visually appealing 3D models.


