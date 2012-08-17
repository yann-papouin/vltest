#ifndef AXIS_CAMERA_UPDATE_CALLBACK_INCLUDE_ONCE
#define AXIS_CAMERA_UPDATE_CALLBACK_INCLUDE_ONCE

#include <vlCore/Colors.hpp>
#include <vlGraphics/Actor.hpp>
#include <vlCore/Matrix4.hpp>

using namespace vl;
/*
Implements an AxisCameraUpdateCallback.
*/
class AxisCameraUpdateCallback: public vl::ActorEventCallback
{
public:
	// initialize the 3 gometry LODs and installs them into the specified Actor
	AxisCameraUpdateCallback(vl::Actor* actor)
	{
		mLastUpdate = 0;

		vl::ref<vl::Geometry> geom;
		const float side = 40;
		const int detail = 60;
	}

	// respond to onActorRenderStarted() event by animating the wave
	virtual void onActorRenderStarted(vl::Actor* act, vl::real frame_clock, const vl::Camera* cam, vl::Renderable* renderable, const vl::Shader*, int pass)
	{
		/* the beauty of this function is that in a few lines of code we update 3 different LOD levels!  */

		// update the geometry only the first time it is drawn
		if (pass > 0)
			return;

		fmat4 vmat = (fmat4)cam->viewMatrix();
		if (act->transform())
			vmat = vmat * (fmat4)act->transform()->worldMatrix();
		fmat4 nmat = vmat.as3x3();
		nmat = nmat.getInverse().transpose();

		//ÒÆ¶¯µ½×óÏÂ½Ç
		vec3 win(10,10,0);
		vec4 out;
		cam->unproject(win, out);

		ref<Geometry> geom = cast<Geometry>(renderable);
		ref<ArrayFloat3> vert_array = cast<ArrayFloat3>(geom->vertexArray());

		vec3 new_pos;
		new_pos.x()=out.x();
		new_pos.y()=out.y();
		new_pos.z()=out.z();

		fmat4 matrix = vl::mat4::getTranslation(-new_pos);
		
	//	geom->transform(matrix);
	}

	// don't respond to onActorDelete() event.
	virtual void onActorDelete(vl::Actor*) { }

protected:
	vl::real mLastUpdate;
};
#endif
