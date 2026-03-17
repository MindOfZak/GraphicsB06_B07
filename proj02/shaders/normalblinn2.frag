#version 410

// for lighting
in vec3 fragPos;
// in vec3 normal;
in vec2 texCoord;

in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

//in mat3 TBN;

// uniform vec3 lightPos;
// uniform vec3 viewPos;

// LabA07 diffuse texture map only
uniform sampler2D textureMap;

// LabA08 
uniform sampler2D normalMap;

out vec4 colour_out;

void main()
{

    // manually set R G B of the surface colour, here is RED
    //vec3 colour = vec3(1.0, 0.0, 0.0);
    vec3 colour = texture(textureMap, texCoord).rgb;

    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap, texCoord).rgb;

    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0); 
    // normal = normalize(TBN * normal); 


    // 1. ambient
    vec3 ambient = 0.05 * colour;

    // 2. diffuse
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    vec3 norm = normalize(normal);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * colour;
    
    // 3. specular
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // the shininess coefficient beta = 32.0 
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);

    // assuming a light source with a bright white colour
    vec3 specular = vec3(0.3) * spec;

    // The final output fragment colour 
    // combination of ambient, diffuse and specular
    colour_out = vec4(ambient + diffuse + specular, 1.0);
}