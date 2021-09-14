#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"
#include "JapaneseFont.cpp"
#include "DIRECTX12.h"
#include "classPARTICLE.h"
#include "classQUAD.h"

using namespace Microsoft::WRL;

class EFFECTTOOL
{
private:
	ComPtr<ID3D12DescriptorHeap> DescriptorHeapForImgui;
	bool ImGuiFlg;
	PARTICLE* Particle;

public:
	bool Init(HWND _hWnd);
	void Update();
	void Draw();
	void UnInit();
	bool GuiInit(HWND _hWnd);
	void GuiDraw();
};

