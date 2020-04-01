
#include "Vulkan2020App.h"

int main()
{
	Vulkan2020App app;

	try
	{
		app.Run();
	}
	catch ( const std::exception & e )
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}