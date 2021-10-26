#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>

// ���C�u���������N
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include "JapaneseFont.cpp"
//#include "GameObject.h"

// ImGui�Ǘ��N���X
namespace ImGuiWrapper
{
	class ImGuiUtil
	{
	private:
		static ImGuiUtil* Instance;
		bool ImGuiFlg = true;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapForImgui;

	public:
		// �V���O���g�� �C���X�^���X�擾
		static ImGuiUtil* GetInstance()
		{
			return Instance;
		}

		static void Create();
		static void Destroy();

		bool Init(HWND _hWnd, ID3D12Device* _Device);
		void BeforRender();
		void AfterRender(ID3D12GraphicsCommandList* _CmdList);
	};
}
