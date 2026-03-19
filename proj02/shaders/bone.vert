#version 410

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;

layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;
	
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
	
const int MAX_BONES = 150;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
	
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;
	
void main()
{
    vec4 totalPosition = vec4(0.0f);
    vec3 totalNormal = vec3(0.0f);
    
    
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(pos,1.0f);
            totalNormal = norm;
            break;
        }
       
        mat4 boneTransform = finalBonesMatrices[boneIds[i]];
        totalPosition += (boneTransform * vec4(pos, 1.0)) * weights[i];
        totalNormal += (mat3(boneTransform) * norm) * weights[i];
    }
		

    vec4 worldPosition = model * totalPosition;
    fragPos = vec3(worldPosition);


    mat3 normalMatrix = transpose(inverse(mat3(model)));
    normal = normalize(normalMatrix * totalNormal);
    
    texCoord = tex;
    gl_Position =  projection * view * worldPosition;

    fragPos = vec3(model * vec4(pos, 1.0));
}