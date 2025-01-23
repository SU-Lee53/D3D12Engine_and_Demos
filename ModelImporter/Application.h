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

private:
	std::unique_ptr<FbxLoader> m_pImporter = nullptr;
	std::shared_ptr<class Model> model;

};

