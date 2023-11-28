#pragma once

#include "Crystal/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Crystal {
	class LayerStack
	{
	public:
		LayerStack();	/* Layerstack Constructor */
		~LayerStack();	/* Virtual Constructor */

		void PushLayer(Layer* layer);		/* PushLayer function which takes in a Layer pointer */
		void PushOverlay(Layer* overlay);	/* PushOverlay function which takes in a Layer pointer */
		void PopLayer(Layer* layer);		/* PopLayer function which takes in a Layer pointer */
		void PopOverlay(Layer* layer);		/* PopOverlay function which takes in a Layer pointer */

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }			 /* a vector of Layer pointers which we want the beginning from that will return an std::vector::iterator that is the beginning */
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }				 /* a vector of Layer pointers which we want the end from that will return an std::vector::iterator that is the end */
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); } /* a vector of Layer pointers which we want the beginning from that will return an std::vector::reverse_iterator that is the beginning */
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }	 /* a vector of Layer pointers which we want the end from that will return an std::vector::reverse_iterator that is the end */

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }				/* a vector of Layer pointers which we want the CONST beginning from that will return an std::vector::const_iterator that is the beginning */
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }				/* a vector of Layer pointers which we want the CONST end from that will return an std::vector::const_iterator that is the end */
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }	/* a vector of Layer pointers which we want the CONST REVERSE beginning from that will return an std::vector::const_reverse_iterator that is the beginning */
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }		/* a vector of Layer pointers which we want the CONST REVERSE end from that will return an std::vector::const_reverse_iterator that is the end */
	private:
		std::vector<Layer*> m_Layers; /* a vector of layer pointers to hold the layers */
		unsigned int m_LayerInsertIndex = 0; /* and unsigned integer for the layerinsertindex */
	};
}
