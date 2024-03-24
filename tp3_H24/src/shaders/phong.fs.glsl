#version 330 core

in ATTRIB_VS_OUT
{
    vec2 texCoords;
    vec3 normal;
    vec3 lightDir[3];
    vec3 spotDir[3];
    vec3 obsPos;
} attribIn;

struct Material
{
    vec3 emission;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct UniversalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 spotDirection;
};

layout (std140) uniform LightingBlock
{
    Material mat;
    UniversalLight lights[3];
    vec3 lightModelAmbient;
    bool useBlinn;
    bool useSpotlight;
    bool useDirect3D;
    float spotExponent;
    float spotOpeningAngle;
};

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

out vec4 FragColor;

float computeSpot(in vec3 spotDir, in vec3 lightDir, in vec3 normal) {
	float cos_delta = cos(radians(spotOpeningAngle));
	float cos_gamma = dot(normalize(spotDir), lightDir);

	if (!useDirect3D) {
		if (cos_gamma > cos_delta)
			return pow(cos_gamma, spotExponent);
		else
			return 0.0;
	} else {
		float cos_inner = cos_delta;
		float cos_outer = pow(cos_delta, 1.01 + spotExponent / 2.0);

		if (cos_gamma > cos_inner)
			return 1.0;
		else if (cos_gamma < cos_outer)
			return 0.0;
		else
			return (cos_gamma - cos_outer) / (cos_inner - cos_outer);
	}
	return 0.0;
}

vec3 computeLight(in int lightIndex, in vec3 normal, in vec3 lightDir, in vec3 obsPos) {
	vec3 color = vec3(0.0);
	vec3 texDiffuse = useTexture ? texture(diffuseSampler, attribIn.texCoords).rgb : vec3(1.0);

	color += mat.ambient * lights[lightIndex].ambient * texDiffuse;

	float LdotN = dot(lightDir, normal);
	if (LdotN > 0.0) {
		float	spot = useSpotlight ?
			computeSpot(attribIn.spotDir[lightIndex], lightDir, normal) :
			1.0f;
		color += spot * mat.diffuse * lights[lightIndex].diffuse * LdotN * texDiffuse;

		float spec = 0.0;
		if (useBlinn) {
			vec3 halfVec = normalize(lightDir + obsPos);
			spec = dot(halfVec, normal);
		} else {
			vec3 reflectDir = reflect(-lightDir, normal);
			spec = dot(reflectDir, obsPos);
		}
		if (spec > 0) {
			float texSpec = useTexture ? texture(specularSampler, attribIn.texCoords).r : 1.0;
			spec = pow(spec, mat.shininess);
			color += spot * mat.specular * lights[lightIndex].specular * spec * texSpec;
		}
	}
	return color;
}

void main() {
	vec3 color = mat.emission;
	vec3 texDiffuse = vec3(1.0);
	if (useTexture) {
		vec4 tex = texture(diffuseSampler, attribIn.texCoords);
		if (tex.a < 0.3) discard;
		texDiffuse = tex.rgb;
	}
	color += mat.ambient * lightModelAmbient * texDiffuse;

	vec3 normal = normalize(attribIn.normal);
	vec3 obsPos = normalize(attribIn.obsPos);

	for (int i = 0; i < 3; ++i) {
		vec3 lightDir = normalize(attribIn.lightDir[i]);
		color += computeLight(i, normal, lightDir, obsPos);
	}

	FragColor = clamp(vec4(color, 1.0), 0.0, 1.0);
}