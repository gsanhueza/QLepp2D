#version 120

attribute vec3 vertex;
attribute vec3 color;
attribute vec3 barycentric;

varying vec3 f_color;
varying vec3 f_barycentric;

uniform mat4 projMatrix;
uniform mat4 modelViewMatrix;

void main(){
    f_color = color;
    f_barycentric = barycentric;
    gl_Position = projMatrix * modelViewMatrix * vec4(vertex, 1.0);
}
