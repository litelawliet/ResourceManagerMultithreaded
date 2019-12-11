#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextCoord; 
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} vs_out;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main()
{
    vs_out.FragPos      = vec3(u_ModelMatrix * vec4(aPos, 1.0));
    vs_out.Normal       = normalize(mat3(transpose(inverse(u_ModelMatrix))) * aNormal);
    vs_out.TexCoords    = aTextCoord;

    vs_out.TBN = mat3
    (
        normalize(vec3(u_ModelMatrix * vec4(aTangent,   0.0))),
        normalize(vec3(u_ModelMatrix * vec4(aBitangent, 0.0))),
        normalize(vec3(u_ModelMatrix * vec4(aNormal,    0.0)))
    );

    mat3 TBNi = transpose(vs_out.TBN);

    vs_out.FragPos          = vec3(u_ModelMatrix * vec4(aPos, 1.0));
    vs_out.Normal           = normalize(mat3(transpose(inverse(u_ModelMatrix))) * aNormal);
    vs_out.TexCoords        = aTextCoord;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(vs_out.FragPos, 1.0);
}