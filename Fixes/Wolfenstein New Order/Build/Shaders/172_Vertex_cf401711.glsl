#version 150
// UI
uniform float g_vertexEnabled;
uniform float g_eye;
uniform float g_eye_separation;
uniform float g_convergence;
uniform vec4 g_custom_params;

float saturate( float v ) { return clamp( v, 0.0, 1.0 ); }
vec2 saturate( vec2 v ) { return clamp( v, 0.0, 1.0 ); }
vec3 saturate( vec3 v ) { return clamp( v, 0.0, 1.0 ); }
vec4 saturate( vec4 v ) { return clamp( v, 0.0, 1.0 ); }
vec4 tex2Dlod( sampler2D sampler, vec4 texcoord ) { return textureLod( sampler, texcoord.xy, texcoord.w ); }

float dot4 ( vec4 a, vec4 b ) { return dot( a, b ); }
float dot4 ( vec2 a, vec4 b ) { return dot( vec4( a, 0.0, 1.0 ), b ); }
uniform vec4 _va_ [6];

in vec4 in_Position;
in vec2 in_TexCoord;

out vec4 vofi_TexCoord0;

void main() {
	gl_Position.x = dot4( in_Position, _va_[0 ] );
	gl_Position.y = dot4( in_Position, _va_[1 ] );
	gl_Position.z = dot4( in_Position, _va_[2 ] );
	gl_Position.w = dot4( in_Position, _va_[3 ] );
	gl_Position.x += g_eye * g_eye_separation * g_custom_params.x;

 if(g_vertexEnabled < 1.0)
{gl_Position = vec4(0.0);}

	vofi_TexCoord0.x = dot4( in_TexCoord, _va_[4 ] );
	vofi_TexCoord0.y = dot4( in_TexCoord, _va_[5 ] );
	vofi_TexCoord0.zw = vec2( 0.0 );
}