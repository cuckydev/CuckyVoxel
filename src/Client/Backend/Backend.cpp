#include "Backend.h"

//Include backends
#ifdef COMPILE_SDL2
	#include "SDL2/Core.h"
	#include "SDL2/Render.h"
	#include "SDL2/Event.h"
#endif

namespace Backend
{
	//Internal interface
	bool Backend::ConstructSubsystems(const Backend_Type type)
	{
		//Delete previous subsystems
		delete core; core = nullptr;
		delete render; render = nullptr;
		delete event; event = nullptr;
		
		//Construct new subsystems
		switch (type)
		{
			case Type_Null:
				return false;
		#ifdef COMPILE_SDL2
			case Type_SDL2:
				core = new Core::Core_SDL2();
				render = new Render::Render_SDL2();
				event = new Event::Event_SDL2();
				break;
		#endif
			default:
				return error.Push("Invalid backend selected");
		}
		
		//Verify subsystems were successfully created
		if (core == nullptr)
			return error.Push("Failed to create core subsystem");
		else if (core->GetError())
			return error.Push(core->GetError());
		
		if (render == nullptr)
			return error.Push("Failed to create render subsystem");
		else if (render->GetError())
			return error.Push(render->GetError());
		
		if (event == nullptr)
			return error.Push("Failed to create event subsystem");
		else if (event->GetError())
			return error.Push(event->GetError());
		
		return false;
	}
	
	//Constructors and destructor
	Backend::Backend(const Backend_Type type)
	{
		//Construct given backend subsystems
		ConstructSubsystems(type);
	}
	
	Backend::Backend(const Backend_Type type, const Config config)
	{
		//Construct given backend subsystems
		if (ConstructSubsystems(type))
			return;
		
		//Set subsystem configs
		if (SetConfig(config))
			return;
	}
	
	Backend::~Backend()
	{
		//Delete subsystems
		ConstructSubsystems(Type_Null);
	}
	
	//Backend interface
	bool Backend::SetConfig(const Config config)
	{
		//Set backend configs
		if (core != nullptr)
			if (core->SetConfig(config.core_config))
				return error.Push(core->GetError());
		if (render != nullptr)
			if (render->SetConfig(config.render_config))
				return error.Push(render->GetError());
		if (event != nullptr)
			if (event->SetConfig(config.event_config))
				return error.Push(event->GetError());
		return false;
	}
}
