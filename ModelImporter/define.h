#pragma once



#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname& GetInstance()			\
	{										\
		static classname s_instance;		\
		return s_instance;					\
	}


#define GET_SINGLE(classname)	classname::GetInstance()

#define HR_ASSERT(hResult)	assert(SUCCEEDED(hResult))


#define CORE			GET_SINGLE(D3D11Core)
#define DEVICE			GET_SINGLE(D3D11Core).GetDevice()
#define DC				GET_SINGLE(D3D11Core).GetDeviceContext()

#define GUI				GET_SINGLE(ImGuiManager)