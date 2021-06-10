#version 400 core

in float disp;

void main(){
	vec3 color;
	if(disp > 0)
		color = mix(vec3(1, 1, 1), vec3(0, 2, 0), disp);
	else				 
		color = mix(vec3(1, 1, 1), vec3(2, 0, 0), -disp);
		
	gl_FragColor = vec4(color, .25);
}
