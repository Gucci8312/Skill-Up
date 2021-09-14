#pragma once
#pragma once
#include	<DirectXMath.h>

using namespace DirectX;

struct PLANE {
	float a, b, c, d;
};

void GetQtfromMatrix(const XMFLOAT4X4& mat, XMFLOAT4& qt);
void QtRotationAxis(XMFLOAT4& qt, XMFLOAT4& axis, float angle);
void QtMul(XMFLOAT4& outqt, const XMFLOAT4& qt1, const XMFLOAT4& qt2);
void MtxFromQt(XMFLOAT4X4& outmtx, const XMFLOAT4& qt);
void QtNormalize(XMFLOAT4& inqt, XMFLOAT4& outqt);
void MtxTranslation(const XMFLOAT3& trans, XMFLOAT4X4& matrix);
void MtxTranspose(XMFLOAT4X4& mtx);
void MakeWorldMatrix(XMFLOAT4X4& mat, XMFLOAT3& angle, XMFLOAT3& trans);
void Vec3Cross(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec1, DirectX::XMFLOAT3& invec2);
void Vec3Normalize(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec);
void MtxIdentity(DirectX::XMFLOAT4X4& mat);
void GetQtfromMatrix(const XMFLOAT4X4& mat, XMFLOAT4& qt);
void QtRotationAxis(XMFLOAT4& qt, XMFLOAT4& axis, float angle);
void QtMul(XMFLOAT4& outqt, const XMFLOAT4& qt1, const XMFLOAT4& qt2);
void MtxFromQt(XMFLOAT4X4& outmtx, const XMFLOAT4& qt);
void QtNormalize(XMFLOAT4& inqt, XMFLOAT4& outqt);

void Vec3Dot(float& dot, const DirectX::XMFLOAT3& invec1, const DirectX::XMFLOAT3& invec2);
void Vec4Length(XMFLOAT4& vec, float& length);
void p2pLength(const XMFLOAT3& p1, const XMFLOAT3& p2, float& length);
void MtxTranslation(const XMFLOAT3& trans, XMFLOAT4X4& matrix);

void QtSlerp(const XMFLOAT4& fromqt, const XMFLOAT4& toqt, float t, XMFLOAT4& ansqt);
void QtIdentity(XMFLOAT4& qt);
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix);
void IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix, float ans[]);
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix, float ans[], PLANE* plane);
bool IsInFrustum(
	const XMFLOAT3& pos,					// BS中心座標
	float radius,							// 半径
	const XMFLOAT4X4& matrix,				// ビュー変換行列＊プロジェクション変換行列 
	float ans[],							// 平面との距離
	bool  canseeflag[],						// フラスタム平面とＢＳの関係
	PLANE* plane);							// 平面方程式;
void GetPlanefromProjectionmatrix(PLANE* p, const XMFLOAT4X4& matrix);
void GetPlanefromProjectionmatrixWithNormalize(PLANE* p, const XMFLOAT4X4& matrix);
void MtxRotationX(float angle, XMFLOAT4X4& outmtx);
void MtxRotationY(float angle, XMFLOAT4X4& outmtx);
void MtxRotationZ(float angle, XMFLOAT4X4& outmtx);
void MtxScale(float sx, float sy, float sz, XMFLOAT4X4& outmtx);
float haltonseq(unsigned int no, unsigned int base);

void Vec3Length(XMFLOAT3& vec, float& length);
void MtxRotationAxis(const XMFLOAT3& axis, float angle, XMFLOAT4X4& outmtx);
void MtxMultiply(XMFLOAT4X4& ansmtx, const XMFLOAT4X4& p1mtx, const XMFLOAT4X4& p2mtx);
void MtxInverse(XMFLOAT4X4& ansmtx, const XMFLOAT4X4& mtx);
void MtxTranspose(XMFLOAT4X4& outmtx, const XMFLOAT4X4& inmtx);
void Vec3MulMatrix(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec, const DirectX::XMFLOAT4X4& mat);
float ToRad(float _Angle);
size_t AlignmentedSize(size_t size, size_t alignment);