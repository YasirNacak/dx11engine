cbuffer alphaBuffer : register(b0)
{
    float alpha;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

Texture2D mainTexture : TEXTURE : register(t0);
SamplerState mainSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 col = mainTexture.Sample(mainSamplerState, input.texCoord);
    return float4(col, alpha);
}