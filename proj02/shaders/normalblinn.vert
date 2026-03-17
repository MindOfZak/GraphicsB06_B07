// Blinn-Phong and Phong shared vertex shader
#version 410

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNormal;
in layout(location=2) vec2 aTexCoord;

in layout(location=3) vec3 aTangent;
in layout(location=4) vec3 aBitangent;

// M V P matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;




// out vec3 colour_vert;
//out vec3 normal;
out vec3 fragPos;
out vec2 texCoord;

// for normal mapping
out mat3 TBN;



void main()
{
    // homogeneous coordinate
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
    
    fragPos = vec3(model * vec4(aPos, 1.0));

    // convert 4x4 modelling matrix to 3x3
    mat3 normalMatrix = mat3(model);
    
    // output normal to the fragment shader
    // !! only correct for rigid body transforms
    //normal = normalMatrix * aNormal;

    texCoord = aTexCoord;

    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    TBN = mat3(T, B, N);
}