#version 120
uniform sampler2D texture;

uniform float outLineSize  = 0.02;
uniform vec4  outLineColor = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    vec2 UV = gl_TexCoord[0].xy;
    vec4 tcol = texture2D(texture, UV);
    
    if (tcol.a == 0.0)
    {
        if (texture2D(texture, UV + vec2(0.0,          outLineSize)).a  != 0.0 ||
            texture2D(texture, UV + vec2(0.0,         -outLineSize)).a  != 0.0 ||
            texture2D(texture, UV + vec2(outLineSize,  0.0)).a          != 0.0 ||
            texture2D(texture, UV + vec2(-outLineSize, 0.0)).a          != 0.0 ||
            texture2D(texture, UV + vec2(-outLineSize, outLineSize)).a  != 0.0 ||
            texture2D(texture, UV + vec2(-outLineSize, -outLineSize)).a != 0.0 ||
            texture2D(texture, UV + vec2(outLineSize,  outLineSize)).a  != 0.0 ||
            texture2D(texture, UV + vec2(outLineSize,  -outLineSize)).a != 0.0) 
            tcol = outLineColor;
    }
    
    gl_FragColor = tcol;
}