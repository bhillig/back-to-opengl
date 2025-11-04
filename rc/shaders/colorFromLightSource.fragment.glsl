#version 460 core

struct Material
{
    sampler2D diffuse; // Diffuse texture
    sampler2D specular; // Specular texture
    float shininess;
};

const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_POINT       = 1;
const int LIGHT_SPOT        = 2;

struct Light
{
    int type;

    vec3 position; // Used by: Point and spot lights
    vec3 direction; // Used by: Directional and spot lights

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Point light attenuation parameters
    float constant;
    float linear;
    float quadratic;

    // Spotlight parameters
    float innerCutOff;
    float outerCutOff;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material u_Material;
uniform Light u_LightSource;
uniform vec3 u_ViewPos;

void main()
{
    // Determine light direction based on its type
    vec3 lightDirection = (u_LightSource.type == LIGHT_DIRECTIONAL)
        ? normalize(-u_LightSource.direction)
        : normalize(u_LightSource.position - FragPos);

    // Calculate ambient
    vec3 ambient = u_LightSource.ambient * vec3(texture(u_Material.diffuse, TexCoords));

    // Calculate diffuse
    vec3 normal = normalize(Normal);;
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = (diffuseStrength * vec3(texture(u_Material.diffuse, TexCoords))) * u_LightSource.diffuse;

    // Calculate specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 viewDirection = normalize(u_ViewPos - FragPos);
    float specStrength = pow(max(dot(reflectDirection, viewDirection), 0.0), u_Material.shininess);
    vec3 specular = (specStrength * vec3(texture(u_Material.specular, TexCoords))) * u_LightSource.specular;

    // If the light source is a point light account for attenuation
    if (u_LightSource.type == LIGHT_POINT)
    {
        float distance = length(u_LightSource.position - FragPos);
        float attenuation = 1.0 / (u_LightSource.constant + u_LightSource.linear * distance + u_LightSource.quadratic * (distance * distance));

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    // If the light source is a spot light account for intensity radius
    if (u_LightSource.type == LIGHT_SPOT)
    {
        float theta = dot(-u_LightSource.direction, lightDirection);
        float epsilon = u_LightSource.innerCutOff - u_LightSource.outerCutOff;
        float intensity = (theta - u_LightSource.outerCutOff) / epsilon;
        intensity = clamp(intensity, 0.f, 1.f);

        diffuse *= intensity;
        specular *= intensity;
    }

    vec3 color = ambient + diffuse + specular;
    FragColor = vec4(color, 1.f);
}
