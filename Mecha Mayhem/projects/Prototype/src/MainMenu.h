#pragma once
#include "Utilities/Scene.h"
#include "Utilities/Catmull.h"

class MainMenu : public Scene
{
public:
	MainMenu(const std::string& name) :
		Scene(name, BLM::GLMzero, false) {}

	virtual void Init(int windowWidth, int windowHeight) override;
	virtual void Update() override;
	virtual void Exit() override;

	virtual Scene* Reattach() override {
		m_bg.play();

		return Scene::Reattach();
	}

private:
	bool m_exit = false;
	bool playerSwapped[4] = {};

	int m_scenePos = 0;
	int width = 10;
	int height = 10;
	const int maxSelect = 4;

	float zoomTime = 1;
	float m_exitHoldTimer = 0;
	float m_confirmTimer = 1.f;

	Sound2D m_bg = { "MainMenu.mp3", "BG" };

	CatmullFollower cameraPath{ 1, {
		glm::vec3(-2.5f, 2.5f, 12.5f),
		glm::vec3(12.5f, 0.f, 12.5f),
		glm::vec3(12.5f, 2.5f, -2.5f),
		glm::vec3(-2.5f, 0.f, -2.5f)
	}, 8 };

	entt::entity camera = entt::null;
	entt::entity title = entt::null;
	entt::entity text = entt::null;
	entt::entity charSelect = entt::null;
	entt::entity backGround = entt::null;
	entt::entity models[4] = {
		entt::null,
		entt::null,
		entt::null,
		entt::null
	};
};

