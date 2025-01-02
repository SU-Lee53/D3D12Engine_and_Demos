#pragma once
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
	void SetVsync(BOOL bVsyncOnOff) { bVsync = bVsyncOnOff; }

private:
	// Pool ������ ����� ����� ����
	ComPtr<ID3D12GraphicsCommandList> m_pCommandLists[MAX_PENDING_FRAME_COUNT] = {};
	ComPtr<ID3D12CommandAllocator> m_pCommandAllocators[MAX_PENDING_FRAME_COUNT] = {};

	BOOL bVsync = FALSE;

};

