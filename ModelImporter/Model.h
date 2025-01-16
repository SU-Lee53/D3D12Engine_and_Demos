#pragma once

class Mesh;
class Transform;

class Model
{
public:
	Model();
	~Model();

private:
	std::unique_ptr<Mesh> m_upMesh = nullptr;
	std::unique_ptr<Transform> m_upTransform = nullptr;


};

