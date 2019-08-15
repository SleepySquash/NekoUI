#version 120
uniform sampler2D texture;

uniform float textureOffset;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
  vec2 v_texCoords = gl_TexCoord[0].xy;
  vec4 col = texture2D(texture, v_texCoords);

  if (col.a == 1.0)
    gl_FragColor = col;
  else {
    float au = texture2D(texture, vec2(v_texCoords.x , v_texCoords.y - textureOffset)).a;
    float ad = texture2D(texture, vec2(v_texCoords.x , v_texCoords.y + textureOffset)).a;
    float al = texture2D(texture, vec2(v_texCoords.x - textureOffset, v_texCoords.y)).a;
    float ar = texture2D(texture, vec2(v_texCoords.x + textureOffset, v_texCoords.y)).a;

    if (au == 1.0 || ad == 1.0 || al == 1.0 || ar == 1.0)
      gl_FragColor = color;
    else
      gl_FragColor = col;
  }
}
