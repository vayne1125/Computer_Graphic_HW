//Fragment shader for per-pixel Phong shading
varying vec3 N;
varying vec3 L;
varying vec3 E;
void main()  {
   vec3 Normal = normalize(N);
   vec3 Light = normalize(L);
   vec3 Eye = normalize(E);
   vec3 Half = normalize(Eye+Light);
	
   float Kd = max(dot(Normal, Light), 0.0);
   float Ks = pow(max(dot(Half, Normal), 0.0), gl_FrontMaterial.shininess);
   if(Kd==0.0) Ks = 0.0;
   vec4 diffuse = Kd*gl_LightSource[0].diffuse*gl_FrontMaterial.diffuse;
  
   vec4 specular = Ks*gl_LightSource[0].specular*gl_FrontMaterial.specular;
   vec4 ambient = gl_LightSource[0].ambient*gl_FrontMaterial.ambient;
	
   gl_FragColor = ambient + diffuse + specular ; 
}
