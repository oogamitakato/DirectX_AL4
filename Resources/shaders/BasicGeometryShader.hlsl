#include "BasicShaderHeader.hlsli"

[maxvertexcount(3)]
void main(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//共通
	element.normal = input[0].normal;
	element.uv = input[0].uv;
	//1点目
	element.svpos = input[0].svpos;
	output.Append(element);
	//2点目
	element.svpos = input[0].svpos + float4(10.0f,10.0f,0,0);
	output.Append(element);
	//3点目
	element.svpos = input[0].svpos + float4(10.0f, 0, 0, 0);
	output.Append(element);

	////一つ目の三角形
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos;
	//	element.normal = input[i].normal;
	//	element.uv = input[i].uv * 2.0f;
	//	output.Append(element);
	//}
	////現在のストリップを終了
	//output.RestartStrip();

	////二つ目の三角形
	//for (uint i = 0; i < 3; i++)
	//{
	//	GSOutput element;
	//	element.svpos = input[i].svpos + float4(20.0f,0.0f,0.0f,0.0f);
	//	element.normal = input[i].normal;
	//	element.uv = input[i].uv * 5.0f;
	//	output.Append(element);
	//}
}