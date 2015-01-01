//====================================================================================================
//Filename: simple.vsh
// createdby matt stinnen
// description simple vertex shader
//====================================================================================================

//====================================================================================================
//Globals
//====================================================================================================

matrix ViewMatrix;
matrix ProjMatrix;

float Timer;

vector Up = {0, 1, 0, 0};
vector Orange = { 1, 0.549, 0, 1};
vector Blue = { 0, 0, 1, 1};
//====================================================================================================
// structs
//====================================================================================================

struct VS_INPUT
{
	vector position : POSITION;
	vector normal	: NORMAL;
};

//----------------------------------------------------------------------------------------------------

struct VS_OUTPUT
{
	vector position : POSITION;
	vector diffuse	: COLOR;
};

//====================================================================================================
// Main
//====================================================================================================

VS_OUTPUT Main(VS_INPUT input)
{
	// initialize output structure
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float tweak = (sin(Timer * .5));
	float tweak2 = (cos(Timer * .5));
	float tweak3 = abs(sin(Timer * 10));
	
	// project normals to 0 1 0
	vector normal = vector(input.normal.xyz,0);
	
	vector offset = Up * dot(normal , Up);
	
	// apply transform
	vector pos = input.position; //+ (offset * tweak * 0.9f);
	pos.x =  pos.x * tweak;
	pos.y = pos.y / tweak2;
	
	pos = mul(pos, ViewMatrix);
	pos = mul(pos, ProjMatrix);
	
	
	//set output
	output.position = pos;
	output.diffuse.r = tweak;
	output.diffuse.g = tweak2;
	output.diffuse.b = tweak3;
	
	return output;
}