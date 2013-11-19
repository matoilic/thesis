attribute   vec4     a_position;    // Vertex position attribute
uniform     mat4     u_mvpMatrix; // = projection * modelView

void main()
{     
   // Set the transformes vertex position   
   gl_Position = u_mvpMatrix * a_position;
}
