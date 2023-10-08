cbuffer transposedMatrices : register (b0)
{
	float4x4 worldMatrix;
	float4x4 vpMatrix;
}
struct VertexShaderInput
{
	float3 position : POSITION;
	float2 uv : UV;				 // Semantics matching input layout and forwarding values through the pipeline
	float3 normal : NORMAL;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;	// System Value Semantic, predetermined purpose in the pipeline
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.position = mul(mul(float4(input.position, 1.0f), worldMatrix), vpMatrix);
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.worldPos = mul(float4(input.position, 1.0f), worldMatrix);
	return output;
}