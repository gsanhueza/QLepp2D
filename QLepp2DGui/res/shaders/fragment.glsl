#version 120

varying highp vec3 f_color;
varying vec3 vBC;

void main() {
    if(any(lessThan(vBC, vec3(0.0))))
    {
//         gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
        gl_FragColor = vec4(f_color, 1.0);
    }
    else
    {
        gl_FragColor = vec4(f_color, 1.0);
    }
}
