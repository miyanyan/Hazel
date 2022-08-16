#include "LayerStack.h"

namespace Hazel {
	LayerStack::LayerStack()
		: m_itLayerInsert(m_layers.begin())
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_layers) {
			delete layer;
		}
	}
	void LayerStack::pushLayer(Layer* layer)
	{
		m_itLayerInsert = m_layers.emplace(m_itLayerInsert, layer);
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end()) {
			m_layers.erase(it);
			m_itLayerInsert--;
		}
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end()) {
			m_layers.erase(it);
		}
	}

	std::vector<Layer*>::iterator LayerStack::begin()
	{
		return m_layers.begin();
	}
	std::vector<Layer*>::iterator LayerStack::end()
	{
		return m_layers.end();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rbegin()
	{
		return m_layers.rbegin();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rend()
	{
		return m_layers.rend();
	}
}