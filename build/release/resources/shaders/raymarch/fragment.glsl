#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
precision highp float;
uniform vec2 u_resolution;  // Width and height of the shader
uniform float u_time;  // Time elapsed
uniform sampler2D iChannel;

// Cubic Polynomial Smooth-minimum
vec2 smin(float a, float b, float k)
{
    k *= 6.0;
    float h = max(k - abs(a - b), 0.0) / k;
    float m = h * h * h * 0.5;
    float s = m * k * (1.0 / 3.0);
    return (a < b) ? vec2(a - s, m) : vec2(b - s, 1.0 - m);
}

float hash1(float n)
{
    return fract(sin(n) * 43758.5453123);
}

vec3 forwardSF(float i, float n)
{
    const float PI = 3.1415926535897932384626433832795;
    const float PHI = 1.6180339887498948482045868343656;
    float phi = 2.0 * PI * fract(i / PHI);
    float zi = 1.0 - (2.0 * i + 1.0) / n;
    float sinTheta = sqrt(1.0 - zi * zi);
    return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, zi);
}

vec2 map(vec3 q)
{
    // plane
    vec2 res = vec2(q.y, 2.0);

    // sphere
    float d = length(q - vec3(0.0, 0.1 + 0.05 * sin(u_time), 0.0)) - 0.1;

    // smooth union
    return smin(res.x, d, 0.05);
}

vec2 intersect(in vec3 ro, in vec3 rd)
{
    const float maxd = 10.0;

    vec2 res = vec2(0.0);
    float t = 0.0;
    for (int i = 0; i < 512; i++)
    {
        vec2 h = map(ro + rd * t);
        if ((h.x < 0.0) || (t > maxd)) break;
        t += h.x;
        res = vec2(t, h.y);
    }

    if (t > maxd) res = vec2(-1.0);
    return res;
}

// https://iquilezles.org/articles/normalsSDF
vec3 calcNormal(in vec3 pos)
{
    vec2 e = vec2(1.0, -1.0) * 0.5773 * 0.005;
    return normalize(e.xyy * map(pos + e.xyy).x +
    e.yyx * map(pos + e.yyx).x +
    e.yxy * map(pos + e.yxy).x +
    e.xxx * map(pos + e.xxx).x);
}

// https://iquilezles.org/articles/nvscene2008/rwwtt.pdf
float calcAO(in vec3 pos, in vec3 nor, float ran)
{
    float ao = 0.0;
    const int num = 32;
    for (int i = 0; i < num; i++)
    {
        vec3 kk;
        vec3 ap = forwardSF(float(i) + ran, float(num));
        ap *= sign(dot(ap, nor)) * hash1(float(i));
        ao += clamp(map(pos + nor * 0.01 + ap * 0.2).x * 20.0, 0.0, 1.0);
    }
    ao /= float(num);

    return clamp(ao, 0.0, 1.0);
}

vec3 render(in vec2 p, vec4 ran)
{
    //-----------------------------------------------------
    // camera
    //-----------------------------------------------------
    float an = 0.1 * u_time;
    vec3 ro = vec3(0.4 * sin(an), 0.15, 0.4 * cos(an));
    vec3 ta = vec3(0.0, 0.05, 0.0);
    // camera matrix
    vec3 ww = normalize(ta - ro);
    vec3 uu = normalize(cross(ww, vec3(0.0, 1.0, 0.0)));
    vec3 vv = normalize(cross(uu, ww));
    // create view ray
    vec3 rd = normalize(p.x * uu + p.y * vv + 1.7 * ww);

    //-----------------------------------------------------
    // render
    //-----------------------------------------------------

    vec3 col = vec3(1.0);

    // raymarch
    vec3 uvw;
    vec2 res = intersect(ro, rd);
    float t = res.x;
    if (t > 0.0)
    {
        vec3 pos = ro + t * rd;
        vec3 nor = calcNormal(pos);
        vec3 ref = reflect(rd, nor);
        float fre = clamp(1.0 + dot(nor, rd), 0.0, 1.0);
        float occ = calcAO(pos, nor, ran.y); occ = occ * occ;

        // blend materials
        col = mix(vec3(0.0, 0.05, 1.0),
                  vec3(1.0, 0.0, 0.0),
                  res.y);

        col = col * 0.72 + 0.2 * fre * vec3(1.0, 0.8, 0.2);
        vec3 lin = 4.0 * vec3(0.7, 0.8, 1.0) * (0.5 + 0.5 * nor.y) * occ;
        lin += 0.8 * vec3(1.0, 1.0, 1.0) * fre * (0.6 + 0.4 * occ);
        col = col * lin;
        col += 2.0 * vec3(0.8, 0.9, 1.00) * smoothstep(0.0, 0.4, ref.y) * (0.06 + 0.94 * pow(fre, 5.0)) * occ;
        col = mix(col, vec3(1.0), 1.0 - exp2(-0.04 * t * t));

    }

    // gamma and postpro
    col = pow(col, vec3(0.4545));
    col *= 0.9;
    col = clamp(col, 0.0, 1.0);
    col = col * col * (3.0 - 2.0 * col);

    // dithering
    col += (ran.x - 0.5) / 255.0;

    return col;
}

void main()
{


    vec2 px = (2.0 * gl_.xy - u_resolution.xy) / u_resolution.y;
    vec4 ran = texelFetch(iChannel, ivec2(gl_FragCoord.xy), 0);
    vec3 col = render(px, ran);
    FragColor = vec4(col, 1.0);

}