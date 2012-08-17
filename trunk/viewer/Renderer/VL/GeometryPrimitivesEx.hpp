#ifndef GeometricalPrimitivesEx_INCLUDE_ONCE
#define GeometricalPrimitivesEx_INCLUDE_ONCE

#include <vlCore/Vector4.hpp>
#include <vlGraphics/Geometry.hpp>
#include <vlCore/Colors.hpp>

 //ref<Geometry> makeTriadAxis( const vec3& origin, real diameter, real height, int phi, bool bottom);
ref<Geometry> makeCross( const vec3& origin, real length = 1.0f);
ref<Geometry> makeTriAxis();

#endif
