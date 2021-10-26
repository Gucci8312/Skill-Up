#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

// リンクライブラリ //
#pragma comment(lib, "winmm.lib")				// リンク対象ライブラリにwinmm.libを追加

#define	CLASS_NAME		"ウィンドウ"			// ウインドウクラスの名前
#define WINDOW_NAME		"EffectTool"
#define WINDOW_WIDTH	1400
#define WINDOW_HEIGHT	900

#define DEBUG