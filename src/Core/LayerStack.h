#pragma once
#include <vector>

#include "Core\Layer.h"

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	std::vector<Layer*>::const_iterator begin() const { return m_Layers.cbegin(); }
	std::vector<Layer*>::const_iterator end() const { return m_Layers.cend(); }
private:
	std::vector<Layer*> m_Layers;
};