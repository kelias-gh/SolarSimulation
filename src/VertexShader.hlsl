cbuffer MatrixBuffer : register(b0) {
	matrix worldMatrix;
};

cbuffer MatrixBuffer : register(b1) {
	matrix ViewAndProjectionMatrix;
};

struct Input {
	float3 position : POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 normal : NORMAL;
}; 

struct Output {
	float4 position : SV_POSITION;
	float3 worldPos : POSITION;
	float2 texCoord : TEXCOORD;
	float3 normal : NORMAL;
}; 

Output main(Input input)
{
	Output output;

	output.worldPos = normalize(mul(input.position, worldMatrix));
	output.position = mul(mul(float4(input.position, 1.0f), worldMatrix), ViewAndProjectionMatrix);
	output.texCoord = input.inTexCoord;
	output.normal = normalize(mul(input.normal, worldMatrix));

	return output;
}

