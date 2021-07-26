#version 330
uniform vec3 u_color;

out vec4 fragColor;

in vec3 v_color;
in vec2 v_uv;
in vec3 v_normal;
in vec3 v_pos;

uniform vec3 g_light_dir;
uniform vec3 u_light_dir;
uniform sampler2D u_texture;
uniform vec3 u_light_color;
uniform vec3 u_eye;
uniform float u_glossiness;

void main(void)
{
	// We're just going to paint the interpolated colour from the vertex shader
	//fragColor =  vec4(u_color,1.0);

	fragColor = vec4(v_uv, 0.0, 1.0);
	vec4 texture_color = texture(u_texture, v_uv);
	//fragColor = vec4(texture_color.xyz, 1.0);

	vec3 N = normalize (v_normal);
	//fragColor = vec4 (N, 1.0);
	
	vec3 L = normalize(u_light_dir);
	float NdotL = max(dot(N,L), 0.0);

	vec3 final_color = texture_color.xyz * NdotL;
	
	vec3 diffuse_color = texture_color.xyz * NdotL;
	vec3 ambient_color = texture_color.xyz * 0.1f;
	
	
	

	vec3 R = reflect (-L,N);
	vec3 E = normalize (u_eye - v_pos);
	float RdotE = max(0.0, dot(R,E));
	vec3 specular_color = u_light_color * pow(RdotE, 50.0);

	//final_color = specular_color + ambient_color + diffuse_color; 
	/*
	vec3 texture_color = texture(u_texture, v_uv).xyz;

	*/
		//fragColor = vec4(final_color, 1.0);
		if ( NdotL > 0){
			fragColor  = vec4(specular_color + ambient_color + diffuse_color, 1.0);
		}else{
			fragColor = vec4( ambient_color + diffuse_color, 1.0);
		}

}
