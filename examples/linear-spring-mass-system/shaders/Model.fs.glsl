#version 430 core

in VertexData
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
    vec3 eyeDirection;
    vec3 lightDirection;
    vec3 lightPosition;
} inData;

out vec4 fragColour;

const vec3 diffuseMaterial = vec3(0.64, 0.64, 0.64);
const vec3 specularMaterial = vec3(0.99, 0.99, 0.99);
const vec3 ambientMaterial = vec3(1.0, 1.0, 1.0);
const vec3 lightPos = vec3(50.0, 50.0, 50.0);

const vec4 metalGray = vec4(0.0, 0.46, 0.69, 1.0);

uniform int renderMode;

vec3 shadedColour()
{
    vec3 lightColour = vec3(1, 1, 1);
    float lightPower = 50.0;

    vec3 materialDiffuseColour = metalGray.xyz;
    vec3 materialAmbientColour = vec3(0.1, 0.1, 0.1) * materialDiffuseColour;
    vec3 materialSpecularColour = vec3(0.3, 0.3, 0.3);

    // Distance to light.
    float dist = length(inData.lightPosition - inData.position);

    // Normal of the fragment.
    vec3 n = normalize(inData.normal);

    // Direction of the light.
    vec3 l = normalize(inData.lightDirection);
    float cosTheta = clamp(dot(n, l), 0, 1);

    // Eye vector.
    vec3 E = normalize(inData.eyeDirection);
    vec3 R = reflect(-l, n);
    float cosAlpha = clamp(dot(E, R), 0, 1);

    return materialAmbientColour + 
        materialDiffuseColour * lightColour * lightPower * 
        cosTheta / (dist * dist); 
}

vec4 idNormal()
{
    vec4 ret;
    ret.x = clamp(abs(inData.normal.x), 0.0, 1.0);
    ret.y = clamp(abs(inData.normal.y), 0.0, 1.0);
    ret.z = clamp(abs(inData.normal.z), 0.0, 1.0);
    ret.w = 1.0;
    return ret;
}

float amplify(float d, float scale, float offset)
{
    d = scale * d + offset;
    d = clamp(d, 0, 1);
    d = 1- exp2(-2 * d * d);
    return d;
}

void main()
{
    vec4 colour = vec4(shadedColour(), 1.0);
    const vec3 vertexColour = vec3(1.0, 0.0, 0.0);
    const vec3 lineColour = vec3(0.0, 0.015, 0.375);

    if (renderMode == 0)
    {
        // Point cloud.
        fragColour = vec4(vertexColour, 1.0);
    }
    else if (renderMode == 1)
    {
        // Wireframe mode.
        fragColour = vec4(lineColour, 1.0);
    }
    else if (renderMode == 2)
    {
        // Shaded.
        fragColour = vec4(shadedColour(), 1.0);
    }
    else if (renderMode ==  3)
    {
        fragColour = idNormal();
    }
    else
    {
        fragColour = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
