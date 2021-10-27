
#include <Test.h>

#include <iostream>

int main()
{
	TestModule::loadLibrary();
	TestModule::Foo();
	std::cout << "bar = " << TestModule::getBar() << std::endl;

	std::cout << "Change test module, recompile, and press enter" << std::endl;
	while (std::cin.get() != '\n') {}

	TestModule::reloadLibrary();
	TestModule::Foo();
	std::cout << "bar = " << TestModule::getBar() << std::endl;

	return 0;
}
