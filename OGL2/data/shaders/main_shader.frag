#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec4 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;

uniform sampler2D gSamplers[2];
uniform float fTextureContributions[2];
uniform vec4 vColor;
uniform int numTextures;

#include "dirLight2.frag"
#include "spotLight.frag"
#include "pointLight.frag"

uniform DirectionalLight sunLight;
uniform SpotLight spotLight[2];
uniform PointLight pointLight[2];
uniform bool receiveLight = true;
uniform bool useShiny = false;

uniform Material matActive;

uniform struct FogParameters
{
	vec4 vFogColor; // Fog color
	float fStart; // This is only for linear fog
	float fEnd; // This is only for linear fog
	float fDensity; // For exp and exp2 equation
	
	int iEquation; // 0 = linear, 1 = exp, 2 = exp2
} fogParams;

float getFogFactor(FogParameters params, float fFogCoord)
{
	float fResult = 1.0;
	if(params.iEquation == 0)
		fResult = (params.fEnd-fFogCoord)/(params.fEnd-params.fStart);
	else if(params.iEquation == 1)
		fResult = exp(-params.fDensity*fFogCoord);
	else if(params.iEquation == 2)
		fResult = exp(-pow(params.fDensity*fFogCoord, 2.0));
		
	fResult = 1.0-clamp(fResult, 0.0, 1.0);
	
	return fResult;
}

void main()
{
	vec3 vNormalized = normalize(vNormal);
	
	vec4 vTexColor1 = texture(gSamplers[0], vTexCoord);
	vec4 vTexColor2 = texture(gSamplers[1], vTexCoord);

	vec4 vMixedTexColor = vTexColor1*fTextureContributions[0];
	if (numTextures > 1)
		 vMixedTexColor += vTexColor2*fTextureContributions[1];

	if(receiveLight)
	{
		vec4 vDirLightColor = getDirectionalLightColor(sunLight, vNormal);

		if (useShiny) 
			vDirLightColor = GetSpecularColor(vWorldPos, vec3(vEyeSpacePos), matActive,sunLight, vNormal);

		vec4 vSpotlightColor = GetSpotLightColor(spotLight[0], vWorldPos);
		vec4 vPointlightColor = getPointLightColor(pointLight[0], vWorldPos, vNormalized);
		vSpotlightColor += GetSpotLightColor(spotLight[1], vWorldPos);
		vPointlightColor += getPointLightColor(pointLight[1], vWorldPos, vNormalized);
		vMixedTexColor = vMixedTexColor*(vDirLightColor+vSpotlightColor+vPointlightColor);
	}

	float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);
	outputColor = mix(vMixedTexColor, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));
}