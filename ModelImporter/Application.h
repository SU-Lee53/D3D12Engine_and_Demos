#pragma once

class Importer;

class Application
{
public:
	Application();
	~Application();

public:
	void Initialize();
	void Update();

private:
	std::unique_ptr<Importer> m_pImporter = nullptr;

	BOOL m_bShown = FALSE;

};

