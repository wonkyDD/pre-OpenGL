// https://www.shadertoy.com/view/Xtcyzr

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
    if (iFrame < 10) {
        fragColor.xyz = hash32(fragCoord) * vec3(0.5, 1., 2.);
        return;
    }

    vec4 c = buf(fragCoord);
    vec4 n = buf(fragCoord + N);
    vec4 e = buf(fragCoord + E);
    vec4 s = buf(fragCoord + S);
    vec4 w = buf(fragCoord + W);

    float plant_growth = 0.75 - 0.25 * cos(2.*PI * iTime / 60.);
    if (iTime < 30.) plant_growth = 1.;
    float reproduction = 2. * fragCoord.y / iResolution.y;
    float predation = 0.9 + 1.1 * fragCoord.x / iResolution.x;

    // generalised Lotka-Volterra equations
    float dx = plant_growth - c.y;
    float dy = reproduction * c.x - predation * c.z - 1.;
    float dz = predation * c.y - 1.;
    float dt = 0.1;
    c.xyz += dt * c.xyz * vec3(dx, dy, dz);
    c = clamp(c, 0.01, 10.);

    // diffusion
    c.xyz += dt * (n + e + s + w - 4. * c).xyz * vec3(0.25, 0.5, 1.);

    fragColor = c;
}