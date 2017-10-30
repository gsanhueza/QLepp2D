#version 120

varying highp vec3 vert;
varying highp vec3 vertNormal;

uniform highp vec3 lightPos;
uniform highp vec3 eyePos;

void main() {
    float brightness = 100.0;
    vec3 lightColSun = vec3(0.5, 0.5, 0.0);

    vec3 LightVectorSun = normalize(lightPos - vert);
    vec3 ReflectedVectorSun = reflect(LightVectorSun, vertNormal);
    vec3 EyeVector = normalize(eyePos - vert);

    // Diffuse
    float Idiff = max(dot(vertNormal, LightVectorSun), 0.0);

    // Specular
    float Ispec = pow(max(dot(EyeVector, ReflectedVectorSun), 0.0), brightness);

    // Ambient
    float Iamb = 0.2;

    gl_FragColor = vec4(lightColSun * (Idiff + Ispec + Iamb), 1.0);
}
