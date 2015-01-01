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
texture Texture2;
texture Texture3;
texture Texture4;

float Time;

float4 AmbientLightIntensity	= { 0.3f, 0.3f, 0.3f, 1.0f };
float4 DiffuseLightIntensity	= { 0.8f, 0.8f, 0.8f, 1.0f }; //.8
float4 SpecularLightIntensity	= { 0.5f, 0.5f, 0.5f, 1.0f }; //.5

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
sampler S2 = sampler_state
{
	Texture		= (Texture2);
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
	MipFilter	= LINEAR;
	AddressU	= WRAP;
	AddressV	= WRAP;
};

sampler S3 = sampler_state
{
	Texture		= (Texture3);
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
	MipFilter	= LINEAR;
	AddressU	= WRAP;
	AddressV	= WRAP;
};

sampler S4 = sampler_state
{
	Texture		= (Texture4);
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

struct A2VBlend
{
	float4 position : POSITION;
	float2 texcoord0 : TEXCOORD0;
	float2 texcoord1 : TEXCOORD1;
};

struct A2VBUMP
{
	float4 position : POSITION;
	float3 tangent	: TANGENT;
	float3 binormal : BINORMAL;
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD0;
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
//----------------------------------------------------------------------------------------------------
struct V2PCell
{
	float4 position		: POSITION;
	float2 texcoord		: TEXCOORD;
};
//----------------------------------------------------------------------------------------------------
struct V2PBlend
{
	float4 position		: POSITION;
	float2 texcoord0	: TEXCOORD0;
	float2 texcoord1	: TEXCOORD1;
};


struct V2PBUMP
{
	float4 position		: POSITION;
	float3 dirToLight	: TEXCOORD0; // dir to light 
	float3 dirToEye		: TEXCOORD1;// dir to eye
	float3 normal		: TEXCOORD2; // normal 
	float2 texcoord		: TEXCOORD3;// texcoord
	
};

//----------------------------------------------------------------------------------------------------


struct P2F
{
	float4 color : COLOR;
};

//----------------------------------------------------------------------------------------------------
// functions
//----------------------------------------------------------------------------------------------------

float4 AdjustSaturation(float4 color, float saturation)

{

    float grey = dot(color.rgb, float3(0.5, 0.5, 0.5));
    
    return lerp(grey, color, saturation);

}
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

V2PPL VSMain_PAcid(A2V input)
{
	
	// initilize output
	V2PPL output = (V2PPL)0;
	
	// get local position for transform
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	
	float distanceSquared = abs(distance(worldPos,ViewPosition) - 10);
	distanceSquared *= distanceSquared;
	
	worldPos.y += 5* sin(distanceSquared * sin(Time /143.0f)/1000);
	float y = worldPos.y;
	float x = worldPos.x;
	float om = sin(distanceSquared*sin(Time/256.0f)/5000.0f) * sin(Time/200.0);
	
	worldPos.y = x * sin(om)+y*cos(om);
	worldPos.x = x * cos(om)-y*sin(om);
	
	
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

V2PPost VSMain_PostProcess(A2VPost input)
{
	// initilize output
	V2PPost output = (V2PPost)0;
	
	output.position = input.position;
	output.texcoord = input.texcoord;
	
	return output;
}
//----------------------------------------------------------------------------------------------------
V2PBlend VSMain_Blend(A2VBlend input)
{
	// initilize output
	V2PBlend output = (V2PBlend)0;
	
	output.position = input.position;
	output.texcoord0 = input.texcoord0;
	output.texcoord1 = input.texcoord1;
	
	return output;
}
//----------------------------------------------------------------------------------------------------

V2PBUMP VSMain_BUMP(A2V input)
{
	// initilize output
	V2PBUMP output = (V2PBUMP)0;
	
	float4 localPos = input.position;
	float4 worldPos = mul(localPos,WorldMatrix);
	float4 viewPos = mul(worldPos,ViewMatrix);
	float4 projPos = mul(viewPos,ProjMatrix);
	// just for test
	float3 normal = input.normal.xyz;
	float3 tangent = float3(0.0f,0.0f,1.0f);
	float3 binormal = float3(1.0f,0.0f,0.0f);
	
	float3x3 TBN = {tangent,
					binormal,
					normal};
	float3x3 worldMat3 = {	WorldMatrix[0].xyz,
							WorldMatrix[1].xyz,
							WorldMatrix[2].xyz
							};
	float3x3 TBNW = mul(TBN, worldMat3);
	
	float3 dirToLight = (LightPosition.xyz - worldPos.xyz);
	dirToLight = normalize(dirToLight);
	
	float3 dirToEye = (ViewPosition.xyz - worldPos.xyz);
	dirToEye = normalize(dirToEye);
	
	dirToLight	= mul(TBNW, dirToLight);
	dirToEye	= mul(TBNW, dirToEye);
	
	
	output.position		= projPos;
	output.dirToEye		= dirToEye;
	output.dirToLight	= dirToLight;
	output.normal		= normal;
	output.texcoord		= input.texcoord;
	return output;
}
//----------------------------------------------------------------------------------------------------

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
	
	
	if(dot(input.dirToEye,input.normal) < 0.0f)
	{
		output.color.rgb = 0.0f;
	}
	else
	{
		output.color.rgb = (texColor * (ambient + diffuse)) + specular;
	}
	return output;
	
}

//----------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------
P2F PSMain_MonoChrome(V2PPost input)
{
// initilize output
	P2F output = (P2F)0;
	float4 texColor = tex2D(S1, input.texcoord);
	//texColor.rgb = (texColor.r + texColor.g + texColor.b)/(abs(sin(Time)*3.0f));
	// set output
	output.color = texColor;
	return output;
	
}

//----------------------------------------------------------------------------------------------------
P2F PSMain_BlendPass(V2PBlend input)
{
// initilize output
	P2F output = (P2F)0;
	float4 texColor = tex2D(S1, input.texcoord0);
	float4 highColor = 0.0f;
	float4 cellColor = 0.0f;
	float2 tempTexCoord = input.texcoord0;
	float blurAmount = 4.0f;
	float blurOffset = 2.0f;
	
	// blur high pass
	for(float i = 0; i <= blurAmount; ++i)
	{
		for(float j = 0; j<= blurAmount; ++j)
		{
			float xCoeff = (i - blurOffset);
			float yCoeff = (j - blurOffset);
			if(xCoeff != 0)
			{
				xCoeff = 0.5f/abs(xCoeff);
			}
			else
			{
				xCoeff = 1.0f;
			}
			if(yCoeff != 0)
			{
				yCoeff = 0.5f/abs(yCoeff);
			}
			else
			{
				yCoeff = 1.0f;
			}
			
			float blurRamp = xCoeff * yCoeff;
					
			tempTexCoord = float2((input.texcoord0.x + (i - blurOffset)),(input.texcoord0.y + (j - blurOffset)));
			highColor += (tex2D(S2, tempTexCoord) * (blurRamp*2));
		}
	}
	
	highColor = AdjustSaturation(highColor, 1.0f) * 0.5;
	

	texColor = AdjustSaturation(texColor, 1.0f) * 1.0f;
	texColor *= (1 - saturate(highColor));
	
	output.color = texColor + highColor;
	return output;
	
}

//----------------------------------------------------------------------------------------------------
P2F PSMain_HighPass(V2PPost input)
{
// initilize output
	P2F output = (P2F)0;
	float4 texColor = tex2D(S1, input.texcoord);
	// set output
	float Threshold = 0.3f;
	texColor.rgb = saturate((texColor.rgb - Threshold) / (1.0f - Threshold));
	output.color = texColor;
	return output;
	
}

//----------------------------------------------------------------------------------------------------
P2F PSMain_CellPass(V2PPL input)
{
// initilize output
	P2F output = (P2F)0;
	float4 texColor = tex2D(S0, input.texcoord);
	// set output
	
	  // Calculate diffuse light amount
    //float intensity = dot(normalize(input.dirToLight), input.normal);
    //if(intensity < 0)
    //{
    //    intensity = 0;
	//}
 
    //// Discretize the intensity, based on a few cutoff points
    //if (intensity > 0.95)
    //{
    //    texColor *= float4(1.0,1,1,1.0);
    //}
    //else if (intensity > 0.5)
    //{
    //    texColor*= float4(0.7,0.7,0.7,1.0);
    //}
    //else if (intensity > 0.05)
    //{
    //    texColor *= float4(0.35,0.35,0.35,1.0);
    //}
    //else
    //{
    //    texColor *= float4(0.1,0.1,0.1,1.0);
	//}	
	output.color = texColor;
	return output;
	
}
//----------------------------------------------------------------------------------------------------

P2F PSMain_BUMP(V2PBUMP input)
{	
	P2F output = (P2F)0;
	float4 texColor = tex2D(S0, input.texcoord);
	float4 texMap = tex2D(S4, input.texcoord);
	// 0 - 1 -> -1 - 1
	float3 normal = normalize((texMap.xyz * 2) - 1);
	
	// calculate ambient
	float4 ambient = AmbientLightIntensity * AmbientMaterial;
	
	// calculate diffuse
		// calc d
		float d = saturate(dot(input.dirToLight, normal));
		float4 diffuse = d * DiffuseLightIntensity * DiffuseMaterial;
	
	
	// calculate specular
		// calculate halfangle
		float3 h = (input.dirToLight + input.dirToEye) * 0.5f;
		h = normalize(h);
		float s = saturate(dot(h, normal));
			
		float4 specular = pow(s,SpecularPower) * SpecularLightIntensity * SpecularMaterial;
	
		texColor = texColor * (ambient + diffuse);
	

	output.color = texColor + specular;
	
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
		vertexshader	= compile vs_2_0 VSMain_PostProcess();
		pixelshader		= compile ps_2_0 PSMain_MonoChrome();
	}
}

//----------------------------------------------------------------------------------------------------

technique HighPass
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSMain_PostProcess();
		pixelshader		= compile ps_2_0 PSMain_HighPass();
	}
}

//----------------------------------------------------------------------------------------------------

technique CellPass
{
	pass P0
	{
		vertexshader	= compile vs_2_0 VSMain_PPL();
		pixelshader		= compile ps_2_0 PSMain_CellPass();
	}
}

//----------------------------------------------------------------------------------------------------

technique BlendPass
{
	pass P0
	{
		vertexshader	= compile vs_3_0 VSMain_Blend();
		pixelshader		= compile ps_3_0 PSMain_BlendPass();
	}
}

technique BumpMap
{
	pass P0
	{
		vertexshader	= compile vs_3_0 VSMain_BUMP();
		pixelshader		= compile ps_3_0 PSMain_BUMP();
	}
}



 