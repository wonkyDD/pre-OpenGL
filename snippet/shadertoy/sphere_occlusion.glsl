// https://www.shadertoy.com/view/4djSDy

// The MIT License
// https://www.youtube.com/c/InigoQuilez
// https://iquilezles.org/
// Copyright © 2014 Inigo Quilez
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


// Analytical ambient occlusion of a sphere. Left side of screen, stochastically 
// sampled occlusion. Right side of the screen, analytical solution (no rays casted).
//
// When the sphere is fully visible to the normal, the solution is easy. More info here:
// https://iquilezles.org/articles/sphereao
//
// When the sphere is only partially visible and see clipping, it gets more complicated:
// http://www.frostbite.com/wp-content/uploads/2014/11/course_notes_moving_frostbite_to_pbr.pdf


// Other shaders with analytical occlusion or approximations:
// 
// Box:                        https://www.shadertoy.com/view/4djXDy
// Box with horizon clipping:  https://www.shadertoy.com/view/4sSXDV
// Triangle:                   https://www.shadertoy.com/view/XdjSDy
// Sphere:                     https://www.shadertoy.com/view/4djSDy
// Ellipsoid (approximation):  https://www.shadertoy.com/view/MlsSzn
// Capsule (approximation):    https://www.shadertoy.com/view/llGyzG


// Sphere intersection
float sphIntersect( in vec3 ro, in vec3 rd, in vec4 sph )
{
	vec3 oc = ro - sph.xyz;
	float b = dot( oc, rd );
	float c = dot( oc, oc ) - sph.w*sph.w;
	float h = b*b - c;
	if( h<0.0 ) return -1.0;
	return -b - sqrt( h );
}

// Sphere occlusion
float sphOcclusion( in vec3 pos, in vec3 nor, in vec4 sph )
{
    vec3  di = sph.xyz - pos;
    float l  = length(di);
    float nl = dot(nor,di/l);
    float h  = l/sph.w;
    float h2 = h*h;
    float k2 = 1.0 - h2*nl*nl;

    // above/below horizon
    // EXACT: Quilez - https://iquilezles.org/articles/sphereao
    float res = max(0.0,nl)/h2;
    
    // intersecting horizon 
    if( k2 > 0.001 ) 
    {
        #if 0
            // EXACT : Lagarde/de Rousiers - https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf
            res = nl*acos(-nl*sqrt( (h2-1.0)/(1.0-nl*nl) )) - sqrt(k2*(h2-1.0));
            res = res/h2 + atan( sqrt(k2/(h2-1.0)));
            res /= 3.141593;
        #else
            // APPROXIMATED : Quilez - https://iquilezles.org/articles/sphereao
            res = (nl*h+1.0)/h2;
            res = 0.33*res*res;
        #endif
    }

    return res;
}

//=====================================================

vec2 hash2( float n ) { return fract(sin(vec2(n,n+1.0))*vec2(43758.5453123,22578.1459123)); }

float iPlane( in vec3 ro, in vec3 rd )
{
    return (-1.0 - ro.y)/rd.y;
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 p = (2.0*fragCoord-iResolution.xy) / iResolution.y;
    float s = (2.0*iMouse.x-iResolution.x) / iResolution.y;
    if( iMouse.z<0.001 ) s=0.0;
    
	vec3 ro = vec3(0.0, 0.0, 4.0 );
	vec3 rd = normalize( vec3(p,-2.0) );
	
    // sphere animation
    vec4 sph = vec4( cos( iTime + vec3(2.0,1.0,1.0) + 0.0 )*vec3(1.5,1.2,1.0), 1.0 );

    vec4 rrr = texture( iChannel0, (fragCoord)/iChannelResolution[0].xy, -99.0  ).xzyw;


    vec3 col = vec3(0.0);

    float tmin = 1e10;
    
    float t1 = iPlane( ro, rd );
    if( t1>0.0 )
    {
        tmin = t1;
        vec3 pos = ro + tmin*rd;
        vec3 nor = vec3(0.0,1.0,0.0);
        float occ = 0.0;
        
        if( p.x > s )
        {
            occ = sphOcclusion( pos, nor, sph );
        }
        else
        {
   		    vec3  ru  = normalize( cross( nor, vec3(0.0,1.0,1.0) ) );
		    vec3  rv  = normalize( cross( ru, nor ) );

            occ = 0.0;
            for( int i=0; i<256; i++ )
            {
                vec2  aa = hash2( rrr.x + float(i)*203.1 );
                float ra = sqrt(aa.y);
                float rx = ra*cos(6.2831*aa.x); 
                float ry = ra*sin(6.2831*aa.x);
                float rz = sqrt( 1.0-aa.y );
                vec3  dir = vec3( rx*ru + ry*rv + rz*nor );
                float res = sphIntersect( pos, dir, sph );
                occ += step(0.0,res);
            }
            occ /= 256.0;
        }

        col = vec3(1.0);
        col *= 1.0 - occ;
    }

    float t2 = sphIntersect( ro, rd, sph );
    if( t2>0.0 && t2<tmin )
    {
        tmin = t2;
        float t = t2;
        vec3 pos = ro + t*rd;
        vec3 nor = normalize( pos - sph.xyz );
		col = vec3(1.2);
        col *= 0.6+0.4*nor.y;
	}

	col *= exp( -0.05*tmin );

    float e = 2.0/iResolution.y;
    col *= smoothstep( 0.0, 2.0*e, abs(p.x-s) );
    
    fragColor = vec4( col, 1.0 );
}