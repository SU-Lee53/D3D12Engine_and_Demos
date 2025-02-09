#include "pch.h"
#include "TextureDemo.h"
#include "TextureDemoResources.h"

using namespace TextureDemoResource;
using namespace std;

void TextureDemo::Initialize()
{
	m_pObj = make_shared<TextureDemoObject>();
	m_pObj->Initialize();

	shared_ptr<TextureDemoObject> originObj = static_pointer_cast<TextureDemoObject>(m_pObj);
	originObj->LoadFromBinaryFile(L"../Models/Binaries/Gunship.bin");
}

void TextureDemo::Update()
{
	m_pObj->Update();
}

void TextureDemo::Render()
{
	RENDER.Add(m_pObj);
}
