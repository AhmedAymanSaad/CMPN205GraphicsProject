#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

uniform float xs=1280;
uniform float ys=720;


const float RADIUS = 4;

const float SOFTNESS = 0.6;

const float blurSize = 1.0/1000.0;

void main(){
    // To apply the grayscale effect, we compute the average of the red/blue/green channels
    // and set that average value to all the channels
    
    vec4 texColor = vec4(0.0); // texture2D(u_texture, vTexCoord)

    // apply gaussian blur by getting the texcoord average of negigbours

    texColor += texture2D(tex, tex_coord - 4.0*blurSize) * 0.05;
    texColor += texture2D(tex, tex_coord - 3.0*blurSize) * 0.09;
    texColor += texture2D(tex, tex_coord - 2.0*blurSize) * 0.12;
    texColor += texture2D(tex, tex_coord - blurSize) * 0.15;
    texColor += texture2D(tex, tex_coord) * 0.16;
    texColor += texture2D(tex, tex_coord + blurSize) * 0.15;
    texColor += texture2D(tex, tex_coord + 2.0*blurSize) * 0.12;
    texColor += texture2D(tex, tex_coord + 3.0*blurSize) * 0.09;
    texColor += texture2D(tex, tex_coord + 4.0*blurSize) * 0.05;

    frag_color = texColor;

    
    
}