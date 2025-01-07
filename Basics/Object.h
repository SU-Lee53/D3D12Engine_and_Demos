#pragma once

// 필요와 경우에 따라 상속받아 사용

class Transform;
class RenderMethod;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object();

	virtual BOOL Initialize();
	virtual void Update();
	virtual void Render();

protected:
	std::shared_ptr<Transform> m_pTransform;
	std::shared_ptr<RenderMethod> m_pRenderMethod;
	ComPtr<ID3D12RootSignature> m_pRootSignature;

	// TODO : add mesh(model), material, etc...

};


class BasicForwardObject : public Object
{
public:
	BasicForwardObject();
	virtual ~BasicForwardObject();

	virtual BOOL Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	BOOL InitRootSignatrue();
	BOOL InitRenderMethod();

};
