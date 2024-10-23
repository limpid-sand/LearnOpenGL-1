#include <iostream>

#include <glad\glad.h>
#include <GLFW\glfw3.h>


const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(position.x,position.y,position.z,1.0f);\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 fragColor;\n"
"void main()\n"
"{\n"
"	fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";


enum class SHADER_TYPE {
	VERTEXSHADER = 0, FRAGMENTSHADER
};


unsigned int create_shader(const char* shaderSource, SHADER_TYPE shaderType) {
	unsigned int shader = 0;
	if (shaderType == SHADER_TYPE::VERTEXSHADER) shader = glCreateShader(GL_VERTEX_SHADER);
	else if (shaderType == SHADER_TYPE::FRAGMENTSHADER) shader = glCreateShader(GL_FRAGMENT_SHADER);

	//配置shader的源码，第二个参数是数量
	glShaderSource(shader, 1, &shaderSource, nullptr);
	//编译shader
	glCompileShader(shader);

	int success;
	char InfoLog[512];
	//获取编译状态
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//状态出错，获取出错信息
		glGetShaderInfoLog(shader, 512, nullptr, InfoLog);
		std::cout << "ERROR::SHADER::COMPILE SHADER FAILED\n" << InfoLog << std::endl;
	}
	return shader;
}

unsigned int create_program() {
	unsigned int vertexShader = create_shader(vertexShaderSource, SHADER_TYPE::VERTEXSHADER);
	unsigned int fragmentShader = create_shader(fragmentShaderSource, SHADER_TYPE::FRAGMENTSHADER);
	
	//创建一个程序
	unsigned int shaderProgram = glCreateProgram();
	//将着色器添加到程序里
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//链接
	glLinkProgram(shaderProgram);

	int success = 0;
	char InfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, InfoLog);
		std::cout << "ERROR::PROGRAM::LINK PROGRAM FAILED\n" << InfoLog << std::endl;
	}

	//删除着色器
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	std::cout << "callback framebuffer_size  change function" << std::endl;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	//初始化窗口
	glfwInit();
	//设置OpenGL的主版本3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//设置OpenGL的副版本3,因此时版本号是3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置OpenGl的模式为核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfwCreateWindow用于创建窗口，前三个参数分别为宽、高、窗口名称，返回一个指针类型
	GLFWwindow* window = glfwCreateWindow(1640, 1080, "Hello OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Fail to create an OpenGL window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//创建完窗口后，需要将该窗口设置为当前环境的上下文
	glfwMakeContextCurrent(window);

	//初始化GLAD，GLAD用于管理OpenGL的函数指针，而glfwGetProcAddress则给出了加载操作系统级的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Fail to Initialize GLAD" << std::endl;
		return -1;
	}

	//设置视口大小，可以小于窗口大小，前两个参数分别是x，y方向的偏移值，后面两个参数是视口的大小
	glViewport(0, 0, 840, 540);
	//这个函数用于设置窗口大小变换,第一个参数是窗口大小,第二个是回调函数指针
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//设置清空屏幕用的颜色
	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);
	
	
	//创建顶点数组对象的索引
	unsigned int VAO;
	//创建顶点数组对象并绑定到V索引上
	glGenVertexArrays(1, &VAO);
	//设置OpenGL的上下文，将创建好的对象绑定到上下文中
	glBindVertexArray(VAO);


	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		-0.5f, 0.5f,0.0f,
		 0.5f,-0.5f,0.0f,
		 0.5f, 0.5f,0.0f
	};

	//创建顶点缓冲对象的索引
	unsigned int VBO;
	//创建顶点缓冲对象并绑定到刚刚创建的索引上
	glGenBuffers(1, &VBO);
	//绑定到上下文
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//填入数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//配置顶点属性指针，第一个参数是属性布局位置，第二个是属性的数据个数，第三个是数据类型，第三个是否标准化，第五个属性之间的步长，最后是偏移
	//调用这个函数的时候会执行将当前的VAO与该配置关联起来
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//启用配置好的属性
	glEnableVertexAttribArray(0);

	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};
	
	//创建元素缓冲对象索引
	unsigned int EBO;
	//创建对象，绑定索引
	glGenBuffers(1, &EBO);
	//绑定到上下文
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//填入数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	unsigned int program = create_program();
	glUseProgram(program);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindVertexArray(VAO);

		glUseProgram(program);
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//将后缓冲加载到前缓冲中
		glfwSwapBuffers(window);
		//监听事件
		glfwPollEvents();
	}

	//终止
	glfwTerminate();
	return 0;
}