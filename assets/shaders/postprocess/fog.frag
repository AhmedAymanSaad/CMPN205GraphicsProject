#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

in float depth;

// float getFogFactor(float d)
// {
//     const float FogMax = 0.2;
//     const float FogMin = 0.1;

//     if (d>=FogMax) return 1;
//     if (d<=FogMin) return 0;

//     return 1 - (FogMax - d) / (FogMax - FogMin);
// }

void main(){
    // To apply the grayscale effect, we compute the average of the red/blue/green channels
    // and set that average value to all the channels
    vec4  fog_colour = vec4(0.4, 0.4, 0.4, 1.0);

    // float d = distance(gl_Position.xyz, CameraEye.xyz);
    // float alpha =getFogFactor(d);
    frag_color = mix(fog_colour,texture(tex, tex_coord), depth);
}