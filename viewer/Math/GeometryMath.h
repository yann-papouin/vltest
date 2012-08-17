#ifndef GEOMETRY_MATH_H
#define GEOMETRY_MATH_H

// Return the length of a vector
float Length( const float* vector );

// Subtract vector2 from vector 1. Put the result into vectorResult and return vectorResult
float * Subtract( const float* vector1, const float* vector2, float* vectorResult );

// Negate vector and return it
float * Negate( float * vector );

// Multiply vector by a factor and return it.
float * Multiply( float * vector, const float factor );

// Normalize a vector and return it. A vector whose length is very near zero remains unchanged.
float * Normalize( float* vector );

// Return the scalar dot product.
float DotProduct( const float* vector1, const float* vector2 );

// Compute the cross product of 2 vector, put the result into vectorResult and return vectorResult
float * CrossProduct( const float* vectorA, const float* vectorB, float* vectorResult );

// Rotate inPoint so many degrees about inOriginPoint and inAxisVector, putting the result into outPoint
void RotatePoint( float* inPoint, double inDegrees, float* inOriginPoint, float* inAxisVector, float* outPoint );

// Get the ray (a point and a vector) based on a point picked on the display screen.
// Results are returned in rayOrigin and rayVector.
void GetRayVector( int screenX, int screenY, int screenWidth, int screenHeight,
			 float * cameraPosition, float * cameraTarget, float * cameraUp, float cameraWidth, float cameraHeight, bool perspective,
			 float * rayOrigin, float * rayVector );

// Find the intersection of a ray (a point and a vector) and a line segment.
bool RayHitsLineSegment( const float * lineStart, const float * lineEnd, const float lineWidth, const float * rayOrigin, const float * rayVector,
							float * hit, float  & param );

// Find the intersection of a ray (a point and a vector) and a triangle.
bool RayHitsTriangle( const float * point0, const float * point1, const float * point2, const float * rayOrigin, const float * rayVector,
						  float * hit, float & param, bool & coincident );

#endif
