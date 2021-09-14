#include "CAMERA.h"

// プレイヤーの周りを回転するカメラ
void UpdateSphereCamera(float Radius,// 半径
	float Elevation,				 // 仰角
	float azimuth,					 // 方位角
	const DirectX::XMFLOAT3& lookat) // 注視点
{
	DirectX::XMFLOAT3 eye;// 視点

	// 仰角、方位角をラジアンに変換
	Elevation = ToRad(Elevation);
	azimuth = ToRad(azimuth);

	// 仰角に応じた半径を計算
	float r = Radius * sinf(Elevation);

	// 視点座標を計算（3D極座標系をデカルト座標系へ変換）
	eye.x = r * cosf(azimuth);
	eye.y = Radius * cosf(Elevation);
	eye.z = r * sinf(azimuth);

	// 上向きベクトルを計算（仰角を90度回転させると計算できる）
	DirectX::XMFLOAT3 up;
	float ElevationPlus90 = Elevation + ToRad(90.0f);

	// 上向きベクトルを計算（3D極座標系をデカルト座標系へ変換）
	r = Radius * sinf(ElevationPlus90);

	up.x = r * cosf(azimuth);
	up.y = Radius * cosf(ElevationPlus90);
	up.z = r * sinf(azimuth);

	//視点ベクトルを注視点分平行移動
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	// ビュー変換行列を作る
	CAMERA::GetInstance()->SetEye(eye);
	CAMERA::GetInstance()->SetLookat(lookat);
	CAMERA::GetInstance()->SetUp(up);
	CAMERA::GetInstance()->CreateCameraMatrix();
}

// 等加速度運動
int toukasoku(int Time, float AddSpd)
{
	float spd = 1 + AddSpd * Time;

	float Flame = pow(spd * Time + 1 / 2 * AddSpd * Time, 2);
	return (int)Flame;
}

void CAMERA::Update(XMFLOAT3 _LookPos)
{
	//XMFLOAT4 axisZ;
	static float OffSet = m_Eye.z;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F1)) {
		OffSet -= 5.1f;
	}
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F2)) {
		OffSet += 5.1f;
	}
	//// Z軸を取り出す
	//axisZ.x = _LookPos.x;
	//axisZ.y = _LookPos.y;
	//axisZ.z = _LookPos.z;
	//axisZ.w = 0.0f;
	//static int Flame = 0;
	//if (Flame < 60)
	//{
	//	Flame++;
	//}
	//else
	//{
	//	Flame = 0;
	//}

	//up = { _LookPos.x,_LookPos.y,_LookPos.z };					// 上向きベクトル

	// 固定カメラ
	if (CameraType == FIXEDLOOK)
	{
		//	XMFLOAT3 TempEye = { FixedPos[FixedCameraIdx].x,FixedPos[FixedCameraIdx].y+OffSet,OffSet};
		CAMERA::GetInstance()->SetCamera(FixedPos[FixedCameraIdx], m_Lookat, m_Up);
	}

	//// TPSカメラ
	//else if (CameraType == TPS)
	//{
	//	lookat.x = _LookPos.x;
	//	lookat.y = _LookPos.y;
	//	lookat.z = _LookPos.z;

	//	eye.x = _LookPos.x + axisZ.x * -15.0f;
	//	eye.y = _LookPos.y + axisZ.y * -15.0f;
	//	eye.z = _LookPos.z + axisZ.z * -15.0f;

	//	CAMERA::GetInstance()->SetCamera(eye, lookat, up);
	//}
	//// FPSカメラ
	//else if (CameraType == FPS)
	//{
	//	eye.x = _LookPos.x;
	//	eye.y = _LookPos.y;
	//	eye.z = _LookPos.z;

	//	lookat.x = _LookPos.x + axisZ.x * 10.0f;
	//	lookat.y = _LookPos.y + axisZ.y * 10.0f;
	//	lookat.z = _LookPos.z + axisZ.z * 10.0f;

	//	CAMERA::GetInstance()->SetCamera(eye, lookat, up);
	//}


	else if (CameraType == SPHERELOOK)
	{
		static XMFLOAT3		angle = { 90.0f,90.0f,0.0f };

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP)) {
			angle.x += 1.0f;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN)) {
			angle.x -= 1.0f;
		}
		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT)) {
			angle.y += 1.0f;
		}

		if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT)) {
			angle.y -= 1.0f;
		}
		UpdateSphereCamera(OffSet, angle.x, angle.y, _LookPos);
	}
}
