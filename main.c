#include <GL/glew.h>
#include "glfw3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shaders.h"
#include <math.h>
#include <ctype.h>

typedef struct
{
    float r, g, b, a;
} Color;

typedef struct
{
    Color emission;
    Color diffuse;
    float index_of_refraction;
} Lambert;

typedef struct
{
    char sid[50]; // Assuming sid won't exceed 50 characters
    Lambert lambert;
} Technique;

typedef struct
{
    Technique technique;
} Profile_COMMON;

typedef struct
{
    char id[50]; // Assuming id won't exceed 50 characters
    Profile_COMMON profile_common;
} Effect;

typedef struct
{
    Effect effects[10]; // Assuming there won't be more than 10 effects
    int effect_count;
} Library_Effects;

typedef struct
{
    char url[50]; // Assuming url won't exceed 50 characters
} Instance_Effect;

typedef struct
{
    Instance_Effect instance_effect;
} Material;

typedef struct
{
    Material materials[10]; // Assuming there won't be more than 10 materials
    int material_count;
} Library_Materials;

typedef struct
{
    char id[50];   // Assuming id won't exceed 50 characters
    char name[50]; // Assuming name won't exceed 50 characters
} Geometry;

typedef struct
{
    Geometry geometries[10]; // Assuming there won't be more than 10 geometries
    int geometry_count;

} Library_Geometries;

typedef struct
{
    Library_Effects library_effects;
    Library_Materials library_materials;
    Library_Geometries library_geometries;
} CCollada;

int main()
{

    // Open the "thing.dae" file for reading
    FILE *file = fopen("thing.dae", "r");

    if (file == NULL)
    {
        // If the file is not found, print an error message and exit
        printf("File not found\n");
        return -1;
    }

    // Variables to read lines from the file
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int maxWhitespaceCount = 0;

    CCollada ccollada;

    // Flags to keep track of what we're reading

    // Effects flags
    int isReadingLibraryEffects = 0;
    int isReadingEffect = 0;
    int isReadingProfileCommon = 0;
    int isReadingTechnique = 0;
    int isReadingLambert = 0;
    int isReadingEmission = 0;
    int isReadingDiffuse = 0;
    int isReadingIndexOfRefraction = 0;
    int effectReadingIndex = 0; // Index for the effect we're reading

    // Material flags
    int isReadingLibraryMaterials = 0;
    int isReadingMaterial = 0;
    int materialReadingIndex = 0; // Index for the material we're reading

    // Geometry flags
    int isReadingLibraryGeometries = 0;
    int isReadingGeometry = 0;
    int geometryReadingIndex = 0; // Index for the geometry we're reading

    // Read the file line by line
    while ((read = getline(&line, &len, file)) != -1)
    {
        int whitespaceCount = 0;
        int lastWhitespaceCount = 0;
        int lineLength = strlen(line);

        for (int i = 0; i < lineLength; i++)
        {
            char tagName[100] = {0};

            if (isspace(line[i]))
            {
                whitespaceCount++;
            }
            else
            {
                if (whitespaceCount > maxWhitespaceCount)
                {
                    maxWhitespaceCount = whitespaceCount;
                }

                if (line[i] == '<')
                {
                    // if (line[i + 1] == '/')
                    // {
                    //     // Tag end
                    //     i++;
                    //     continue;
                    // }

                    // Tag start
                    // Getting the tag name... Go to the right until you find a space or end of tag
                    int j = i + 1;
                    int k = 0; // Index for tag name.

                    while (j < lineLength && line[j] != ' ' && line[j] != '>')
                    {
                        if (k < sizeof(tagName) - 1) // Prevent buffer overflow
                        {
                            tagName[k++] = line[j];
                        }
                        j++;
                    }
                    tagName[k] = '\0'; // Null terminate the string
                    // printf("Tag name: %s\n", tagName);

                    // Effects flag handling
                    if (strcmp(tagName, "library_effects") == 0)
                    {
                        isReadingLibraryEffects = 1;
                    }
                    else if (strcmp(tagName, "/library_effects") == 0)
                    {
                        isReadingLibraryEffects = 0;
                        isReadingEffect = 0;
                    }
                    else if (strcmp(tagName, "effect") == 0)
                    {
                        isReadingEffect = 1;
                        ccollada.library_effects.effect_count++;
                    }
                    else if (strcmp(tagName, "/effect") == 0)
                    {
                        effectReadingIndex++;
                    }
                    else if (strcmp(tagName, "profile_COMMON") == 0)
                    {
                        isReadingProfileCommon = 1;
                    }
                    else if (strcmp(tagName, "/profile_COMMON") == 0)
                    {
                        isReadingProfileCommon = 0;
                    }
                    else if (strcmp(tagName, "technique") == 0)
                    {
                        isReadingTechnique = 1;
                    }
                    else if (strcmp(tagName, "/technique") == 0)
                    {
                        isReadingTechnique = 0;
                    }
                    else if (strcmp(tagName, "lambert") == 0)
                    {
                        isReadingLambert = 1;
                    }
                    else if (strcmp(tagName, "/lambert") == 0)
                    {
                        isReadingLambert = 0;
                    }
                    else if (strcmp(tagName, "emission") == 0)
                    {
                        isReadingEmission = 1;
                    }
                    else if (strcmp(tagName, "/emission") == 0)
                    {
                        isReadingEmission = 0;
                    }
                    else if (strcmp(tagName, "diffuse") == 0)
                    {
                        isReadingDiffuse = 1;
                    }
                    else if (strcmp(tagName, "/diffuse") == 0)
                    {
                        isReadingDiffuse = 0;
                    }
                    else if (strcmp(tagName, "index_of_refraction") == 0)
                    {
                        isReadingIndexOfRefraction = 1;
                    }
                    else if (strcmp(tagName, "/index_of_refraction") == 0)
                    {
                        isReadingIndexOfRefraction = 0;
                    } // Material flag handling
                    else if (strcmp(tagName, "library_materials") == 0)
                    {
                        isReadingLibraryMaterials = 1;
                    }
                    else if (strcmp(tagName, "/library_materials") == 0)
                    {
                        isReadingLibraryMaterials = 0;
                    }
                    else if (strcmp(tagName, "material") == 0)
                    {
                        isReadingMaterial = 1;
                        ccollada.library_materials.material_count++;
                    }
                    else if (strcmp(tagName, "/material") == 0)
                    {
                        isReadingMaterial = 0;
                        materialReadingIndex++;
                    }
                    else if (strcmp(tagName, "library_geometries") == 0)
                    {
                        isReadingLibraryGeometries = 1;
                    }
                    else if (strcmp(tagName, "/library_geometries") == 0)
                    {
                        isReadingLibraryGeometries = 0;
                    }
                    else if (strcmp(tagName, "geometry") == 0)
                    {
                        isReadingGeometry = 1;
                        ccollada.library_geometries.geometry_count++;
                    }
                    else if (strcmp(tagName, "/geometry") == 0)
                    {
                        isReadingGeometry = 0;
                        geometryReadingIndex++;
                    }

                    // If there's a space, read attributes
                    if (line[j] == ' ')
                    {
                        int attributeReaderLine = j + 1;
                        char attributeName[100] = {0};
                        int attributeNameIndex = 0;
                        int isReadingValue = 0;

                        char attributeValue[100] = {0};
                        int attributeValueIndex = 0;

                        while (attributeReaderLine < lineLength && line[attributeReaderLine] != '>')
                        {
                            if (line[attributeReaderLine] == '=')
                            {
                                isReadingValue = 1;
                                attributeReaderLine += 2; // Skip '=' and opening quote
                                continue;
                            }

                            if (isReadingValue && line[attributeReaderLine] != '"')
                            {
                                attributeValue[attributeValueIndex++] = line[attributeReaderLine];
                                attributeReaderLine++;
                                continue;
                            }
                            else if (line[attributeReaderLine] == '"')
                            {
                                attributeValue[attributeValueIndex] = '\0';
                                // Effect attribute readings
                                if (isReadingLibraryEffects && isReadingEffect)
                                {
                                    if (strcmp(attributeName, "id") == 0)
                                    {
                                        strcpy(ccollada.library_effects.effects[effectReadingIndex].id, attributeValue);
                                    }
                                } // Material attribute readings
                                else if (isReadingLibraryMaterials && isReadingMaterial)
                                {
                                    if (strcmp(attributeName, "id") == 0)
                                    {
                                        strcpy(ccollada.library_materials.materials[materialReadingIndex].instance_effect.url, attributeValue);
                                    }
                                    else if (strcmp(tagName, "instance_effect") == 0 && strcmp(attributeName, "url") == 0)
                                    {
                                        strcpy(ccollada.library_materials.materials[materialReadingIndex].instance_effect.url, attributeValue);
                                    }
                                }
                                else if (isReadingLibraryGeometries)
                                {
                                    if (isReadingGeometry && strcmp(tagName, "geometry") == 0)
                                    {
                                        if (strcmp(attributeName, "id") == 0)
                                        {
                                            strcpy(ccollada.library_geometries.geometries[geometryReadingIndex].id, attributeValue);
                                        }
                                        else if (strcmp(attributeName, "name") == 0)
                                        {
                                            strcpy(ccollada.library_geometries.geometries[geometryReadingIndex].name, attributeValue);
                                        }
                                    }
                                }

                                isReadingValue = 0;
                                attributeReaderLine++; // Skip closing quote

                                attributeName[attributeNameIndex] = '\0';   // Null terminate the string
                                attributeValue[attributeValueIndex] = '\0'; // Null terminate the string
                                // printf("Attribute name: %s\n", attributeName);
                                // printf("Attribute value: %s\n", attributeValue);
                                attributeNameIndex = 0;
                                attributeValueIndex = 0;
                                attributeName[0] = '\0';
                                attributeValue[0] = '\0';

                                continue;
                            }

                            if (!isReadingValue && line[attributeReaderLine] != ' ')
                            {
                                if (attributeNameIndex < sizeof(attributeName) - 1) // Prevent buffer overflow
                                {
                                    attributeName[attributeNameIndex++] = line[attributeReaderLine];
                                }
                            }
                            attributeReaderLine++;
                        }

                        if (line[attributeReaderLine] == '>')
                        {
                            // End of tag, process content.

                            // Go to the right until you find a '<' character
                            int contentStart = attributeReaderLine + 1;
                            int contentEnd = 0;
                            int contentLength = 0;

                            for (int k = contentStart; k < lineLength; k++)
                            {
                                if (line[k] == '<')
                                {
                                    contentEnd = k;
                                    contentLength = contentEnd - contentStart;
                                    break;
                                }
                            }

                            if (contentLength > 0)
                            {
                                char content[contentEnd];
                                int contentIndex = 0;

                                for (int k = contentStart; k < contentEnd; k++)
                                {
                                    content[contentIndex++] = line[k];
                                }

                                // Effects value readings.
                                if (isReadingEffect)
                                {
                                    if (isReadingEmission && strcmp(tagName, "color") == 0)
                                    {
                                        float r, g, b, a;
                                        sscanf(content, "%f %f %f %f", &r, &g, &b, &a);
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.emission.r = r;
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.emission.g = g;
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.emission.b = b;
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.emission.a = a;
                                    }
                                    else if (isReadingDiffuse && strcmp(tagName, "color") == 0)
                                    {
                                        float r, g, b, a;
                                        sscanf(content, "%f %f %f %f", &r, &g, &b, &a);
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.diffuse.r = r;
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.diffuse.g = g;
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.diffuse.b = b;
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.diffuse.a = a;
                                    }
                                    else if (isReadingIndexOfRefraction && strcmp(tagName, "float") == 0)
                                    {
                                        float index_of_refraction;
                                        sscanf(content, "%f", &index_of_refraction);
                                        ccollada.library_effects.effects[effectReadingIndex].profile_common.technique.lambert.index_of_refraction = index_of_refraction;
                                    }
                                }

                                content[contentIndex] = '\0'; // Null terminate the string
                                // printf("Content: %s\n", content);
                            }
                        }
                    }
                    i = j; // Move i to the end of the tag
                    whitespaceCount = 0;
                    continue;
                }
            }
        }
    }

    printf("\n\nEffects\n\n");
    for (int i = 0; i < ccollada.library_effects.effect_count; i++)
    {
        printf("Effect id: %s\n", ccollada.library_effects.effects[i].id);
        float r, g, b, a;
        r = ccollada.library_effects.effects[i].profile_common.technique.lambert.emission.r;
        g = ccollada.library_effects.effects[i].profile_common.technique.lambert.emission.g;
        b = ccollada.library_effects.effects[i].profile_common.technique.lambert.emission.b;
        a = ccollada.library_effects.effects[i].profile_common.technique.lambert.emission.a;
        printf("Emission: %f %f %f %f\n", r, g, b, a);
        r = ccollada.library_effects.effects[i].profile_common.technique.lambert.diffuse.r;
        g = ccollada.library_effects.effects[i].profile_common.technique.lambert.diffuse.g;
        b = ccollada.library_effects.effects[i].profile_common.technique.lambert.diffuse.b;
        a = ccollada.library_effects.effects[i].profile_common.technique.lambert.diffuse.a;
        printf("Diffuse: %f %f %f %f\n", r, g, b, a);
        printf("Index of refraction: %f\n", ccollada.library_effects.effects[i].profile_common.technique.lambert.index_of_refraction);
        printf("\n");
    }

    printf("\nMaterials\n\n");
    for (int i = 0; i < ccollada.library_materials.material_count; i++)
    {
        printf("Material id: %s\n", ccollada.library_materials.materials[i].instance_effect.url);
        printf("\n");
    }

    printf("\nGeometries\n\n");
    for (int i = 0; i < ccollada.library_geometries.geometry_count; i++)
    {
        printf("Geometry id: %s\n", ccollada.library_geometries.geometries[i].id);
        printf("Geometry name: %s\n", ccollada.library_geometries.geometries[i].name);
        printf("\n");
    }

    fclose(file);

    return 0;

    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW window hints for OpenGL version (adjust to your needs)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(1024, 1024, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Ensure GLEW uses modern techniques for managing OpenGL functionality
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        return -1;
    }

    // Query and print the OpenGL version
    const GLubyte *version = glGetString(GL_VERSION);
    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *vendor = glGetString(GL_VENDOR);
    const GLubyte *shadingLanguageVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    GLint maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);

    printf("OpenGL Version: %s\n", version);
    printf("Renderer: %s\n", renderer);
    printf("Vendor: %s\n", vendor);
    printf("GLSL Version: %s\n", shadingLanguageVersion);
    printf("Max Attributes: %d\n", maxVertexAttribs);

    GLint maxVertexUniforms, maxFragmentUniforms, maxGeometryUniforms;

    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniforms);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniforms);
    glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &maxGeometryUniforms);

    printf("Max Vertex Uniforms: %d\n", maxVertexUniforms);
    printf("Max Fragment Uniforms: %d\n", maxFragmentUniforms);
    printf("Max Geometry Uniforms: %d\n", maxGeometryUniforms);

    GLuint shaders[2];
    shaders[0] = genShader("vertexShader.glsl", GL_VERTEX_SHADER);
    shaders[1] = genShader("fragmentShader.glsl", GL_FRAGMENT_SHADER);

    GLuint shaderProgram = genShaderProgram(shaders, 2);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {

        // Set the clear color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glUseProgram(shaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    printf("\nExiting...\n");

    // Clean up

    glDeleteProgram(shaderProgram);

    for (int i = 0; i < 2; i++)
    {
        glDeleteShader(shaders[i]);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}