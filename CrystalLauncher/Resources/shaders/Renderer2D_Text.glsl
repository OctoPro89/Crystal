// Renderer2D Signed Distance Field MSDF Text Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

// Goofy spir-v???
//struct VertexOutput
//{
//	vec4 Color;
//	vec2 TexCoord;
//};

layout (location = 0) out vec4 Color;
layout (location = 1) out vec2 TexCoord;
layout (location = 2) out flat int v_EntityID;

void main()
{
	Color = a_Color;
	TexCoord = a_TexCoord;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) in vec4 Color;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_FontAtlas;

float screenPxRange()
{
	const float pxRange = 2.0; /* Set to distance field's pixel range */
	vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlas, 0));
	vec2 screenTexSize = vec2(1.0) / fwidth(TexCoord);

	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec3 msd = texture(u_FontAtlas, TexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if(opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
	o_Color = mix(bgColor, Color, opacity); /* Fade anti-aliasing */
	if(o_Color.a == 0.0)
		discard;

	o_EntityID = v_EntityID;
}
