//Vertex shader for per-vertex Phong shading

varying vec3 N; //normal vector
varying vec3 L; //light direction vector
varying vec3 E; //V vector for specular.

void main()
{
   vec4 eyePosition = gl_ModelViewMatrix*gl_Vertex;
   vec4 eyeLightPos = gl_ModelViewMatrix*gl_LightSource[0].position;

  //Compute the key vector parameters for the vertex.
   N =  normalize(gl_NormalMatrix*gl_Normal);
   L = eyeLightPos.xyz - eyePosition.xyz ;
   E = -eyePosition.xyz;

   gl_Position =gl_ModelViewProjectionMatrix*gl_Vertex;
}