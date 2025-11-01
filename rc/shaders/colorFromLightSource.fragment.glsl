#version 460 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Material u_Material;
uniform Light u_LightSource;
uniform vec3 u_ViewPos;

void main()
{
    // Calculate ambient
    vec3 ambient = u_Material.ambient * u_LightSource.ambient;

    // Calculate diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(u_LightSource.position - FragPos);
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = (diffuseStrength * u_Material.diffuse) * u_LightSource.diffuse;

    // Calculate specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 viewDirection = normalize(u_ViewPos - FragPos);
    float specStrength = pow(max(dot(reflectDirection, viewDirection), 0.0), u_Material.shininess);
    vec3 specular = (specStrength * u_Material.specular) * u_LightSource.specular;

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.f);
}
