// https://www.shadertoy.com/view/llt3R4
// https://jamie-wong.com/2016/07/15/ray-marching-signed-distance-functions/
// https://iquilezles.org/articles/distfunctions/


const float RADIUS = 2.0;
const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float EPSILON = 0.0001;


float sphereSDF(vec3 samplePoint) 
{
    return length(samplePoint) - RADIUS;
}

float sceneSDF(vec3 samplePoint) 
{
    return sphereSDF(samplePoint);
}

/**
 * Return the shortest distance 
 * from the eyepoint to the scene surface along the marching direction.
 * If no part of the surface is found between start and end,
 * return end.
 * 
 * eye:                 ray origin
 * marchingDirection:   normalized direction
 * start:               the starting distance away from the eye
 * end:                 the max distance away from the eye to march before giving up
 */
float shortestDistanceToSurface(vec3 eye, vec3 marchingDirection, float start, float end) 
{
    float depth = start;
    for (int i = 0; i < MAX_MARCHING_STEPS; ++i) 
    {
        float dist = sceneSDF(eye + depth * marchingDirection);

        if (dist < EPSILON) return depth;
        depth += dist;
        if (depth >= end) return end;
    }
    return end;
}
            
/**
 * Return the normalized direction to march in from the eye point for a single pixel.
 * 
 * fieldOfView:     vertical field of view in degrees
 * size:            resolution of the output image
 * fragCoord:       the x,y coordinate of the pixel in the output image
 */
vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) 
{
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy, -z));
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
	vec3 dir = rayDirection(60.0, iResolution.xy, fragCoord);
    vec3 eye = vec3(0.0, 0.0, 10.0);
    float dist = shortestDistanceToSurface(eye, dir, MIN_DIST, MAX_DIST);
    
    if (dist > MAX_DIST - EPSILON) {
        // Didn't hit anything
        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
		return;
    }
    
    fragColor = vec4(1.0, 1.0, 0.0, 1.0);
}