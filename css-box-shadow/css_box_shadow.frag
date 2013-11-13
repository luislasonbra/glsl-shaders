uniform sampler2D texture;

void main(void){
  vec4 pix = texture2D(texture,gl_TexCoord[0].xy);

  float offset_x = -0.01;
  float offset_y = -0.01;

  vec4 offset_pix = texture2D(texture,gl_TexCoord[0].xy+
                         vec2(offset_x, offset_y));

//  gl_FragColor = vec4(0.0, 1.0, 0.0, 0.5);
//  gl_FragColor = color;

  gl_FragColor = pix;

  if (offset_pix.a != 0.0 && pix.a == 0.0) {
    gl_FragColor = vec4(0.3, 0.3f, 0.3, 1.0);
  }
}

