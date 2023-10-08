cbuffer materialBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float shininess;
	float3 padd;
}

struct PixelShaderInput
{
	float4 position : SV_POSITION;
	float2 uv : UV;
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
};

struct PSOutput
{
	float4 position : SV_Target0;
	float4 normal : SV_Target1;
	float4 ambient : SV_Target2;
	float4 diffuse : SV_Target3;
};

PSOutput main(PixelShaderInput input)
{
	PSOutput output;
	output.position = float4((float3)input.worldPos, specular.x);
	output.normal = float4(input.normal, specular.y);
	output.ambient = float4((float3)ambient, specular.z);
	output.diffuse = float4((float3)diffuse, shininess);

	return output;
}