#include "crystalpch.h"
#include "LayerStack.h"

namespace Crystal {
	LayerStack::LayerStack()
	{
		/* Do nothing lol */
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers) /* loop through each layer in the layer stack */
		{
			layer->OnDetach(); /* for each layer in the stack call OnDetach */
			delete layer;	   /* then delete that layer from memory */
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer); /* The vector::emplace() is an STL in C++ which extends the container by inserting a new element at the position. Reallocation happens only if there is a need for more space. */
		m_LayerInsertIndex++;											/* Then here we add 1 to m_LayerInsert Index */
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay); /* emplace back emplaces the object in-place within the container */
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer); /* an iterator of the layers to go through them*/
		if (it != m_Layers.begin() + m_LayerInsertIndex)									 /* Go through the layers and make sure it's in the correct index */
		{
			layer->OnDetach();																 /* if so then call OnDetach on the layer */
			m_Layers.erase(it);																 /* erase the iterator */
			m_LayerInsertIndex--;															 /* subtract 1 from the layerinsertindex */
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay); /* an iterator of the overlays to go through them */
		if (it != m_Layers.end())															 /* go through the overlays and make sure they are at the correct index */
		{
			overlay->OnDetach();															 /* call OnDetach on the overlay */
			m_Layers.erase(it);																 /* erase the iterator / overlay */
		}
	}
}
