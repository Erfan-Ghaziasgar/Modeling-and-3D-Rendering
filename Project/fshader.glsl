#version 130
// per-fragment interpolated values from the vertex shader
in vec3 fN; // Normal vector
in vec3 fL; // Light vector
in vec3 fE; // View vector
out vec4 fColor; // output goes to the rasterizer
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct; // lighting products for each vertex
uniform float Shininess; // shininess exponent for the material

// main function: compute the color of the fragment
// I = Ka * La + Kd * Ld * max(N . L, 0) + Ks * Ls * max(N . H, 0)^Shininess
void main() {
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    vec3 H = normalize(L + E);
    vec4 ambient = AmbientProduct;
    float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd * DiffuseProduct;
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks * SpecularProduct;
    // discard the specular highlight if the light's behind the vertex
    if(dot(L, N) < 0.0) {
        specular = vec4(0.0, 0.0, 0.0, 1.0);
    }
    fColor = specular + diffuse + ambient; // set the fragment color
    fColor.a = 1.0; // set the alpha value to 1.0
   // gl_FragColor=specular+diffuse+ambient;// +  + ;
}
