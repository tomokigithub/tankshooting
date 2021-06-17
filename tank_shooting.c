#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define PI 3.14159265
// �G��
#define E 16
#define Er  0.8
//�e��
#define S 20
#define Sr 0.4
#define RAD (M_PI / 180.0)

// ���@�̈ʒu
typedef struct {
	float ShiftX;
	float ShiftY;
	float ShiftZ;
	float RotateX;
	float RotateY;
	float RotateZ;
	int hp;
	int point;
} Geometry;
Geometry Cube;

//�J�����̋���
double CameraD = 8;
int far_flag = 0;
int near_flag = 0;

/* �E�C���h�E�̃T�C�Y */
int winWidth = 1000;
int winHeight = 600;
// �t���O�Ȃǂ̐錾�Ə�����
int  PressButton;
int  i = 0, m = 0, n = 0, j = 0;
char left_r = 0;
char right_r = 0;
char back = 0;
char advance = 0;
char right_advance = 0;
char left_advance = 0;
char die_flag = 99;
//�e
char shot_flag = 1;
char shotflag[S] = { 0.0 };
double shotx[S] = { 0.0 };
double shoty[S] = { 0.0 };
double shotz[S] = { 0.0 };
double shot_theta[S] = { 0.0 };

//�G
int tim = 0;
char enemy_flag = 0;
char enemyflag[E];
double enemyX[E], enemyY[E], enemyZ[E];
int enemy_life[E];
float speed;

//�񕜃A�C�e��
double recovX, recovY, recovZ;
double recovR = 0.4;
char recov_flag = 0;

//�J����
int	jcount = 0;
float CameraAzimuth = 90.0;   /* �J�����̈ʒu�i���ʊp�j */
float CameraElevation = 90.0;    /* �J�����̈ʒu�i�p�j */
float CameraDistance = 11;    /* �J�����̈ʒu�i���_����̋����j */
float CameraX;
float CameraY;
float CameraZ;
float g_theta = -1.5;
float advance_theta=0;

//�F
static GLfloat red[] = { 2.0, 0.3, 0.4, 1.0 };
static GLfloat green[] = { 0.2, 0.8, 0.3, 1.0 };
static GLfloat blue[] = { 0.2, 0.2, 2.0, 1.0 };
static GLfloat yellow[] = { 2.0, 2.4, 0.2, 1.0 };
static GLfloat ground[] = { 1.5, 0.7, 0.5, 1.0};
static GLfloat ground2[] = { 1.5, 0.9, 0.5, 1.0};
static GLfloat bullet[] = { 0.3, 0.3, 0.3, 1.0};


//����
static GLfloat lightpos[] = { 0.0, 40.0, 0.0, 1.0 }; /* �����̈ʒu */
static GLfloat lightpos2[] = { 20.0, 5.0, 20.0, 1.0 }; /* �����̈ʒu */
static GLfloat lightDiffuse[3] = { 1.0, 1.0, 1.0 }; //�g�U��
//��ʏ�̕���
int list;

// �֐��錾
void display(void);
void geometrySet(Geometry geom);
void keyboard(unsigned char key, int x, int y);
void keyboardup(unsigned char key, int x, int y);
void specialkeyboard(int key, int x, int y);
void specialkeyboardup(int key, int x,int y);
void myInit(char *windowTitle);
void *font = GLUT_BITMAP_TIMES_ROMAN_24;
void DRAW_STRING(int x, int y, char *string, void *font);
void DISPLAY_TEXT(int x, int y, char *string);
void DRAW_MONITER_1(int x, int y, int w, int h);
void DRAW_MONITER_2(int x, int y, int w, int h);

//���C���֐�
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	
	// ���@�ʒu�̏�����
	Cube.ShiftX = 0;
	Cube.ShiftY = 0;
	Cube.ShiftZ = 0;
	Cube.RotateX = 0;
	Cube.RotateY = 0;
	Cube.RotateZ = 0;
	Cube.hp = 5;
	Cube.point = 0;
	
	glutMainLoop();

	return(0);
}

// �`��֐�(���܂��Ă�)
void drawString3D(const char *str, float charSize, float lineWidth)
{
	glPushMatrix();
	glPushAttrib(GL_LINE_BIT);
	glScaled(0.001 * charSize, 0.001 * charSize, 0.01);
	glLineWidth(lineWidth);
	while (*str){
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
		++str;
	}
	glPopAttrib();
	glPopMatrix();
}

//��ʕ\��
void display(void)
{
	/* ������ */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   /* ��ʂ����� */
	glMatrixMode(GL_MODELVIEW);             /* �􉽁i�`��ʒu�Ȃǐݒ肷��j���[�h */
	glLoadIdentity();                       /* �􉽂����������� */
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);  // �����̈ʒu
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2); 
	DRAW_MONITER_1(0, 0, winWidth, winHeight);
	glutSwapBuffers();
}

//���C�����
void DRAW_MONITER_1(int x, int y, int w, int h)
{
	glViewport(x, y, w, h); //�r���[�|�[�g�ϊ�
	gluLookAt(Cube.ShiftX - CameraD * cos(g_theta), Cube.ShiftY+3, Cube.ShiftZ - CameraD * sin(g_theta),
		Cube.ShiftX + cos(g_theta), Cube.ShiftY, Cube.ShiftZ + sin(g_theta),
		0.0, 1.0, 0.0);

	//�����̕`��
	char t_char[20];
	char t_char2[20];
	char t_char3[20];

	//HP�\���i��ʍ���j
	sprintf(t_char, "HP : %d", Cube.hp);
	DISPLAY_TEXT(1, 95, t_char);

	//Point
	sprintf(t_char2, "SCORE : %d", Cube.point);
	DISPLAY_TEXT(1, 90, t_char2);

	// �������(�����̕`��)
	if (die_flag == 99){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char3, "Press  's'  to start ");
		DISPLAY_TEXT(37, 65, t_char3);
	}

	//�Q�[���I�[�o�[�̕���
	if (die_flag == 0){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char2, "GAME OVER");
		DISPLAY_TEXT(37, 55, t_char2);
	}

	GLfloat mat[] = {50.0};
	
	// �n�� 
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ground);
	glColor3d(0.0,1.0,0.0);
	glVertex3d(-16, 0, -16);
	glVertex3d(16, 0, -16);
	glVertex3d(16, 0, 16);
	glVertex3d(-16, 0, 16);     
	glEnd();
	
	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ground2);
	glColor3d(0.0,1.0,0.0);
	glVertex3d(0, -0.1, -50);
	glVertex3d(-35.5, -0.1, -35.5);
	glVertex3d(-50, -0.1, 0);
	glVertex3d(-35.5, -0.1, 35.5);
	glVertex3d(0, -0.1, 50);
	glVertex3d(35.5, -0.1, 35.5);
	glVertex3d(50, -0.1, 0);
	glVertex3d(35.5, -0.1, -35.5);
	glEnd();

	glBegin(GL_POLYGON);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glColor3d(0.0,1.0,0.0);
	glVertex3d(-100, -0.2, -100);
	glVertex3d(100, -0.2, -100);
	glVertex3d(100, -0.2, 100);
	glVertex3d(-100, -0.2, 100);
	glEnd();

        //���@
	//���̂̊p�x�����߂�
	if(right_advance == 1 && advance == 1 ||
		left_advance == 1 && back == 1){
	  advance_theta = PI/4;
	}else if(left_advance == 1 && advance == 1 ||
		 right_advance == 1 && back == 1){
	  advance_theta = -PI/4;
	}else if(right_advance == 1){
	  advance_theta = PI/2;
	}else if(left_advance == 1){
	  advance_theta = -PI/2;
	}else{
	  advance_theta = 0;
	}
	
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glTranslatef(Cube.ShiftX, Cube.ShiftY, Cube.ShiftZ);
	glScalef(0.7,0.7,0.7);   
	glRotatef( -(GLdouble)g_theta/PI*180  ,0, 1, 0);
	//��
	glPushMatrix();
	glTranslatef(0,0.5,0);
	glScalef(1.0,1.0,1.0);
	glutSolidSphere(0.5,4,5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9,0.5,0);
	glScalef(1.4,0.4,0.4);
	glutSolidCube(1.0);
	glPopMatrix();
	
	//����
	glPushMatrix();	
	glRotatef( -(GLdouble)advance_theta/PI*180  ,0, 1, 0);
	glPushMatrix();
	glTranslatef(0,0.1,0);
	glScalef(1.5,0.5,1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, bullet);
	
	glPushMatrix();
	glTranslatef(0,0,0.6);
	glScalef(1.5,0.55,0.55);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0,-0.6);
	glScalef(1.5,0.55,0.55);
	glutSolidCube(1.0);
	glPopMatrix();		
	glPopMatrix();
	glPopMatrix();
	
	
	//�e�̕`��
	for (i = 0; i < S; i++){
		if (shotflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, bullet);
			glTranslatef(shotx[i], shoty[i], shotz[i]);
			glRotatef( -(GLdouble)shot_theta[i]/PI*180  ,0, 1, 0);
			glutSolidSphere(Sr, 4, 5);
			glPopMatrix();
		}
	}


	//�G�̕`��
	for (i = 0; i < E; i++){
		if (enemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glTranslatef(enemyX[i], enemyY[i], enemyZ[i]);
			glRotatef((GLdouble)(atan2l((enemyX[i]-Cube.ShiftX),(enemyZ[i]-Cube.ShiftZ))/PI*180 + 90)  ,0, 1, 0);

			//��
			glPushMatrix();
			glTranslatef(0,0.35,0);
			glScalef(1.5,1.2,1.2);
			glutSolidSphere(0.5,6,6);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0.9,0.5,0);
			glScalef(1.4,0.2,0.2);
			glutSolidCube(1.0);
			glPopMatrix();
			
			//����
			glPushMatrix();
			glTranslatef(0,0.1,0);
			glScalef(1.5,0.5,1.2);
			glutSolidCube(1.0);
			glPopMatrix();

			glMaterialfv(GL_FRONT, GL_DIFFUSE, bullet);

			glPushMatrix();
			glTranslatef(0,0,0.7);
			glScalef(1.6,0.55,0.45);
			glutSolidCube(1.0);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(0,0,-0.7);
			glScalef(1.4,0.55,0.45);
			glutSolidCube(1.0);
			glPopMatrix();
			
			glPopMatrix();
		}
	}
	//�񕜃A�C�e���̕`��
	if (recov_flag == 1){
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		glTranslatef(recovX, recovY, recovZ);
		glutSolidCube(recovR);
		glPopMatrix();
	}
	


}

//////////////////////////////////////////////////////////////////////////
// �����`��
//////////////////////////////////////////////////////////////////////////
void DISPLAY_TEXT(int x, int y, char *string){
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	glPushAttrib(GL_ENABLE_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 100, 0, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0.0, 0.0, 0.0);
	glCallList(list);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	DRAW_STRING(x, y, string, GLUT_BITMAP_TIMES_ROMAN_24);
	glEndList();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void DRAW_STRING(int x, int y, char *string, void *font){
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++){
		glutBitmapCharacter(font, string[i]);
	}
}

//�^�C�}�[�֐�
void timer_shot(int value)
{
  int bullet = 1;
  for (i = 0; i < S; i++){
    if (die_flag == 1 && bullet == 1){
      if (shotflag[i] == 0){

	shotx[i] = Cube.ShiftX + cos(g_theta) * 3;   
	shoty[i] = 0; 
	shotz[i] = Cube.ShiftZ + sin(g_theta) * 3;
	shotflag[i] = 1;
	bullet = 0;
	shot_theta[i] = g_theta;
      }                                                                  
    }
  }
  glutTimerFunc(200, timer_shot, 0);
}

void timer(int value)
{
	glutTimerFunc(3, timer, 0);
	switch (die_flag)	{

		// �ʏ펞
	case	1:

		tim++;

		//�J�����ʒu���߂Â���
		if(near_flag == 1){
		  if(CameraD>=2){
		    CameraD -= 0.064;
		     }
		}
		//�J�����ʒu����������
		if(far_flag == 1){
		  if(CameraD<=14){
		    CameraD += 0.064;
		      }
		}
		
		// ���_�̈ړ�(�E��])
		if (right_r == 1){
			g_theta += 0.032;
		}
		// ���_�̈ړ�(����])
		if (left_r == 1){
			g_theta -= 0.032;
		}
		// ���@�ړ�(�i��)
		if (advance == 1){
			Cube.ShiftX += cos(g_theta) * 0.08;
			Cube.ShiftZ += sin(g_theta) * 0.08;
		}
		// ���@�ړ�(�E�ɐi��)
		if (right_advance == 1){
		  Cube.ShiftX += -sin(g_theta) * 0.08;
		  Cube.ShiftZ += cos(g_theta) * 0.08;
		}
		// ���@�ړ�(���ɐi��)
		if (left_advance == 1){
		  Cube.ShiftX += sin(g_theta) * 0.08;
		  Cube.ShiftZ += -cos(g_theta) * 0.08;
		}
		//���@�ړ�(�o�b�N)
		if (back == 1){
			Cube.ShiftX -= cos(g_theta) * 0.08;
			Cube.ShiftZ -= sin(g_theta) * 0.08;
		}

		////��ʊO�ɏo�Ȃ��悤�ɂ���
		
		if (Cube.ShiftX >= 15){ Cube.ShiftX = 15; }
		else if (Cube.ShiftX <= -15){ Cube.ShiftX = -15; }
		if (Cube.ShiftZ >= 15){ Cube.ShiftZ = 15; }
		else if (Cube.ShiftZ <= -15){ Cube.ShiftZ = -15; }
		
		for (i = 0; i < E; i++){
		  //�e�ƓG�̂����蔻��
		  for (m = 0; m < S; m++){
		    if (shotflag[m] == 1 && enemyflag[i] == 1){
		      if (sqrt(pow(shotx[m] - enemyX[i], 2.0) + pow(shotz[m] - enemyZ[i], 2.0)) < (Sr + Er)){
			enemy_life[i]--;
			shotflag[m] = 0;
			if(enemy_life[i] <= 0){
			  enemyflag[i] = 0;
			  Cube.point++;
			}
		      }
		    }
		  }
		  
		  //�G�Ǝ��@�̂����蔻��
		  if (enemyflag[i] == 1){
		    if (sqrt(pow(Cube.ShiftX - enemyX[i], 2.0) + pow(Cube.ShiftZ - enemyZ[i], 2.0)) < (2 * Er)){
		      Cube.hp--;
		      enemyflag[i] = 0;
		    }
		  }
		}
		
		////���@�Ɖ񕜃A�C�e���̂����蔻��
		if (recov_flag == 1){
		  if (sqrt(pow(Cube.ShiftX - recovX, 2.0) + pow(Cube.ShiftY - recovY, 2.0) + pow(Cube.ShiftZ - recovZ, 2.0)) < (4 * recovR)){
		    Cube.hp++;
		    recov_flag = 0;
		  }
		  //��ʊO�ɏo�������
		  if ((recovX > 16.0) || (recovX < -16.0) || (recovZ > 16.0) || (recovZ < -16.0)){
		    recov_flag = 0;
		  }
		}
		
		//�e�̈ړ�
		for (i = 0; i < S; i++){
		  if (shotflag[i] == 1){
		    shotz[i] += sin(shot_theta[i]) * 0.4;
		    shotx[i] += cos(shot_theta[i]) * 0.4;
		  }
		  if ((shotx[i] > 16.0) || (shotx[i] < -16.0) || (shotz[i] > 16.0) || (shotz[i] < -16.0)){//��ʊO�ɏo������� 
		    shotflag[i] = 0;
		  }
		}
		
		//�G�̈ړ�
		speed = ((GLdouble)Cube.point)/5000;
		for (i = 0; i < E; i++){
		  if (enemyflag[i] == 1){
		    //���@�Ɍ������Ă���
		    if (enemyX[i] - Cube.ShiftX>0){ enemyX[i] -= 0.012 + speed; }
		    if (enemyX[i] - Cube.ShiftX<0){ enemyX[i] += 0.012 + speed; }
		    if (enemyZ[i] - Cube.ShiftZ>0){ enemyZ[i] -= 0.012 + speed; }
		    if (enemyZ[i] - Cube.ShiftZ<0){ enemyZ[i] += 0.012 + speed; }
		    //��ʊO�ɏo�������
		    if ((enemyX[i] > 16.0) || (enemyX[i] < -16.0) || (enemyZ[i] > 16.0) || (enemyZ[i] < -16.0)){
		      enemyflag[i] = 0;
				}
		  }
		}
		
		//�G�̒ǉ�
		if (tim % 50 == 0){
		  i = 0;
		  while (i < E){
		    if (enemyflag[i] == 0){
		      enemy_life[i] = 2;
		      if(rand()%2==1){
			enemyX[i] = -15.5 + (31 * (rand() % 2));
			enemyY[i] = 0.0;
			enemyZ[i] = -8 + (16 * (rand() % 2));
			enemyflag[i] = 1;
			break;
		      }else{
			enemyX[i] = -8 + (16 * (rand() % 2));
			enemyY[i] = 0.0;
			enemyZ[i] = -15.5 + (31 * (rand() % 2));
			enemyflag[i] = 1;
			break;
		      }
		    }
		    i++;
		  }
		}
		
		////�񕜃A�C�e���̒ǉ�
		if ((tim % 5000 == 0)){
		  if (recov_flag == 0){
				recovX = -12 + rand() % 24;
				recovY = 0;
				recovZ = -12 + rand() % 24;
				recov_flag = 1;
		  }
			tim = 0;
		}
		
		//�Q�[���I�[�o�[����
		if (Cube.hp <= 0){
			die_flag = 0;
		}
		break;

		// �Q�[���I�[�o�[��
	case	0:
		break;
	}
	
	glutPostRedisplay();

}



//�������
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  
	  //a�ŋ߂Â�
	case 'a':
	  near_flag = 1;
	  break;
	  //z�ŗ����
	case 'z':
	  far_flag = 1;
	  break;
	  //s�ŃQ�[���X�^�[�g
	case 's':
	  die_flag = 1;
	  break;
	  //x�ō���]
	case 'q':
	  left_r=1;
	  break;
	  //c�ŉE��]
	case 'w':
	  right_r=1;
	  break;
	}

	glutPostRedisplay();

	x = y = 0;
}

void keyboardup(unsigned char key, int x, int y)
{
  switch (key) {
  case 'a':
    near_flag = 0;
    break;
  case 'z':
    far_flag = 0;
    break;
  case 'q':
    left_r=0;
    break;
  case 'w':
    right_r=0;
    break;
  }

  glutPostRedisplay();

  x = y = 0;
}

void specialkeyboard(int key, int x, int y)
{
	switch (key) {
	  //�E�{�^���Ŏ��@�E��]
	case GLUT_KEY_LEFT:
	  //left_r = 1;
	  left_advance = 1;
	  break;
	  //���{�^���Ŏ��@����]
	case GLUT_KEY_RIGHT:
	  //right_r = 1;
	  right_advance = 1;
	  break;
	  //��{�^���őO
	case GLUT_KEY_UP:
	  advance = 1;
	  break;
	  //���{�^���Ńo�b�N
	case GLUT_KEY_DOWN:
	  back = 1;
	  break;
	}
	
	
	glutPostRedisplay();
}

void specialkeyboardup(int key, int x, int y){
	switch (key) {
	  
	case GLUT_KEY_LEFT:
		 left_advance = 0;
		break;

	case GLUT_KEY_RIGHT:
		right_advance = 0;
		break;

	case GLUT_KEY_UP:
		advance = 0;
		break;

	case GLUT_KEY_DOWN:
		back = 0;
		break;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
  switch(button){
  case GLUT_LEFT_BUTTON:
    g_theta -= 0.2;
    break;
  case GLUT_RIGHT_BUTTON:
    g_theta += 0.2;
    break;
  }
}

//�e�֐��̌Ăяo��
void myInit(char *windowTitle)
{

	/* �E�C���h�E�̏c���̔���v�Z */
	float aspect = (float)winWidth / (float)winHeight;

	/* OpenGL�E�C���h�E�쐬�܂ł̏����� */
	glutInitWindowPosition(0, 0);  /* �E�C���h�E�\���ʒu */
	glutInitWindowSize(winWidth, winHeight); /* �E�C���h�E�T�C�Y */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);/* �`�惂�[�h */
	glutCreateWindow(windowTitle); /* �E�C���h�E�̕\�� */
	glClearColor(0.5, 0.7, 2.5, 1.0);  /* ��ʏ����F�̐ݒ� */

	/* �C�x���g�������ɌĂяo���֐��̓o�^ */
	glutKeyboardFunc(keyboard); /* �L�[�{�[�h���������� */
	glutKeyboardUpFunc(keyboardup);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialkeyboard);//����ȃL�[�{�[�h���������Ƃ�
	glutSpecialUpFunc(specialkeyboardup);//�L�[�{�[�h�𗣂����Ƃ�
	
	glutDisplayFunc(display); /* ��ʕ\�� */

	/* CG�`��ݒ� */
	glMatrixMode(GL_PROJECTION);/* �������e(���ߓ��e�@)�ݒ胂�[�h�ɐ؂�ւ� */
	glLoadIdentity(); /* �������e�s��������� */
	gluPerspective(90.0, aspect, 1.0, 100.0);/* �������e�s��̐ݒ� */

	glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(200, timer_shot, 0);/*���@�̒e�ۂ�200�~���b���Ƃɏ���*/
	glutTimerFunc(3, timer, 0); /* �^�C�}�[��3�~���b��ɐݒ� */
	
	/* ����p45�x, �c���� aspect�C�`��O�ʂ܂ł̉��s 1.0�C�`��w�ʂ܂ł̉��s 20.0 */
	glEnable(GL_DEPTH_TEST);        /* �B�ʏ�����L���ɂ��� */
	glEnable(GL_LIGHT0);            /* 0�Ԃ̏Ɩ����g��*/
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);         /* �Ɩ��v�Z�̖@���𐳋K�� */
	glEnable(GL_LIGHTING);          /* �Ɩ���L���ɂ���  */
}
