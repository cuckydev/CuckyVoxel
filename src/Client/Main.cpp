#include <iostream>
#include "Backend/Backend.h"

int main(int argc, char *argv[])
{
	//Initialize backend
	static const Backend::Config config = {
		{}, //core_config
		{ //render_config
			"CuckyVoxel", //title
			false, //fullscreen
			true, //resizable
			854, //width
			480, //height
		},
	};;
	
	Backend::Backend *backend = new Backend::Backend(Backend::Type_SDL2, config);
	if (backend == nullptr)
	{
		std::cout << "Failed to create backend instance" << std::endl;
		return 1;
	}
	else if (backend->GetError())
	{
		std::cout << backend->GetError() << std::endl;
		return 1;
	}
	
	//Delete backend
	delete backend;
	return 0;
}
