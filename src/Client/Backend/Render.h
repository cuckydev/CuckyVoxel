#pragma once
#include <string>
#include <Common/Util/Error.h>

namespace Backend
{
	namespace Render
	{
		//Render config
		struct Config
		{
			std::string title;
			bool fullscreen, resizable;
			unsigned width, height;
		};
		
		//Render classes
		
		
		//Render base class
		class Render_Base
		{
			protected:
				//Error
				Error error;
				
			public:
				//Destructor
				virtual ~Render_Base() {}
				
				//Interface
				virtual bool SetConfig(const Config config) = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}
