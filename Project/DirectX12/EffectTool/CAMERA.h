#pragma once

#include	<directxmath.h>
#include	"MathUtil.h"
#include	"Memory.h"
#include    "CDirectInput.h"

#define CAMERA_FIXEDPOS_NUM	4

using namespace DirectX;

class CAMERA {
	CAMERA() {
	}
public:
	enum CAMERATYPE
	{
		SPHERELOOK,
		FIXEDLOOK,
		//TPSLOOK,
		//FPSLOOK,
	};
	CAMERA(const CAMERA&) = delete;
	CAMERA& operator=(const CAMERA&) = delete;
	CAMERA(CAMERA&&) = delete;
	CAMERA& operator=(CAMERA&&) = delete;

	static CAMERA* GetInstance() {
		static CAMERA instance;
		return &instance;
	}

	void Init(float nearclip, float farclip, float fov,
		float width, float height,
		XMFLOAT3 eye, XMFLOAT3 lookat, XMFLOAT3 up) {
		FixedPos[0] = { 0.0f,300.0f,300.0f };
		FixedPos[1] = { -300.0f,300.0f,0.0f };
		FixedPos[2] = { 300.0f,300.0f,0.0f };
		FixedPos[3] = { 0.0f,300.0f,-300.0f };
		SetProjection(nearclip, farclip, fov, width, height);
		SetCamera(eye, lookat, up);
	}

	void SetNear(float nearclip) {
		m_near = nearclip;
	}

	void SetFar(float farclip) {
		m_far = farclip;
	}

	void SetFov(float fov) {
		m_Fov = fov;
	}

	void SetAspect(float width, float height) {
		m_Aspect = width / height;
	}

	void SetProjection(float nearclip, float farclip, float fov, float width, float height) {
		SetNear(nearclip);
		SetFar(farclip);
		SetFov(fov);
		SetAspect(width, height);
		CreateProjectionMatrix();
	}

	void SetCamera(const XMFLOAT3& eye, const XMFLOAT3& lookat, const XMFLOAT3& up) {

		SetEye(eye);
		SetLookat(lookat);
		SetUp(up);
		CreateCameraMatrix();
	}

	void SetEye(const XMFLOAT3& eye) {
		m_Eye = eye;
	}

	void SetLookat(const XMFLOAT3& lookat) {
		m_Lookat = lookat;
	}

	void SetUp(const XMFLOAT3& up) {
		m_Up = up;
	}

	void CreateCameraMatrix() {
		ALIGN16 XMVECTOR Eye = XMVectorSet(m_Eye.x, m_Eye.y, m_Eye.z, 0.0f);
		ALIGN16 XMVECTOR At = XMVectorSet(m_Lookat.x, m_Lookat.y, m_Lookat.z, 0.0f);
		ALIGN16 XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

		ALIGN16 XMMATRIX camera;
		camera = XMMatrixLookAtLH(Eye, At, Up);

		XMStoreFloat4x4(&m_Camera, camera);
	}

	void CreateProjectionMatrix() {

		ALIGN16 XMMATRIX projection;

		projection = XMMatrixPerspectiveFovLH(m_Fov, m_Aspect, m_near, m_far);

		XMStoreFloat4x4(&m_Projection, projection);

	}
	// ビュー変換行列取得
	const XMFLOAT4X4& GetViewMtx() {
		return m_Camera;
	}
	// プロジェクション変換行列取得
	const XMFLOAT4X4& GetProjMtx() {
		return m_Projection;
	}

	float GetFov() const {
		return m_Fov;
	}

	const XMFLOAT3& GetEye() const {
		return m_Eye;
	}

	const XMFLOAT3& GetLookat() const {
		return m_Lookat;
	}

	const XMFLOAT3& GetUp() const {
		return m_Up;
	}

	const CAMERATYPE GetCameraType() { return CameraType; }

	void Update(XMFLOAT3 _LookPos);
	void SetCameraType(CAMERATYPE _CameraType) { CameraType = _CameraType; }
	void SetFixedCameraIndex(int _Idx) { FixedCameraIdx = _Idx; }
	const char* GetCameraTypeName() {
		switch (CameraType)
		{
		case CAMERA::SPHERELOOK:
			return u8"球体";
			break;
		case CAMERA::FIXEDLOOK:
			return u8"固定";
			break;
		default:
			return nullptr;
			break;
		}
	}
private:
	XMFLOAT4X4		m_Projection;

	XMFLOAT4X4		m_Camera;

	XMFLOAT3		m_Eye;				// カメラ位置
	XMFLOAT3		m_Lookat;			// 注視点
	XMFLOAT3		m_Up;				// 上向きベクトル

	float			m_near;
	float			m_Aspect;
	float			m_Fov;
	float			m_far;

	CAMERATYPE CameraType;
	XMFLOAT3 FixedPos[4];
	int FixedCameraIdx;
};