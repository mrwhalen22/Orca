#include "oapch.h"
#include "LayerStack.h"

namespace Orca {

	LayerStack::LayerStack() {

	}

	LayerStack::~LayerStack() {
		OA_PROFILE_FUNCTION();
		for (Layer* layer : m_Layers) delete layer;
	}

	void LayerStack::PushLayer(Layer* layer) {
		OA_PROFILE_FUNCTION();
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;

	}

	void LayerStack::PopLayer(Layer* layer) {
		OA_PROFILE_FUNCTION();
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}
	void LayerStack::PushOverlay(Layer* overlay) {
		OA_PROFILE_FUNCTION();
		m_Layers.emplace_back(overlay);
	}
	void LayerStack::PopOverlay(Layer* overlay) {
		OA_PROFILE_FUNCTION();
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}


}
