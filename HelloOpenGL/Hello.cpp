

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float screenWidth = 800.0f;
float screenHeight = 600.0f;

void matest() 
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans;
	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "framebuffer_size_callback change " << width << " " << height << std::endl;
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
//"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
//"out vec3 ourColor;\n"   //定义颜色提供给片段着色器
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
//"   gl_Position = vec4(aPos, 1.0);\n"
//"   ourColor = aColor;\n"
//"   TexCoord  = aTexCoord;\n"
"	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
//"in vec3 ourColor;\n"    //获取顶点着色器中定义的颜色,名称相同即可
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"       //纹理,默认为0, https://learnopengl-cn.github.io/01%20Getting%20started/06%20Textures/
"uniform sampler2D texture2;\n"
"void main()\n"
"{\n"
//"   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
//"   FragColor = vec4(vertexColor,1.0f);\n"
//"   FragColor = texture(ourTexture,TexCoord) * vec4(ourColor,1.0f);\n"
//"   FragColor = texture(texture2,TexCoord);\n"
"   FragColor = mix(texture(texture1,TexCoord),  texture(texture2,TexCoord), 0.2);\n"
"}\n\0";

void textureCreate(const char* file_path, unsigned int* texture)
{
	int width, height, nrChannels;
	// 加载图片
	unsigned char* data = stbi_load(file_path, &width, &height, &nrChannels, 0);
	std::cout << "width = " << width << " height = " << height << std::endl;
	// 生成纹理id
	glGenTextures(1, texture);
	// 绑定纹理
	glBindTexture(GL_TEXTURE_2D, *texture);

	// 纹理对象设置环绕,过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// 载入生成纹理            
	glTexImage2D(GL_TEXTURE_2D, 0/*多级纹理,默认设置0*/, GL_RGBA/*图片位数*/, width, height, 0/*历史遗留问题,填0*/, GL_RGBA, GL_UNSIGNED_BYTE, data);
	// 自动生成多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);
	// 释放图片内存
	stbi_image_free(data);
}

void bindUniformMatrix4f(int program,const char* name, glm::mat4 mat4)
{
	glUseProgram(program);
	int location = glGetUniformLocation(program, name);
	std::cout << "location = " << location << std::endl;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Mac OS X 需要下面代码
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
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




	//float vertices[] = {
	//	// 位置                 //颜色                //纹理
	//	0.5f, 0.5f, 0.0f,       1.0f,0.0f,0.0f,     1.0f, 1.0f,
	//	0.5f,-0.5f,0.0f,        0.0f,1.0f,0.0f,     1.0f,0.0f,
	//	-0.5f, -0.5f, 0.0f,     0.0f,0.0f,1.0f,     0.0f, 0.0f,
	//	-0.5f, 0.5f, 0.0f,      2.0f,1.0f,0.0f,     0.0f, 1.0f
	//};



	//unsigned int indices[] = {
	//	0,1,3,// 第一个三角形
	//	1,2,3// 第二个三角形
	//};


	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};



	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	// 顶点数组对象
	glGenVertexArrays(1, &VAO);
	// 创建顶点缓冲对象 vbo
	glGenBuffers(1, &VBO);
	// 索引缓冲对象
	glGenBuffers(1, &EBO);

	// 先绑定VAO, 后续的VBO 会存储到 VAO 中
	glBindVertexArray(VAO);

	// 绑定到 GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//复制顶点到缓冲区
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/// 设置如何解析顶点数据
	// 参数0为 location=0 设置的位置
	// 参数说明: 0 location=0 设置的位置, 3 表示vec3  //步长             //起始位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	// 颜色关联
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// 纹理
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


	//-------------------纹理创建---------------
	// 加载图片
	unsigned int texture1;
	unsigned int  texture2;
	textureCreate("container(1).png", &texture1);
	textureCreate("awesomeface.png", &texture2);

	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 5);

	// 矩阵变化
	//glm::mat4 trans = glm::mat4(1.0f);
	// 矩阵执行是从右到左的,如下执行顺序是先缩放再旋转
	//trans = glm::rotate(trans, glm::radians(-50.f), glm::vec3(1, 0.0, 0.0/*绕Z轴旋转*/));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1,GL_FALSE,glm::value_ptr(trans));

	// 坐标系
	//模型矩阵
	glm::mat4 model = glm::mat4(1.0f);
	// x轴旋转
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// 观察矩阵
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

	bindUniformMatrix4f(shaderProgram, "model", model);
	bindUniformMatrix4f(shaderProgram, "view", view);
	bindUniformMatrix4f(shaderProgram, "projection", projection);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// 输入
		processInput(window);
		glClearColor(0.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE5);	//glUniform1i 对应"...texture2"), 5);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//glm::mat4 trans = glm::mat4(1.0f);
		// 矩阵执行是从右到左的,如下执行顺序是旋转再移动
		////trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(1.0, 0.0, 0.0)/*绕Z轴旋转*/);
		//float ff = (int)glfwGetTime() % 10 / 10.0;
		//std::cout << "scale" << ff << std::endl;
		//trans = glm::scale(trans, glm::vec3(ff, ff, ff));
		glUseProgram(shaderProgram);
		//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		bindUniformMatrix4f(shaderProgram, "model", model);

		// 检查并调用事件
		glfwPollEvents();
		// 交换缓冲, 绘制
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}