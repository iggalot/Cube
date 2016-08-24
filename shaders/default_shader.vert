#version 300 es
layout (location = 0) in vec3 position;

out highp vec4 fragColor;

uniform highp vec4 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//  gl_Position = vec4(position.x, position.y, position.z, 1.0);
	fragColor = vertexColor;
  	gl_Position = 0.1 * projection * view * model * vec4(position.x, position.y, position.z, 1.0);

}