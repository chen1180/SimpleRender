#version 330 core
#define MAX_LIGHT 5
in vec2 TexCoord;
in vec3 Normal;
in vec3 fragPos;

struct Material{
        vec3 ka;
        vec3 kd;
        vec3 ks;
        float shiniess;
};
struct Texture{
        sampler2D ka;
        sampler2D kd;
        sampler2D ks;};
struct Light{
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 position;
};
uniform vec3 viewPos;
uniform Light light[MAX_LIGHT];
uniform Material material;
uniform Texture materialTexture;

uniform bool have_texture_ka=false;
uniform bool have_texture_kd=false;
uniform bool have_texture_ks=false;

out vec4 fragColor;

vec3 CalcPointLight(Light light,vec3 Normal,vec3 fragPos,vec3 cameraPos){
  vec3 diffuse,specular,ambient;
    vec3 lightDir=normalize(light.position-fragPos);
    vec3 viewDir=normalize(cameraPos-fragPos);
    vec3 norm=normalize(Normal);
    float r=length(light.position-fragPos);
    //Ambient
    if (have_texture_ka==true)
        ambient=light.ambient*texture2D(materialTexture.ka,TexCoord).rgb;
    else
        ambient=light.ambient*material.ka;
     //Diffuse
     if (have_texture_kd==true)
        diffuse=max(dot(norm,lightDir),0.0)*light.diffuse*texture2D(materialTexture.kd,TexCoord).rgb;
     else
        diffuse=max(dot(norm,lightDir),0.0)*light.diffuse*material.kd;
    vec3 bisector=normalize(viewDir+lightDir);
        //Specular
    if (have_texture_ks==true)
        specular=pow(max(dot(reflect(bisector,norm),viewDir),0.0),material.shiniess)*light.specular*texture2D(materialTexture.ks,TexCoord).rgb;
    else
        specular=pow(max(dot(reflect(bisector,norm),viewDir),0.0),material.shiniess)*light.specular*material.ks;
    return (diffuse+specular+ambient);
}

void main()
{
     vec3 result=vec3(0.0,0.0,0.0);
    // phase 2: point lights
   for (int i=0;i<MAX_LIGHT;i++)
        result += CalcPointLight(light[i], Normal, fragPos, viewPos);   
    fragColor = vec4(result, 1.0);
}