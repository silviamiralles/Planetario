#version 330
 
in vec3 a_vertex;
in vec2 a_uv;
in vec3 a_color;
in vec3 a_normal;

out vec3 v_color;
out vec2 v_uv;
out vec3 v_normal;
out vec3 v_pos;

uniform mat4 u_model;
uniform mat4 u_projection;
uniform mat4 u_view;

void main()
{
	// position of the vertex
	//gl_Position = vec4( a_vertex , 1.0 );

	// pass the colour to the fragment shader
	v_color = a_color;
	v_normal = a_normal;

	// translate vertex position using u_model transform matrix; is a 4x4 * 4x1 matrix operation
	vec3 position = ( u_model * vec4(a_vertex,1.0)).xyz; //vectors are columns!

	//gl_Position = vec4( position , 1.0 );
	v_pos = (u_model * vec4(a_vertex,1.0)).xyz;

	// position of the vertex
	// gl_Position = u_projection * u_model * vec4( a_vertex , 1.0 );

	//position view matrix P5
	gl_Position = u_projection * u_view * u_model * vec4(a_vertex, 1.0);

	//texturitzacio
	v_uv = a_uv;

}

