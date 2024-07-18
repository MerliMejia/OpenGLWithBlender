#include <GL/glew.h>
#include "glfw3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shaders.h"
#include <math.h>
#include <ctype.h>

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
                    if (line[i + 1] == '/')
                    {
                        // Tag end
                        i++;
                        continue;
                    }

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
                    printf("Tag name: %s\n", tagName);

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
                                isReadingValue = 0;
                                attributeReaderLine++; // Skip closing quote

                                attributeName[attributeNameIndex] = '\0'; // Null terminate the string
                                attributeValue[attributeValueIndex] = '\0'; // Null terminate the string
                                printf("Attribute name: %s\n", attributeName);
                                printf("Attribute value: %s\n", attributeValue);
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
                    }
                    i = j; // Move i to the end of the tag
                    whitespaceCount = 0;
                    continue;
                }
            }
        }

        if (whitespaceCount > lastWhitespaceCount)
        {
            // New tag
        }
        else
        {
            // Same tag
        }

        lastWhitespaceCount = whitespaceCount;
    }

    printf("\nMax whitespace count: %d\n\n", maxWhitespaceCount);

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