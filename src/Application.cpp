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

	//����shader��Դ�룬�ڶ�������������
	glShaderSource(shader, 1, &shaderSource, nullptr);
	//����shader
	glCompileShader(shader);

	int success;
	char InfoLog[512];
	//��ȡ����״̬
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//״̬������ȡ������Ϣ
		glGetShaderInfoLog(shader, 512, nullptr, InfoLog);
		std::cout << "ERROR::SHADER::COMPILE SHADER FAILED\n" << InfoLog << std::endl;
	}
	return shader;
}

unsigned int create_program() {
	unsigned int vertexShader = create_shader(vertexShaderSource, SHADER_TYPE::VERTEXSHADER);
	unsigned int fragmentShader = create_shader(fragmentShaderSource, SHADER_TYPE::FRAGMENTSHADER);
	
	//����һ������
	unsigned int shaderProgram = glCreateProgram();
	//����ɫ����ӵ�������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//����
	glLinkProgram(shaderProgram);

	int success = 0;
	char InfoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, InfoLog);
		std::cout << "ERROR::PROGRAM::LINK PROGRAM FAILED\n" << InfoLog << std::endl;
	}

	//ɾ����ɫ��
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
	//��ʼ������
	glfwInit();
	//����OpenGL�����汾3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//����OpenGL�ĸ��汾3,���ʱ�汾����3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//����OpenGl��ģʽΪ����ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfwCreateWindow���ڴ������ڣ�ǰ���������ֱ�Ϊ���ߡ��������ƣ�����һ��ָ������
	GLFWwindow* window = glfwCreateWindow(1640, 1080, "Hello OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Fail to create an OpenGL window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//�����괰�ں���Ҫ���ô�������Ϊ��ǰ������������
	glfwMakeContextCurrent(window);

	//��ʼ��GLAD��GLAD���ڹ���OpenGL�ĺ���ָ�룬��glfwGetProcAddress������˼��ز���ϵͳ����OpenGL����ָ���ַ�ĺ���
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Fail to Initialize GLAD" << std::endl;
		return -1;
	}

	//�����ӿڴ�С������С�ڴ��ڴ�С��ǰ���������ֱ���x��y�����ƫ��ֵ�����������������ӿڵĴ�С
	glViewport(0, 0, 840, 540);
	//��������������ô��ڴ�С�任,��һ�������Ǵ��ڴ�С,�ڶ����ǻص�����ָ��
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//���������Ļ�õ���ɫ
	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);
	
	
	//��������������������
	unsigned int VAO;
	//��������������󲢰󶨵�V������
	glGenVertexArrays(1, &VAO);
	//����OpenGL�������ģ��������õĶ���󶨵���������
	glBindVertexArray(VAO);


	float vertices[] = {
		-0.5f,-0.5f,0.0f,
		-0.5f, 0.5f,0.0f,
		 0.5f,-0.5f,0.0f,
		 0.5f, 0.5f,0.0f
	};

	//�������㻺����������
	unsigned int VBO;
	//�������㻺����󲢰󶨵��ոմ�����������
	glGenBuffers(1, &VBO);
	//�󶨵�������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//��������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//���ö�������ָ�룬��һ�����������Բ���λ�ã��ڶ��������Ե����ݸ��������������������ͣ��������Ƿ��׼�������������֮��Ĳ����������ƫ��
	//�������������ʱ���ִ�н���ǰ��VAO������ù�������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//�������úõ�����
	glEnableVertexAttribArray(0);

	unsigned int indices[] = {
		0,1,2,
		1,2,3
	};
	
	//����Ԫ�ػ����������
	unsigned int EBO;
	//�������󣬰�����
	glGenBuffers(1, &EBO);
	//�󶨵�������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//��������
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

		//���󻺳���ص�ǰ������
		glfwSwapBuffers(window);
		//�����¼�
		glfwPollEvents();
	}

	//��ֹ
	glfwTerminate();
	return 0;
}