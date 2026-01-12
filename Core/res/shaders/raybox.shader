#version 450

#define Point3  vec3;
#define Vector3 vec3;
#define Vector3 vec3;
#define Matrix3 mat4;

layout(location=0) out vec4 outColor;

uniform vec3 uCameraPos;
uniform mat4 uInvViewProj;
uniform vec2 uResolution;

struct Ray
{
    vec3 origin;
    vec3 dir;
    vec3 invDir;
};

struct Box // For the intersection test
{
    vec3 center;
    vec3 radius;
    vec3 invRadius;
    mat3 rotation;
};

struct Voxel
{
    vec4 center;
    vec4 radius;
    mat4 rotation;

    vec4 color;
};

layout(binding = 0, std430) readonly buffer Voxels
{
    Voxel voxels[];
};

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
        //ray.dir = ray.dir * box.rotation;
        ray.dir = box.rotation * ray.dir;
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

    //normal = oriented ? (box.rotation * sgn) : sgn;
    normal = oriented ? (transpose(box.rotation) * sgn) : sgn;

    return (sgn.x != 0.0) || (sgn.y != 0.0) || (sgn.z != 0.0);
}

void main()
{
    Ray ray = makePrimaryRay(gl_FragCoord.xy);

    Voxel voxel = voxels[0];

    Box box;
    box.center     = voxel.center.xyz;
    box.radius     = voxel.radius.xyz;
    box.invRadius  = safeInverse(box.radius);
    box.rotation = mat3(voxel.rotation);

    float distance;
    vec3 normal;
    bool hit = intersect(box, ray, distance, normal, /*rayCanStartInBox*/ true, /*oriented*/ true, ray.invDir /*if oriented not needed*/);

    //outColor = hit ? vec4(normal * 0.5 + 0.5, 1.0) : vec4(0,0,0,1);

    // Lambert diffusion model
    normal = normalize(normal);
    vec3 lightDir = normalize(vec3(0.6, 0.8, 0.4));
    float NdotL = max(dot(normal, lightDir), 0.0);

    vec3 baseColor = voxel.color.rgb;
    vec3 color = baseColor * (0.2 + 0.8 * NdotL);

    outColor = vec4(color, voxel.color.a);

    // Combine normal and base color for visualization
    //vec3 normalColor = normal * 0.5 + 0.5;
    //vec3 baseColor   = voxel.color.rgb;

    //vec3 color = mix(baseColor, normalColor, 0.5);
    //outColor = vec4(color, 1.0);
}
