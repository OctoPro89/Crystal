#pragma once

#include "Crystal/Core/Core.h"
#include "Crystal/Core/Timestep.h"
#include "Crystal/Events/Event.h"

namespace Crystal {
	class Layer /* Layer class */
	{
	public:
		Layer(const std::string& name = "Layer"); /* Layer Constructer which takes in a const std::string reference which has a default name of "Layer" */
		virtual ~Layer(); /* Virtual deconstructor for inherited members */

		virtual void OnAttach() {} /* virtual OnAttach function */
		virtual void OnDetach() {} /* virtual OnDetach function */
		virtual void OnUpdate(Timestep ts) {} /* virtual OnUpdate function which takes in a Timestep */
		virtual void OnImGuiRender() {} /* virtual OnImGuiRender function */
		virtual void OnEvent(Event& event) {} /* virtual OnEvent function that takes in an Event reference */

		inline const std::string& GetName() const { return m_DebugName; } /* inline const string reference GetName function which returns the debugName */
	protected:
		std::string m_DebugName; /* string to hold the debugName */
	};
}
