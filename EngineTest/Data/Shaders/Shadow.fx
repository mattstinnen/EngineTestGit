//====================================================================================================
//Filename: shadow.fx
// createdby matt stinnen
// description effect file containing the following techniques
//				shadow mapping
//====================================================================================================

//====================================================================================================
//Globals
//====================================================================================================

matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;

matrix LightViewMatrix;
matrix LightProjMatrix;

float4 ViewPosition;
float4 LightPosition;

texture Texture0;
texture Texture1;



float4 AmbientLightIntensity	= { 0.3f, 0.3f, 0.3f, 1.0f };
float4 DiffuseLightIntensity	= { 0.8f, 0.8f, 0.8f, 1.0f };
float4 SpecularLightIntensity	= { 0.5f, 0.5f, 0.5f, 1.0f };

float4 AmbientMaterial			= { 0.8f, 0.8f, 0.8f, 1.0f };
float4 DiffuseMaterial			= { 0.8f, 0.8f, 0.8f, 1.0f };
float4 SpecularMaterial			= { 0.5f, 0.5f, 0.5f, 1.0f };

float SpecularPower = 10.0f;

sampler S0 = sampler_state
{
	Texture		= (Texture0);
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
	MipFilter	= LINEAR;
	AddressU	= WRAP;
	AddressV	= WRAP;
};

sampler S1 = sampler_state
{
	Texture		= (Texture1);
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
	MipFilter	= LINEAR;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

//====================================================================================================
// structs
//====================================================================================================

struct A2V
{
	float4 position : POSITION;
	float4 normal	: NORMAL;
	float2 texcoord : TEXCOORD;
};

//----------------------------------------------------------------------------------------------------

struct V2P
{
	float4 position		: POSITION;
	float4 position2	: TEXCOORD0;
	float3 normal		: TEXCOORD1;
	float3 dirToLight	: TEXCOORD2;
	float3 dirToEye		: TEXCOORD3;
	float2 texcoord		: TEXCOORD4;
};

//----------------------------------------------------------------------------------------------------

struct P2F
{
	float4 color : COLOR;
};

//====================================================================================================
// Vertex shaders
//====================================================================================================

V2P VSShadowMap(A2V input)
{
	
	// initilize output
	V2P output = (V2P)0;
	
	// get local position for transform
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	float4 viewPos = mul(worldPos,LightViewMatrix);
	float4 projPos = mul(viewPos,LightProjMatrix);
	
	output.position = projPos;
	output.position2 = projPos;
	
	return output;	
	
}

//----------------------------------------------------------------------------------------------------

V2P VSShadowing(A2V input)
{
	
	// initilize output
	V2P output = (V2P)0;
	
	// get local position for transform
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	float4 viewPos = mul(worldPos,ViewMatrix);
	float4 projPos = mul(viewPos,ProjMatrix);
	
	// ApplyLightTransform
	float4 posViewLight = mul(worldPos,LightViewMatrix);
	float4 posProjLight = mul(posViewLight,LightProjMatrix);
	
	//calc dir to viewer
	float3 dirToViewer = ViewPosition.xyz - worldPos.xyz;
	dirToViewer = normalize(dirToViewer);
	
	// calc dir to light
	float3 dirToLight = LightPosition.xyz - worldPos.xyz;
	dirToLight = normalize(dirToLight);
	
	output.position		= projPos;
	output.position2	= posProjLight;
	output.normal		= normalize(mul(float4(input.normal.xyz, 0.0f), WorldMatrix)).xyz;
	output.dirToLight	= dirToLight;
	output.dirToEye		= dirToViewer;
	output.texcoord		= input.texcoord;
		
	return output;	
	
}

//----------------------------------------------------------------------------------------------------


//====================================================================================================
// PixelShaders
//====================================================================================================\

P2F PSShadowMap(V2P input)
{
// initilize output
	P2F output = (P2F)0;
	
	// store depth value 
	output.color = input.position2.z / input.position2.w;
	return output;
	
}

//----------------------------------------------------------------------------------------------------

P2F PSShadowing(V2P input)
{
// initilize output
	P2F output = (P2F)0;
	
	float4 texColor = tex2D(S0, input.texcoord);
	
	float3 normal = normalize(input.normal);
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToEye = normalize(input.dirToEye);
		
	// calculate ambient
	float4 ambient = AmbientLightIntensity * AmbientMaterial;
	
	// calculate diffuse
		// calc d
		float4 d = saturate(dot(dirToLight,normal));
		float4 diffuse = d * DiffuseLightIntensity * DiffuseMaterial;
	
	// calculate specular
		// calculate halfangle
		float3 h = (dirToLight + dirToEye) * 0.5f;
		h = normalize(h);
		float s = saturate(dot(h, normal));
			
		float4 specular = pow(s,SpecularPower) * SpecularLightIntensity * SpecularMaterial;
	
	// conver screen pos to tex coord
	float2 shadowUV;
	shadowUV.x = ((input.position2.x/input.position2.w) + 1.0f) * 0.5f;
	shadowUV.y = ((-input.position2.y/input.position2.w) + 1.0f) * 0.5f;
	
	
	float shadowMapDepth = tex2D(S1,shadowUV).r;
	float actualDepth = input.position2.z / input.position2.w;
	
	if(actualDepth - 0.005f > shadowMapDepth)
	{
		output.color =  texColor * ambient;	
	}
	else
	{
		output.color =  (texColor * (ambient + diffuse)) + specular;
	}
	
	
	return output;
	
}

//----------------------------------------------------------------------------------------------------



//====================================================================================================
// Techniques
//====================================================================================================


//----------------------------------------------------------------------------------------------------

technique ShadowMap
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSShadowMap();
		pixelshader		= compile ps_2_0 PSShadowMap();
	}
}

//----------------------------------------------------------------------------------------------------

technique Shadowing
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSShadowing();
		pixelshader		= compile ps_2_0 PSShadowing();
		
	}
}

//----------------------------------------------------------------------------------------------------
