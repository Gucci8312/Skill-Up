#pragma once
#include <string>
#include <DirectXMath.h>

#define EFFECTFILE_PASS		"Assets/EffectFile/"

// エミッタータイプ
#define TYPE_SPREAD			0
#define TYPE_CIRCLE			1
#define TYPE_BURST			2
#define TYPE_TORNADO		3

#define TYPENUM				4

// パーティクルタイプ
#define TYPE_QUAD			0
#define TYPE_SPHERE			1
#define TYPE_CUBE			2
#define TYPE_TRIANGLE		3

// マテリアルタイプ
#define TYPE_NORMAL			0
#define TYPE_TEXTURE		1
#define TYPE_PPL			2
#define TYPE_PBR			3
#define TYPE_MASKTEXTURE	4

// パーティクルパラメータ
#define MAX_SCALE			100.0f
#define MAX_SPEED			50.0f
#define MAX_ACCELERATION	5.0f
#define MAX_VOLUME			10000
#define MAX_LIFETIME		200
#define MAX_NOISE			10.0f
#define MAX_GRAVITY			1.0f

// ポストエフェクトタイプ
#define TYPE_BLOOM			0

using namespace DirectX;

// 大きさ
typedef struct
{
	float Width, Height, Depth;
}SCALE;

// 色
typedef struct
{
	float R, G, B, A;
}COLOR;

// パーティクルデータ
typedef struct
{
	XMFLOAT3 Pos;
	int LifeTime;
}PARTICLEDATA;

typedef struct
{
	int EffectNum;
}FILEHEADER;

// エフェクト
typedef struct
{
	bool TextureFlg;
	bool MaskTextureFlg;
	char TextureName[256];
	char MaskTextureName[256];
	XMFLOAT3 Pos;
	XMFLOAT3 Speed;
	XMFLOAT3 Acceleration;
	SCALE Scale;
	XMINT3 Angle;
	XMFLOAT3 Noise;
	COLOR Deffuse;
	float Roughness;
	float Metalness;
	bool OptionFlg[16];
	unsigned int OptionParameter[16];
	int EmitterType;
	int ParticleType;
	int MaterialType;
	unsigned int Volume;
	int Lifetime;
	float Gravity;
}EFFECTDATA;

typedef struct
{
	XMFLOAT3 Speed;
	XMFLOAT3 Noise;
	XMINT3 Angle;
}BUFFER_IN;

class EFFECT
{
private:
	static int CreateIdx;
	EFFECTDATA SendEffectData;

public:
	EFFECT()
	{
		SendEffectData.Scale = { 10.0f,10.0f,10.0f };
		SendEffectData.Speed = { 0.0f,0.0f,0.0f };
		SendEffectData.Acceleration = { 1.0f,1.0f,1.0f };
		SendEffectData.Angle = { 90,45,1 };
		SendEffectData.Noise = { 0,0,0 };
		SendEffectData.Volume = 1;
		SendEffectData.Lifetime = 1;
		SendEffectData.Deffuse = { 1.0f,1.0f,1.0f,1.0f };
		SendEffectData.Gravity = 0.0f;
	}
	void SetPos(XMFLOAT3 _Pos) { SendEffectData.Pos = _Pos; }
	void SetScale(float _Width, float Height, float _Depth) { SendEffectData.Scale.Width = _Width, SendEffectData.Scale.Height = Height, SendEffectData.Scale.Depth = _Depth; }
	void SetVolume(int _Volume) { SendEffectData.Volume = _Volume; }
	void SetLifeTime(int _LifeTime) { SendEffectData.Lifetime = _LifeTime; }
	void SetEmitterType(int _Type) { SendEffectData.EmitterType = _Type; }
	void SetParticleType(int _Type) { SendEffectData.ParticleType = _Type; }
	void SetAngle(int _x, int _y, int _z) { SendEffectData.Angle.x = _x, SendEffectData.Angle.y = _y, SendEffectData.Angle.z = _z; }
	void SetNoise(float _x, float _y, float _z) { SendEffectData.Noise.x = _x, SendEffectData.Noise.y = _y, SendEffectData.Noise.z = _z; }
	void SetSpeed(float _x, float _y, float _z) { SendEffectData.Speed.x = _x, SendEffectData.Speed.y = _y, SendEffectData.Speed.z = _z; }
	void SetAcceleration(XMFLOAT3 _AddSpeed) { SendEffectData.Acceleration.x = _AddSpeed.x, SendEffectData.Acceleration.y = _AddSpeed.y, SendEffectData.Acceleration.z = _AddSpeed.z; }
	void SetColor(float _r, float _g, float _b, float _a) { SendEffectData.Deffuse.R = _r, SendEffectData.Deffuse.G = _g, SendEffectData.Deffuse.B = _b, SendEffectData.Deffuse.A = _a; }
	void SetTextureFlg(bool _Flg) { SendEffectData.TextureFlg = _Flg; }
	void SetMaskTextureFlg(bool _Flg) { SendEffectData.MaskTextureFlg = _Flg; }
	void SetTextureName(const char* _TextureName) { strcpy_s(SendEffectData.TextureName, _TextureName); }
	void SetMaskTextureName(const char* _MaskTextureName) { strcpy_s(SendEffectData.MaskTextureName, _MaskTextureName); }
	void SetEffectData(EFFECTDATA _SetData) { SendEffectData = _SetData; }
	void SetOptionFlg(int _Idx, bool _Flg) { SendEffectData.OptionFlg[_Idx] = _Flg; }
	void SetOptionParameter(int _Idx, int _Parameter) { SendEffectData.OptionParameter[_Idx] = _Parameter; }
	void SetRoughness(float _Roughness) { SendEffectData.Roughness = _Roughness; }
	void SetMetalness(float _Metalness) { SendEffectData.Metalness = _Metalness; }
	void SetMaterial(int _MaterialType) { SendEffectData.MaterialType = _MaterialType; }
	void SetGravity(float _Gravity) { SendEffectData.Gravity = _Gravity; }

	static void AddCreateIdx() { CreateIdx++; }
	static void PullCreateIdx() { CreateIdx--; }
	static void ReseteCreateIdx() { CreateIdx = 0; }
	char* GetTextureName() { return SendEffectData.TextureName; }

	const XMFLOAT3 GetPos() { return SendEffectData.Pos; }
	const XMFLOAT3 GetNoise() { return SendEffectData.Noise; }
	const XMINT3	 GetAngle() { return SendEffectData.Angle; }
	const SCALE	 GetScale() { return SendEffectData.Scale; }
	const XMFLOAT3 GetSpeed() { return SendEffectData.Speed; }
	const XMFLOAT3 GetAcceleration() { return SendEffectData.Acceleration; }
	const int GetVolume() { return SendEffectData.Volume; }
	const int GetLifeTime() { return SendEffectData.Lifetime; }
	const int GetEmitterType() { return SendEffectData.EmitterType; }
	const int GetParticleType() { return SendEffectData.ParticleType; }
	static const int GetCreateIdx() { return CreateIdx; }
	const bool GetTextureFlg() { return SendEffectData.TextureFlg; }
	const EFFECTDATA GetSendEffectData() { return SendEffectData; }
	const bool GetOptionFlg(int _Idx) { return  SendEffectData.OptionFlg[_Idx]; }
	const int  GetOptionParameter(int _Idx) { return SendEffectData.OptionParameter[_Idx]; }
	const COLOR GetDeffuse() { return SendEffectData.Deffuse; }
	const float GetRoughness() { return SendEffectData.Roughness; }
	const float GetMetalness() { return SendEffectData.Metalness; }
	const int   GetMaterial() { return SendEffectData.MaterialType; }
	const float GetGravity() { return SendEffectData.Gravity; }
};

