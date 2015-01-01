//====================================================================================================
//Filename: diffuse.vsh
// createdby matt stinnen
// description diffuse lighting vertex shader
//====================================================================================================

//====================================================================================================
//Globals
//====================================================================================================

matrix ViewMatrix;
matrix ProjMatrix;
matrix WorldMatrix;

float4 ViewPosition;
float4 LightPosition;

float4 AmbientLightIntensity;
float4 DiffuseLightIntensity;
float4 SpecularLightIntensity;

float4 AmbientMaterial;
float4 DiffuseMaterial;
float4 SpecularMaterial;

float SpecularPower;


//====================================================================================================
// structs
//====================================================================================================

struct VS_INPUT
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
};

//----------------------------------------------------------------------------------------------------

struct VS_OUTPUT
{
	float4 position : POSITION;
	float4 diffuse	: COLOR;
};

//====================================================================================================
// Main
//====================================================================================================

VS_OUTPUT Main(VS_INPUT input)
{
	
// initilize output
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	// get local position for transform
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	float4 viewPos = mul(worldPos,ViewMatrix);
	float4 projPos = mul(viewPos,ProjMatrix);
	output.position = projPos;
	
	// calculate ambient
	float4 ambient = AmbientLightIntensity * AmbientMaterial;
	
	// calculate diffuse
		// calc dir to light
		float4 dirToLight = float4(LightPosition.xyz - worldPos.xyz,0.0f);
		dirToLight = normalize(dirToLight);
		// calc d
		float4 d = saturate(dot(dirToLight,input.normal));
		
		float4 diffuse = d * DiffuseLightIntensity * DiffuseMaterial;
	
	// calculate specular
	
		//calc dir to viewer
		float4 dirToViewer = float4(ViewPosition.xyz - worldPos.xyz,0.0f);
		dirToViewer = normalize(dirToViewer);
		
		// calculate halfangle
		float4 h = float4((dirToLight.xyz + dirToViewer.xyz)/2,0.0f);
		h = normalize(h);
		float4 s = saturate(dot(h, input.normal));
		
		float4 specular = pow(s,SpecularPower) * SpecularLightIntensity * SpecularMaterial;
	
	output.diffuse = ambient + diffuse + specular; 
	
	return output;
	
}