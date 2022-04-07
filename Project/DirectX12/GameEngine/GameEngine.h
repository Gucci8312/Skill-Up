#pragma once
#include "ImGuiWrapper.h"
#include <vector>
//#include "ComponentObject.h"
#include <string>

// ゲームエンジン管理クラス
class GameEngine
{
private:
	//std::vector<ComponentObject*> Object;
	bool InspecterFlg = false;
	bool HierarchieFlg = false;
	bool ConsoleFlg = false;
	int ChoiceIdx = -1;

public:
	GameEngine();
	bool Init();
	void Update();
	void Draw();
	void UnInit();
	void GuiDraw();
};

