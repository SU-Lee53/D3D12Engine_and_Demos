#pragma once

class Object;
class Camera;

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	BOOL Initialize();

private:
	BOOL CreateCommandList();

public:
	void RenderBegin();
	void Render();
	void RenderEnd();
	void Present();

public:
	ComPtr<ID3D12GraphicsCommandList> GetCurrentCommandList() 
	{
		return m_pCommandLists[CORE.GetCurrentContextIndex()];
	}
	ComPtr<ID3D12CommandAllocator> GetCurrentCommandAllocator()
	{
		return m_pCommandAllocators[CORE.GetCurrentContextIndex()];
	}

public:
	void Add(std::shared_ptr<Object> pObj, std::shared_ptr<Camera> pCam) { m_pRenderQueue.push_back(std::make_pair(pObj, pCam)); }

public:
	void SetVsync(BOOL bVsyncOnOff) { bVsync = bVsyncOnOff; }

private:
	std::vector<std::pair<std::shared_ptr<Object>, std::shared_ptr<Camera>>> m_pRenderQueue = {};

private:
	// Pool 구조를 만들어 변경될 예정
	ComPtr<ID3D12GraphicsCommandList> m_pCommandLists[MAX_PENDING_FRAME_COUNT] = {};
	ComPtr<ID3D12CommandAllocator> m_pCommandAllocators[MAX_PENDING_FRAME_COUNT] = {};

	BOOL bVsync = FALSE;

	// Camera test
	Matrix m_matCamView = Matrix::Identity;
	Matrix m_matCamProj = Matrix::Identity;


};

