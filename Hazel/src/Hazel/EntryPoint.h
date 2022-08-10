#pragma once

#include "Log.h"

extern Hazel::Application* Hazel::createApplication();

int main()
{
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("hello var = {0}", a);
	auto app = Hazel::createApplication();
	app->run();
	delete app;
}