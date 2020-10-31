#pragma once
#include "Core.h"
#include "Render.h"
#include "Event.h"

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
			Core::Core *core = nullptr;
			Render::Render *render = nullptr;
			Event::Event *event = nullptr;
			
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
			inline Core::Core *GetCore() { return core; }
			inline Render::Render *GetRender() { return render; }
			inline Event::Event *GetEvent() { return event; }
			
			//Get error
			const Error &GetError() const { return error; }
	};
}
