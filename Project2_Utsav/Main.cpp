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
unsigned int numNormals;
cy::Point3f *t_normals;

cy::GLSLProgram t_program;
cy::GLSLShader vertShader;
cy::GLSLShader fragShader;

GLuint testLocation;

cy::Matrix4f teapotModelToWorldMatrix;
cy::Matrix4f teapotModelToWorldMatrix2;
cy::Matrix4f teapotWorldToCameraMatrix;
cy::Matrix4f cameraToScreenMatrix;

static bool t_Button1Down = false;
static bool t_Button2Down = false;


void render()
{
	//glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei> (numVerts));
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
	teapot.ComputeNormals();
	t_normals = &(teapot.VN(0));
	numNormals = teapot.NVN();
	GLuint teapotVertArray;
	GLuint vertBufferObj;
	GLuint normBufferObj;
	glGenVertexArrays(1, &teapotVertArray);
	glBindVertexArray(teapotVertArray);

	glGenBuffers(1, &vertBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferObj);

	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(cy::Point3f), t_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &normBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER, normBufferObj);

	glBufferSubData(GL_ARRAY_BUFFER, normBufferObj, numNormals * sizeof(cy::Point3f), t_normals);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cy::Point3f), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(cy::Point3f), 0);

	
	t_program.CreateProgram();
	installShaders();

	//glutIdleFunc(idle);

	cy::Point3f teapotPosition;
	teapotPosition.Set(0.0, 0.0, 5.0);


	teapotModelToWorldMatrix.SetRotationX(-2.0); 
	//teapotModelToWorldMatrix2.SetRotationZ(90.0*(180 / 3.14));
	teapotWorldToCameraMatrix.SetTrans(teapotPosition);
	cameraToScreenMatrix.SetPerspective(1.0, 1.0, 1.0, 200.0);
	teapotWorldToCameraMatrix.Invert();

	cy::Matrix4f mvp = cameraToScreenMatrix * teapotWorldToCameraMatrix * teapotModelToWorldMatrix;// * teapotModelToWorldMatrix2;


	testLocation = glGetUniformLocation(t_program.GetID(), "mvp");
	glUniformMatrix4fv(testLocation, 1, GL_FALSE, mvp.data);

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