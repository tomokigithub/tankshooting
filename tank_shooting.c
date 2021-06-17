#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

#define PI 3.14159265
// 敵数
#define E 16
#define Er  0.8
//弾数
#define S 20
#define Sr 0.4
#define RAD (M_PI / 180.0)

// 自機の位置
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

//カメラの距離
double CameraD = 8;
int far_flag = 0;
int near_flag = 0;

/* ウインドウのサイズ */
int winWidth = 1000;
int winHeight = 600;
// フラグなどの宣言と初期化
int  PressButton;
int  i = 0, m = 0, n = 0, j = 0;
char left_r = 0;
char right_r = 0;
char back = 0;
char advance = 0;
char right_advance = 0;
char left_advance = 0;
char die_flag = 99;
//弾
char shot_flag = 1;
char shotflag[S] = { 0.0 };
double shotx[S] = { 0.0 };
double shoty[S] = { 0.0 };
double shotz[S] = { 0.0 };
double shot_theta[S] = { 0.0 };

//敵
int tim = 0;
char enemy_flag = 0;
char enemyflag[E];
double enemyX[E], enemyY[E], enemyZ[E];
int enemy_life[E];
float speed;

//回復アイテム
double recovX, recovY, recovZ;
double recovR = 0.4;
char recov_flag = 0;

//カメラ
int	jcount = 0;
float CameraAzimuth = 90.0;   /* カメラの位置（方位角） */
float CameraElevation = 90.0;    /* カメラの位置（仰角） */
float CameraDistance = 11;    /* カメラの位置（原点からの距離） */
float CameraX;
float CameraY;
float CameraZ;
float g_theta = -1.5;
float advance_theta=0;

//色
static GLfloat red[] = { 2.0, 0.3, 0.4, 1.0 };
static GLfloat green[] = { 0.2, 0.8, 0.3, 1.0 };
static GLfloat blue[] = { 0.2, 0.2, 2.0, 1.0 };
static GLfloat yellow[] = { 2.0, 2.4, 0.2, 1.0 };
static GLfloat ground[] = { 1.5, 0.7, 0.5, 1.0};
static GLfloat ground2[] = { 1.5, 0.9, 0.5, 1.0};
static GLfloat bullet[] = { 0.3, 0.3, 0.3, 1.0};


//光源
static GLfloat lightpos[] = { 0.0, 40.0, 0.0, 1.0 }; /* 光源の位置 */
static GLfloat lightpos2[] = { 20.0, 5.0, 20.0, 1.0 }; /* 光源の位置 */
static GLfloat lightDiffuse[3] = { 1.0, 1.0, 1.0 }; //拡散光
//画面上の文字
int list;

// 関数宣言
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

//メイン関数
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	
	// 自機位置の初期化
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

// 描画関数(決まってる)
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

//画面表示
void display(void)
{
	/* 初期化 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   /* 画面を消去 */
	glMatrixMode(GL_MODELVIEW);             /* 幾何（描画位置など設定する）モード */
	glLoadIdentity();                       /* 幾何を初期化する */
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);  // 光源の位置
	glLightfv(GL_LIGHT1, GL_POSITION, lightpos2); 
	DRAW_MONITER_1(0, 0, winWidth, winHeight);
	glutSwapBuffers();
}

//メイン画面
void DRAW_MONITER_1(int x, int y, int w, int h)
{
	glViewport(x, y, w, h); //ビューポート変換
	gluLookAt(Cube.ShiftX - CameraD * cos(g_theta), Cube.ShiftY+3, Cube.ShiftZ - CameraD * sin(g_theta),
		Cube.ShiftX + cos(g_theta), Cube.ShiftY, Cube.ShiftZ + sin(g_theta),
		0.0, 1.0, 0.0);

	//文字の描画
	char t_char[20];
	char t_char2[20];
	char t_char3[20];

	//HP表示（画面左上）
	sprintf(t_char, "HP : %d", Cube.hp);
	DISPLAY_TEXT(1, 95, t_char);

	//Point
	sprintf(t_char2, "SCORE : %d", Cube.point);
	DISPLAY_TEXT(1, 90, t_char2);

	// 初期状態(文字の描画)
	if (die_flag == 99){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char3, "Press  's'  to start ");
		DISPLAY_TEXT(37, 65, t_char3);
	}

	//ゲームオーバーの文字
	if (die_flag == 0){
		glColor3f(0.0, 1.0, 1.0);
		strcpy(t_char2, "GAME OVER");
		DISPLAY_TEXT(37, 55, t_char2);
	}

	GLfloat mat[] = {50.0};
	
	// 地面 
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

        //自機
	//胴体の角度を決める
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
	//頭
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
	
	//胴体
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
	
	
	//弾の描画
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


	//敵の描画
	for (i = 0; i < E; i++){
		if (enemyflag[i] == 1){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glTranslatef(enemyX[i], enemyY[i], enemyZ[i]);
			glRotatef((GLdouble)(atan2l((enemyX[i]-Cube.ShiftX),(enemyZ[i]-Cube.ShiftZ))/PI*180 + 90)  ,0, 1, 0);

			//頭
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
			
			//胴体
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
	//回復アイテムの描画
	if (recov_flag == 1){
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
		glTranslatef(recovX, recovY, recovZ);
		glutSolidCube(recovR);
		glPopMatrix();
	}
	


}

//////////////////////////////////////////////////////////////////////////
// 文字描画
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

//タイマー関数
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

		// 通常時
	case	1:

		tim++;

		//カメラ位置を近づける
		if(near_flag == 1){
		  if(CameraD>=2){
		    CameraD -= 0.064;
		     }
		}
		//カメラ位置を遠ざける
		if(far_flag == 1){
		  if(CameraD<=14){
		    CameraD += 0.064;
		      }
		}
		
		// 視点の移動(右回転)
		if (right_r == 1){
			g_theta += 0.032;
		}
		// 視点の移動(左回転)
		if (left_r == 1){
			g_theta -= 0.032;
		}
		// 自機移動(進む)
		if (advance == 1){
			Cube.ShiftX += cos(g_theta) * 0.08;
			Cube.ShiftZ += sin(g_theta) * 0.08;
		}
		// 自機移動(右に進む)
		if (right_advance == 1){
		  Cube.ShiftX += -sin(g_theta) * 0.08;
		  Cube.ShiftZ += cos(g_theta) * 0.08;
		}
		// 自機移動(左に進む)
		if (left_advance == 1){
		  Cube.ShiftX += sin(g_theta) * 0.08;
		  Cube.ShiftZ += -cos(g_theta) * 0.08;
		}
		//自機移動(バック)
		if (back == 1){
			Cube.ShiftX -= cos(g_theta) * 0.08;
			Cube.ShiftZ -= sin(g_theta) * 0.08;
		}

		////画面外に出ないようにする
		
		if (Cube.ShiftX >= 15){ Cube.ShiftX = 15; }
		else if (Cube.ShiftX <= -15){ Cube.ShiftX = -15; }
		if (Cube.ShiftZ >= 15){ Cube.ShiftZ = 15; }
		else if (Cube.ShiftZ <= -15){ Cube.ShiftZ = -15; }
		
		for (i = 0; i < E; i++){
		  //弾と敵のあたり判定
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
		  
		  //敵と自機のあたり判定
		  if (enemyflag[i] == 1){
		    if (sqrt(pow(Cube.ShiftX - enemyX[i], 2.0) + pow(Cube.ShiftZ - enemyZ[i], 2.0)) < (2 * Er)){
		      Cube.hp--;
		      enemyflag[i] = 0;
		    }
		  }
		}
		
		////自機と回復アイテムのあたり判定
		if (recov_flag == 1){
		  if (sqrt(pow(Cube.ShiftX - recovX, 2.0) + pow(Cube.ShiftY - recovY, 2.0) + pow(Cube.ShiftZ - recovZ, 2.0)) < (4 * recovR)){
		    Cube.hp++;
		    recov_flag = 0;
		  }
		  //画面外に出たら消去
		  if ((recovX > 16.0) || (recovX < -16.0) || (recovZ > 16.0) || (recovZ < -16.0)){
		    recov_flag = 0;
		  }
		}
		
		//弾の移動
		for (i = 0; i < S; i++){
		  if (shotflag[i] == 1){
		    shotz[i] += sin(shot_theta[i]) * 0.4;
		    shotx[i] += cos(shot_theta[i]) * 0.4;
		  }
		  if ((shotx[i] > 16.0) || (shotx[i] < -16.0) || (shotz[i] > 16.0) || (shotz[i] < -16.0)){//画面外に出たら消す 
		    shotflag[i] = 0;
		  }
		}
		
		//敵の移動
		speed = ((GLdouble)Cube.point)/5000;
		for (i = 0; i < E; i++){
		  if (enemyflag[i] == 1){
		    //自機に向かってくる
		    if (enemyX[i] - Cube.ShiftX>0){ enemyX[i] -= 0.012 + speed; }
		    if (enemyX[i] - Cube.ShiftX<0){ enemyX[i] += 0.012 + speed; }
		    if (enemyZ[i] - Cube.ShiftZ>0){ enemyZ[i] -= 0.012 + speed; }
		    if (enemyZ[i] - Cube.ShiftZ<0){ enemyZ[i] += 0.012 + speed; }
		    //画面外に出たら消去
		    if ((enemyX[i] > 16.0) || (enemyX[i] < -16.0) || (enemyZ[i] > 16.0) || (enemyZ[i] < -16.0)){
		      enemyflag[i] = 0;
				}
		  }
		}
		
		//敵の追加
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
		
		////回復アイテムの追加
		if ((tim % 5000 == 0)){
		  if (recov_flag == 0){
				recovX = -12 + rand() % 24;
				recovY = 0;
				recovZ = -12 + rand() % 24;
				recov_flag = 1;
		  }
			tim = 0;
		}
		
		//ゲームオーバー判定
		if (Cube.hp <= 0){
			die_flag = 0;
		}
		break;

		// ゲームオーバー時
	case	0:
		break;
	}
	
	glutPostRedisplay();

}



//操作入力
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	  
	  //aで近づく
	case 'a':
	  near_flag = 1;
	  break;
	  //zで離れる
	case 'z':
	  far_flag = 1;
	  break;
	  //sでゲームスタート
	case 's':
	  die_flag = 1;
	  break;
	  //xで左回転
	case 'q':
	  left_r=1;
	  break;
	  //cで右回転
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
	  //右ボタンで自機右回転
	case GLUT_KEY_LEFT:
	  //left_r = 1;
	  left_advance = 1;
	  break;
	  //左ボタンで自機左回転
	case GLUT_KEY_RIGHT:
	  //right_r = 1;
	  right_advance = 1;
	  break;
	  //上ボタンで前
	case GLUT_KEY_UP:
	  advance = 1;
	  break;
	  //下ボタンでバック
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

//各関数の呼び出し
void myInit(char *windowTitle)
{

	/* ウインドウの縦横の比を計算 */
	float aspect = (float)winWidth / (float)winHeight;

	/* OpenGLウインドウ作成までの初期化 */
	glutInitWindowPosition(0, 0);  /* ウインドウ表示位置 */
	glutInitWindowSize(winWidth, winHeight); /* ウインドウサイズ */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);/* 描画モード */
	glutCreateWindow(windowTitle); /* ウインドウの表示 */
	glClearColor(0.5, 0.7, 2.5, 1.0);  /* 画面消去色の設定 */

	/* イベント発生時に呼び出す関数の登録 */
	glutKeyboardFunc(keyboard); /* キーボードを押した時 */
	glutKeyboardUpFunc(keyboardup);
	glutMouseFunc(mouse);
	glutSpecialFunc(specialkeyboard);//特殊なキーボードを押したとき
	glutSpecialUpFunc(specialkeyboardup);//キーボードを離したとき
	
	glutDisplayFunc(display); /* 画面表示 */

	/* CG描画設定 */
	glMatrixMode(GL_PROJECTION);/* 透視投影(遠近投影法)設定モードに切り替え */
	glLoadIdentity(); /* 透視投影行列を初期化 */
	gluPerspective(90.0, aspect, 1.0, 100.0);/* 透視投影行列の設定 */

	glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(200, timer_shot, 0);/*自機の弾丸を200ミリ秒ごとに処理*/
	glutTimerFunc(3, timer, 0); /* タイマーを3ミリ秒後に設定 */
	
	/* 視野角45度, 縦横比 aspect，描画前面までの奥行 1.0，描画背面までの奥行 20.0 */
	glEnable(GL_DEPTH_TEST);        /* 隠面消去を有効にする */
	glEnable(GL_LIGHT0);            /* 0番の照明を使う*/
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);         /* 照明計算の法線を正規化 */
	glEnable(GL_LIGHTING);          /* 照明を有効にする  */
}
