#include "DemoScene.h"

void DemoScene::Init(int windowWidth, int windowHeight)
{
	//generic init, use as guidelines
	ECS::AttachRegistry(&m_reg);
	PhysBody::Init(m_world);
	ECS::AttachWorld(m_world);
	std::string input = "testmap";
	std::cout << "filename: " + input + "\n";
	//std::cin >> input;
	if (!m_colliders.Init(m_world, input, false, false))
		std::cout << input + " failed to load, no collision boxes loaded\n";

	width = windowWidth;
	height = windowHeight;


	/// Creating Entities
	for (int x(0); x < 3; ++x)
	{
		unsigned cameraEnt2 = ECS::CreateEntity();
		auto& camCam2 = ECS::AttachComponent<Camera>(cameraEnt2);
		camCam2.SetFovDegrees(60.f).ResizeWindow(width, height);
		glm::vec3 axis = glm::vec3(rand() % 2, rand() % 2, rand() % 2 + 1);
		ECS::GetComponent<Transform>(cameraEnt2).
			SetPosition(glm::vec3(rand() % 21 - 10, 0, rand() % 21 - 10)).
			SetRotation(glm::rotate(glm::quat(1, 0, 0, 0), float(rand() % 360), axis));
	}
	cameraEnt = ECS::CreateEntity();
	auto& camCam = ECS::AttachComponent<Camera>(cameraEnt);
	camCam.SetFovDegrees(60.f).ResizeWindow(width, height);

	bodyEnt = ECS::CreateEntity();
	ECS::AttachComponent<PhysBody>(bodyEnt).Init(0.5f, 2, glm::vec3(0, 10, 0), 1, true).
		SetGravity(glm::vec3(0)).GetBody()->setAngularFactor(btVector3(0, 0, 0));
	ECS::AttachComponent<ObjLoader>(bodyEnt).LoadMesh("models/Pistol.obj");
	ECS::GetComponent<Transform>(bodyEnt).SetScale(0.1f);

	Dio = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(Dio).LoadMesh("models/Pistol.obj", true);

	P = ECS::CreateEntity();
	ECS::GetComponent<Transform>(P).SetPosition(glm::vec3(0, 1, 0)).ChildTo(bodyEnt).SetScale(10.f);
	ECS::GetComponent<Transform>(Dio).SetPosition(glm::vec3(0.5f, 0, -0.5f)).ChildTo(P).SetScale(0.25f);
	ECS::GetComponent<Transform>(cameraEnt).SetPosition(glm::vec3(-0.5f, 0, 5)).ChildTo(P);
	{
		unsigned entity = ECS::CreateEntity();
		auto& temp = ECS::AttachComponent<ObjLoader>(entity).LoadMesh("models/cringe.obj", true);
		ECS::GetComponent<Transform>(entity).SetPosition(glm::vec3(0, -30, 0)).SetScale(4.f);
	}

	/// End of creating entities
	Rendering::DefaultColour = glm::vec4(1.f, 0.5f, 0.5f, 1.f);
	Rendering::hitboxes = &m_colliders;
}

void DemoScene::Update()
{
	//auto& camTrans = ECS::GetComponent<Transform>(cameraEnt);
	auto& ballPhys = ECS::GetComponent<PhysBody>(bodyEnt).SetAwake();

	/// start of loop
	
	//if (Input::GetKeyDown(KEY::ESC)) {
	if (Input::GetKeyDown(KEY::ESC)) {
		if (Input::GetKey(KEY::LSHIFT)) {
			if (screen = !screen)	BackEnd::SetTabbed(width, height);
			else					BackEnd::SetFullscreen();
		}
		else {
			m_camCount++;
			if (m_camCount > 4)		m_camCount = 1;
			if (m_camCount == 2)	ECS::GetComponent<Camera>(cameraEnt).ResizeWindow(width / 2, height);
			else					ECS::GetComponent<Camera>(cameraEnt).ResizeWindow(width, height);
		}
	}

	if (Input::GetKey(KEY::UP))			rot.x += 3.f * m_dt;
	if (Input::GetKey(KEY::DOWN))		rot.x -= 3.f * m_dt;
	if (Input::GetKey(KEY::LEFT))		rot.y -= 3.f * m_dt;
	if (Input::GetKey(KEY::RIGHT))		rot.y += 3.f * m_dt;
	if (rot.x > pi)			rot.x = pi;
	else if (rot.x < -pi)	rot.x = -pi;

	{
		//glm::quat rotf = glm::rotate(startQuat, rot.y, glm::vec3(0, -1, 0));
		//rotf = glm::rotate(rotf, rot.x, glm::vec3(1, 0, 0));
		ballPhys.SetRotation(glm::rotate(startQuat, rot.y, glm::vec3(0, -1, 0)));
		ECS::GetComponent<Transform>(P).SetRotation(glm::rotate(startQuat, rot.x, glm::vec3(1, 0, 0)));
	}
	/*glm::mat4 rotf = glm::rotate(glm::mat4(1.f), rot.x, glm::vec3(1, 0, 0));
	rotf = glm::rotate(rotf, rot.y, glm::vec3(0, 1, 0));
	camTrans.SetRotation(glm::mat3(rotf));*/

	glm::vec3 pos = glm::vec3(0.f);
	if (Input::GetKey(KEY::W))		pos.z -= 15;
	if (Input::GetKey(KEY::S))		pos.z += 15;
	if (Input::GetKey(KEY::A))		pos.x -= 15;
	if (Input::GetKey(KEY::D))		pos.x += 15;
	if (Input::GetKey(KEY::LSHIFT)) {
		pos.y -= 15;
		//ballPhys.SetVelocity(btVector3(0.f, 0.f, 0.f));
	}
	if (Input::GetKey(KEY::SPACE)) {
		//pos.y += 50 /*x at least two times this*/;
		pos.y += 15;
	}
	//if (pos.x != 0 || pos.y != 0 || pos.z != 0) {
		pos = glm::vec4(pos, 1) * glm::rotate(glm::mat4(1.f), rot.y, glm::vec3(0, 1, 0));
		//camTrans.SetPosition(camTrans.GetPosition() + pos);
		//btVector3 temp = ballPhys.GetVelocity();
		//if (pos.x != 0)		temp.setX(pos.x);
		//if (pos.y != 0)		temp.setY(pos.y);
		//if (pos.z != 0)		temp.setZ(pos.z);
		//ballPhys.SetVelocity(temp);
		ballPhys.SetVelocity(pos);
	//}

	if (Input::GetKey(KEY::P)) {
		if (Input::GetKey(KEY::LSHIFT))
			Rendering::LightColour = glm::vec3(3.f);
		else
			Rendering::LightColour = glm::vec3((rand() % 30) / 10.f, (rand() % 30) / 10.f, (rand() % 30) / 10.f);
	}

	/// End of loop
	if (Input::GetKeyDown(KEY::FSLASH))	m_colliders.ToggleDraw();
	m_colliders.Update(m_dt);
	if (Input::GetKeyDown(KEY::F10))	if (!m_colliders.SaveToFile())		std::cout << "file save failed\n";
	if (Input::GetKeyDown(KEY::F9))	if (!m_colliders.LoadFromFile())	std::cout << "file load failed\n";
}