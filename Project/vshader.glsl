#version 130

in vec4 vPosition;
in vec3 vNormal;

out vec3 fN; // Normal vector
out vec3 fE; // View vector
out vec3 fL; // Light vector

uniform mat4 ModelView; // ModelView matrix
uniform vec4 LightPosition; // Light position
uniform mat4 Projection; // Projection matrix

void main() {
    fN = normalize(mat3(ModelView) * vNormal); // Normal vector in eye coordinates
    vec4 eyePosition = ModelView * vPosition; // Vertex position in eye coordinates
    fE = -eyePosition.xyz; // View vector in eye coordinates

    if(LightPosition.w == 0.0) { // Directional light
        fL = normalize(LightPosition.xyz); // Light vector in eye coordinates
    } else { // Point light
        fL = normalize(LightPosition.xyz - vPosition.xyz); // Light vector in eye coordinates
    }

    gl_Position = Projection * eyePosition; // Vertex position in clip coordinates
}
