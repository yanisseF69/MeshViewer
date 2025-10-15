#ifndef SHADERS_H
#define SHADERS_H

static const char* vertexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n\n"

    "out vec3 Normal;\n\n"

    "void main() {\n"
    "    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\n";

static const char* vertexTexShader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "layout (location = 2) in vec2 aTexCoords;\n\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n\n"

    "out vec3 Normal;\n"
    "out vec2 TexCoords;\n\n"

    "void main() {\n"
    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "    TexCoords = aTexCoords;\n"
    "    Normal = aNormal;\n"
    "}\n";

static const char* fragmentShader =
    "#version 330 core\n\n"

    "in vec3 Normal;\n\n"

    "out vec4 FragColor;\n\n"

    "vec3 fragmentShader(vec3 n, vec3 l) {\n"
    "    float cos_theta = dot(normalize(n), normalize(l));\n"
    "    return vec3(cos_theta);\n"
    "}\n\n"

    "vec3 toonShading(vec3 n, vec3 l, float levels) {\n"
    "    float cos_theta = dot(normalize(n), normalize(l));\n"
    "    float quantize = floor(cos_theta * levels) / levels;\n"
    "    float brightness = 0.25 + 0.5 * quantize;\n"
    "    return vec3(brightness);\n"
    "}\n\n"


    "void main() {\n"
    "    vec3 lightDir = normalize(vec3(0.0, 0.0, 1.0));\n"
    "    vec3 color = fragmentShader(Normal, lightDir);\n"
    "    // vec3 color = toonShading(Normal, lightDir, 2.0);\n"
    "    FragColor = vec4(abs(color), 1.0);\n"
    "}\n";

static const char* fragmentTexShader =
    "#version 330 core\n\n"

    "in vec3 Normal;\n"
    "in vec2 TexCoords;\n\n"

    "out vec4 FragColor;\n\n"

    "uniform sampler2D textureSampler;\n\n"

    "vec3 fragmentShader(vec3 n, vec3 l) {\n"
    "    float cos_theta = dot(normalize(n), normalize(l));\n"
    "    return vec3(cos_theta);\n"
    "}\n\n"

    "void main() {\n"
    "    vec3 light = fragmentShader(Normal, vec3(normalize(vec3(0.0, 0.0, 1.0))));\n"
    "    FragColor = texture(textureSampler, TexCoords) * vec4(abs(light), 1.0);\n"
    "}\n";

#endif // SHADERS_H
