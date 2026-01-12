#version 450

layout(location=0) out vec4 outColor;

struct Box
{
    vec3 center;
    vec3 radius;
    vec3 invRadius;
    mat3 rotation; // world->box, ha így használod lent
    // color?
};

struct Ray
{
    vec3 origin;
    vec3 dir;
    vec3 invDir;
};

uniform vec3 uCameraPos;
uniform mat4 uInvViewProj;
uniform vec2 uResolution;

float maxComponent(vec3 a) { return max(a.x, max(a.y, a.z)); }
float safeInverse(float x) { return (abs(x) < 1e-8) ? 1e12 : (1.0 / x); }
vec3 safeInverse(vec3 v) { return vec3(safeInverse(v.x), safeInverse(v.y), safeInverse(v.z)); }

Ray makePrimaryRay(vec2 fragCoord)
{
    vec2 ndc = (fragCoord / uResolution) * 2.0 - 1.0;
    vec4 pClip   = vec4(ndc, 1.0, 1.0);
    vec4 pWorldH = uInvViewProj * pClip;
    vec3 pWorld  = pWorldH.xyz / pWorldH.w;

    Ray r;
    r.origin = uCameraPos;
    r.dir    = normalize(pWorld - uCameraPos);
    r.invDir = safeInverse(r.dir);

    return r;
}

// Based on Majercik & Shirley, 2018
bool intersect(Box box, Ray ray, out float distance, out vec3 normal, const bool rayCanStartInBox, const bool oriented, vec3 invRayDir)
{
    ray.origin = box.rotation * (ray.origin - box.center);
    if (oriented) {
        ray.dir = ray.dir * box.rotation;
    }

    float winding = 1.0;
    if (rayCanStartInBox) {
        winding = (maxComponent(abs(ray.origin) * box.invRadius) < 1.0) ? -1.0 : 1.0;
    }

    vec3 sgn = -sign(ray.dir);

    vec3 distanceToPlane = box.radius * winding * sgn - ray.origin;
    distanceToPlane = oriented ? (distanceToPlane / ray.dir) : (distanceToPlane * invRayDir);

#   define TEST(U, VW) \
        (distanceToPlane.U >= 0.0) && \
        all(lessThan(abs(ray.origin.VW + ray.dir.VW * distanceToPlane.U), box.radius.VW))

    bvec3 test = bvec3(TEST(x, yz), TEST(y, zx), TEST(z, xy));
#   undef TEST

    sgn = test.x ? vec3(sgn.x, 0.0, 0.0) :
          (test.y ? vec3(0.0, sgn.y, 0.0) :
                    vec3(0.0, 0.0, test.z ? sgn.z : 0.0));

    distance = (sgn.x != 0.0) ? distanceToPlane.x :
               ((sgn.y != 0.0) ? distanceToPlane.y : distanceToPlane.z);

    normal = oriented ? (box.rotation * sgn) : sgn;
    //normal = oriented ? (transpose(box.rotation) * sgn) : sgn;

    return (sgn.x != 0.0) || (sgn.y != 0.0) || (sgn.z != 0.0);
}

void main()
{
    Ray ray = makePrimaryRay(gl_FragCoord.xy);

    Box box;
    box.center    = vec3(0.0, 0.0, -5.0);
    box.radius    = vec3(0.5);
    box.invRadius = safeInverse(box.radius);
    box.rotation  = mat3(1.0);

    float distance;
    vec3 normal;
    bool hit = intersect(box, ray, distance, normal, /*rayCanStartInBox*/ true, /*oriented*/ false, ray.invDir);

    outColor = hit ? vec4(normal * 0.5 + 0.5, 1.0) : vec4(0,0,0,1);
}
