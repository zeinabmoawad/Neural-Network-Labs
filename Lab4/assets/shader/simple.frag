#version 330
//================ ====used to make pixel by pixel==================//
out vec4 frag_color;
//====================from vertex shader=============================//
// in vec4 vertex_color;
//==============================from c++ to frag=====================//
// uniform float time;
//uniform for texture
in Varying{
  vec4 color;
  vec2 tex_coord;
}fs_in;
uniform sampler2D tex;

void main(){
    //==============change color in different phase==================//
//    vec4 tint=vec4(sin(10.0*time),sin(10.0*time+1.0),sin(10.0*time+2.0),1.0);
//     tint=0.5*tint+0.5;
    // frag_color=vertex_color;//*tint;
    // frag_color=texelFetch(tex,ivec2(gl_fragCoord),0);
    frag_color=texture(tex,fs_in.tex_coord);
} 