#include "RenderCommand.h"

namespace Hazel {

	std::unique_ptr<RendererAPI> RenderCommand::s_rendererAPI = RendererAPI::create();

}