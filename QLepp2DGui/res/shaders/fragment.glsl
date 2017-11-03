#version 120

varying highp vec3 f_color;
varying highp vec3 f_barycentric;

void main() {
    float mindist = min(min(f_barycentric.x, f_barycentric.y), f_barycentric.z);
    float edgeIntensity = 1.0 - step(0.005, mindist);

    gl_FragColor = edgeIntensity * vec4(0.0, 1.0, 1.0, 1.0) + (1.0 - edgeIntensity) * vec4(f_color, 1.0);
}
