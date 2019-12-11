#version 450 core

struct LightSource
{
	vec3 Position;
    vec3 Color;
    vec3 Direction;
    int Type;
    float Constant;
    float Linear;
    float Quadratic;
    float Cutoff;
    float OuterCutoff;
    float Intensity;
};

// uniforms
uniform LightSource[8] u_LightSources;
uniform vec3 u_ViewPos;
uniform bool u_UseTexture;
uniform bool u_EnableNormalMapping;

uniform sampler2D u_DiffuseMap;
uniform sampler2D u_NormalMap;

// OutColor
out vec4 FragColor;

// Constants
const float shininess = 64.0;
const vec3 diffuseColor = vec3(1.0);
const vec3 specularColor = vec3(1.0);

// Globals
vec3 g_Normal;
vec4 g_DiffuseTexel;
vec4 g_SpecularTexel;
vec4 g_ViewDir;
vec2 g_TexCoords;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;


vec3 BlinnPhong(vec3 p_LightDir, vec3 p_LightColor, float p_Luminosity)
{
    const vec3  halfwayDir          = normalize(vec4(p_LightDir, 1.0) + g_ViewDir).xyz;
    const float diffuseCoefficient  = max(dot(g_Normal, p_LightDir), 0.0);
    const float specularCoefficient = pow(max(dot(g_Normal, halfwayDir), 0.0), shininess * 2.0);

    return p_LightColor * g_DiffuseTexel.rgb * diffuseCoefficient * p_Luminosity + ((p_Luminosity > 0.0) ? (p_LightColor * g_SpecularTexel.rgb * specularCoefficient * p_Luminosity) : vec3(0.0));
}

float LuminosityFromAttenuation(LightSource p_Light)
{
    const vec3  lightPosition   = p_Light.Position;
    const float constant        = p_Light.Constant;
    const float linear          = p_Light.Linear;
    const float quadratic       = p_Light.Quadratic;

    const float distanceToLight = length(lightPosition - fs_in.FragPos);
    const float attenuation     = (constant + linear * distanceToLight + quadratic * (distanceToLight * distanceToLight));
    return 1.0 / attenuation;
}

vec3 CalcDirectionalLight(LightSource light)
{
    return BlinnPhong
    (
        -light.Direction,
        light.Color,
        1.0
    );
}

vec3 CalcPointLight(LightSource light)
{
    float luminosity = LuminosityFromAttenuation(light);
    const float intensity     = light.Intensity;
    return BlinnPhong
    (
        -light.Direction,
        light.Color,
        luminosity
    );
}

void main()
{
    g_ViewDir = vec4(normalize(u_ViewPos - fs_in.FragPos), 1.0);
    g_TexCoords = fs_in.TexCoords;

    if (u_EnableNormalMapping)
    {
        g_Normal = texture(u_NormalMap, g_TexCoords).rgb;
        g_Normal = normalize(g_Normal * 2.0 - 1.0);   
        g_Normal = normalize(fs_in.TBN * g_Normal);
    }
    else
    {
        g_Normal = normalize(fs_in.Normal);
    }

    if (u_UseTexture)
    {
        g_DiffuseTexel = texture(u_DiffuseMap, fs_in.TexCoords) * vec4(diffuseColor, 1.0);
        g_SpecularTexel = vec4(specularColor, 1.0);
    }
    else
    {
        g_DiffuseTexel = vec4(diffuseColor, 1.0);
        g_SpecularTexel = vec4(specularColor, 1.0);
    }
    
    vec3 lightSum = vec3(0.0);
    for (int i = 0; i < u_LightSources.length(); ++i)
    {
        switch (u_LightSources[i].Type)
        {
        case 1:
        {
            lightSum += CalcDirectionalLight(u_LightSources[i]);
            break;
        }
        case 2:
        {
            lightSum += CalcPointLight(u_LightSources[i]);
            break;
        }
        //case 3:
        //{
            //FragColor += CalcSpotLight(u_LightSources[i]);
            //break;
        //}
        }
    }
    
    FragColor = vec4(lightSum, g_DiffuseTexel.a);
}