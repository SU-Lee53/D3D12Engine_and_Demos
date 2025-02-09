#pragma once

class FbxLoader;
class AssimpConverter;

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
	std::unique_ptr<AssimpConverter> m_pImporter = nullptr;
	std::shared_ptr<class Model> m_pLoadedFromImporter;
	std::shared_ptr<class Model> m_pLoadedFromBinaries;

	std::shared_ptr<class Camera> m_pCamera = nullptr;
	float m_fMouseDeltaX = 0.f;
	float m_fMouseDeltaY = 0.f;
	POINT m_MousePoint;

	// Show m_pLoadedFromImporter when m_bShowWhat == TRUE
	// Show m_pLoadedFromBinaries when m_bShowWhat == FALSE
	BOOL m_bShowWhat = TRUE;

};

