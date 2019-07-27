#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;
uniform float u_opacity;

void main() {
    vec4 texColor=texture2D(CC_Texture0, v_texCoord);
    //gl_FragColor=texColor*u_opacity;
	gl_FragColor= vec4((texColor.xyz/0.4)*vec3(1.0,1.0,1.0),texColor.w);
}