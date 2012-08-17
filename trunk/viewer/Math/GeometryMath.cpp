#include "stdafx.h"
#include "math.h"
#include "float.h"

#include "GeometryMath.h"

static float nearZero = 1e-9f;

float Length( const float* vector )
{
	return sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

float * Subtract( const float * vector1, const float * vector2, float * vectorResult )
{
	 vectorResult[0] = vector1[0] - vector2[0];
	 vectorResult[1] = vector1[1] - vector2[1];
	 vectorResult[2] = vector1[2] - vector2[2];
	 return vectorResult;
}

float * Negate( float * vector )
{
	 vector[0] = -vector[0];
	 vector[1] = -vector[1];
	 vector[2] = -vector[2];
	 return vector;
}

float * Multiply( float * vector, const float factor )
{
	 vector[0] = vector[0] * factor;
	 vector[1] = vector[1] * factor;
	 vector[2] = vector[2] * factor;
	 return vector;
}

float * Normalize( float* vector )
{
	float length = Length(vector);
	if ( length > nearZero )
	{
		for (int i = 0; i < 3; i++)
			vector[i] /= length;
	}
	return vector;
}

float DotProduct( const float* vector1, const float* vector2 )
{
	return vector1[0] * vector2[0] + vector1[1] * vector2[1] + vector1[2] * vector2[2];
}

float * CrossProduct( const float* vectorA, const float* vectorB, float* vectorResult )
{
	 vectorResult[0] = vectorA[ 1 ] * vectorB[ 2 ] - vectorA[ 2 ] * vectorB[ 1 ];
	 vectorResult[1] = vectorA[ 2 ] * vectorB[ 0 ] - vectorA[ 0 ] * vectorB[ 2 ];
	 vectorResult[2] = vectorA[ 0 ] * vectorB[ 1 ] - vectorA[ 1 ] * vectorB[ 0 ];
	 return vectorResult;
}

void RotatePoint(float* inPoint, double inDegrees, float* inOriginPoint, float* inAxisVector, float* outPoint)
{
	double radians = 3.1415926 / 180.0 * inDegrees;
	float temp[3];
	temp[0] = inPoint[0] - inOriginPoint[0];
	temp[1] = inPoint[1] - inOriginPoint[1];
	temp[2] = inPoint[2] - inOriginPoint[2];
	float s = (float)sin(radians);
	float c = (float)cos(radians);
	float C = 1.0f - c;

	outPoint[0] =	temp[0] * (inAxisVector[0] * inAxisVector[0] * C + c)
				  + temp[1] * (inAxisVector[0] * inAxisVector[1] * C - inAxisVector[2] * s)
				  + temp[2] * (inAxisVector[0] * inAxisVector[2] * C + inAxisVector[1] * s)
				  + inOriginPoint[0];
	outPoint[1] =	temp[0] * (inAxisVector[1] * inAxisVector[0] * C + inAxisVector[2] * s)
				  + temp[1] * (inAxisVector[1] * inAxisVector[1] * C + c)
				  + temp[2] * (inAxisVector[1] * inAxisVector[2] * C - inAxisVector[0] * s)
				  + inOriginPoint[1];
	outPoint[2] =	temp[0] * (inAxisVector[2] * inAxisVector[0] * C - inAxisVector[1] * s)
				  + temp[1] * (inAxisVector[2] * inAxisVector[1] * C + inAxisVector[0] * s)
				  + temp[2] * (inAxisVector[2] * inAxisVector[2] * C + c)
				  + inOriginPoint[2];
}

void GetRayVector( int screenX, int screenY, int screenWidth, int screenHeight,
			 float * cameraPosition, float * cameraTarget, float * cameraUp, float cameraWidth, float cameraHeight, bool perspective, 
			 float * rayOrigin, float * rayVector )
{
	float xPercent = ( screenX + 0.5f) / screenWidth * 2.0f - 1;  // range is -1 to +1
	float yPercent = ( screenY + 0.5f) / screenHeight * 2.0f - 1;  // range is -1 to +1
	yPercent = -yPercent;
	float viewVector[ 3 ];
	for ( int i = 0; i < 3; i++ )
		viewVector[ i ] = cameraPosition[ i ] - cameraTarget[ i ];
	float rightVector[ 3 ];
	CrossProduct( cameraUp, viewVector, rightVector );
	Normalize( rightVector );
	float xVector[ 3 ];
	for ( int i = 0; i < 3; i++ )
		xVector[ i ] = (xPercent * cameraWidth / 2.0f) * rightVector[ i ];
	float yVector[ 3 ];
	for ( int i = 0; i < 3; i++ )
		yVector[ i ] = (yPercent * cameraHeight / 2.0f) * cameraUp[ i ];
	for ( int i = 0; i < 3; i++ )
		rayOrigin[ i ] = (float)( cameraTarget[ i ] + xVector[ i ] + yVector[ i ] );
	if ( perspective )
	{
		Subtract( rayOrigin, cameraPosition, rayVector );
		for ( int i = 0; i < 3; i++ )
			rayOrigin[ i ] = cameraPosition[ i ];
	}
	else
	{
		Subtract( cameraTarget, cameraPosition, rayVector );
		float distance = Length( viewVector );
		Normalize( rayVector );
		Multiply( rayVector, distance );
		Subtract( rayOrigin, rayVector, rayOrigin );
	}
}

bool SameHalfSpace( const float * point1, const float * point2, const float * planePoint, const float *planeNormal )
{	// Do 2 points lie in the same half-space defined by the given plane?
	float vector1[ 3 ], vector2[ 3 ];
	Subtract( point1, planePoint, vector1 );
	Subtract( point2, planePoint, vector2 );
	if ( DotProduct( vector1, planeNormal ) * DotProduct( vector2, planeNormal ) < 0 )
		return false;
	return true;
}

bool RayHitsPlane( const float * planePoint, const float * planeNormal, const float * rayOrigin, const float * rayVector,
					float * hit, float & param )
{	// Find the intersection of a ray with a plane.
	float planeNormalLength = Length( planeNormal );
	if ( planeNormalLength < nearZero )
		return FALSE;
	float planeNormalNormalized[ 3 ];
	for ( int i = 0; i < 3; i++ )
		planeNormalNormalized[ i ] = planeNormal[ i ] / planeNormalLength;
	float rayPlaneVector[ 3 ];
	Subtract( planePoint, rayOrigin, rayPlaneVector );
	float drop = DotProduct( planeNormalNormalized, rayPlaneVector );
	float dropVector[ 3 ];
	for ( int i = 0; i < 3; i++ )
		dropVector[ i ] = drop * planeNormalNormalized[ i ];
	float dropVectorLength = Length( dropVector );
	if ( dropVectorLength < nearZero )
	{
		for ( int i = 0; i < 3; i++ )
			hit[ i ] = rayOrigin[ i ];
		param = 0.0f;
		return true;
	}
	float dropUnitVector[ 3 ];
	for ( int i = 0; i < 3; i++ )
		dropUnitVector[ i ] = dropVector[ i ] / dropVectorLength;
	float rayUnitVector[ 3 ];
	for ( int i = 0; i < 3; i++ )
		rayUnitVector[ i ] = rayVector[ i ];
	Normalize( rayUnitVector );
	float dot = DotProduct( dropUnitVector, rayUnitVector );
	if ( fabs( dot ) < nearZero )
		return FALSE;
	param = dropVectorLength / dot;
	if ( param < 0 )
		return false;
	for ( int i = 0; i < 3; i++ )
		hit[ i ] = rayOrigin[ i ] + param * rayUnitVector[ i ];
	return true;
}

float LineHitsLine( const float *line0Point, const float *line0Vector, float &line0Param,
				   const float *line1Point, const float *line1Vector, float &line1Param, float *hit)
{
	float vector1To0[ 3 ];
	Subtract( line0Point, line1Point, vector1To0 );
	float dot01 = DotProduct( line0Vector, line1Vector );
	float dot00 = DotProduct( line0Vector, line0Vector );
	float dot11 = DotProduct( line1Vector, line1Vector );
	float hit1[ 3 ];
	if ( fabs( dot01 ) < nearZero )
	{
		float negateLine0Vector[ 3 ];
		for ( int i = 0; i < 3; i++ )
			negateLine0Vector[ i ] = -line0Vector[ i ];
		line0Param = DotProduct( negateLine0Vector, vector1To0 ) / dot00;
		line1Param = DotProduct( line1Vector, vector1To0 ) / dot11;
	}
	else
	{
		float d = dot01 / dot00 - dot11 / dot01;
		if ( fabs( d ) < nearZero )
		{
			line0Param = 0.0f;
			line1Param = DotProduct( line1Vector, vector1To0 ) / dot11;
		}
		else
		{
			line1Param = ( DotProduct( line0Vector, vector1To0 ) / dot00  -  DotProduct( line1Vector, vector1To0 ) / dot01 ) / d;
			line0Param = line1Param * dot01 / dot00 - DotProduct( line0Vector, vector1To0 ) / dot00;
		}
	}
	for ( int i = 0; i < 3; i++ )
	{
		hit[ i ] = line0Point[ i ] + line0Param * line0Vector[ i ];
		hit1[ i ] = line1Point[ i ] + line1Param * line1Vector[ i ];
	}
	float distanceVector[ 3 ];
	Subtract( hit, hit1, distanceVector );
	return Length( distanceVector );
}

bool RayHitsLineSegment( const float * lineStart, const float * lineEnd, const float lineWidth, const float * rayOrigin, const float * rayVector,
							float * hit, float  & param )
{
	float lineParam;
	float lineVector[ 3 ];
	Subtract( lineEnd, lineStart, lineVector );
	float distance = LineHitsLine( lineStart, lineVector, lineParam, rayOrigin, rayVector, param,hit );
	return ( distance < lineWidth && ( lineParam <= 1.0 + nearZero && lineParam >= -nearZero ) && ( param >= -nearZero ) );
}

bool RayHitsTriangle( const float * point0, const float * point1, const float * point2, const float * rayOrigin, const float * rayVector,
						  float * hit, float & param, bool & coincident )
{
	bool answer = true;
	bool hit_plane = false;
	if ( & coincident )
		coincident = false;
	float planeNormal[ 3 ];
	float vector01[ 3 ];
	float vector02[ 3 ];
	Subtract( point1, point0, vector01 );
	Subtract( point2, point0, vector02 );
	CrossProduct( vector01, vector02, planeNormal );
	hit_plane = answer = RayHitsPlane( point0, planeNormal, rayOrigin, rayVector, hit, param );
	if ( answer )
	{
		float vector21[ 3 ], normal21[ 3 ];
		Subtract(point1, point2, vector21);
		CrossProduct( vector21, planeNormal, normal21 );
		answer = SameHalfSpace( point0, hit, point1, normal21 );
		if ( answer ) 
		{
			float normal20[ 3 ];
			CrossProduct( vector02, planeNormal, normal20 );
			answer = SameHalfSpace( point1, hit, point2, normal20 );
		}
		if ( answer ) 
		{	
			float vector10[ 3 ], normal10[ 3 ];
			Subtract( point0, point1, vector10 );
			CrossProduct( vector10, planeNormal, normal10 );
			answer = SameHalfSpace( point2, hit, point0, normal10 );
		}
	}
	bool coin = FALSE;
	if ( hit_plane && ( !answer || &coincident ) && hit == rayOrigin )
	{
		float planeUnitNormal[ 3 ];
		for ( int i = 0; i < 3; i++ )
			planeUnitNormal[ i ] = planeNormal[ i ];
		Normalize( planeUnitNormal );
		float rayUnitVector[ 3 ];
		for ( int i = 0; i < 3; i++ )
			rayUnitVector[ i ] = rayVector[ i ];
		Normalize( rayUnitVector );
		if ( fabs( DotProduct( rayUnitVector, planeUnitNormal ) ) < nearZero )
		{
			coin = true;
			if ( &coincident )
				coincident = true;
		}
	}
	if ( ! answer && coin )
	{
		answer = RayHitsLineSegment( point0, point1, nearZero, rayOrigin, rayVector, hit, param );
		float betterHit[ 3 ], betterParam;
		bool betterAnswer;
		if ( ! answer )
			answer = RayHitsLineSegment( point1, point2, nearZero, rayOrigin, rayVector, hit, param );
		else
		{
			betterAnswer = RayHitsLineSegment( point1, point2, nearZero, rayOrigin, rayVector, betterHit, betterParam );
			if ( betterAnswer && betterParam < param )
			{
				for ( int i = 0; i < 3; i++ )
					hit[ i ] = betterHit[ i ];
				param = betterParam;
			}
		}
		if ( ! answer )
			answer = RayHitsLineSegment( point0, point2, nearZero, rayOrigin, rayVector, hit, param );
		else
		{
			betterAnswer = RayHitsLineSegment( point0, point2, nearZero, rayOrigin, rayVector, betterHit, betterParam );
			if ( betterAnswer && betterParam < param )
			{
				for ( int i = 0; i < 3; i++ )
					hit[ i ] = betterHit[ i ];
				param = betterParam;
			}
		}
	}
	return answer;
}
