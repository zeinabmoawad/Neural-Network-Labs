#version 330
//============comunicate with frag shader==============//
// out vec4 vertex_color;
//===================from c++ to vertex================//
// uniform float time;
//=======================//
layout(location=0)in vec3 position;
layout(location=1)in vec4 color;
layout(location=2)in vec2 tex_coord;
out Varying{
  vec4 color;
  vec2 tex_coord;
}vs_out;
//==================3D========================//
uniform mat4 MVP;
void main(){

    //===========================just data for frag shader================//
    //==each corner has color and in between is linear interpolation 
      //  const vec4 colors[3]=vec4[3](
      //   vec4(1.0,0.0,0.0,1.0),
      //     vec4(0.0,1.0,0.0,1.0),
      //       vec4(0.0,0.0,1.0,1.0)
      // );
    // gl_Position=vec4(position*sin(time),1.0);
    //  gl_Position=vec4(position,1.0);
    // vertex_color=colors[gl_VertexID];
    //======================3D===============//
    gl_Position=MVP*vec4(position,1.0);
    // vertex_color=color;
    vs_out.color=color;
    vs_out.tex_coord=tex_coord;

} 