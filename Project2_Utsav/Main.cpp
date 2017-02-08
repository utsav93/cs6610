#include <glew.h>
#include <GL\gl.h>
#include <GL\GLU.h>
#include <GL\freeglut.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <cyGL.h>
#include <ShapeGenerator.h>
#include <Camera.h>

using namespace glm;



cy::GLSLProgram t_program;
cy::GLSLShader vertShader;
cy::GLSLShader fragShader;

GLuint teapotNumIndices;
GLuint teapotIndexByteOffset;

glm::vec3 diffuseLightPosition(0.0f, 0.5f, 0.0f);
GLfloat lightPositionWorldChange = 0.2f;

GLfloat teapotRotation = 60.0f;
vec3 teapotTranslation(3.0f, 0.0f, 0.0f);

mat4 teapotModelToWorldMatrix;
mat4 teapotModelToWorldMatrix2;
mat4 teapotWorldToCameraMatrix;
mat4 cameraToScreenMatrix;
mat4 modelToProjectionMatrix;

GLuint modelToProjectionUniformLocation;
GLint lightPositionWorldUniformLocation;
GLint ambientLightUniformLocation;
GLint modelToWorldMatrixUniformLocation;
GLint cameraPositionUniformLocation;

Camera camera;

static bool t_Button1Down = false;
static bool t_Button2Down = false;

mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)1200) / 800, 0.1f, 20.0f);
mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;


void render()
{
	//glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei> (numVerts));
	glDrawElements(GL_TRIANGLES, teapotNumIndices, GL_UNSIGNED_SHORT, (void*)teapotIndexByteOffset);
	glutSwapBuffers();
}


void keyPressed(unsigned char key, int x, int y)
{
	//ASCII value for escape key
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}

void installShaders()
{
	vertShader.CompileFile("vertShader.glsl", GL_VERTEX_SHADER);
	fragShader.CompileFile("fragShader.glsl", GL_FRAGMENT_SHADER);
	t_program.AttachShader(vertShader);
	t_program.AttachShader(fragShader);
	t_program.Link();
	t_program.Bind();

}

void keyPressedSpecial(int key, int x, int y)
{
	if (key == GLUT_KEY_F6)
	{
		t_program.CreateProgram();
		installShaders();
		glutPostRedisplay();
	}
}

void mouseEvents(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		t_Button1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
		//gluLookAt(0.0, 0.0, 0.0, 5.0, 5.0, 5.0, 0.0, 1.0, 0.0);
	}
}



int main(int argc, char* argv[]) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Hello OpenGL World");
	glewInit();

	GLuint teapotVertArray;
	GLuint vertBufferObj;
	glGenVertexArrays(1, &teapotVertArray);
	glBindVertexArray(teapotVertArray);

	ShapeData teapot = ShapeGenerator::makeTeapot();
	teapotNumIndices = teapot.numIndices;
	teapotIndexByteOffset += teapot.vertexBufferSize();

	glGenBuffers(1, &vertBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferObj);

	glBufferData(GL_ARRAY_BUFFER, teapot.vertexBufferSize() + teapot.indexBufferSize() , 0, GL_STATIC_DRAW);

	GLsizeiptr currentOffset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.vertexBufferSize(), teapot.vertices);
	currentOffset += teapot.vertexBufferSize();
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, teapot.indexBufferSize(), teapot.indices);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)sizeof(vec3));


	t_program.CreateProgram();
	installShaders();


	worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	viewToProjectionMatrix = glm::perspective(60.0f, ((float)1200) / 800, 0.1f, 200.0f);
	worldToViewMatrix = camera.getWorldToViewMatrix();

	modelToWorldMatrixUniformLocation = glGetUniformLocation(t_program.GetID(), "modelToWorldMatrix");
	modelToProjectionUniformLocation = glGetUniformLocation(t_program.GetID(), "modelToProjectionMatrix");
	lightPositionWorldUniformLocation = glGetUniformLocation(t_program.GetID(), "lightPositionWorld");
	ambientLightUniformLocation = glGetUniformLocation(t_program.GetID(), "ambientLight");
	cameraPositionUniformLocation = glGetUniformLocation(t_program.GetID(), "cameraPositionWorld");

	glm::mat4 teapotModelToWorldMatrix = glm::translate(teapotTranslation) * glm::rotate(teapotRotation, 0.0f, 1.0f, 0.0f) * glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f) * glm::scale(0.5f, 0.5f, 0.5f);
	modelToProjectionMatrix = worldToProjectionMatrix * teapotModelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &teapotModelToWorldMatrix[0][0]);

	glm::vec3 ambientLight(0.2f, 0.2f, 0.2f);
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);
	lightPositionWorldUniformLocation = glGetUniformLocation(t_program.GetID(), "lightPositionWorld");
	glm::vec3 lightPositionWorld(diffuseLightPosition);
	glUniform3fv(lightPositionWorldUniformLocation, 1, &lightPositionWorld[0]);

	


	//glutIdleFunc(idle);


	//gluLookAt(0.0, 0.0, 0.0, 5.0, 5.0, 5.0, 0.0, 1.0, 0.0);
	glutDisplayFunc(render);

	//comment for no animation
	//glutTimerFunc(2, callBack, 1);
	glutMouseFunc(mouseEvents);

	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(keyPressedSpecial);
	
	glutPostRedisplay();


	glutMainLoop();
	return 0;
}