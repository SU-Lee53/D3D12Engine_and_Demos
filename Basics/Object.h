#pragma once

#include "Mesh.h"

class Transform;
class RenderMethod;
class Model;

// Derive it when need
class Object : public std::enable_shared_from_this<Object>
{
public:
	virtual BOOL Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;


};