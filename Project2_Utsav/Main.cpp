#include <glew.h>
#include <GL\gl.h>
#include <GL\GLU.h>
#include <GL\freeglut.h>
#include <iostream>
#include <cyTriMesh.h>	
#include <cyPoint.h>
#include <cyGL.h>
#include <cyMatrix.h>

cy::Point3f *t_vertices;
unsigned int numVerts;

cy::GLSLProgram t_program;
cy::GLSLShader vertShader;
cy::GLSLShader fragShader;



void render()
{
	//glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_POINTS, 0, static_cast<GLsizei> (numVerts));
	glutSwapBuffers();
}


void keyPressed(unsigned char key, int x, int y)
{
	//ASCII value for escape key
	if (key == 27)
		exit(0);
}


int main(int argc, char* argv[]) {

	cy::TriMesh teapot;
	char fileName[50];
	std::cout << "Enter name of obj file, include .obj in the filename: (Entire absolute system path or relative path as set by IDE)" << std::endl;
	std::cin >> fileName;
	bool hasMat = false;
	char isMat = 'A';
	std::cout << "Does the mesh have material? Y/N?" << std::endl;
	std::cin >> isMat;
	switch (isMat)
	{
	case 'Y': hasMat = true;
		break;
	case 'y': hasMat = true;
		break;
	case 'n': hasMat = false;
		break;
	case 'N': hasMat = false;
		break;
	default: std::cout << "Wrong input" << std::endl;
		break;
	}
	teapot.LoadFromFileObj(fileName, hasMat);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Hello OpenGL World");
	glewInit();

	t_vertices = &(teapot.V(0));
	numVerts = teapot.NV();

	GLuint teapotVertArray;
	GLuint vertBufferObj;
	glGenVertexArrays(1, &teapotVertArray);
	glBindVertexArray(teapotVertArray);

	glGenBuffers(1, &vertBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER,vertBufferObj);

	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(cy::Point3f), t_vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cy::Point3f), 0);

	t_program.CreateProgram();
	vertShader.CompileFile("vertShader.glsl", GL_VERTEX_SHADER);

	fragShader.CompileFile("fragShader.glsl", GL_FRAGMENT_SHADER);

	t_program.AttachShader(vertShader);
	t_program.AttachShader(fragShader);

	t_program.Link();

	t_program.Bind();

	cy::Point3f teapotPosition;
	teapotPosition.Set(0.0, 0.0, 5.0);

	cy::Matrix4f teapotModelToWorldMatrix;
	cy::Matrix4f teapotWorldToCameraMatrix;
	cy::Matrix4f cameraToScreenMatrix;
	teapotModelToWorldMatrix.SetRotationX(2.0);
	//teapotMatrix.SetRotationY(1.0);
	teapotWorldToCameraMatrix.SetTrans(teapotPosition);
	cameraToScreenMatrix.SetPerspective(1.0, 1.0, 1.0, 200.0);
	teapotWorldToCameraMatrix.Invert();

	cy::Matrix4f mvp = cameraToScreenMatrix * teapotWorldToCameraMatrix * teapotModelToWorldMatrix;


	GLuint testLocation = glGetUniformLocation(t_program.GetID(), "mvp");
	glUniformMatrix4fv(testLocation, 1, GL_FALSE, mvp.data);


	glutDisplayFunc(render);

	//comment for no animation
	//glutTimerFunc(2, callBack, 1);


	glutKeyboardFunc(keyPressed);
	glutPostRedisplay();


	glutMainLoop();
	return 0;
}