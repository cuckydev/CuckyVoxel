#pragma once
#include "Core.h"
#include "Render.h"

#include <Common/Util/Error.h>

namespace Backend
{
	//Backend type enum
	enum Backend_Type
	{
		Type_Null,
		Type_SDL2,
	};
	
	//Backend subsystem configs
	struct Config
	{
		Core::Config core_config;
		Render::Config render_config;
	};
	
	//Backend container class
	class Backend
	{
		private:
			//Error
			Error error;
			
			//Subsystems
			Core::Core_Base *core = nullptr;
			Render::Render_Base *render = nullptr;
			
		private:
			//Internal interface
			bool ConstructSubsystems(const Backend_Type type);
			
		public:
			//Constructors and destructor
			Backend(const Backend_Type type);
			Backend(const Backend_Type type, const Config config);
			~Backend();
			
			//Backend interface
			bool SetConfig(const Config config);
			
			//Get subsystems
			inline Render::Render_Base *GetRender() { return render; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}
