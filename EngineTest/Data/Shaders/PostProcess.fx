//====================================================================================================
//Filename: postProcess.fx
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
	AddressU	= WRAP;
	AddressV	= WRAP;
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

struct A2VPost
{
	float4 position : POSITION;
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

struct V2PPL
{
	float4 position		: POSITION;
	float3 normal		: TEXCOORD0;
	float3 dirToLight	: TEXCOORD1;
	float3 dirToEye		: TEXCOORD2;
	float2 texcoord		: TEXCOORD3;
	
};

//----------------------------------------------------------------------------------------------------

struct V2PPost
{
	float4 position		: POSITION;
	float2 texcoord		: TEXCOORD;
};


struct P2F
{
	float4 color : COLOR;
};

//====================================================================================================
// Vertex shaders
//====================================================================================================


V2PPL VSMain_PPL(A2V input)
{
	
	// initilize output
	V2PPL output = (V2PPL)0;
	
	// get local position for transform
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	float4 viewPos = mul(worldPos,ViewMatrix);
	float4 projPos = mul(viewPos,ProjMatrix);
	
	//calc dir to viewer
	float4 dirToViewer = float4(ViewPosition.xyz - worldPos.xyz,0.0f);
	dirToViewer = normalize(dirToViewer);
	
	// calc dir to light
	float4 dirToLight = float4(LightPosition.xyz - worldPos.xyz,0.0f);
	dirToLight = normalize(dirToLight);
	
	output.position		= projPos;
	output.normal		= normalize(mul(float4(input.normal.xyz, 0.0f), WorldMatrix)).xyz;
	output.dirToLight	= dirToLight.xyz;
	output.dirToEye		= dirToViewer.xyz;
	output.texcoord		= input.texcoord;
		
	return output;	
	
}
//----------------------------------------------------------------------------------------------------

V2PPost VSMain_MonoChrome(A2VPost input)
{
	// initilize output
	V2PPost output = (V2PPost)0;
	
	output.position = input.position;
	output.texcoord = input.texcoord;
	
	return output;
}

//====================================================================================================
// PixelShaders
//====================================================================================================\

//----------------------------------------------------------------------------------------------------

P2F PSMain_PPL(V2PPL input)
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
	
	output.color.rgb = (texColor * (ambient + diffuse)) + specular;
	return output;
	
}

//----------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------
P2F PSMain_MonoChrome(V2PPost input)
{
// initilize output
	P2F output = (P2F)0;
	float4 texColor = tex2D(S1, input.texcoord);
	//texColor.rgb = (texColor.r + texColor.g + texColor.b)/3.0f;
	texColor.r = 0.0f;
	texColor.b = 0.0f;
	// set output
	output.color = texColor;
	return output;
	
}


//====================================================================================================
// Techniques
//====================================================================================================

//----------------------------------------------------------------------------------------------------

technique PerPixelLighting
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSMain_PPL();
		pixelshader		= compile ps_2_0 PSMain_PPL();
	}
}

//----------------------------------------------------------------------------------------------------

technique MonoChrome
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSMain_MonoChrome();
		pixelshader		= compile ps_2_0 PSMain_MonoChrome();
		
	}
}

//----------------------------------------------------------------------------------------------------
