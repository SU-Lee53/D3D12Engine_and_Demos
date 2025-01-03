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


#define GAME			GET_SINGLE(Game)
#define CORE			GET_SINGLE(Core)
#define RENDER			GET_SINGLE(RenderManager)

#define DEVICE					CORE.GetDevice()
#define COMMAND_QUEUE			CORE.GetCommandQueue()