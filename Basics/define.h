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
#define SHADER			GET_SINGLE(ShaderManager)
#define INPUT			GET_SINGLE(InputManager)
#define TIME			GET_SINGLE(TimeManager)
#define RESOURCE		GET_SINGLE(ResourceManager)
#define GUI				GET_SINGLE(ImGuiManager)
#define TEXTURE			GET_SINGLE(TextureManager)

#define DEVICE				CORE.GetDevice()
#define COMMAND_QUEUE		CORE.GetCommandQueue()
#define DT					TIME.GetDeltaTime()