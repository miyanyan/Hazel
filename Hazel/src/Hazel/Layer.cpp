#include "Layer.h"

namespace Hazel {
	Layer::Layer(const std::string name)
		: m_debugName(name)
	{
	}

	Layer::~Layer()
	{
	}

	void Layer::onAttach()
	{
	}

	void Layer::onDetach()
	{
	}

	void Layer::onUpdate(Timestep ts)
	{
	}

	void Layer::onImGuiRender()
	{
	}

	void Layer::onEvent(Event& event)
	{
	}
}