uniform sampler2D texture;

void main(void){
  vec4 color = texture2D(texture,gl_TexCoord[0].xy+vec2(0.1,0.1));
//  gl_FragColor = vec4(0.0, 1.0, 0.0, 0.5);
//  gl_FragColor = color;

  int offset_x = 10;
  int offset_y = 10;

  if (color.a != 0.0) {
    gl_FragColor = vec4(0.0, 1.0f, 0.0, 1.0);
  }
}

