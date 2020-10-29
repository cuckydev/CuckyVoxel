#pragma once
#include "../Core.h"

namespace Backend
{
	namespace Core
	{
		class Core_SDL2 : public Core_Base
		{
			public:
				//Constructors and destructor
				Core_SDL2();
				Core_SDL2(const Config config);
				~Core_SDL2() override;
				
				//Core interface
				bool SetConfig(const Config config) override;
		};
	}
}
