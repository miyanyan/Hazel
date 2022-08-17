#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example") {}
	
	void onUpdate() override {
		//HZ_INFO("ExampleLayer::update");
	}

	void onEvent(Hazel::Event& e) override {
		HZ_INFO("ExampleLayer::event {}", e.toString());
	}
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox(){
		pushLayer(new ExampleLayer());
	}
	~Sandbox(){}
};

Hazel::Application* Hazel::createApplication()
{
	return new Sandbox();
}