#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main(){
    // To apply the grayscale effect, we compute the average of the red/blue/green channels
    // and set that average value to all the channels
    frag_color = texture(tex, tex_coord); 

    // fog mix
    vec4  fog_colour = vec4(0.4, 0.4, 0.4, 1.0);
    // const float LOG2 = 1.442695;
    // float z = gl_FragCoord.z / gl_FragCoord.w;
    // float fogFactor = exp2( -5 * 
    //               5 * 
    //                z * 
    //                z * 
    //                LOG2 );
    // fogFactor = clamp(fogFactor, 0.0, 1.0);


    frag_color = mix(fog_colour,frag_color, 0.1);

}