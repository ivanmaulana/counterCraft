#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include "tga.h"
#include "tga.c"
#include "glm.cpp"

using namespace std;

// ukuran layar
float width = 1200, height = 620;

float x0[100]={-16.0, -28.0, 8.0, -5.0, -10.0, 2.0, -5.0, -10.0, 2.0, -15.0, 10.0 ,-15.0, 10.0, -9.0, -19};
float x1[100]={-14.0, -23.0, 15.0, -3.5, -7.0, 5.0, -3.5, -7.0, 5.0, -10.0, 15.0, -10.0, 15.0, 8.0, -16};
float z0[100]={-36.0, -36.0, -36.0, -15.0, -5.0, -5.0, -100.0, -90.0, -90.0, 13.0, 13.0, -110.0, -110.0, -50.0, -70};
float z1[100]={-39.0, -39.0, -39.0, -18.0, -2.0, -2.0, -103.0, -93.0, -93.0, 16.0, 1.0, -114.0, -113.0, -75.0, -73};
float t[100]={10.0, 6.0, 6.0, 8.0, 10.0, 7.0, 8.0, 10.0, 10.0, 10.0, 12.0, 10.0, 11.0, 15.0, 5.0};

//animate
double intervalPeluru = 0.0005; 
double intervalMaju = 1; 

//peluru Player
int peluruP1 = 20, peluruP2 = 20;
int cpeluruP1 = 0, cpeluruP2 = 0;


float aa = 0;
float luncur = 0, luncur2 = 0;
int i = 0, j = 1;

time_t start, start2;
int maju = 0, mundur = 0;
int maju2 = 0, mundur2 = 0;

int hitP1 = 0, hitP2 = 0;

//player1
float cameraAngle = 0.0;
float x = 0, y = 3, z = 20; // default camera position
float lx = 0, ly = 0, lz = -1;
float ax = 0, ay = 0, az = -1;

//player2
float cameraAnglea = 0.0;
float xa = 0, ya = 3, za = -100; // default camera position
float lxa = 0, lya = 0, lza = -1;
float axa = 0, aya = 0, aza = -1;

//peluru Player1
float peluruP1x, peluruP1y = 0, peluruP1z, angleP1, axP1;
float peluruP1lx, peluruP1ly = 0, peluruP1lz;

//peluru Player2
float peluruP2x, peluruP2y = 0, peluruP2z, angleP2, axP2;
float peluruP2lx, peluruP2ly = 0, peluruP2lz;

GLMmodel* model1 = NULL;
GLMmodel* model2 = NULL;
GLMmodel* model3 = NULL;

// arena : -30 : 30  ; 27 - (-120)
int cekGerak1()
{
	//if(x+lx <= -30 && x+lx >= 30 && z+lz >= 27 && z+lz <= -120){
		for(int i=0; i<16; i++) {
			if (x+lx >= x0[i]-1 && x+lx <= x1[i]+1 && z+lz <= z0[i]+1.5 && z+lz >= z1[i]-1.5)
			{
				return 0;
			}
		}
		
		if(x+lx <= -29 || x+lx >= 29 || z+lz+1 >= 26 || z+lz+1 <= -117){
			return 0;
		}
		
		return 1;
	//}
	//return 1;
}

int cekGerak2()
{
	//if(x+lx <= -30 && x+lx >= 30 && z+lz >= 27 && z+lz <= -120){
		for(int i=0; i<16; i++) {
			if (xa+lxa >= x0[i]-1 && xa+lxa <= x1[i]+1 && za+lza <= z0[i]+1.5 && za+lza >= z1[i]-1.5)
			{
				return 0;
			}
		}
		
		if(xa+lxa <= -29 || xa+lxa >= 29 || za+lza+1 >= 26 || za+lza+1 <= -117){
			return 0;
		}
		
		return 1;
	//}
	//return 1;
}

//Player1
void setCamera()
{
	glLoadIdentity();
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0, 1, 0);
}

void rotateCamera(float ang)
{
    // rotate the camera (left / right)
	lx = sin(ang);
	lz = -cos(ang);
    setCamera();
}

void moveCamera(int h_direction, int v_direction)
{
    // move the camera (forward, backward, up, down)
	x = x + h_direction*(lx)*0.17;
	z = z + h_direction*(lz)*0.17;
	y = y + v_direction*0.1;

	setCamera();
}

//player2
void setCameraa()
{
	glLoadIdentity();
	gluLookAt(xa, ya, za, xa + lxa, ya + lya, za + lza, 0, 1, 0);
}
void rotateCameraa(float ang)
{
    // rotate the camera (left / right)
	lxa = sin(ang);
	lza = -cos(ang);
    setCameraa();
}
void moveCameraa(int h_direction, int v_direction)
{
    // move the camera (forward, backward, up, down)
	xa = xa + h_direction*(lxa)*0.17;
	za = za + h_direction*(lza)*0.17;
	ya = ya + v_direction*0.1;
	setCameraa();
}

void glTga(void)
{
	glEnable (GL_DEPTH_TEST);
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

       loadTGA("box.tga", 1);//load gambar texture box
       loadTGA("texture_boxtop.tga", 2);//load gambar texture box
       loadTGA("texture_wall.tga", 3);//load gambar texture wall
       loadTGA("texture_floor.tga", 4);//load gambar texture wall
       loadTGA("sky.tga", 5);//load gambar texture wall
       loadTGA("texture_p1face.tga", 6);//load gambar texture wall


}

void box(float x0, float z0, float x1, float z1, float t)
{

    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(x0,0,z0);
        glTexCoord2f(1, 0);
        glVertex3f(x1,0,z0);
        glTexCoord2f(1, 1);
        glVertex3f(x1,t,z0);
        glTexCoord2f(0, 1);
        glVertex3f(x0,t,z0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tampak samping kanan box
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(x1,0,z0);
        glTexCoord2f(1, 0);
        glVertex3f(x1,0,z1);
        glTexCoord2f(1, 1);
        glVertex3f(x1,t,z1);
        glTexCoord2f(0, 1);
        glVertex3f(x1,t,z0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tampak samping kiri box
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(x0,0,z0);
        glTexCoord2f(1, 0);
        glVertex3f(x0,0,z1);
        glTexCoord2f(1, 1);
        glVertex3f(x0,t,z1);
        glTexCoord2f(0, 1);
        glVertex3f(x0,t,z0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tampak belakang box
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 1);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(x0,0,z1);
        glTexCoord2f(1, 0);
        glVertex3f(x1,0,z1);
        glTexCoord2f(1, 1);
        glVertex3f(x1,t,z1);
        glTexCoord2f(0, 1);
        glVertex3f(x0,t,z1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //tampak atas box
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 2);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(x0,t,z0);
        glTexCoord2f(1, 0);
        glVertex3f(x1,t,z0);
        glTexCoord2f(1, 1);
        glVertex3f(x1,t,z1);
        glTexCoord2f(0, 1);
        glVertex3f(x0,t,z1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void peluru(float x0, float z0, float x1, float z1, float t)
{


   glBegin(GL_QUADS);
    glVertex3f(x1,0,z0);
    glVertex3f(x1,0,z1);
    glVertex3f(x1,t,z1);
    glVertex3f(x1,t,z0);
    glEnd(); 


   glBegin(GL_QUADS);
    glVertex3f(x0,0,z0);
    glVertex3f(x0,t,z0);
    glVertex3f(x0,t,z1);
    glVertex3f(x0,0,z1);
    glEnd(); 
    
    glBegin(GL_QUADS);
    glVertex3f(x0,t,z1);
    glVertex3f(x1,t,z1);
    glVertex3f(x1,0,z1);
    glVertex3f(x0,0,z1);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(x0,t,z0);
    glVertex3f(x1,t,z0);
    glVertex3f(x1,t,z1);
    glVertex3f(x0,t,z1);
    glEnd(); 

    //tampak bawah box
    
    glBegin(GL_QUADS);
    glVertex3f(x0,0,z0);
    glVertex3f(x1,0,z0);
    glVertex3f(x1,0,z1);
    glVertex3f(x0,0,z1);
    glEnd(); 
}

void munculbox()
{
for(int i=0;i<16;i++){
    glColor3ub(43,43,43);
   // box(0.5,-22.5, 2.5, -25.5, 6); //parameter (x0,z0,x1,z1,t);

    box(x0[i], z0[i], x1[i], z1[i], t[i]);
}
}

void bawahan()
{

    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 4);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-30,0,27);
        glTexCoord2f(1, 0);
        glVertex3f(30,0,27);
        glTexCoord2f(1, 1);
        glVertex3f(30,0,-120);
        glTexCoord2f(0, 1);
        glVertex3f(-30,0,-120);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 5);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-30,20,27);
        glTexCoord2f(1, 0);
        glVertex3f(30,20,27);
        glTexCoord2f(1, 1);
        glVertex3f(30,20,-120);
        glTexCoord2f(0, 1);
        glVertex3f(-30,20,-120);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void dinding()
{
    //dinding kanan
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 3);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(30,0,27);
        glTexCoord2f(1, 0);
        glVertex3f(30,20,27);;
        glTexCoord2f(1, 1);
        glVertex3f(30,20,-120);
        glTexCoord2f(0, 1);
        glVertex3f(30,0,-120);
    glEnd();
    glDisable(GL_TEXTURE_2D);


     //dinding kiri
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 3);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-30,0,27);
        glTexCoord2f(1, 0);
        glVertex3f(-30,20,27);
        glTexCoord2f(1, 1);
        glVertex3f(-30,20,-120);
        glTexCoord2f(0, 1);
        glVertex3f(-30,0,-120);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //dinding belakang
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 3);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-30,0,27);
        glTexCoord2f(1, 0);
        glVertex3f(30,0,27);
        glTexCoord2f(1, 1);
        glVertex3f(30,20,27);
        glTexCoord2f(0, 1);
        glVertex3f(-30,20,27);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //dinding depan
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 3);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(-30,0,-120);
        glTexCoord2f(1, 0);
        glVertex3f(0,0,-120);
        glTexCoord2f(1, 1);
        glVertex3f(0,20,-120);
        glTexCoord2f(0, 1);
        glVertex3f(-30,20,-120);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 3);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(0,0,-120);
        glTexCoord2f(1, 0);
        glVertex3f(30,0,-120);
        glTexCoord2f(1, 1);
        glVertex3f(30,20,-120);
        glTexCoord2f(0, 1);
        glVertex3f(0,20,-120);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

void kepala(float x0, float y0, float z0, float x1, float y1, float z1)
{
    //tampak depan box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x0,y1,z0);
    glEnd();


    //tampak samping kanan box
    glBegin(GL_QUADS);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y1,z0);
    glEnd();


    //tampak samping kiri box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x0,y1,z0);
    glVertex3f(x0,y1,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

    //tampak belakang box
    glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, 6);
    glBegin(GL_POLYGON);
        glTexCoord2f(0, 0);
        glVertex3f(x0,y0,z1);
        glTexCoord2f(1, 0);
        glVertex3f(x1,y0,z1);
        glTexCoord2f(1, 1);
        glVertex3f(x1,y1,z1);
        glTexCoord2f(0, 1);
        glVertex3f(x0,y1,z1);
    glEnd();
    glDisable(GL_TEXTURE_2D);

 /* //tampak belakang box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();
*/
    //tampak atas box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x1,y1,z1);
    glVertex3f(x0,y1,z1);
    glEnd();


    //tampak bawah box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

}

void badan(float x0, float y0, float z0, float x1, float y1, float z1)
{
    //tampak depan box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x0,y1,z0);
    glEnd();


    //tampak samping kanan box
    glBegin(GL_QUADS);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y1,z0);
    glEnd();

    //tampak samping kiri box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x0,y1,z0);
    glVertex3f(x0,y1,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

    //tampak belakang box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

    //tampak atas box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x1,y1,z1);
    glVertex3f(x0,y1,z1);
    glEnd();

    //tampak bawah box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

}

void tangan(float x0, float y0, float z0, float x1, float y1, float z1)
{


    //tampak depan box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x0,y1,z0);
    glEnd();

    //tampak samping kanan box
    glBegin(GL_QUADS);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y1,z0);
    glEnd();

    //tampak samping kiri box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x0,y1,z0);
    glVertex3f(x0,y1,z1);
    glVertex3f(x0,y0,z1);
    glEnd();


   //tampak belakang box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

    //tampak atas box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x1,y1,z1);
    glVertex3f(x0,y1,z1);
    glEnd();

    //tampak bawah box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

}

void kaki(float x0, float y0, float z0, float x1, float y1, float z1)
{
    //tampak depan box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x0,y1,z0);
    glEnd();


    //tampak samping kanan box
    glBegin(GL_QUADS);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y1,z0);
    glEnd();

    //tampak samping kiri box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x0,y1,z0);
    glVertex3f(x0,y1,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

    //tampak belakang box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z1);
    glVertex3f(x1,y1,z1);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

    //tampak atas box
    glBegin(GL_QUADS);
    glVertex3f(x0,y1,z0);
    glVertex3f(x1,y1,z0);
    glVertex3f(x1,y1,z1);
    glVertex3f(x0,y1,z1);
    glEnd();

    //tampak bawah box
    glBegin(GL_QUADS);
    glVertex3f(x0,y0,z0);
    glVertex3f(x1,y0,z0);
    glVertex3f(x1,y0,z1);
    glVertex3f(x0,y0,z1);
    glEnd();

}

void player1(){
	glPushMatrix();
	glColor3f(0.1,0.1,0.1);
    //kepala
    kepala(0,6.3,0,0.7,8.5,1);
    //badan
    badan(0,2,0,0.7,6,1);
    //tangan
    tangan(-0.30,1,0.4,0,6,0.7); //kanan
    tangan(0.7,4,1.5,1,5.5,0.5); //kiri
    //kaki
    kaki(0.15,0,0,0.55,1.85,1);
    glPopMatrix();

}

void init() 
{
	model1 = glmReadOBJ("model/m4a1_s.obj");
	glmUnitize(model1);
	glmFacetNormals(model1);
	glmVertexNormals(model1, 90.0);
}

void senjata()
{
	glColor3f(0.1,0.1,0.1);
	glPushMatrix();
        glRotatef(180,0,1,0);
        glTranslatef(0,-0.5,1);
        glScalef(0.4,0.8,0.5);
        glRotatef(-6,1,0,0);
        glmDraw(model1, GLM_SMOOTH);
	glPopMatrix();
}

int cekTembak()
{
	if (peluruP1x + peluruP1lx >= xa+lxa-0.2 && peluruP1x+peluruP1lx <= xa+lxa+0.2 && peluruP1z+peluruP1lz <= za+lza+0.5 && peluruP1z+peluruP1lz >=za+lza-0.5)
	{
		luncur = 0;
		peluruP1x = 0;
		peluruP1y = 0;
		peluruP1z = 0;
		return 1;
	}
	
	for(int i=0; i<16; i++) {
		if (peluruP1x + peluruP1lx >= x0[i] && peluruP1x + peluruP1lx <= x1[i] && peluruP1z+peluruP1lz <= z0[i] && peluruP1z+peluruP1lz >= z1[i])
		{
			luncur = 0;
			peluruP1x = 0;
			peluruP1y = 0;
			peluruP1z = 0;
			return 0;
		}
	}
	
	if(peluruP1x + peluruP1lx <= -30 || peluruP1x + peluruP1lx >= 30 || peluruP1z+peluruP1lz >= 27 || peluruP1z+peluruP1lz <= -120){
		return 0;
	}
	
	else return 0;
}

int cekTembak2()
{
	if (peluruP2x + peluruP2lx >= x+lx-0.5 && peluruP2x+peluruP2lx <= x+lx+0.5 && peluruP2z+peluruP2lz <= z+lz+0.5 && peluruP2z+peluruP2lz >=z+lz-0.5)
	{
		luncur2 = 0;
		peluruP2x = 0;
		peluruP2y = 0;
		peluruP2z = 0;
		return 1;
	}
	
	for(int i=0; i<16; i++) {
		if (peluruP2x + peluruP2lx >= x0[i] && peluruP2x + peluruP2lx <= x1[i] && peluruP2z+peluruP2lz <= z0[i] && peluruP2z+peluruP2lz >= z1[i])
		{
			luncur = 0;
			peluruP1x = 0;
			peluruP1y = 0;
			peluruP1z = 0;
			return 0;
		}
	}
	
	if(peluruP2x + peluruP2lx <= -30 || peluruP2x + peluruP2lx >= 30 || peluruP2z+peluruP2lz >= 27 || peluruP2z+peluruP2lz <= -120){
		return 0;
	}
	
	else return 0;
}

void clear()
{
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

void score()
{
	//cout << "-----------------------------------------------------------------------------" << endl;
	cout << "||                              Counter Craft                              ||" << endl;
	//cout << "-----------------------------------------------------------------------------" << endl << endl;
	
	cout << "||            Player 1            ||  " << "   " << "||            Player 2            ||" << endl;
	//cout << "------------------------------------" << "   " << "------------------------------------" << endl;
	 
	cout << "            Score   : " << hitP1;
	if (hitP1 < 10)
		cout << " ";
	cout << "                             Score   : " << hitP2 <<endl;
	cout << "            Peluru  : " << peluruP1;
	if (peluruP1 < 10)
		cout << " ";
	cout << "                             Peluru  : " << peluruP2 << endl;
	
	clear();
}

void tembak()
{	
	peluruP1lx = sin(angleP1);
	peluruP1lz = -cos(angleP1);
	int kena = 0;
	
	peluruP1x = peluruP1x + (peluruP1lx)*0.1;
	peluruP1z = peluruP1z + (peluruP1lz)*0.1;

    glColor3f(1,0,0);
    glPushMatrix();
        glTranslatef(peluruP1x+peluruP1lx, peluruP1y+peluruP1ly, peluruP1z+peluruP1lz);
        glTranslatef(-0.02,-0.5,0);
        glRotatef(axP1,0,1,0);
        peluru(0,0,0.01,1,0.1);
    glPopMatrix();
    
    if (cekTembak() == 1){
		hitP1++;
	}
		
}

void tembak2()
{	
	peluruP2lx = sin(angleP2);
	peluruP2lz = -cos(angleP2);
	int kena = 0;
	
	peluruP2x = peluruP2x + (peluruP2lx)*0.1;
	peluruP2z = peluruP2z + (peluruP2lz)*0.1;

    glColor3f(1,0,0);
    glPushMatrix();
        glTranslatef(peluruP2x+peluruP2lx, peluruP2y+peluruP2ly, peluruP2z+peluruP2lz);
        glTranslatef(-0.02,-0.5,0);
        glRotatef(axP2,0,1,0);
        peluru(0,0,0.01,1,0.1);
    glPopMatrix();
    
    if (cekTembak2() == 1){
		hitP2++;
	}
		
}

void resetMenang()
{
	hitP1 = 0;
	hitP2 = 0;
	//player1
	cameraAngle = 0.0;
	x = 0; y = 3; z = 20; // default camera position
	lx = 0; ly = 0; lz = -1;
	ax = 0; ay = 0; az = -1;

	//player2
	cameraAnglea = 0.0;
	xa = 0; ya = 3; za = -100; // default camera position
	lxa = 0; lya = 0; lza = -1;
	axa = 0; aya = 0; aza = -1;
	peluruP1 = 20;
	peluruP2 = 20;
}

void menangP1()
{
	//cout << "-----------------------------------------------------------------------------" << endl;
	cout << "||                              Counter Craft                              ||" << endl;
	//cout << "-----------------------------------------------------------------------------" << endl << endl;
	
	cout << "||            Player 1            ||  " << "   " << "||            Player 2            ||" << endl << endl;
	//cout << "------------------------------------" << "   " << "------------------------------------" << endl;
	 
	cout << "            Player 1 WIN              " << endl;
	
	resetMenang();
}

void menangP2()
{
	//cout << "-----------------------------------------------------------------------------" << endl;
	cout << "||                              Counter Craft                              ||" << endl;
	//cout << "-----------------------------------------------------------------------------" << endl << endl;
	
	cout << "||            Player 1            ||  " << "   " << "||            Player 2            ||" << endl << endl;
	//cout << "------------------------------------" << "   " << "------------------------------------" << endl;
	 
	cout << "                                                      Player 2 WIN              " << endl;
	
	resetMenang();
}

void arena()
{
	if (hitP1 == 5)
	{
		menangP1();
		sleep(3);
	}
	else if (hitP2 == 5)
	{
		menangP2();
		sleep(3);
	}	
	else score();
	
	glColor3f(1,1,1);
	munculbox();
	dinding();
	bawahan();
    
    glColor3f(1,1,1);
    munculbox();
    box(-0.5,-0.5,0.5,0.5,3);
	glColor3f(0.1, 0.1, 0.1);
	
	
	if (luncur == 1)
		tembak();
	
	if (luncur2 == 1)
		tembak2();
	
	glPushMatrix();
		glTranslatef(x+lx, y+ly, z+lz);
		glRotatef(ax,0,1,0);
		senjata();
		glTranslatef(-0.1,-2,-1);
		glTranslatef(0.44,-0.7,0.4);
		glRotatef(180,0,1,0);
		glScalef(0.4,0.4,0.4);
		player1();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(xa+lxa, ya+lya, za+lza);
		glRotatef(axa,0,1,0);
		senjata();
		glTranslatef(-0.1,-2,-1);
		glTranslatef(0.44,-0.7,0.4);
		glRotatef(180,0,1,0);
		glScalef(0.4,0.4,0.4);
		player1();
	glPopMatrix();
}

void display()
{
	//player 1
    glViewport(0,height/2,width,height/2);
    arena();
    
    //player 2
    glViewport(0,0,width,height/2);
    glLoadIdentity();
    gluLookAt(xa, ya, za, xa + lxa, ya + lya, za + lza, 0, 1, 0);
    arena();
    
}

void setup_viewport(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,1,1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    setCamera();
}

void multiple(GLFWwindow* window)
{
    
    //control Player1
	if (glfwGetKey(window,GLFW_KEY_A)){
		cameraAngle -= 0.02;
		ax += 1.149;
		rotateCamera(cameraAngle);
	}
	
	if (glfwGetKey(window,GLFW_KEY_D)){
		cameraAngle += 0.02;
		ax -= 1.149;
		rotateCamera(cameraAngle);
	}
	

	if (glfwGetKey(window,GLFW_KEY_W)){
		if(cekGerak1() == 1 || mundur == 1){
			maju = 1;
			moveCamera(1, 0);
			mundur = 0;
			}
	}
    
    if (glfwGetKey(window,GLFW_KEY_S)){
		if(cekGerak1() == 1 || maju == 1){
			mundur = 1;
			moveCamera(-1, 0);
			maju = 0;
		}
	}
	
	
	//Control Player 2
	if (glfwGetKey(window,GLFW_KEY_LEFT)){
		cameraAnglea -= 0.02;
		axa += 1.149;
		rotateCameraa(cameraAnglea);
	}
	
	if (glfwGetKey(window,GLFW_KEY_RIGHT)){
		cameraAnglea += 0.02;
		axa -= 1.149;
		rotateCameraa(cameraAnglea);
	}
	
	if (glfwGetKey(window,GLFW_KEY_UP)){
		if(cekGerak2() == 1 || mundur2 == 1){
			maju2 = 1;
			moveCameraa(1, 0);
			mundur2 = 0;
		}
	}
	
	if (glfwGetKey(window,GLFW_KEY_DOWN)){
		if(cekGerak2() == 1 || maju2 == 1){
			mundur2 = 1;
			moveCameraa(-1, 0);
			maju2 = 1;
			}
	}       
	
	
	//control Up/Down
	if (glfwGetKey(window,GLFW_KEY_PAGE_UP)){
		moveCamera(0,1);
	}
	
	if (glfwGetKey(window,GLFW_KEY_PAGE_DOWN)){
		moveCamera(0,-1);
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
	if (action==GLFW_PRESS || action==GLFW_RELEASE) {
		if (glfwGetKey(window,GLFW_KEY_SPACE)){
			if (peluruP1 == 0){
				int xx = difftime (time(NULL), start);
				if ( xx >= 3)
					{
						xx = 0;
						peluruP1 = 20;
					}
			}
			else {
			start = time(NULL);
			peluruP1--;
			luncur = 1;
			peluruP1x = x;
			peluruP1y = y;
			peluruP1z = z;
			axP1 = ax;
			angleP1 = cameraAngle;
			}
			
		}
		
		if (glfwGetKey(window,GLFW_KEY_R)){
			//player1
			cameraAngle = 0.0;
			x = 0, y = 3, z = 20; // default camera position
			lx = 0, ly = 0, lz = -1;
			ax = 0, ay = 0, az = -1;
			
		}
		
		if (glfwGetKey(window,GLFW_KEY_ESCAPE)){
			exit(0);
			
		}
		
		if (glfwGetKey(window,GLFW_KEY_T)){		
			//player2
			cameraAnglea = 0.0;
			xa = 0, ya = 3, za = 0; // default camera position
			lxa = 0, lya = 0, lza = -1;
			axa = 0, aya = 0, aza = -1;
			
		}
		
		if (glfwGetKey(window,GLFW_KEY_L) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)){
			if (peluruP2 == 0){
				int yy = difftime (time(NULL), start2);
				if ( yy >= 3)
					{
						yy = 0;
						peluruP2 = 20;
					}
			}
			else {
			start2 = time(NULL);
			peluruP2--;
			luncur2 = 1;
			peluruP2x = xa;
			peluruP2y = ya;
			peluruP2z = za;
			axP2 = axa;
			angleP2 = cameraAnglea;
			}
			
		}
		
	}	
}

void lighting()
{

    // Fungsi mengaktifkan pencahayaan
    // Pembahasan di materi selanjutnya

    const GLfloat light_ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    const GLfloat mat_ambient[]    = { 1.0f, 1.f, 1.f, 1.0f };
    const GLfloat mat_diffuse[]    = { 1.0f, 1.f, 1.f, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 255.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    //glShadeModel(GL_FLAT);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

}


int main(void)
{

    GLFWwindow* window;
    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(width,height, "Counter Craft", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    init();
    glTga();
    lighting();

	int ticks = 0;
	int ticks1 = 0;

    while (!glfwWindowShouldClose(window))
    {
		double currentTime = glfwGetTime();
        while (currentTime / intervalPeluru > ticks) {
            tembak();
            tembak2();
            ticks++;
            currentTime = glfwGetTime();
        }
		
        setup_viewport(window);

        //Kedalaman agar lebih real
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
        display();
        multiple(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
