#pragma once
#include "DirectX12Wrapper.h"
#include "ImGuiWrapper.h"
#include <list>
#include <string>

enum ComponentNameList
{
	Transform
};

// �O���錾
class GameObject;

// �R���|�[�l���g�Ǘ��N���X
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

// �ꏊ�������R���|�[�l���g
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


// �R���|�[�l���g�I�u�W�F�N�g�Ǘ��N���X
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

	// �R���|�[�l���g�S�̂̍X�V
	void Update()
	{
		for (auto component : ComponentList)
		{
			component->Update();
		}
	}

	// �R���|�[�l���g�S�̂̕`��
	void Draw()
	{
		for (auto component : ComponentList)
		{
			component->Draw();
		}
	}

	// �I�u�W�F�N�g�������Ă���R���|�[�l���g���擾
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

	// �I�u�W�F�N�g�������Ă���R���|�[�l���g��ǉ�
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

	// �I�u�W�F�N�g�������Ă���R���|�[�l���g���폜
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

	// �I�u�W�F�N�g�������Ă���R���|�[�l���g����\��
	void ParameterOutput()
	{
		// ���O�\��
		char* TempName = Name.data();
		ImGui::InputText("ObjectName", TempName, 256);
		Name = TempName;

		// �R���|�[�l���g�p�����[�^�\��
		for (auto component : ComponentList)
			//for (auto itr = ComponentList.begin(); itr != ComponentList.end(); itr++)
		{
			//if ((*itr) != nullptr)
			{
				component->ParameterDraw();
				//(*itr)->ParameterDraw();

				// �폜�{�^��
				auto DeleteName = component->GetName() + " Delete";
				if (ImGui::Button(DeleteName.c_str()))
				{
					//DeleteComponent<typeid(component)>();
					DeleteComponent<TransformComponent>();
				}
			}
		}

		//// �폜�{�^��
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
//			//�ړ���if��
//			{
//				pos->x++;
//			}
//		}
//	}
//};
//obj->AddCompnent<Player>(); // �R���|�[�l���g�ǉ�
// �ʒu���W�ǉ�
///	Position* pos = nullptr;
//	pos = Parent->GetComponent<Position>();