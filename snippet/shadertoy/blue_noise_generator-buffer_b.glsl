// https://www.shadertoy.com/view/XtdyW2

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    ivec2 sz = ivec2(iChannelResolution[0].xy);
    ivec2 p0 = ivec2(fragCoord);
    vec2 maskf = hash21(float(iFrame));
    int M = 60 * 60;
    int F = (iFrame % M);
    float framef = float(F) / float(M);
    float chance_limit = 0.5;
    float force_limit = 1.0 - clamp(framef * 8.0, 0.0, 1.0);
    force_limit = force_limit * force_limit;
    force_limit = force_limit * force_limit;
    if (F == 0) {
        int c = (p0.x * 61 + p0.y) % 256;
        fragColor = vec4(float(c) / 255.0, 0.0, 0.0, 1.0);
    } else {
        ivec2 mask = ivec2(maskf * vec2(sz) + maskf * vec2(sz) * framef);
        ivec2 p1 = (p0 ^ mask) % sz;       
        ivec2 pp0 = (p1 ^ mask) % sz;  

        float chance0 = hash13(vec3(p0, float(iFrame)));
        float chance1 = hash13(vec3(p1, float(iFrame)));
        float chance = max(chance0, chance1);
        
        float v0 = texelFetch(iChannel0, p0, 0).r;
        float v1 = texelFetch(iChannel0, p1, 0).r;
        
        vec2 s0_x0 = quantify_error(iChannel0, p0, sz, v0, v1);
        vec2 s1_x1 = quantify_error(iChannel0, p1, sz, v1, v0);
        
        float err_s = s0_x0.x + s1_x1.x;
        float err_x = s0_x0.y + s1_x1.y;
        
        float p = v0;
        if (pp0 == p0) {
            if ((chance < force_limit) || ((chance < chance_limit) && (err_x < err_s))) {
                p = v1;
            }
        }
        fragColor = vec4(p, 0.0, 0.0, 1.0);
    }
}