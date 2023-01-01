// The texture holding the scene pixels
#version 330
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main()
{
    // Read the pixel at the given texture coordinates
    vec4 pixel = texture(tex, tex_coord);

    // Apply a simple gamma correction
    pixel.rgb = pow(pixel.rgb, vec3(1.0/2.2));

    // mix with a blue color
    pixel.rgb = mix(pixel.rgb, vec3(0.0, 0.0, 1.0), 0.5);
    // Write the pixel to the output
    frag_color = pixel;

     
}