#version 330

const float RED_LUM_CONSTANT   = 0.2126;
const float GREEN_LUM_CONSTANT = 0.7152;
const float BLUE_LUM_CONSTANT  = 0.0722;
const float LINEAR_TERM        = 0.0005;  // linear term
const float QUADRATIC_TERM     = 0.00009; // quadratic term
const float MIN_BRIGHTNESS     = 0.05;    // avoid total darkness

uniform sampler2D texture0;

uniform vec2 lightPosition;
uniform int ursalaCollide;
uniform int netCollide;
uniform float ericDis;
uniform float caveDis;
uniform float itemDis;

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 finalColor;

float attenuate(float distance, float linearTerm, float quadraticTerm)
{
    float attenuation = 1.0 / (1.0 +
                               linearTerm * distance +
                               quadraticTerm * distance * distance);

    return max(attenuation, MIN_BRIGHTNESS);
}

void main()
{
    vec4 color = texture(texture0, fragTexCoord);

    float tint = 0.0;
    vec3 tintColor = color.rgb;

    if (ursalaCollide == 1) {
        tintColor = vec3(0.5, 0.0, 1.0);
        tint = 0.22;
    }

    else if (netCollide == 1) {
        tintColor = vec3(1.0, 0.0, 0.0);
        tint = 0.22;
    }

    else if (itemDis >= 0.0 && itemDis <= 600.0) {
        tintColor = vec3(1.0, 1.0, 0.0);
        tint = 0.22;
    }

    else if (caveDis >= 0.0 && caveDis <= 500.0) {
            tintColor = vec3(0.0, 0.4, 1.0);
            tint = 0.22;
        }

    else if (ericDis >= 0.0 && ericDis <= 300.0) {
        tintColor = vec3(1.0, 0.4, 0.7);
        tint = 0.22;
    }


    finalColor = vec4(color.rgb * (0.78) + tintColor * tint, color.a);
}