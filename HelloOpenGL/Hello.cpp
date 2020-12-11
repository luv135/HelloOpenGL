

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "framebuffer_size_callback change "<<width<<" "<<height<< std::endl;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"   //定义颜色提供给片段着色器
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   vertexColor = aColor;"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 vertexColor;\n"    //获取顶点着色器中定义的颜色,名称相同即可
    "void main()\n"
    "{\n"
    //"   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
    "   FragColor = vec4(vertexColor,1.0f);\n"
    "}\n\0";


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Mac OS X 需要下面代码
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //---------------------111---------------------------------
    unsigned int vertexShader;
    // 创建顶点着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 关联源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    // 检查编译结果
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // 输出日志信息
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR " << infoLog << std::endl;
    }


    unsigned int fragmentShader;
    // 创建片段着色器
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 检查编译结果
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR " << infoLog << std::endl;
    }


    
    unsigned int shaderProgram;
    // 创建着色器程序
    shaderProgram = glCreateProgram();
    // 着色器附加到程序
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // 链接
    glLinkProgram(shaderProgram);
    // 检查编译结果
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR " << infoLog << std::endl;
    }


    // 链接到程序后,不再需要,删除
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    


    float vertices[] = {
        // 位置
        0.5f, 0.5f, 0.0f,       1.0f,0.0f,0.0f,
        0.5f,-0.5f,0.0f,        0.0f,1.0f,0.0f,
        -0.5f, -0.5f, 0.0f,     0.0f,0.0f,1.0f,
        - 0.5f, 0.5f, 0.0f,     2.0f,1.0f,0.0f,
    };

    unsigned int indices[] = {
        0,1,3,// 第一个三角形
        1,2,3// 第二个三角形
    };

    unsigned int VBO[2];
    unsigned int VAO[2];
    unsigned int EBO;
    // 顶点数组对象
    glGenVertexArrays(2, VAO);
    // 创建顶点缓冲对象 vbo
    glGenBuffers(2, VBO);
    // 索引缓冲对象
    glGenBuffers(1, &EBO);

    // 先绑定VAO, 后续的VBO 会存储到 VAO 中
    glBindVertexArray(VAO[0]);

    // 绑定到 GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    //复制顶点到缓冲区
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /// 设置如何解析顶点数据
    // 参数0为 location=0 设置的位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


   

    int width, height, nrChannels;
    // 加载图片
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    
    unsigned int texture;
    // 生成纹理id
    glGenTextures(1, &texture);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);

    // 纹理对象设置环绕,过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 载入生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0/*历史遗留问题,填0*/, GL_RGB, GL_UNSIGNED_BYTE,data);
    // 自动生成多级渐远纹理
    glGenerateMipmap(GL_TEXTURE_2D);
    // 释放图片内存
    stbi_image_free(data);




    while (!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);

        glClearColor(0.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //float timeValue = glfwGetTime();
        //float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        //// 当前激活的着色器程序中设置值
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO[0]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glUseProgram(shaderProgram2);
        //glBindVertexArray(VAO[1]);
        //glDrawArrays(GL_TRIANGLES, 0, 3);

        // 检查并调用事件
        glfwPollEvents();
        // 交换缓冲, 绘制
        glfwSwapBuffers(window);
       
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    //glDeleteProgram(shaderProgram2);
    glfwTerminate();

    return 0;
}