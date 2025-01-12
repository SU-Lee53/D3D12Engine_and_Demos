#pragma once

// ��� �������Ұ��ΰ� 
// MRT �� ���� Shader-Pass �� RenderMethod Ŭ������ ����
// RenderMethod �� ��ӹ޾Ƽ� ���� ������ ��� �� ������� �����ϵ��� ��
// RenderManager �� RenderMethod::Render �� �̿��Ͽ� �������� ����

// �ϴ� owner �� Object �� ��
// �� Mesh �� �ٸ� ���𰡷� �ٲ�Ű��� ��

// TODO : ����

class Object;
class Pipeline;

class RenderMethod
{
public:
	RenderMethod();
	virtual ~RenderMethod();

	virtual void Initialize(std::shared_ptr<Object> owner, const std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC>& psoDescs) = 0;
	virtual void Render() = 0;

protected:
	std::weak_ptr<Object> m_wpOwner;
	
	std::vector<std::shared_ptr<Pipeline>> m_Pipelines;
	std::vector<ComPtr<ID3D12Resource>> m_RTVs;
	DWORD m_dwPassCount = 0;
	DWORD m_dwRTVCount = 0;

};


// Basic Forward rendering RenderMethod
// PassCount = 0, RTVCount = 0
class ForwardRender : public RenderMethod
{
public:
	ForwardRender();
	virtual ~ForwardRender();

	virtual void Initialize(std::shared_ptr<Object> owner, const std::vector<D3D12_GRAPHICS_PIPELINE_STATE_DESC>& psoDescs);
	virtual void Render();


private:


};