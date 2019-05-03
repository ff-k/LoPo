#version 150 core

in vec3 FragPos;
in vec3 FragColor;
in vec3 FragNormal;
in vec2 FragTexCoord;

uniform vec4 AmbientLight;

uniform sampler2D Texture0;

out vec4 FinalFragColor;

void main() {

    vec3 LightPos = vec3(0.0f, 190.0f, 0.0f);

    vec3 N = normalize(FragNormal);
    vec3 L = normalize(LightPos - FragPos);

    vec3 Diffuse = vec3(0.7f * max(dot(N, L), 0.0f));

    float ColorAlpha = AmbientLight.w;
    vec4 TexSample = texture(Texture0, FragTexCoord);
    
    float Alpha = min(ColorAlpha, TexSample.w);

    FinalFragColor = TexSample * 
                     vec4(FragColor * (AmbientLight.xyz + Diffuse), Alpha);
                     
    FinalFragColor = 0.5f*FinalFragColor + 0.5f*TexSample;
}
