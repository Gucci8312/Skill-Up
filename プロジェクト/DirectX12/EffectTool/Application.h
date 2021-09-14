#pragma once
#include "Top.h"
#include "DIRECTX12.h"
#include "classSPRITE.h"
#include "CDirectInput.h"
//#include "EFFECTTOOL.h"

#pragma comment(lib, "dinput8.lib")

bool Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
void Update();
void Draw();
void UnInit();