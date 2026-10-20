#version 330

in vec2 fragTexCoord;
out vec4 fragColor;
uniform sampler2D texture0;
uniform float threshold;

void main() {
    vec4 color = texture(texture0, fragTexCoord);
    float brightness = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    fragColor = brightness > threshold ? color : vec4(0.0);
}
