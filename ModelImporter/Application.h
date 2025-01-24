#pragma once

class FbxLoader;

class Application
{
public:
	Application();
	~Application();

public:
	void Initialize();
	void Update();
	void Render();

private:
	std::unique_ptr<FbxLoader> m_pImporter = nullptr;
	std::shared_ptr<class Model> m_pModel;

	std::shared_ptr<class Camera> m_pCamera = nullptr;

};

