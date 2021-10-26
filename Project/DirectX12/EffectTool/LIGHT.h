#pragma once
#include	<directxmath.h>
#include	"MathUtil.h"
#include	"Memory.h"
#include    "CDirectInput.h"

using namespace DirectX;

class LIGHT 
{
private:
	LIGHT() {}
	XMFLOAT3 LightPos;
public:
	LIGHT(const LIGHT&) = delete;
	LIGHT& operator=(const LIGHT&) = delete;
	LIGHT(LIGHT&&) = delete;
	LIGHT& operator=(LIGHT&&) = delete;

	static LIGHT* GetInstance() {
		static LIGHT instance;
		return &instance;
	}
	void PosSet(XMFLOAT3 _Pos) { LightPos = _Pos; }

	XMFLOAT3 GetPos() { return LightPos; }
};