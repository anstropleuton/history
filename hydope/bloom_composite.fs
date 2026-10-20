#version 330

in vec2 fragTexCoord;
out vec4 fragColor;
uniform sampler2D sceneTex;
uniform sampler2D bloomTex;
uniform float bloomIntensity;

void main() {
    vec3 scene = texture(sceneTex, fragTexCoord).rgb;
    vec3 bloom = texture(bloomTex, fragTexCoord).rgb;
    fragColor = vec4(scene + bloom * bloomIntensity, 1.0);
}
