#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>


#define WIDTH 480
#define HEIGHT 480

const GLchar* vertexSrc = R"glsl(
    #version 150 core

    in vec2 position;

    void main()
    {
        gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    }
)glsl";


const char* fragmentSrc = R"glsl(
    #version 150 core

    uniform int frame;
    out vec4 fragColor;
    layout(origin_upper_left, pixel_center_integer) in vec4 gl_FragCoord;


    void main() {
        if ((int(gl_FragCoord.x/2) ^ int(gl_FragCoord.y/2)) > frame) {
            fragColor = vec4(0.0,0.0,0.0,1.0);
        } else {
            fragColor = vec4(0.0,1.0,0.0,1.0);
        }
    }
)glsl";

void errors(GLuint shader){
    char buffer[512];
    int length = 0;
    glGetShaderInfoLog(shader, 512, &length, buffer);
    if (length)
        std::cout << buffer << std::endl;
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "XorGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // vertex buffers
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);

    // a square screen sized
    GLfloat vertices[] = {
        -1, 1,
        1, -1,
        1, 1,

        -1, 1,
        -1, -1,
        1, -1
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);

    errors(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);

    errors(fragmentShader);

    // load program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // uniform for the shader
    int frame = 0;
    GLint shader_frame = glGetUniformLocation(shaderProgram, "frame");

    int count = 0;
    auto start = std::chrono::system_clock::now();
    std::chrono::seconds sec(1);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glUniform1i(shader_frame, frame++);

        if (frame > 510) frame = 0;

        glfwSwapBuffers(window);
        glfwPollEvents();


        // FPS stuff
        auto diff = std::chrono::system_clock::now() - start;
        ++count;
        if (diff > sec) {
            std::cout << "FPS: " << count << '\n';
            start = std::chrono::system_clock::now();
            count = 0;
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
