#include "ResourceLoad.h"

GLFWwindow* window;

int main(void)
{
	// Window 창 생성 코드
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1024, 768, "20161638 - 3D Graphics Final Report", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	int windowWidth = 1024;
	int windowHeight = 768;
	glfwGetFramebufferSize(window, &windowHeight, &windowHeight);

	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// 셰이더 로딩
	GLuint lightProgramID = LoadShaders("basicShading_vs.shader", "basicShading_fs.shader");
	GLuint backgroundProgramID = LoadShaders("background_vs.shader", "background_fs.shader");
	GLuint rttProgramID = LoadShaders("rtt_vs.shader", "rtt_fs.shader");

	// 핸들 획득
	// 물체 렌더링에 필요한 MVP 행렬 핸들
	GLuint MatrixID = glGetUniformLocation(lightProgramID, "MVP");
	GLuint BackMatrixID = glGetUniformLocation(backgroundProgramID, "MVP");
	GLuint RttMatrixID = glGetUniformLocation(rttProgramID, "MVP");

	// 광원 렌더링에 필요한 M, V 행렬 핸들
	GLuint ViewMatrixID = glGetUniformLocation(lightProgramID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(lightProgramID, "M");

	GLuint RttViewMatrixID = glGetUniformLocation(rttProgramID, "V");
	GLuint RttModelMatrixID = glGetUniformLocation(rttProgramID, "M");

	// 광원 좌표 설정에 필요한 핸들
	GLuint LightID = glGetUniformLocation(lightProgramID, "LightPosition_worldspace");

	GLuint RttLightID = glGetUniformLocation(rttProgramID, "LightPosition_worldspace");

	// 시간 설정 핸들
	GLuint RttTimeID = glGetUniformLocation(rttProgramID, "time");

	// 텍스처 설정 핸들
	GLuint TextureID = glGetUniformLocation(lightProgramID, "myTextureSampler");

	GLuint BackTextureID = glGetUniformLocation(backgroundProgramID, "MyTextureSampler");

	GLuint Rtt_TextureID = glGetUniformLocation(rttProgramID, "renderedTexture");

	// 모델 파일로부터 정점 및 UV, Normal 버퍼 생성
	// 손 모델
	vector<glm::vec3> hand_vertices;
	vector<glm::vec2> hand_uvs;
	vector<glm::vec3> hand_normals;
	bool hand_res = LoadOBJFile("hand.obj", hand_vertices, hand_uvs, hand_normals);

	GLuint hand_vertexBuffer;
	glGenBuffers(1, &hand_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, hand_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, hand_vertices.size() * sizeof(glm::vec3), &hand_vertices[0], GL_STATIC_DRAW);

	GLuint hand_uvBuffer;
	glGenBuffers(1, &hand_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, hand_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, hand_uvs.size() * sizeof(glm::vec2), &hand_uvs[0], GL_STATIC_DRAW);

	GLuint hand_normalBuffer;
	glGenBuffers(1, &hand_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, hand_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, hand_normals.size() * sizeof(glm::vec3), &hand_normals[0], GL_STATIC_DRAW);

	// 구체 모델
	vector<glm::vec3> sphere_vertices;
	vector<glm::vec2> sphere_uvs;
	vector<glm::vec3> sphere_normals;
	bool sphere_res = LoadOBJFile("sphere.obj", sphere_vertices, sphere_uvs, sphere_normals);

	GLuint sphere_vertexBuffer;
	glGenBuffers(1, &sphere_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(glm::vec3), &sphere_vertices[0], GL_STATIC_DRAW);

	GLuint sphere_uvBuffer;
	glGenBuffers(1, &sphere_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sphere_uvs.size() * sizeof(glm::vec2), &sphere_uvs[0], GL_STATIC_DRAW);

	GLuint sphere_normalBuffer;
	glGenBuffers(1, &sphere_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, sphere_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sphere_normals.size() * sizeof(glm::vec3), &sphere_normals[0], GL_STATIC_DRAW);

	// 원숭이(suzanne) 모델
	vector<glm::vec3> suzanne_vertices;
	vector<glm::vec2> suzanne_uvs;
	vector<glm::vec3> suzanne_normals;
	bool suzanne_res = LoadOBJFile("suzanne.obj", suzanne_vertices, suzanne_uvs, suzanne_normals);

	GLuint suzanne_vertexBuffer;
	glGenBuffers(1, &suzanne_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, suzanne_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, suzanne_vertices.size() * sizeof(glm::vec3), &suzanne_vertices[0], GL_STATIC_DRAW);

	GLuint suzanne_uvBuffer;
	glGenBuffers(1, &suzanne_uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, suzanne_uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, suzanne_uvs.size() * sizeof(glm::vec2), &suzanne_uvs[0], GL_STATIC_DRAW);

	GLuint suzanne_normalBuffer;
	glGenBuffers(1, &suzanne_normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, suzanne_normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, suzanne_normals.size() * sizeof(glm::vec3), &suzanne_normals[0], GL_STATIC_DRAW);

	// 텍스쳐 로딩
	GLuint hand_Texture = LoadDDS("texture_Skin.DDS");
	GLuint sphere_AL_Texture = LoadDDS("texture_Sun.DDS");
	GLuint sphere_AR_Texture = LoadDDS("texture_Neptune.DDS");
	GLuint sphere_BL_Texture = LoadDDS("texture_Moon.DDS");
	GLuint sphere_BR_Texture = LoadDDS("texture_PurplePlanet.DDS");

	GLuint background_Texture = LoadDDS("texture_MilkyWay.DDS");

	GLuint suzanne_Texture = LoadDDS("uvmap.DDS");

	// MVP 행렬 생성
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0, 5, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);

	// 손 MVP 행렬
	glm::mat4 hand_Model = glm::mat4(1.0);
	hand_Model = glm::translate(hand_Model, vec3(0.5, -3, -1));

	glm::mat4 hand_Scale = glm::mat4(1.0);
	hand_Scale = glm::scale(hand_Scale, vec3(2.0, 2.0, 2.0));

	glm::mat4 hand_MVP = projection * view * hand_Model * hand_Scale;

	// 구체 MVP 행렬
	// 중앙 구체
	glm::mat4 sphere_Mid_Model = glm::mat4(1.0);
	sphere_Mid_Model = glm::translate(sphere_Mid_Model, vec3(0, 2, 0));
	
	glm::mat4 sphere_Mid_MVP = projection * view * sphere_Mid_Model;

	// 작은 구체를 위한 스케일 행렬
	glm::mat4 sphere_Smaller_Scale = glm::mat4(1.0);
	sphere_Smaller_Scale = glm::scale(sphere_Smaller_Scale, vec3(0.5, 0.5, 0.5));

	// 맨 왼쪽 구체
	glm::mat4 sphere_AL_Model = glm::mat4(1.0);
	sphere_AL_Model = glm::translate(sphere_AL_Model, vec3(-4, 2, 0));

	glm::mat4 sphere_AL_MVP = projection * view * sphere_AL_Model * sphere_Smaller_Scale;

	// 왼쪽 구체
	glm::mat4 sphere_AR_Model = glm::mat4(1.0);
	sphere_AR_Model = glm::translate(sphere_AR_Model, vec3(2, 2, 0));

	glm::mat4 sphere_AR_MVP = projection * view * sphere_AR_Model * sphere_Smaller_Scale;

	// 오른쪽 구체
	glm::mat4 sphere_BL_Model = glm::mat4(1.0);
	sphere_BL_Model = glm::translate(sphere_BL_Model, vec3(-2, 2, 0));

	glm::mat4 sphere_BL_MVP = projection * view * sphere_BL_Model * sphere_Smaller_Scale;

	// 맨 오른쪽 구체
	glm::mat4 sphere_BR_Model = glm::mat4(1.0);
	sphere_BR_Model = glm::translate(sphere_BR_Model, vec3(4, 2, 0));

	glm::mat4 sphere_BR_MVP = projection * view * sphere_BR_Model * sphere_Smaller_Scale;

	// 뒷배경용 구체 MVP 행렬
	glm::mat4 background_Model = glm::mat4(1.0);
	glm::mat4 background_Scale = glm::mat4(1.0);
	background_Scale = glm::scale(background_Scale, vec3(20, 20, 20));

	glm::mat4 background_MVP = projection * view * background_Model * background_Scale;

	// 원숭이(suzanne) MVP 행렬
	glm::mat4 suzanne_View = glm::lookAt(
		glm::vec3(0, 0, 4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 suzanne_Model = glm::mat4(1.0);

	glm::mat4 suzanne_MVP = projection * suzanne_View * suzanne_Model;

	// 회전 행렬
	glm::mat4 matRot = glm::mat4(1.0);
	glm::mat4 matRot_Inverse = glm::mat4(1.0);

	glm::mat4 background_Rotation = glm::mat4(1.0);

	// rtt 텍스처 세팅
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	do {
		// RTT 텍스처를 프레임 버퍼에 렌더링함.
		glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		glViewport(0, 0, windowWidth, windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(backgroundProgramID);

		// 원숭이(suzanne) 그리기
		glUniformMatrix4fv(BackMatrixID, 1, GL_FALSE, &suzanne_MVP[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, suzanne_Texture);
		glUniform1i(BackTextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, suzanne_vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, suzanne_uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, suzanne_vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// 화면 출력

		// Render to the screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Render on the whole framebuffer, complete from the lower left corner to the upper right
		glViewport(0, 0, windowWidth, windowHeight);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 행렬 재계산
		matRot = glm::rotate(matRot, glm::radians(1.0f), vec3(0, 1, 0));
		matRot_Inverse = glm::rotate(matRot_Inverse, glm::radians(-1.0f), vec3(0, 1, 0));

		background_Rotation = glm::rotate(background_Rotation, glm::radians(0.01f), vec3(0, 1, 0));

		sphere_AL_MVP = projection * view * matRot * sphere_AL_Model * sphere_Smaller_Scale;
		sphere_AR_MVP = projection * view * matRot_Inverse * sphere_AR_Model * sphere_Smaller_Scale;
		sphere_BL_MVP = projection * view * matRot_Inverse * sphere_BL_Model * sphere_Smaller_Scale;
		sphere_BR_MVP = projection * view * matRot * sphere_BR_Model * sphere_Smaller_Scale;

		background_MVP = projection * view * background_Model * background_Rotation * background_Scale;

		// 셰이더 선택
		glUseProgram(lightProgramID);

		// 광원 좌표 입력
		glm::vec3 lightPos = glm::vec3(0, 0, 3);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// 손 그리기

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &hand_MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &hand_Model[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, hand_Texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, hand_vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, hand_uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, hand_normalBuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, hand_vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// 구체 그리기

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_normalBuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 맨 왼쪽 구체
		// 광원 재계산을 위한 행렬
		glm::mat4 newCalcMat = matRot * sphere_AL_Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sphere_AL_MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &newCalcMat[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sphere_AL_Texture);
		glUniform1i(TextureID, 0);

		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		// 왼쪽 구체
		newCalcMat = matRot_Inverse * sphere_AR_Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sphere_AR_MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &newCalcMat[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sphere_AR_Texture);
		glUniform1i(TextureID, 0);

		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		// 오른쪽 구체
		newCalcMat = matRot_Inverse * sphere_BL_Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sphere_BL_MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &newCalcMat[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sphere_BL_Texture);
		glUniform1i(TextureID, 0);

		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		// 맨 오른쪽 구체
		newCalcMat = matRot * sphere_BR_Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &sphere_BR_MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &newCalcMat[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, sphere_BR_Texture);
		glUniform1i(TextureID, 0);

		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// 뒷배경용 구체 그리기

		// 셰이더 선택
		glUseProgram(backgroundProgramID);

		glUniformMatrix4fv(BackMatrixID, 1, GL_FALSE, &background_MVP[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, background_Texture);
		glUniform1i(BackTextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// 중앙 구체 그리기(RTT 적용)

		// 셰이더 선택
		glUseProgram(rttProgramID);

		glUniformMatrix4fv(RttMatrixID, 1, GL_FALSE, &sphere_Mid_MVP[0][0]);
		glUniformMatrix4fv(RttModelMatrixID, 1, GL_FALSE, &sphere_Mid_Model[0][0]);
		glUniformMatrix4fv(RttViewMatrixID, 1, GL_FALSE, &view[0][0]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		glUniform1i(Rtt_TextureID, 0);

		glUniform1f(RttTimeID, (float)(glfwGetTime() * 10.0f));

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_uvBuffer);
		glVertexAttribPointer(
			1,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, sphere_normalBuffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, sphere_vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// 프로그램 종료 전 자원 해제
	glDeleteBuffers(1, &hand_vertexBuffer);
	glDeleteBuffers(1, &hand_uvBuffer);
	glDeleteBuffers(1, &hand_normalBuffer);

	glDeleteBuffers(1, &sphere_vertexBuffer);
	glDeleteBuffers(1, &sphere_uvBuffer);
	glDeleteBuffers(1, &sphere_normalBuffer);

	glDeleteBuffers(1, &suzanne_vertexBuffer);
	glDeleteBuffers(1, &suzanne_uvBuffer);
	glDeleteBuffers(1, &suzanne_normalBuffer);

	glDeleteTextures(1, &hand_Texture);
	glDeleteTextures(1, &sphere_AL_Texture);
	glDeleteTextures(1, &sphere_AR_Texture);
	glDeleteTextures(1, &sphere_BL_Texture);
	glDeleteTextures(1, &sphere_BR_Texture);
	glDeleteTextures(1, &suzanne_Texture);

	glDeleteProgram(lightProgramID);
	glDeleteProgram(backgroundProgramID);
	glDeleteProgram(rttProgramID);

	glDeleteFramebuffers(1, &FramebufferName);
	glDeleteTextures(1, &renderedTexture);
	glDeleteRenderbuffers(1, &depthrenderbuffer);

	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();

	return 0;
}