#pragma once
#include "DIRECTX12.h"

namespace DirectX12_Wrapper
{
	HRESULT CompileShader(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	bool CreateSRV(const char* _FileName, ID3D12Resource** _TexBuffer, ID3D12DescriptorHeap* _TexDescriptorHeap);
	bool CreateBuffer(ID3D12Resource** _Buffer, UINT64 _Size);
	bool CreateWriteBuffer(ID3D12Resource** _Buffer, UINT64 _Size);
	void FenceWait();
	void BarrierResouce(ID3D12Resource* _Resouce, D3D12_RESOURCE_STATES _BeforeState, D3D12_RESOURCE_STATES _AfterState);

	enum ComputeRootParameters : UINT32
	{
		ComputeRootCBV = 0,
		ComputeRootSRVTable,
		ComputeRootUAVTable,
		ComputeRootParametersCount
	};

	// �R���s���[�g�V�F�[�_�[�N���X
	class COMPUTESHADER
	{
	private:
		ComPtr<ID3D12Resource>				UAVBuffer;
		ComPtr<ID3D12CommandAllocator>		CSCmdAllocator;				// �R���s���[�g�p�R�}���h�A���P�[�^�[
		ComPtr<ID3D12GraphicsCommandList>	CSCmdList;				// �R���s���[�g�p�R�}���h�A���P�[�^�[
		ComPtr<ID3D12RootSignature>			RootSignature;				// �R���s���[�g�p���[�g�V�O�l�`��
		ComPtr<ID3D12PipelineState>			CSPipelineState;				// �R���s���[�g�p�p�C�v���C��
		ComPtr<ID3D12DescriptorHeap>		DescriptorHeap;				// �f�B�X�N���v�^�[�q�[�v
		ComPtr<ID3D12Resource>				Buffer;		
		ComPtr<ID3DBlob>					ComputeShader;				// �R���s���[�g�V�F�[�_�[
	
	public:
		bool Init(const char* _FileName);
		void Run();
	};
}
