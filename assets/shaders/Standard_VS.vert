#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

in vec3 Position;
in vec3 Color;
in vec3 Normal;
in vec3 Tangent;
in vec3 Binormal;
in vec2 TexCoord0;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;

out vec3 FragPos;
out vec3 FragColor;
out vec3 FragNormal;
out vec2 FragTexCoord;

void main() {
    gl_Position = ProjMat * ViewMat * ModelMat * vec4(Position, 1.0);

    FragPos = vec3(ModelMat * vec4(Position, 1.0));
    FragColor = Color;
    FragNormal = mat3(transpose(inverse(ModelMat))) * Normal; 
    FragTexCoord = TexCoord0;
}
