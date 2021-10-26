#pragma once
#include <Windows.h>
#include "DirectX12Wrapper.h"
#include "ImGuiWrapper.h"
#include "GameEngine.h"

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow,int _WindowWidth,int _WindowHeight);
void Update();
void Draw();
void UnInit();