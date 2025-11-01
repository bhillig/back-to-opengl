#version 460 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 u_Color;
uniform vec3 u_LightSourceColor;
uniform vec3 u_LightSourcePos;
uniform vec3 u_ViewPos;

void main()
{
    // Calculate ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_LightSourceColor;

    // Calculate diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(u_LightSourcePos - FragPos);
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diffuseStrength * u_LightSourceColor;

    // Calculate specular
    float specularStrength = 0.5f;
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 viewDirection = normalize(u_ViewPos - FragPos);
    float spec = pow(max(dot(reflectDirection, viewDirection), 0.0), 64);
    vec3 specular = specularStrength * spec * u_LightSourceColor;

    vec3 color = (ambient + diffuse + specular) * u_Color;
    FragColor = vec4(color, 1.f);
}
