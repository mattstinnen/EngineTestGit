//====================================================================================================
//Filename: Effex.fx
// createdby matt stinnen
// description effect file containing the following techniques
//					a) FFPTechnique - preforms lighting and texturing with fixed function pipeline
//					b) PPTechnique - preforms lighting and texturing with vertex and pixel shaders
//					c) PerPixelLighting - preforms per-pixel lightig with bertex and pixel shaders
//====================================================================================================

//====================================================================================================
//Globals
//====================================================================================================

matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;

float4 ViewPosition;

texture Texture0;
texture Texture1;

float4 LightPosition = { -5.0f, 5.0f, -5.0f, 1.0f };

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

//----------------------------------------------------------------------------------------------------

struct V2P
{
	float4 position : POSITION;
	float4 diffuse	: COLOR0;
	float4 specular	: COLOR1;
	float2 texcoord : TEXCOORD;
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

struct P2F
{
	float4 color : COLOR;
};

//====================================================================================================
// Vertex shaders
//====================================================================================================

V2P VSMain(A2V input)
{
	
	// initilize output
	V2P output = (V2P)0;
	
	// get local position for transform
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	float4 viewPos = mul(worldPos,ViewMatrix);
	float4 projPos = mul(viewPos,ProjMatrix);
	output.position = projPos;
	
	float4 worldNorm = mul(float4(input.normal.xyz, 0.0f), WorldMatrix);
	
	// calculate ambient
	float4 ambient = AmbientLightIntensity * AmbientMaterial;
	
	// calculate diffuse
		// calc dir to light
		float4 dirToLight = float4(LightPosition.xyz - worldPos.xyz, 0.0f);
		dirToLight = normalize(dirToLight);
		// calc d
		float4 d = saturate(dot(dirToLight, worldNorm));
		
		float4 diffuse = d * DiffuseLightIntensity * DiffuseMaterial;
	
	// calculate specular
	
		//calc dir to viewer
		float4 dirToViewer = float4(ViewPosition.xyz - worldPos.xyz,0.0f);
		dirToViewer = normalize(dirToViewer);
		
		// calculate halfangle
		float4 h = float4((dirToLight.xyz + dirToViewer.xyz)/2,0.0f);
		h = normalize(h);
		float4 s = saturate(dot(h, worldNorm));
		
		float4 specular = pow(s,SpecularPower) * SpecularLightIntensity * SpecularMaterial;
	
	output.diffuse	= ambient + diffuse; 
	output.specular = specular;
	output.texcoord = input.texcoord;
	return output;	
	
}

//----------------------------------------------------------------------------------------------------

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


//====================================================================================================
// PixelShaders
//====================================================================================================\

P2F PSMain(V2P input)
{
// initilize output
	P2F output = (P2F)0;
	
	float4 texColor = tex2D(S0, input.texcoord);
	
	output.color = (texColor * input.diffuse) + input.specular;
	return output;
	
}

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
	
	output.color =  (texColor * (ambient + diffuse)) + specular;
	return output;
	
}

//----------------------------------------------------------------------------------------------------

P2F PSMain_SM(V2PPL input)
{
// initilize output
	P2F output = (P2F)0;
	
	float4 texColor = tex2D(S0, input.texcoord);
	float4 specColor = tex2D(S1, input.texcoord);
	
	float3 normal = normalize(input.normal);
	float3 dirToLight = normalize(input.dirToLight);
	float3 dirToEye = normalize(input.dirToEye);
		
	// calculate ambient
	float4 ambient = AmbientLightIntensity * AmbientMaterial;
	
	// calculate diffuse
		// calc d
		float d = saturate(dot(dirToLight,normal));
		float4 diffuse = d * DiffuseLightIntensity * DiffuseMaterial;
	
	
	// calculate specular
		// calculate halfangle
		float3 h = (dirToLight + dirToEye) * 0.5f;
		h = normalize(h);
		float s = saturate(dot(h, normal));
			
		float4 specular = pow(s,SpecularPower) * SpecularLightIntensity * SpecularMaterial;
	
	texColor = texColor * (ambient + diffuse);
	specColor = specColor * specular;

	output.color = texColor + specColor;
	
	return output;
	
}

//====================================================================================================
// Techniques
//====================================================================================================

technique FFP
{
	pass P0
	{
		// shaders
		vertexshader		= null;
		pixelshader			= null;
		
		// render states
		FVF					= XYZ | Normal | Tex1;
		Lighting			= true;
		NormalizeNormals	= true;
		SpecularEnable		= true;
		
		// transforms
		WorldTransform[0]	= (WorldMatrix);
		ViewTransform		= (ViewMatrix);
		ProjectionTransform	= (ProjMatrix);
		
		//SetLight
		LightType[0]		= Point;
		LightAmbient[0]		= (AmbientLightIntensity);
		LightDiffuse[0]		= (DiffuseLightIntensity);
		LightSpecular[0]	= (SpecularLightIntensity);
		LightPosition[0]	= (LightPosition);
		LightRange[0]		= 100.0f;
		LightFalloff[0]		= 100.0f;
		LightAttenuation0[0] = 1.0f;
		
		//enable light
		LightEnable[0]		= true;
		
		// set material
		
		MaterialAmbient		= (AmbientMaterial);
		MaterialDiffuse		= (DiffuseMaterial);
		MaterialSpecular	= (SpecularMaterial);
		MaterialPower		= (SpecularPower);
		
		//set texture
		Sampler[0]			= (S0);
	}
}

//----------------------------------------------------------------------------------------------------

technique PP
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSMain();
		pixelshader		= compile ps_2_0 PSMain();
	}
}

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

technique SpecularMap
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSMain_PPL();
		pixelshader		= compile ps_2_0 PSMain_SM();
		
	}
}