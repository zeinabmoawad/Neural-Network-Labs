#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

struct Color
{
    uint8_t r, g, b, a;
};

struct Vertex
{
    glm::vec3 position;
    Color color;
    glm::vec2 tex_coord;

};

GLuint load_shader(const std::string &path, GLenum shader_type)
{
    //=======================1- Reading File=====================//
    std::ifstream file(path);
    std::string sourceCode = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char *sourceCodeCStr = sourceCode.c_str();
    //======================2- Creating Shader=================//
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
    glCompileShader(shader);
    //===================3- check shader in compiling=============//
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::cerr << "ERROR IN " << path << std::endl;
        std::cerr << logStr << std::endl;
        delete[] logStr;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

int main()
{
    // GLFW only for window but not for Drawing
    //==============1- initiate GLFW=================//
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        exit(1);
    }
    //==============2- configure GLFW=================//
    // verion 3.3 of OPENGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //===================3- Creating window===========//
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
    //===================4- Checking window===========//
    if (!window)
    {
        std::cerr << "Failed to create Window\n";
        glfwTerminate();
        exit(1);
    }
    //===========5- Making window contain whole context of OpenGL======//
    // to make current window before loading
    glfwMakeContextCurrent(window);
    //=============6- load all function by name (take string return pointer)====//
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to load OpenGL\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }
    //===========-create Shaders & link it to program ==================//
    GLuint program = glCreateProgram();
    GLuint vs = load_shader("assets/shader/simple.vert", GL_VERTEX_SHADER);
    GLuint fs = load_shader("assets/shader/simple.frag", GL_FRAGMENT_SHADER);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    //================18- GPU memory====================//
    Vertex vertices[4] = {
        {{-0.5, -0.5, 0.0}, {255, 0, 0, 255},{0.0,0.0}},
        {{0.5, -0.5, 0.0}, {0, 255, 0, 255},{1.0,0.0}},
        {{0.5, 0.5, 0.0}, {0, 0, 255, 255},{1.0,1.0}},
        {{-0.5, 0.5, 0.0}, {255, 255, 0, 255},{0.0,1.0}},
    };

    uint16_t elements[6] = {0, 1, 2, 2, 3, 0};
    //=================vertext buffer=======================//
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    //=================Element buffer=======================//
    GLuint element_buffer;
    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint16_t), elements, GL_STATIC_DRAW);
    //================7- Vertex Array============================//
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    //==========Determined loctions
    GLint position_loc = 0; // glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, false, sizeof(Vertex), 0);

    GLint color_loc = 1; // glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void *)offsetof(Vertex, color));
    
    GLint tex_coord_loc = 2; // glGetAttribLocation(program, "tex_coord_loc");
    glEnableVertexAttribArray(tex_coord_loc);
    glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));

    //=====bind vertex array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    //==================unbind
    glBindVertexArray(0);
    //=================15- get loction for uniforms(vert==>c++)========//
    //== each varible in shaders has loction in memory=======//
    //=====if not exit wiil return 0 =====//
    // GLuint time_loc = glGetUniformLocation(program, "time");
    // std::cout << "Time uniform loction" << time_loc << std::endl;
    //=================================3D===============================//
    GLint mvp_loc = glGetUniformLocation(program, "MVP");
    //==============================Depth============================//
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClearDepth(1.0f);
    //========================================Draw in window size====================//
    glViewport(0, 0, 320, 240);//window uses to draw

    //================================To color part to draw====================//
    // glEnable(GL_SCISSOR_TEST);// only that part can put color here
    // glScissor(0, 0, 320, 240);
    //======================================Texture=============================//
    Color B={0,0,0,255};
    Color W={255,255,255,255};
    Color Y={255,255,0,255};
    Color image[]={
    W,W,Y,Y,Y,Y,W,W,
    W,Y,Y,B,B,Y,Y,W,
    Y,Y,B,Y,Y,B,Y,Y, 
    Y,Y,B,Y,Y,B,Y,Y,
    Y,Y,Y,Y,Y,Y,Y,Y,
    Y,Y,B,Y,Y,B,Y,Y,
    W,Y,Y,Y,Y,Y,Y,W,
    W,W,Y,Y,Y,Y,W,W,
    };
    //=========================TO DRAW IN GPU=====================================//
    GLuint texture;
    glGenTextures(1,&texture);//1=># of texture
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB8,8,8,0,GL_RGBA,GL_UNSIGNED_BYTE,image);//(0=>level,# of bits,width,height,border,formate of input,datatype,pixels)
     glGenerateMipmap(GL_TEXTURE_2D);
    //================================Prevent Filtering=========================
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     GLint tex_loc = glGetUniformLocation(program, "tex");
    //==================8- condition when user click on close button to start==========//
    while (!glfwWindowShouldClose(window))
    {
        //=========================calculate Ascpect ratio in each frame================//
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);// to make window size variable to draw
        glViewport(0, 0, width, height);
        // glClearColor(0.5 * sin(time) + 0.5,
        //              0.5 * sin(time + 1.0) + 0.5,
        //              0.5 * sin(time + 2.0) + 0.5,
        //              1.0);
        float time = (float)glfwGetTime();
        //==============9- Clear Window========================//
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // glClearColor(0.0, 0.0, 0.0, 1.0);
        // glClear(GL_COLOR_BUFFER_BIT);
        //==============10- using vertex of program========================//
        glUseProgram(program);
        glBindVertexArray(vertex_array);
        //==============================Bind texture==================================//
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glUniform1i(tex_loc,0);
        //=============================3D================================//
        glm::mat4 P = glm::perspective(glm::pi<float>() * 0.5f, float(width) / height, 0.01f, 1000.0f);
        glm::mat4 V = glm::lookAt(
            glm::vec3(2.0f * cosf(time), 1.0f, 2.0f * sinf(time)),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = -2; i <= 2; i++)
        {
            glm::mat4 M = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (float)i));

            glm::mat4 MVP = P * V * M;

            glUniformMatrix4fv(mvp_loc, 1, false, &MVP[0][0]);
            //=============17- after use program ========================//
            //=======(loction,value)
            // glUniform1f(time_loc, time);
            // glDrawArrays(GL_TRIANGLES, 0, 6); // call vertex shader 3 times
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        }
        // glUniform1f(time_loc, time + 1.0); // not in same phase
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        //==============11- to display in buffer (to make it front buffer)================//
        glfwSwapBuffers(window);
        //=============12- sensing input(all action in screen)================//
        glfwPollEvents();
    }
    //================13- delete program =============//
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &element_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteProgram(program);
    //===================14- destroy window for ===========//
    glfwDestroyWindow(window);
    //==============15- terminate GLFW=================//
    glfwTerminate();

    return 0;
}
