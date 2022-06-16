cbuffer MATRIX : register(b0)
{
    row_major matrix g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
}

cbuffer MATERIAL : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;

    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;

    float2 g_float2_0;
    float2 g_float2_1;
    float2 g_float2_2;
    float2 g_float2_3;

    float4 g_float4_0;
    float4 g_float4_1;
    float4 g_float4_2;
    float4 g_float4_3;

    row_major Matrix g_mat_0;
    row_major Matrix g_mat_1;
    row_major Matrix g_mat_2;
    row_major Matrix g_mat_3;
}

cbuffer ANIM2D : register(b2)
{
    float2 vLT;
    float2 vSize;
    int iAnimCheck;
    float3 vPadding;
}

cbuffer TILE_VALUE : register(b3)
{
    int4 g_int_tile[225];
    float4 g_float_tileX[225];
    float4 g_float_tileY[225];
    float4 g_float_tileTexX[225];
    float4 g_float_tileTexY[225];
}

cbuffer GLOBAL_VALUE : register(b4)
{
    uint iLightCount; // 광원 개수
    float fDT; // deltatime
    float fAccTime; // 누적시간
    float fPadding;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);

Texture2D g_tex_anim : register(t17);


SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);
SamplerState g_sam_2 : register(s2);
SamplerState g_sam_3 : register(s3);


// ===================
// Vertex Color Shader
// ===================
struct VTX_INPUT
{
    float3 vPos : POSITION;
    float4 vCol : COLOR;
};

struct VTX_OUTPUT
{
    float4 vPos : SV_Position;
    float4 vCol : COLOR;
};

VTX_OUTPUT VS(VTX_INPUT _input)
{
    VTX_OUTPUT output = (VTX_OUTPUT) 0.f;
    
    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProj = mul(vViewPos, g_matProj);

    output.vPos = vProj;
    output.vCol = _input.vCol;
    
    return output;
}

// 픽셀 쉐이더의 입력 값은, 각 정점으로부터의 거리에 따른 보간값이 들어온다.(레스터라이져 단계에서 계산됨)
struct PS_OUT
{
    float4 vOutCol : SV_Target;
};

PS_OUT PS(VTX_OUTPUT _input)
{
    PS_OUT output = (PS_OUT) 0.f;
    output.vOutCol = _input.vCol;
    return output;
}

// ==========================
// Texture Shader
// g_tex_0 : Samling Texture
// ==========================
struct VTX_TEX_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_TEX_OUTPUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};

VTX_TEX_OUTPUT VS_Tex(VTX_TEX_INPUT _input)
{
    VTX_TEX_OUTPUT output = (VTX_TEX_OUTPUT) 0.f;
    
    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProj = mul(vViewPos, g_matProj);

    output.vPos = vProj;
    output.vUV = _input.vUV;
    
    return output;
}

PS_OUT PS_Tex(VTX_TEX_OUTPUT _input)
{
    PS_OUT output = (PS_OUT) 0.f;
    output.vOutCol = g_tex_0.Sample(g_sam_0, _input.vUV);
    
    if (g_int_0)
    {
        output.vOutCol.r *= 1.1f;
        output.vOutCol.g *= 0.2f;
        output.vOutCol.b *= 0.2f;
    }
    //타이틀화면 쉐이딩
    if(g_int_0 == 2)
    {

       if(g_float_0 <= 3.f)
       {
            output.vOutCol.r *= 0.7f * 3;
            output.vOutCol.g *= 1.3f * 3;
            output.vOutCol.b *= 1.3f * 3;

           output.vOutCol.r *= 1.1f  * (g_float_0 );
           output.vOutCol.g *= 0.9f * (1 - g_float_0);
           output.vOutCol.b *= 0.9f * (1 - g_float_0);
       }
       if (g_float_0 >= 3.f && g_float_0 <= 6.f)
       {
            output.vOutCol.r *= 1.1f * 3;
            output.vOutCol.g *= 0.9f * 3;
            output.vOutCol.b *= 0.9f * 3;

           output.vOutCol.r *= 0.7f * (1 - (g_float_0 - 3));
           output.vOutCol.g *= 1.3f * ((g_float_0 - 3));
           output.vOutCol.b *= 0.9f * (1 - (g_float_0 - 3));
       
       }
       if (g_float_0 >= 6.f && g_float_0 <= 9.f)
       {
            output.vOutCol.r *= 0.7f * 3;
            output.vOutCol.g *= 1.3f * 3;
            output.vOutCol.b *= 0.9f * 3;

           output.vOutCol.r *= 0.7f * (1 - (g_float_0 - 6));
           output.vOutCol.g *= 1.3f * (1 - (g_float_0 - 6));
           output.vOutCol.b *= 1.3f * ((g_float_0 - 6));
       
       }
       if (g_float_0 >= 9.f && g_float_0 <= 12.f)
       {
            output.vOutCol.r *= 0.7f * 3;
            output.vOutCol.g *= 1.3f * 3;
            output.vOutCol.b *= 0.9f * 3;

           output.vOutCol.r *= 0.7f * ((g_float_0 - 9));
           output.vOutCol.g *= 1.3f * ((g_float_0 - 9));
           output.vOutCol.b *= 1.3f * ((g_float_0 - 9));
       
       }
      
    }
    //hp바 쉐이딩
    if(g_int_0 == 3)
    {
        if (_input.vUV.x > g_float_0 / 10.f)
        {
            output.vOutCol.a = 0.f;
        }

    }
    return output;
}

// ==========================
// Std 2D Shader
// g_tex_0 : Samling Texture
// ==========================
VTX_TEX_OUTPUT VS_Std2D(VTX_TEX_INPUT _input)
{
    VTX_TEX_OUTPUT output = (VTX_TEX_OUTPUT) 0.f;
    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProj = mul(vViewPos, g_matProj);

    output.vPos = vProj;
    output.vUV = _input.vUV;
    
    return output;
}

PS_OUT PS_Std2D(VTX_TEX_OUTPUT _input)
{
    PS_OUT output = (PS_OUT) 0.f;

    //애니 렌더링
    if (iAnimCheck == 1)
    {
        float2 vAnimUV = vLT + (vSize * _input.vUV);
        output.vOutCol = g_tex_anim.Sample(g_sam_0, vAnimUV);

        //깜빡임 처리
        if (g_int_0 == 1)
        {
            if (output.vOutCol.a != 0.f)
                output.vOutCol.a = g_float_0;
        }
       //노이즈 처리
        else if (g_int_0 == 2)
        {
            if (output.vOutCol.a != 0.f)
            {
                if (g_float_0 < 0.25f)
                {
                    output.vOutCol = g_tex_1.Sample(g_sam_0, _input.vUV);
                    output.vOutCol.a *= g_float_0 * 4.f;
                }
                if (g_float_0 >= 0.25f)
                {
                    output.vOutCol = g_tex_1.Sample(g_sam_0, _input.vUV);
                    output.vOutCol.a *= 1 + (1 - g_float_0 * 4.f);
                }
            }
        }

    }
     //일반 렌더링
    if (iAnimCheck != 1)
    {
        output.vOutCol = g_tex_0.Sample(g_sam_0, _input.vUV);
        //깜빡임 처리
        if (g_int_0 == 1)
        {
            if (output.vOutCol.a != 0.f)
                output.vOutCol.a = g_float_0;
        }

      //노이즈 처리
        else if (g_int_0 == 2)
        {
            if (output.vOutCol.a != 0.f)
            {
                if (g_float_0 < 0.25f)
                {
                    output.vOutCol = g_tex_1.Sample(g_sam_0, _input.vUV);
                    output.vOutCol.a *= g_float_0 * 4.f;
                }
                if (g_float_0 >= 0.25f)
                {
                    output.vOutCol = g_tex_1.Sample(g_sam_0, _input.vUV);
                    output.vOutCol.a *= 1 + (1 - g_float_0 * 4.f);
                }
            }
        }
    }

    //타일조각 렌더링
    if (g_int_2)
    {
        //vec2_0 -> LT, vec2-1 -> Size
		float2 vTileUV;
		vTileUV = g_float2_0 + (g_float2_1 * _input.vUV);
        output.vOutCol = g_tex_0.Sample(g_sam_0, vTileUV);
    }
    //타일맵 렌더링
    if (2 == g_int_2)
    {
        float2 vTileUV;
        vTileUV = (g_float2_1 * _input.vUV);
        float fEdgeX = 1 / g_float2_2.x;
        float fEdgeY = 1 / g_float2_2.y;
        int iTileNum = 0;
        for (float i = 0; i < g_float2_2.y; i += 1)
        {
            for (float j = 0; j < g_float2_2.x; j += 1)
            {
                if (_input.vUV.x > fEdgeX * j && _input.vUV.x < fEdgeX * (j + 1)
                     && _input.vUV.y > fEdgeY * i && _input.vUV.y < fEdgeY * (i + 1))
                {
                    vTileUV.x -= fEdgeX * g_float2_1.x * j;
                    vTileUV.y -= fEdgeY * g_float2_1.y * i;
                    vTileUV.x += g_float_tileX[iTileNum / 4][iTileNum % 4];
                    vTileUV.y += g_float_tileY[iTileNum / 4][iTileNum % 4];
                    vTileUV.x += g_float_tileTexX[iTileNum / 4][iTileNum % 4];
                    vTileUV.y += g_float_tileTexY[iTileNum / 4][iTileNum % 4];
                    if (g_int_tile[iTileNum / 4][iTileNum % 4] == 0)
                    {
                        output.vOutCol = g_tex_0.Sample(g_sam_0, vTileUV);
                    }
                    else if (g_int_tile[iTileNum / 4][iTileNum % 4] == 1)
                    {
                        output.vOutCol = g_tex_1.Sample(g_sam_0, vTileUV);
                    }

                }
                ++iTileNum;
            }
        }
    }
    //총알 렌더링
    if (3 == g_int_2)
    {
         //vec2_0 -> LT, vec2-1 -> Size
        float2 vTileUV;
        vTileUV = g_float2_0 + (g_float2_1 * _input.vUV);
        output.vOutCol = g_tex_0.Sample(g_sam_0, vTileUV);

       // if (output.vOutCol.r == 1 && output.vOutCol.g == 1 && output.vOutCol.b == 1)
       // {
       //     output.vOutCol.rgb *= 0;
       // }
       // else
       // {
       //     output.vOutCol.r *= 1.2f;
       //     output.vOutCol.g *= 0.2f;
       //     output.vOutCol.b *= 0.2f;
       // }

    }
    //페이드 렌더링 off
    if(4 == g_int_2)
    {
        output.vOutCol.a *= 0.001f;
    }
     //페이드 아웃 렌더링 on
    if (5 == g_int_2)
    {
        if (g_float_0 < 0.5f)
        {
            output.vOutCol.a *= g_float_0 * 2.f;
        }

    }
    //페이드 인 렌더링 on
    if (6 == g_int_2)
    {
        if (g_float_0 < 0.5f)
        {
            output.vOutCol.a *= 1 - (g_float_0 * 2.f);
        }
    }

    return output;
}


// ==========================
// Collider2D Shader
// g_int_0 : Collision Check
// ==========================
VTX_TEX_OUTPUT VS_Collider2D(VTX_TEX_INPUT _input)
{
    VTX_TEX_OUTPUT output = (VTX_TEX_OUTPUT) 0.f;

    float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);

    output.vPos = mul(vViewPos, g_matProj);
    output.vUV = _input.vUV;

    return output;
}

float4 PS_Collider2D(VTX_TEX_OUTPUT _input) : SV_Target
{
    if (g_int_0)
        return float4(1.f, 0.2f, 0.2f, 1.f);
    else
        return float4(0.2f, 1.f, 0.2f, 1.f);
}