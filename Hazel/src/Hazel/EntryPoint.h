#pragma once

extern Hazel::Application* Hazel::createApplication();

int main()
{
	auto app = Hazel::createApplication();
	app->run();
	delete app;
}