#pragma once

#include <Common/Util/Error.h>

namespace Backend
{
	namespace Core
	{
		//Core config
		struct Config
		{
			
		};
		
		//Core base class
		class Core_Base
		{
			protected:
				//Error
				Error error;
				
			public:
				//Destructor
				virtual ~Core_Base() {}
				
				//Interface
				virtual bool SetConfig(const Config config) = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}
