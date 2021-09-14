StructuredBuffer<uint> inputBuffer0:register(t0);
StructuredBuffer<uint> inputBuffer1:register(t1);
RWStructuredBuffer<uint> resultBuffer:register(u0);	//UAV

[numthreads(8, 1, 1)]
void main(uint3 id:SV_DispatchThreadID)
{
	uint index = id.x;
	resultBuffer[index] = inputBuffer0[index] * inputBuffer1[index]; 
}