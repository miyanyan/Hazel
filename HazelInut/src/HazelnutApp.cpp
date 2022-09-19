#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Hazel {

	class Hazelnut : public Application
	{
	public:
		Hazelnut()
			: Application("Hazelnut")
		{
			pushLayer(new EditorLayer());
		}

		~Hazelnut()
		{
		}
	};

	Application* createApplication()
	{
		return new Hazelnut();
	}

}