#pragma once
#include "classPARTICLE.h"

using namespace DirectX;

class QUAD:public PARTICLE
{
private:

public:
	~QUAD();
	bool Init(const char* _FileName, XMFLOAT3 _Pos, float _Width, float _Height)override final;
	void Update()override final;
	void Draw()override final;
	void PosSet(XMFLOAT3 _Pos) { Pos.x = _Pos.x, Pos.y = _Pos.y, Pos.z = _Pos.z; }
	void AngleSet(XMFLOAT3 _Angle) { Angle.x = _Angle.x, Angle.y = _Angle.y, Angle.z = _Angle.z; }
};

