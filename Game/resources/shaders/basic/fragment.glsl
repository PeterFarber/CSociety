#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform struct Light {
    vec3 position;
    vec3 intensities; //a.k.a the color of the light
    float attenuation;
    float ambientCoefficient;
} light;

uniform sampler2D materialTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

uniform vec3 lightPos;
uniform vec3 cameraPosition;
uniform bool blinn;

uniform mat4 model;


void main() {
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fs_in.Normal);
    vec3 surfacePos = vec3(model * vec4(fs_in.FragPos, 1));
    vec4 surfaceColor = texture(materialTex, fs_in.TexCoords);
    vec3 surfaceToLight = normalize(light.position - surfacePos);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    //ambient
    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

    //diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

    //specular
    float specularCoefficient = 0.0;
    if (diffuseCoefficient > 0.0)
    specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    //attenuation
    float distanceToLight = length(light.position - surfacePos);
    float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

    //linear color (color before gamma correction)
    vec3 linearColor = ambient + attenuation * (diffuse + specular);

    //final color (after gamma correction)
    vec3 gamma = vec3(1.0 / 2.2);
    FragColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}
//
//void main()
//{
//    vec3 color = texture(image, fs_in.TexCoords).rgb;
//    // ambient
//    vec3 ambient = 0.05 * color;
//    // diffuse
//    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//    vec3 normal = normalize(fs_in.Normal);
//    float diff = max(dot(lightDir, normal), 0.0);
//    vec3 diffuse = diff * color;
//    // specular
//    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = 0.0;
//    if (blinn)
//    {
//        vec3 halfwayDir = normalize(lightDir + viewDir);
//        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
//    }
//    else
//    {
//        vec3 reflectDir = reflect(-lightDir, normal);
//        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
//    }
//    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
//    FragColor = vec4(ambient + diffuse + specular, 1.0);
//}