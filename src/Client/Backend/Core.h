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
		class Core
		{
			protected:
				//Error
				Error error;
				
			public:
				//Virtual destructor
				virtual ~Core() {}
				
				//Core interface
				virtual bool SetConfig(const Config config) = 0;
				
				//Get error
				const Error &GetError() const { return error; }
		};
	}
}
