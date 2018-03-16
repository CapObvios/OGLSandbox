#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec3 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;

uniform sampler2D gSamplers[3];
uniform float fTextureContributions[3];
uniform vec4 vColor;
uniform int numTextures;

#include "dirLight.frag"

uniform DirectionalLight sunLight;

void main()
{
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor1 = texture(gSamplers[0], vTexCoord);
	vec4 vTexColor2 = texture(gSamplers[1], vTexCoord);
	vec4 vTexColor3 = texture(gSamplers[2], vTexCoord);
	vec4 vMixedTexColor = vTexColor1*fTextureContributions[0];
	if (numTextures > 1)
	{
		 vMixedTexColor += vTexColor2*fTextureContributions[1];
		 vMixedTexColor += vTexColor3*fTextureContributions[2];
	}
	vec4 vDirLightColor = getDirectionalLightColor(sunLight, vNormal);
	outputColor = vColor*vMixedTexColor*vDirLightColor;
}