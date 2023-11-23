#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 vNormal;
in vec3 vFragPos;

// texture sampler
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
//uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float specExponent;
uniform bool useDiffuse;
uniform bool useSpec;

//Directional Light
uniform vec3 dirLightDirection;
uniform vec3 dirLightColor;

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
vec3 lightDir = normalize(-dirLightDirection);
// diffuse shading
float diff = max(dot(normal, lightDir), 0.0);
// specular shading
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0),
32.0f);
// combine results
vec3 ambient = dirLightColor * 0.2 * vec3(texture(texture0,
TexCoord));
vec3 diffuse = dirLightColor * diff * vec3(texture(texture0,
TexCoord));
vec3 specular = dirLightColor * spec;

return (ambient + diffuse + specular);
}


void main()
{
	vec3 norm = normalize(vNormal);
	float distance = length(lightPos - vFragPos);
	float attenuation = 1.0 / (1.0f + 0.09f * distance + 0.032f * (distance * distance));

	//norm = texture(texture2,TexCoord).rgb;
	//norm = normalize(norm * 2.0 - 1.0);
	
	vec3 lightDir = normalize(lightPos - vFragPos);
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float specularStrength = 0.5;
	float spec = pow(max(dot(norm,halfwayDir),0.0),specExponent);
	//float spec = pow(max(dot(norm,halfwayDir),0.0),texture(texture1,TexCoord).r*32.0f);
	vec3 specular = spec * lightColor ;

	float diff = max(dot(lightDir,norm),0.0f);
	vec3 diffuse = diff * lightColor * texture(texture0, TexCoord).rgb;
	
	float ambientStr = 0.2f;
	vec3 ambient = ambientStr * lightColor * texture(texture0, TexCoord).rgb;
	
	vec3 result = ambient;
	if(useDiffuse && useSpec)
		result = ambient * attenuation + diffuse * attenuation + specular * attenuation ;
	else if (useDiffuse && !useSpec)
		result = ambient + diffuse;
	else if (!useDiffuse && useSpec)
		result = ambient + specular;
	else if (!useDiffuse && !useSpec)
		result = ambient;
	
	//result += CalcDirLight(norm,viewDir);
	FragColor = vec4(result,1.0f);
}