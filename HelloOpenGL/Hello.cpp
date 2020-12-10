

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Mac OS X ��Ҫ�������
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

    
    unsigned int vertexShader;
    // ����������ɫ��
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // ����Դ��
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // ����
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    // ��������
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // �����־��Ϣ
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR " << infoLog << std::endl;
    }

    

    unsigned int fragmentShader;
    // ����Ƭ����ɫ��
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // ��������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR " << infoLog << std::endl;
    }

    
    unsigned int shaderProgram;
    // ������ɫ������
    shaderProgram = glCreateProgram();
    // ��ɫ�����ӵ�����
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // ����
    glLinkProgram(shaderProgram);
    // ��������
    glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR " << infoLog << std::endl;
    }
    // ���ӵ������,������Ҫ,ɾ��
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);



    float vertices[] = {
        0.5f, 0.5f, 0.0f,
        0.5f,-0.5f,0.0f,
        -0.5f, -0.5f, 0.0f,
        - 0.5f, 0.5f, 0.0f
    };

    unsigned int indices[] = {
        0,1,3,// ��һ��������
        1,2,3// �ڶ���������
    };

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    // �����������
    glGenVertexArrays(1, &VAO);
    // �������㻺����� vbo
    glGenBuffers(1, &VBO);
    // �����������
    glGenBuffers(1, &EBO);

    // �Ȱ�VAO, ������VBO ��洢�� VAO ��
    // 
    glBindVertexArray(VAO);

    // �󶨵� GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //���ƶ��㵽������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    /// ������ν�����������
    // ����0Ϊ location=0 ���õ�λ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    while (!glfwWindowShouldClose(window))
    {
        // ����
        processInput(window);

        glClearColor(0.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ��鲢�����¼�
        glfwPollEvents();
        // ��������, ����
        glfwSwapBuffers(window);
       
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}