#version 330 core
#define NR_POINT_LIGHTS 4
in vec2 TexCoord;
in vec3 Normal;
in vec3 fragPos;

uniform vec3 cameraPos;
uniform vec3 lightColor;
uniform vec3 lightPos[NR_POINT_LIGHTS];
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 ambientColor;

out vec4 fragColor;

vec3 CalcPointLight(vec3 lightPos,vec3 Normal,vec3 fragPos,vec3 cameraPos){
  vec3 diffuse,specular,ambient;
    vec3 lightDir=normalize(lightPos-fragPos);
    vec3 viewDir=normalize(cameraPos-fragPos);
    vec3 norm=normalize(Normal);
    float r=length(lightPos-fragPos);

    ambient=lightColor;
    diffuse=max(dot(norm,lightDir),0.0)*lightColor;
    vec3 bisector=normalize(viewDir+lightDir);
    specular=pow(max(dot(reflect(lightDir,norm),viewDir),0.0),32)*lightColor;
    return (diffuse+specular+ambient)*diffuseColor;

}
void main()
{
     vec3 result=vec3(0.0,0.0,0.0);
    // phase 2: point lights
    for(int i = 0; i < 1; i++)
        result += CalcPointLight(lightPos[i], Normal, fragPos, cameraPos);    
    fragColor = vec4(result, 1.0);
}