#ifdef GL_ES
precision mediump float;
#endif 

uniform sampler2D u_texture; 
varying vec2 v_texCoord;
uniform float ratio; 
uniform float hsize;

void main(void)  
{  
    vec4 sum = vec4(0.0);  
    float blurSize = ratio / hsize; 
    sum += texture2D(u_texture, vec2(v_texCoord.x - 4.0 * blurSize, v_texCoord.y)) *  1.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x - 3.0 * blurSize, v_texCoord.y)) *  2.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x - 2.0 * blurSize, v_texCoord.y)) *  3.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x -   blurSize, v_texCoord.y)) * 10.0/47.0;  

    sum += texture2D(u_texture, vec2(v_texCoord.x , v_texCoord.y)) * 15.0/47.0;  

    sum += texture2D(u_texture, vec2(v_texCoord.x +  blurSize, v_texCoord.y)) * 10.0/47.0;  

    sum += texture2D(u_texture, vec2(v_texCoord.x + 2.0 * blurSize, v_texCoord.y)) *  3.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x + 3.0 * blurSize, v_texCoord.y)) *  2.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x + 4.0 * blurSize, v_texCoord.y)) *  1.0/47.0;  

    gl_FragColor = sum;  
}
