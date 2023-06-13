#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;


// Gets the Texture Unit from the main function
uniform sampler2D tex0;


void main()
{
	vec4 baseColor = texture(tex0, texCoord);
	FragColor = vec4(baseColor.z, baseColor.y, baseColor.x, baseColor.w);
}