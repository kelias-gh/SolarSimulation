struct Input {
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal: NORMAL;
};

struct Output {
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct Light {
	float4 pos;
	float3 dir;
	float padding;
	float4 diffuse;
};

struct MaterialData {
	float4 ambient;
	float4 diffuse;
};

cbuffer cbWorldLightPerFrame : register(b0) //Ambient & Directional Light
{
	Light light;
};
cbuffer cbWorldLightPerFrame : register(b1) //Ambient & Directional Light
{
	MaterialData material;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(Output input) : SV_TARGET{
	float3 view = normalize(light.pos - input.worldPos);
	float4 diff = saturate(dot(light.dir, input.normal));
	float3 Reflect = normalize(2 * diff * input.normal - light.dir);
	float4 specular = pow(saturate(dot(Reflect, view)), 4); // R.V^n

	return ObjTexture.Sample(ObjSamplerState, input.texCoord); //* (material.ambient + light.diffuse * diff + specular);
}