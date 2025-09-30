#version 330 core

in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D textureSampler;

vec3 fragmentShader(vec3 n, vec3 l)
{
    float cos_theta = dot(normalize(n), normalize(l));
    return vec3(cos_theta);
}

void main() {
    vec3 light = fragmentShader(Normal, vec3(normalize(vec3(0.0, 0.0, 1.0))));
    FragColor = texture(textureSampler, TexCoords) * vec4(abs(light), 1.0);
}
