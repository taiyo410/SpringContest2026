#include "../Common/Pixel/PixelShader2DHeader.hlsli"
// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color_1;
    float4 g_color_2;
    float4 g_over_hp_color;
    float g_hp_Per;
    float g_hp_lerp;
    float2 dummy;
}
float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    float2 uv = PSInput.uv;
    float4 srcCol = tex.Sample(texSampler, uv);
    if (srcCol.a < 0.01f)
    {
        discard;
    }
    //ゲージの枠を描画
    else if (all(srcCol.rgb < 0.95f))
    {
        return srcCol;
    }
    else if (uv.x > g_hp_lerp)
    {
        if (g_over_hp_color.a<0.01f)
        {
            discard;
        }
        //現在HP以上の部分の色を塗る
        srcCol.rgba = g_over_hp_color;
    }
    if (uv.x < g_hp_Per)
    {
        float t = uv.x;
        srcCol.rgb = lerp(g_color_1, g_color_2, t);
    }
    else if (uv.x < g_hp_lerp)
    {
        srcCol.rgb = float3(1.0f, 0.0f, 0.0f);
    }
    
    return srcCol;
   
}