#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_texture;
varying vec2 v_texCoord;
uniform float ratio; 
uniform float vsize;

void main(void)  
{   
    vec4 sum = vec4(0.0);  
    float blurSize = ratio / vsize;
    // blur in y (vertical)  
    // take nine samples, with the distance blurSize between them  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 4.0 * blurSize)) *  1.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 3.0 * blurSize)) *  2.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 2.0 * blurSize)) *  3.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y -       blurSize)) * 10.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y  )) * 15.0/47.0;  
  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y +       blurSize)) * 10.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 2.0 * blurSize)) *  3.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 3.0 * blurSize)) *  2.0/47.0;  
    sum += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 4.0 * blurSize)) *  1.0/47.0;  
  
    gl_FragColor = sum;  
}  
