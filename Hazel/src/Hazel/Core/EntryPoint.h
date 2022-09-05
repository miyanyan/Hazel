#pragma once

#include "Log.h"

extern Hazel::Application* Hazel::createApplication();

int main()
{
	HZ_CORE_INFO("Hello Hazel");
	auto app = Hazel::createApplication();
	app->run();
	delete app;
}