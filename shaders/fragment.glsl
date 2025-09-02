#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

vec3 fragmentShader(vec3 n, vec3 l)
{
    float cos_theta = dot(normalize(n), normalize(l));
    return vec3(cos_theta);
}

vec3 toonShading(vec3 n, vec3 l, float levels) {
    float cos_theta = dot(normalize(n), normalize(l));
    float quantize = floor(cos_theta * levels) / levels;

    float brightness = 0.25 + 0.5 * quantize;

    return vec3(brightness);
}


void main() {
    vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));
    vec3 color = fragmentShader(Normal, lightDir);
    FragColor = vec4(color, 1.0);
}
