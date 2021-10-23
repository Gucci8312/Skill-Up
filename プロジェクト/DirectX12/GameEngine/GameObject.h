#pragma once
#include "DirectX12Wrapper.h"
#include "ImGuiWrapper.h"
#include <list>
#include <string>

enum ComponentNameList
{
	Transform
};

// 前方宣言
class GameObject;

// コンポーネント管理クラス
class Component
{
public:
	virtual ~Component() {}
	std::string ComponentName;
	GameObject* Parent;
	virtual void Init() {}
	virtual void Update() {}
	virtual void Draw() {}
	virtual void ParameterDraw() {}
	std::string GetName() { return ComponentName; }
};

// 場所を示すコンポーネント
class TransformComponent :public Component
{
	XMFLOAT3 Pos;
	XMFLOAT3 Angle;
	XMFLOAT3 Scale;

public:
	void Init()
	{
		ComponentName = "TransformComponent";
	}
	void ParameterDraw();
};


// コンポーネントオブジェクト管理クラス
class GameObject
{
private:
	std::list<Component*> ComponentList;
	std::string Name = "No Name ";
	static int CreateIdx;
	int ThisIdx;
public:
	GameObject()
	{
		ThisIdx = CreateIdx;
		CreateIdx++;
		Name = Name + std::to_string(ThisIdx);
	}
	~GameObject()
	{
		for (auto component : ComponentList)
		{
			delete component;
		}
	}

	std::string GetName()
	{
		return Name;
	}

	void SetName(char* _Name)
	{
		Name = _Name;
	}

	// コンポーネント全体の更新
	void Update()
	{
		for (auto component : ComponentList)
		{
			component->Update();
		}
	}

	// コンポーネント全体の描画
	void Draw()
	{
		for (auto component : ComponentList)
		{
			component->Draw();
		}
	}

	// オブジェクトが持っているコンポーネントを取得
	template<class T> T* GetComponent()
	{
		for (auto component : ComponentList)
		{
			T* buff = dynamic_cast<T*>(component);
			if (buff != nullptr)
			{
				return buff;
			}
			return nullptr;
		}
	}

	// オブジェクトが持っているコンポーネントを追加
	template<class T> T* AddComponent()
	{
		T* buff = new T();
		buff->Parent = this;
		bool CoverFlg = false;
		for (auto component : ComponentList)
		{
			if (typeid(*buff) == typeid(component))
			{
				CoverFlg = true;
				break;
			}
		}

		if (!CoverFlg)
		{
			ComponentList.push_back(buff);
		}
		buff->Init();
		return buff;
	}

	// オブジェクトが持っているコンポーネントを削除
	template<class T> void DeleteComponent()
	{
		T* buff = new T();
		//buff->Parent = this;
		//int Idx = 0;
		for (auto component : ComponentList)
		//for (auto itr = ComponentList.begin(); itr != ComponentList.end(); ++itr)
		{
			if (typeid(*buff) == typeid(*component))
			{
				//itr = ComponentList.erase(itr);
				GetName();
			}
			//Idx++;
		}
	}

	// オブジェクトが持っているコンポーネント情報を表示
	void ParameterOutput()
	{
		// 名前表示
		char* TempName = Name.data();
		ImGui::InputText("ObjectName", TempName, 256);
		Name = TempName;

		// コンポーネントパラメータ表示
		for (auto component : ComponentList)
			//for (auto itr = ComponentList.begin(); itr != ComponentList.end(); itr++)
		{
			//if ((*itr) != nullptr)
			{
				component->ParameterDraw();
				//(*itr)->ParameterDraw();

				// 削除ボタン
				auto DeleteName = component->GetName() + " Delete";
				if (ImGui::Button(DeleteName.c_str()))
				{
					//DeleteComponent<typeid(component)>();
					DeleteComponent<TransformComponent>();
				}
			}
		}

		//// 削除ボタン
		//if (ImGui::Button(u8"Object Delete"))
		//{

		//}
	}
};

//std::list<GameObject*> g_ObjectList;

//
//class Player :public Component
//{
//	TransformComponent* pos = nullptr;
//public:
//	void Init()
//	{
//		if (pos == nullptr)
//		{
//			pos = Parent->GetComponent<TransformComponent>();
//			pos->x = 5; pos->y = 8;
//		}
//	}
//	void Draw()
//	{
//		if (pos == nullptr)
//		{
//			pos = Parent->GetComponent<TransformComponent>();
//		}
//	}
//	void Update()
//	{
//		if (pos == nullptr)
//		{
//			pos = Parent->GetComponent<TransformComponent>();
//			//移動のif文
//			{
//				pos->x++;
//			}
//		}
//	}
//};
//obj->AddCompnent<Player>(); // コンポーネント追加
// 位置座標追加
///	Position* pos = nullptr;
//	pos = Parent->GetComponent<Position>();