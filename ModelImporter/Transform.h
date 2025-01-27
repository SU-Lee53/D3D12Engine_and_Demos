#pragma once

class Transform
{
public:
	Transform();
	~Transform();

public:
	BOOL Initialize();
	BOOL Update();

public:
	// Getter
	XMFLOAT3 GetLocalPosition() { return m_vLocalPosition; }
	XMFLOAT3 GetLocalScale() { return m_vLocalScale; }
	XMFLOAT3 GetLocalRotation() { return m_vLocalRotation; }
	XMFLOAT4X4 GetLocalMatrix() { return m_matLocal; }
	XMFLOAT4X4 GetLocalMatrixTransposed() 
	{ 
		XMMATRIX xmLocalTransposed = XMMatrixTranspose(XMLoadFloat4x4(&m_matLocal)); 
		XMFLOAT4X4 ret;
		XMStoreFloat4x4(&ret, xmLocalTransposed);
		return ret;
	}

	XMFLOAT3 GetWorldPosition() { return m_vWorldPosition; }
	XMFLOAT3 GetWorldScale() { return m_vWorldScale; }
	XMFLOAT3 GetWorldRotation() { return m_vWorldRotation; }
	XMFLOAT4X4 GetWorldMatrix() { return m_matWorld; }
	XMFLOAT4X4 GetWorldMatrixTransposed()
	{
		XMMATRIX xmWorldTransposed = XMMatrixTranspose(XMLoadFloat4x4(&m_matWorld));
		XMFLOAT4X4 ret;
		XMStoreFloat4x4(&ret, xmWorldTransposed);
		return ret;
	}

public:
	// Setter
	void SetLocalPosition(const XMFLOAT3& pos);
	void SetLocalScale(const XMFLOAT3& scale);
	void SetLocalRotation(const XMFLOAT3& rot);
	 
	void SetWorldPosition(const XMFLOAT3& pos);
	void SetWorldScale(const XMFLOAT3& scale);
	void SetWorldRotation(const XMFLOAT3& rot);

private:
	// Local Transform
	XMFLOAT3	m_vLocalPosition = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vLocalRotation = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vLocalScale = { 1.f, 1.f, 1.f };
	XMFLOAT4X4	m_matLocal;

	// World Transform
	XMFLOAT3	m_vWorldPosition = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vWorldRotation = { 0.f, 0.f, 0.f };
	XMFLOAT3	m_vWorldScale = { 1.f, 1.f, 1.f };
	XMFLOAT4X4	m_matWorld;

	BOOL m_bLocalUpdated = FALSE;
	BOOL m_bWorldUpdated = FALSE;

public:
	friend std::ostream& operator<<(std::ostream& os, const Transform& transform)
	{
		// Export Order : Position - Rotation - Scale
		os << "<Transform>" << std::endl;

		os << "<Local>" << std::endl;
		os.write(reinterpret_cast<const char*>(&transform.m_vLocalPosition), sizeof(transform.m_vLocalPosition));
		os.write(reinterpret_cast<const char*>(&transform.m_vLocalRotation), sizeof(transform.m_vLocalRotation));
		os.write(reinterpret_cast<const char*>(&transform.m_vLocalScale), sizeof(transform.m_vLocalScale));
		os << std::endl;
		os << "</Local>" << std::endl;
		
		os << "<World>" << std::endl;
		os.write(reinterpret_cast<const char*>(&transform.m_vWorldPosition), sizeof(transform.m_vWorldPosition));
		os.write(reinterpret_cast<const char*>(&transform.m_vWorldRotation), sizeof(transform.m_vWorldRotation));
		os.write(reinterpret_cast<const char*>(&transform.m_vWorldScale), sizeof(transform.m_vWorldScale));
		os << std::endl;
		os << "</World>" << std::endl;

		os << "</Transform>" << std::endl;

		return os;
	}

	friend std::istream& operator>>(std::istream& is, Transform& transform)
	{
		std::string read;
		while (read != "</Transform>")
		{
			std::getline(is, read);
			
			if (read == "<Local>")
			{
				is.read(reinterpret_cast<char*>(&transform.m_vLocalPosition), sizeof(transform.m_vLocalPosition));
				is.read(reinterpret_cast<char*>(&transform.m_vLocalRotation), sizeof(transform.m_vLocalRotation));
				is.read(reinterpret_cast<char*>(&transform.m_vLocalScale), sizeof(transform.m_vLocalScale));
				transform.m_bLocalUpdated = TRUE;
			}
		
			if (read == "<World>")
			{
				is.read(reinterpret_cast<char*>(&transform.m_vWorldPosition), sizeof(transform.m_vWorldPosition));
				is.read(reinterpret_cast<char*>(&transform.m_vWorldRotation), sizeof(transform.m_vWorldRotation));
				is.read(reinterpret_cast<char*>(&transform.m_vWorldScale), sizeof(transform.m_vWorldScale));
				transform.m_bWorldUpdated = TRUE;
			}
		}

		transform.Update();

		return is;
	}

};

