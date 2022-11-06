// https://www.shadertoy.com/view/Xtcyzr

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    float vege = buf(fragCoord).x;
    float prey = buf(fragCoord).y * 2.;
    float pred = buf(fragCoord).z;

    fragColor = vec4(0,0,0,1);
    fragColor = mix(fragColor, vec4(0., 1., 0.1, 1), smoothstep(0., 10., vege - prey - pred));
    fragColor = mix(fragColor, vec4(0., 0.5, 1., 1), smoothstep(0., 10., prey - pred));
    fragColor = mix(fragColor, vec4(0.5, 0., 0., 1), smoothstep(0., 10., pred));

    float plant_growth = 0.75 - 0.25 * cos(2.*PI * iTime / 60.);
    if (iTime < 30.) plant_growth = 1.;

    vec2 uv = fragCoord / iResolution.xy;
    if (uv.y < 0.02) fragColor = vec4(0,0,0,1);
    if (uv.y < 0.01 && uv.x < plant_growth) fragColor = vec4(0., 1., 0.1, 1);

    fragColor.rgb = pow(fragColor.rgb, vec3(1./2.2));
}