#pragma once

struct DescriptorHeap;

class ImGuiManager
{
	DECLARE_SINGLE(ImGuiManager);

public:
	void Initialize();
	void Update();
	void Render();

private:
	std::unique_ptr<DescriptorHeap> m_pFontSrvDescriptorHeap = nullptr;

};

