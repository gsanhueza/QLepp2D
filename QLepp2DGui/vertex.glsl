#version 120

attribute vec3 vertex;
attribute vec3 normal;

varying vec3 vert;
varying vec3 vertNormal;

uniform mat4 projMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

void main(){
    vert = vertex;
    vertNormal = normalMatrix * normal;
    gl_Position = projMatrix * modelViewMatrix * vec4(vertex, 1.0);
}
