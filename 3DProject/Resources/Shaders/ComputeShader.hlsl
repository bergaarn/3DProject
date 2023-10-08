RWTexture2D<float4> backBufferUAV : register(u0);

Texture2D<float4> posSpecX : register(t0);
Texture2D<float4> norSpecY : register(t1);
Texture2D<float4> ambSpecZ : register(t2);
Texture2D<float4> diffShini : register(t3);

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	float4 pos = float4(posSpecX[DTid.xy].xyz, 1.0f);
	float4 normal = float4(normalize(norSpecY[DTid.xy].xyz), 1.0f);
	float4 ambient = float4(ambSpecZ[DTid.xy].xyz, 1.0f);
	float4 specular = float4(posSpecX[DTid.xy].a, norSpecY[DTid.xy].a, ambSpecZ[DTid.xy].a, 1.0f);
	float4 diffuse = float4(diffShini[DTid.xy].xyz, 1.0f);
	float shininess = diffShini[DTid.xy].a;

	float4 finalAmbient = pos * ambient;
	float4 finalDiffuse = pos * diffuse;
	float4 finalSpecular = specular;
	float4 finalResult = finalAmbient + finalDiffuse + finalSpecular;
	backBufferUAV[DTid.xy] = finalResult;
}