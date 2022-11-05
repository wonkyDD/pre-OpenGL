// https://www.shadertoy.com/view/XtdyW2

vec2 hash21(float p)
{
	vec3 p3 = fract(vec3(p) * vec3(.1031, .1030, .0973));
	p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.xx+p3.yz)*p3.zy);

}

float hash13(vec3 p3)
{
	p3  = fract(p3 * .1031);
    p3 += dot(p3, p3.yzx + 19.19);
    return fract((p3.x + p3.y) * p3.z);
}

#define R2 19

#define SIGMA 1.414
#define M_PI 3.14159265359

float gaussian (float x, float sigma) {
    float h0 = x / sigma;
    float h = h0 * h0 * -0.5;
    float a = 1.0 / (sigma * sqrt(2.0 * M_PI));
    return a * exp(h);
}

float distf(float v, float x) {
    return 1.0 - x;
}

vec2 quantify_error (sampler2D channel, ivec2 p, ivec2 sz, float val0, float val1) {
    float Rf = float(R2) / 2.0;
    int R = int(Rf);
    float has0 = 0.0;
    float has1 = 0.0;
    float w = 0.0;
    
    for (int sy = -R; sy <= R; ++sy) {
        for (int sx = -R; sx <= R; ++sx) {
            float d = length(vec2(sx,sy));
            if ((d > Rf) || ((sx == 0) && (sy == 0)))
                continue;
            ivec2 t = (p + ivec2(sx,sy) + sz) % sz;            
			float v = texelFetch(channel, t, 0).r;

            float dist0 = abs(v - val0);
            float dist1 = abs(v - val1);

            float q = gaussian(d, SIGMA);

            w += q;            
            has0 += distf(val0, dist0) * q;
            has1 += distf(val1, dist1) * q;
            
        }
    }

    vec2 result = vec2(has0 / w, has1 / w);
    //result = result * result;
    return result;
}