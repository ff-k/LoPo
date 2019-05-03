#version 150 core

in vec3 FragPos;
in vec3 FragColor;
in vec3 FragNormal;
in vec2 FragTexCoord;

uniform vec4 AmbientLight;

uniform sampler2D Texture0;

out vec4 FinalFragColor;

void main() {

    vec4 TexSample = texture(Texture0, FragTexCoord);
    vec3 Albedo   = TexSample.xyz * FragColor;
    
    vec3 FinalRGB = Albedo*AmbientLight.xyz;
    float FinalAlpha = TexSample.w*AmbientLight.w;
    
    const int   PLcount = 4;
    // 0: Upper left
    // 1: Lower
    // 2: Upper left
    // 3: Platform
    
    vec3 PLcolors[PLcount] = vec3[PLcount](
        vec3(0.6f, 1.0f, 0.6f),
        vec3(0.9f, 1.0f, 0.7f),
        vec3(0.9f, 0.9f, 1.0f),
        vec3(1.0f, 0.7f, 0.6f)
    );
    
    vec3  PLps[PLcount] = vec3[PLcount](
        vec3(-14.85f, 5.8f, 16.07f),
        vec3( 18.38f, 5.8f,  1.24f),
        vec3( -6.40f, 7.5f,-19.00f),
        vec3(  0.00f, 8.4f,  0.00f)
    );
    
    
    
    float PLintensities[PLcount] = float[PLcount](
        7.75f, 7.5f, 7.25f, 1.0f
    );
    
    for(int PLidx=0; PLidx<PLcount; PLidx++){
    
        vec3  PLcolor       = PLcolors[PLidx];
        vec3  PLp           = PLps[PLidx];
        float PLintensity   = PLintensities[PLidx];
    
        vec3 Fn    = normalize(FragNormal);
        vec3 FpPLp = PLp - FragPos;
        vec3 PLd   = normalize(FpPLp);
        
        float PLdist   = max(0.001f, length(FpPLp));
        float PLweight = max(dot(Fn, PLd), 0.0f) * (PLintensity/PLdist);
    
        FinalRGB     += Albedo*PLcolor*PLweight;
    }
    
    FinalFragColor = vec4(FinalRGB, FinalAlpha);
}
