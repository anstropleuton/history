#version 330

in vec2 fragTexCoord;
out vec4 fragColor;
uniform sampler2D texture0;
uniform vec2 blurDirection;

void main() {
float weights[9] = float[](
    0.18,   // weight for offsets ±4
    0.18,   // ±3
    0.18,   // ±2
    0.18,   // ±1
    0.18    // center (offset 0)
);

vec2 texelSize = blurDirection / textureSize(texture0, 0);
vec4 result = texture(texture0, fragTexCoord) * weights[4]; // center

for (int i = 1; i <= 4; ++i) {
    result += texture(texture0, fragTexCoord + texelSize * float(i)) * weights[4 - i];
    result += texture(texture0, fragTexCoord - texelSize * float(i)) * weights[4 - i];
}

fragColor = result;
}
