#ifndef AxisRenderer_INCLUDE_ONCE
#define AxisRenderer_INCLUDE_ONCE

#include <vlGraphics/Renderer.hpp>
#include <vlGraphics/Geometry.hpp>

using namespace vl;

class AxisRenderer: public vl::Renderer
{
	VL_INSTRUMENT_CLASS(AxisRenderer, vl::Renderer)
public:
	AxisRenderer()
	{
		VL_DEBUG_SET_OBJECT_NAME()
			mClearFlags = CF_DO_NOT_CLEAR; // 7/30/2012 mwu : not clear the previous scene 
	}

	const RenderQueue* render(const RenderQueue* in_render_queue, Camera* camera, real frame_clock);
};
#endif
