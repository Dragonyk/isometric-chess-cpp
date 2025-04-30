#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <stdlib.h>
#include "ImgUtils.h"
#include "Tile.h"
#include "Object.h"

#define ROWS 8
#define COLS 8
#define TEXTURE_NUMBER 9

#define OBJ_REI 1
#define OBJ_RAINHA 2
#define OBJ_PEAO 3
#define OBJ_CAVALO 4
#define OBJ_TORRE 5
#define OBJ_BISPO 6

using namespace std;
int winWidth = 1024;
int winHeight = 720;
bool checkState = false;

int isSelect = 0;
int selX = 3;
int selY = 3;
int obj_sel = 0;
int cur_player = 0;
int rod = 1;
bool gameOver = false;

int p1_score = 0;
int p2_score = 0;

float tw = 0.06;
float th = 0.04;

int objx = selX;
int objy = selY;

int up_k = 0;
int down_k = 0;
int left_k = 0;
int right_k = 0;

float msx, msy;

ImgUtils imgutils = ImgUtils(winWidth, winHeight);
Image * img = new Image[TEXTURE_NUMBER];
GLuint texture_id[TEXTURE_NUMBER];
Tile tiles [ROWS][COLS];

vector<Object> objs(32);
vector<int> cur_pathX(32, -1);
vector<int> cur_pathY(32, -1);

int mat_logic[8][8];

void LoadTileMap(float w, float h, float x, float y);

void LoadTextures() {
	//ID 0 = TILE BRANCO
	//ID 1 = TILE PRETO

	//ID 2 = PEAO
	//ID 3 = CAVALO
	//ID 4 = TORRE
	//ID 5 = BISPO
	//ID 6 = REI
	//ID 7 = RAINHA

	//ID 8 = TEXTURA MADEIRA

	img[0] = imgutils.loadImageNormal("white_tile.ptm");
	img[1] = imgutils.loadImageNormal("black_tile.ptm");

	img[2] = imgutils.loadImageNormal("peao.ptm");
	img[3] = imgutils.loadImageNormal("cavalo.ptm");
	img[4] = imgutils.loadImageNormal("torre.ptm");
	img[5] = imgutils.loadImageNormal("bispo.ptm");
	img[6] = imgutils.loadImageNormal("rei.ptm");
	img[7] = imgutils.loadImageNormal("rainha.ptm");

	img[8] = imgutils.loadImageNormal("wood_txt.ptm");

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(TEXTURE_NUMBER, texture_id);

	for (int i = 0; i < TEXTURE_NUMBER; i++) {
		texture_id[0] = i;
		glBindTexture(GL_TEXTURE_2D, i);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img[i].getWidth(), img[i].getHeight(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, img[i].getPixels());
	}
}

float tilesx[ROWS][COLS];
float tilesy[ROWS][COLS];
int tileclr[ROWS][COLS];

void ResetPath(){
	for (int i = 0; i < 32; i++) {
		cur_pathX[i] = -1;
		cur_pathY[i] = -1;
	}
}

Object createObject(int id, int txd, int isActive, int player, float w, float h, float x, float y) {
	Object obj;
	obj.id = id;
	obj.texture = txd;
	obj.isActive = isActive;
	obj.player = player;
	obj.w = w;
	obj.h = h;
	obj.x = x;
	obj.y = y;
	return obj;
}

void LoadSys() {
	objx = 1;
	objy = 2;

	LoadTileMap(tw, th, 0.5, 0.2);

	for (int i = 0; i < 16; i++) {
		if (i < 8) 
			objs[i] = createObject(i, 2, 1, 0, 0.025, 0.1, tiles[1][i].x, tiles[1][i].y);
		else
			objs[i] = createObject(i, 2, 1, 1, 0.025, 0.1, tiles[6][i-8].x, tiles[6][i-8].y);
		objs[i].type = OBJ_PEAO;
	}
	objs[16] = createObject(16, 3, 1, 0, 0.025, 0.1, tiles[0][1].x, tiles[0][1].y);
	objs[17] = createObject(17, 3, 1, 0, 0.025, 0.1, tiles[0][6].x, tiles[0][6].y);
	objs[16].type = OBJ_CAVALO;
	objs[17].type = OBJ_CAVALO;

	objs[18] = createObject(18, 4, 1, 0, 0.025, 0.1, tiles[0][0].x, tiles[0][0].y);
	objs[19] = createObject(19, 4, 1, 0, 0.025, 0.1, tiles[0][7].x, tiles[0][7].y);
	objs[18].type = OBJ_TORRE;
	objs[19].type = OBJ_TORRE;

	objs[20] = createObject(20, 5, 1, 0, 0.025, 0.13, tiles[0][2].x, tiles[0][2].y);
	objs[21] = createObject(21, 5, 1, 0, 0.025, 0.13, tiles[0][5].x, tiles[0][5].y);
	objs[20].type = OBJ_BISPO;
	objs[21].type = OBJ_BISPO;

	objs[22] = createObject(22, 6, 1, 0, 0.025, 0.1, tiles[0][3].x, tiles[0][3].y);
	objs[23] = createObject(23, 7, 1, 0, 0.025, 0.1, tiles[0][4].x, tiles[0][4].y);
	objs[22].type = OBJ_REI;
	objs[23].type = OBJ_RAINHA;

	//#############################################################################

	objs[24] = createObject(24, 3, 1, 1, 0.025, 0.1, tiles[7][1].x, tiles[7][1].y);
	objs[25] = createObject(25, 3, 1, 1, 0.025, 0.1, tiles[7][6].x, tiles[7][6].y);
	objs[24].type = OBJ_CAVALO;
	objs[25].type = OBJ_CAVALO;

	objs[26] = createObject(26, 4, 1, 1, 0.025, 0.1, tiles[7][0].x, tiles[7][0].y);
	objs[27] = createObject(27, 4, 1, 1, 0.025, 0.1, tiles[7][7].x, tiles[7][7].y);
	objs[26].type = OBJ_TORRE;
	objs[27].type = OBJ_TORRE;

	objs[28] = createObject(28, 5, 1, 1, 0.025, 0.13, tiles[7][2].x, tiles[7][2].y);
	objs[29] = createObject(29, 5, 1, 1, 0.025, 0.13, tiles[7][5].x, tiles[7][5].y);
	objs[28].type = OBJ_BISPO;
	objs[29].type = OBJ_BISPO;

	objs[30] = createObject(30, 6, 1, 1, 0.025, 0.1, tiles[7][3].x, tiles[7][3].y);
	objs[31] = createObject(31, 7, 1, 1, 0.025, 0.1, tiles[7][4].x, tiles[7][4].y);
	objs[30].type = OBJ_REI;
	objs[31].type = OBJ_RAINHA;
}

void DrawTxt(char *text, int txt_size, int r, int g, int b, float x, float y) {
	glColor3ub(r, g, b);
	glRasterPos2f(x, y);
	for (int ii = 0; ii < txt_size; ii++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *(text++));
	}
}

void DrawObject(float w, float h, float x, float y) {
	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(x - w, y - h, 0.0);
	glVertex3f(x + w, y - h, 0.0);
	glVertex3f(x, y + h, 0.0);
	glEnd();
}

void DrawPiece(int texture, float w, float h, float x, float y, int isBlack) {
	if(!isBlack)
		glColor4f(1, 1, 1, 1);
	else
		glColor4f(0.45, 0.45, 0.45, 1);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x - w, y);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x + w, y);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x + w, y + h);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x - w, y + h);
	glEnd();
}

void DrawColorTile(float w, float h, float x, float y, int isBlack) {
	if (isSelect) {
		if (isBlack == 1)
			glColor3f(0, 0, 0);
		else if (isBlack == 0)
			glColor3f(1, 1, 1);
		else if (isBlack == -1)
			glColor3f(0, 1, 0);
		else if (isBlack == 2)
			glColor4f(0, 0, 1, 0.5f);
		else if (isBlack == 3)
			glColor3f(1, 0, 0);
		else {
			glColor3f(float(isBlack) / 255, float(isBlack) / 255, float(isBlack) / 255);
		}
		glBegin(GL_POLYGON);
		glVertex3f(x - w, y, 0.0);
		glVertex3f(x, y - h, 0.0);
		glVertex3f(x + w, y, 0.0);
		glVertex3f(x, y + h, 0.0);
		glEnd();
	}
}

void DrawTile(int texture, float w, float h, float x, float y) {
	glColor4f(1, 1, 1, 1);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x - w, y);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x, y - h);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x + w, y);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x, y + h);
	glEnd();
}

void DrawInvertTile(int texture, float w, float h, float x, float y, int def, int mid = 0) {
	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
	float md = 0.0f;
	if (mid) md = 1.0f;

	if (def) {
		glTexCoord2f(md, md);
		glVertex2f(x - w, y);
	}

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x, y - h);

	if (!def) {
		glTexCoord2f(md, md);
		glVertex2f(x + w, y);
	}

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x, y + h);
	glEnd();
}

void DrawTestTile(int texture, float w, float h, float x, float y, int dir) {
	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(x - w, y);
	

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(x, y - h);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(x + w, y);
	

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(x, y + h);
	glEnd();
}

void LoadTileMap(float w, float h, float x, float y) {
	float xt = x;
	float yt = y;
	int ln = 1;
	int par = 0;
	int impar = 1;
	int dp = -1;

	int txd = 0;
	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLS; i++) {
			dp++;
			tiles[j][i].x = xt;
			tiles[j][i].y = yt;
			xt = xt + w;
			yt = yt + h;
		}
		dp = j;
		xt = x - (ln * w);
		yt = y + (ln * h);
		ln++;
	}
}

void DrawIsometricMap() {
	int par = 0;
	int impar = 1;
	int txd_table = 8;

	DrawInvertTile(txd_table, tw, th, tiles[7][0].x - tw, tiles[7][0].y - th, 0);
	DrawInvertTile(txd_table, tw, th, tiles[0][7].x + tw, tiles[0][7].y - th, 1);

	DrawInvertTile(txd_table, tw, th, tiles[0][0].x, tiles[0][0].y - (th * 2), 0, 1);
	DrawInvertTile(txd_table, tw, th, tiles[0][0].x, tiles[0][0].y - (th * 2), 1, 1);

	int txd = 0;
	for (int j = 0; j < ROWS; j++) {
		for (int i = 0; i < COLS; i++) {
			if (i % 2 == 0) txd = par;
			else			txd = impar;
			DrawTile(txd, tw, th, tiles[j][i].x, tiles[j][i].y);

			if (i == 0 && j < 7) {
				DrawTestTile(txd_table, tw, th, tiles[j][i].x - tw, tiles[j][i].y - th, 1);
			}
			if (j == 0 && i < 7) {
				DrawTile(txd_table, tw, th, tiles[j][i].x+tw, tiles[j][i].y-th);
			}
		}
		if (j % 2 == 0) {	par = 1; impar = 0;	}
		else{	par = 0; impar = 1;	}
	}
}

float TriangleArea(float ax, float ay,
				   float bx, float by,
			   	   float cx, float cy) {
	//float s = (s1+s2+s3)/2;
	//float area = sqrtf(s*(s-s1)*(s-s2)*(s-s3));

	float area = (ax*(by-cy))+ (bx*(cy - ay))+ (cx*(ay - by));
	return area;
}

int InsideTile(float t1, float t2, float t3) {
	float soma = t2 + t3;
	cout << t1 <<" T1 \n";
	cout << soma << " SOMA \n";
	if (fabs(soma) <= fabs(t1)) {
		return 1;
	}
	else return 0;
}

void MouseMap(float mx, float my) {
	int tpx = objx;
	int tpy = objy;
	for (int x = 0; x < ROWS; x++) {
		for (int y = 0; y < COLS; y++) {
			if (mx >= (tiles[x][y].x -tw) && mx <= (tiles[x][y].x + tw)) {
				if (my >= (tiles[x][y].y - th) && my <= (tiles[x][y].y + th)) {
					float t1,t2,t3;
					tpx = x;
					tpy = y;
					if (mx < (tiles[x][y].x)) {
						if (my >= (tilesy[x][y])) {
							t1 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x - tw, tiles[x][y].y,
								tiles[x][y].x, tiles[x][y].y + th);

							t2 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x - tw, tiles[x][y].y,
								mx, my);

							t3 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								mx, my,
								tiles[x][y].x, tiles[x][y].y + th);

							//tpx--;
							tpx++;
						}
						else {

							t1 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x, tiles[x][y].y -th,
								tiles[x][y].x -tw, tiles[x][y].y);

							t2 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x, tiles[x][y].y - th,
								mx, my);

							t3 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								mx, my,
								tiles[x][y].x - tw, tiles[x][y].y);

							tpy--;
							//tpy--;
						}
					}
					else {
						if (my >= (tiles[x][y].y)) {
							t1 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x + tw, tiles[x][y].y,
								tiles[x][y].x, tiles[x][y].y + th);

							t2 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x +tw, tiles[x][y].y,
								mx, my);

							t3 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								mx, my,
								tiles[x][y].x, tiles[x][y].y + th);

							tpy++;
							//tpy++;
						}
						else {
							t1 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x, tiles[x][y].y - th,
								tiles[x][y].x + tw, tiles[x][y].y);

							t2 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								tiles[x][y].x, tiles[x][y].y - th,
								mx, my);

							t3 = TriangleArea(tiles[x][y].x, tiles[x][y].y,
								mx, my,
								tiles[x][y].x + tw, tiles[x][y].y);

							//tpx++;
							tpx--;
						}
					}
					int inTile = InsideTile(t1, t2, t3);
					if (inTile) {
						cout << "DENTRO\n";
						objx = x;
						objy = y;
					}
					else {
						cout << "FORA\n";
						objx = tpx;
						objy = tpy;
					}
					return;
				}
			}
		}
	}
}

int checkObject(int x, int y) {
	for (int i = 0; i < 32; i++) {
		if (objs[i].x == tiles[x][y].x && objs[i].y == tiles[x][y].y) {
			if (objs[i].isActive) {
				if (objs[i].type == OBJ_REI)
					checkState = true;
				return i;
			}
		}
	}
	return -1;
}

void DrawPosPeao(){
	int i_n = 0;

	int p_l = 1;

	if (cur_player)
		p_l = -1;

	int obj_temp = checkObject(objx + p_l, objy);

	if ((objx == 7 && !cur_player) || (objx == 0 && cur_player)) {
		obj_temp = checkObject(objx, objy);
		if (cur_player) 
			objs[obj_temp] = createObject(obj_temp, 7, 1, 1, 0.025, 0.1, tiles[objx][objy].x, tiles[objx][objy].y);
		else 
			objs[obj_temp] = createObject(obj_temp, 7, 1, 0, 0.025, 0.1, tiles[objx][objy].x, tiles[objx][objy].y);
		objs[obj_temp].type = OBJ_RAINHA;
		isSelect = 0;
	}
	else {
		if (obj_temp < 0) {
			DrawColorTile(tw, th, tiles[objx + p_l][objy].x, tiles[objx + p_l][objy].y, 2);
			cur_pathX[i_n] = objx + p_l;
			cur_pathY[i_n] = objy;
			i_n++;
			if ((objx == 1 && !cur_player) || (objx == 6 && cur_player)) {
				obj_temp = checkObject(objx + (p_l * 2), objy);
				if (rod < 3) {
					DrawColorTile(tw, th, tiles[objx + p_l * 2][selY].x, tiles[objx + p_l * 2][objy].y, 2);
					cur_pathX[i_n] = objx + (p_l * 2);
					cur_pathY[i_n] = objy;
					i_n++;
				}
			}
		}
		obj_temp = checkObject(objx + p_l, objy + p_l);
		if ((objx + p_l >= 0) && (objx + p_l <= 7) && (objy + p_l >= 0) && (objy + p_l <= 7)) {
			if (obj_temp >= 0) {
				if (objs[obj_temp].player != cur_player) {
					DrawColorTile(tw, th, tiles[objx + p_l][objy + p_l].x, tiles[objx + p_l][objy + p_l].y, 3);
					cur_pathX[i_n] = objx + p_l;
					cur_pathY[i_n] = objy + p_l;
					i_n++;
				}
			}
		}
		obj_temp = checkObject(objx + p_l, objy - p_l);
		if ((objx + p_l >= 0) || (objx + p_l <= 7) || (objy - p_l >= 0) || (objy - p_l <= 7)) {
			if (obj_temp >= 0) {
				if (objs[obj_temp].player != cur_player) {
					DrawColorTile(tw, th, tiles[objx + p_l][objy - p_l].x, tiles[objx + p_l][objy - p_l].y, 3);
					cur_pathX[i_n] = objx + p_l;
					cur_pathY[i_n] = objy - p_l;
					i_n++;
				}
			}
		}
	}
}

void DrawPosCavalo(){
	int i_n = 0;

	int tx[8] = { objx + 1, objx - 1, objx + 1, objx - 1 , objx + 2 , objx + 2 ,objx - 2, objx - 2 };
	int ty[8] = { objy + 2 , objy + 2 , objy - 2 , objy - 2, objy + 1, objy - 1, objy + 1, objy - 1 };

	for (int i = 0; i < 8; i++) {
		if (tx[i] < 0 || ty[i] < 0)
			continue;
		if (tx[i] > 7 || ty[i] > 7)
			continue;
		int obj_temp = checkObject(tx[i], ty[i]);
		if (obj_temp < 0) {
			DrawColorTile(tw, th, tiles[tx[i]][ty[i]].x, tiles[tx[i]][ty[i]].y, 2);
			cur_pathX[i_n] = tx[i];
			cur_pathY[i_n] = ty[i];
			i_n++;
		}
		else {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[tx[i]][ty[i]].x, tiles[tx[i]][ty[i]].y, 3);
				cur_pathX[i_n] = tx[i];
				cur_pathY[i_n] = ty[i];
				i_n++;
			}
		}
	}
}

void DrawPosRei() {
	int i_n = 0;

	int tx[8] = {objx+1, objx+1, objx+1, objx, objx, objx-1, objx-1,objx-1};
	int ty[8] = {objy, objy+1, objy-1, objy+1, objy-1, objy, objy+1,objy-1};

	for (int i = 0; i < 8; i++) {
		if (tx[i] < 0 || ty[i] < 0)
			continue;
		if (tx[i] > 7 || ty[i] > 7)
			continue;
		int obj_temp = checkObject(tx[i], ty[i]);
		if (checkObject(tx[i], ty[i]))
		if (obj_temp < 0) {
			DrawColorTile(tw, th, tiles[tx[i]][ty[i]].x, tiles[tx[i]][ty[i]].y, 2);
			cur_pathX[i_n] = tx[i];
			cur_pathY[i_n] = ty[i];
			i_n++;
		}
		else {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[tx[i]][ty[i]].x, tiles[tx[i]][ty[i]].y, 3);
				cur_pathX[i_n] = tx[i];
				cur_pathY[i_n] = ty[i];
				i_n++;
			}
		}
	}
}

int t_n = 0;
void DrawPosTorre(){

	for (int x = objx; x < 7; x++){
		int obj_temp = checkObject(x + 1, objy);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[x + 1][objy].x, tiles[x + 1][objy].y, 3);
				cur_pathX[t_n] = x + 1;
				cur_pathY[t_n] = objy;
				t_n++;
			}
			break;
		}
		DrawColorTile(tw, th, tiles[x + 1][objy].x, tiles[x+1][objy].y, 2);
		cur_pathX[t_n] = x + 1;
		cur_pathY[t_n] = objy;
		t_n++;
	}
	for (int x = objx; x > 0; x--){
		int obj_temp = checkObject(x - 1, objy);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[x - 1][objy].x, tiles[x - 1][objy].y, 3);
				cur_pathX[t_n] = x - 1;
				cur_pathY[t_n] = objy;
				t_n++;
			}
			break;
		}
		DrawColorTile(tw, th, tiles[x - 1][objy].x, tiles[x - 1][objy].y, 2);
		cur_pathX[t_n] = x - 1;
		cur_pathY[t_n] = objy;
		t_n++;
	}
	
	for (int y = objy; y < 7; y++){
		int obj_temp = checkObject(objx, y + 1);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[objx][y + 1].x, tiles[objx][y + 1].y, 3);
				cur_pathX[t_n] = objx;
				cur_pathY[t_n] = y + 1;
				t_n++;
			}
			break;
		}
		DrawColorTile(tw, th, tiles[objx][y + 1].x, tiles[objx][y + 1].y, 2);
		cur_pathX[t_n] = objx;
		cur_pathY[t_n] = y + 1;
		t_n++;
	}
	for (int y = objy; y > 0; y--){
		int obj_temp = checkObject(objx, y - 1);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[objx][y - 1].x, tiles[objx][y - 1].y, 3);
				cur_pathX[t_n] = objx;
				cur_pathY[t_n] = y - 1;
				t_n++;
			}
			break;
		}
		DrawColorTile(tw, th, tiles[objx][y - 1].x, tiles[objx][y - 1].y, 2);
		cur_pathX[t_n] = objx;
		cur_pathY[t_n] = y - 1;
		t_n++;
	}
	
}

void DrawPosBispo(){
	int i = 0;

	int t = (8 - objy);
	if (objy < objx) t = 8 - objx;
	for (int y = 1; y < t; y++) {
		i++;

		int obj_temp = checkObject(objx + i, objy + y);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[objx + i][objy + y].x, tiles[objx + i][objy + y].y, 3);
				cur_pathX[t_n] = objx + i;
				cur_pathY[t_n] = objy + y;
				t_n++;
			}
			break;
		}

		DrawColorTile(tw, th, tiles[objx + i][objy + y].x, tiles[objx + i][objy + y].y, 2);
		cur_pathX[t_n] = objx + i;
		cur_pathY[t_n] = objy + y;
		t_n++;
	}
	cout << i << " <<< NUMERO 1\n";

	i = 0;
	t = objx;
	if (objy < objx) t = objy;
	for (int x = 1; x <= t; x++) {
		i++;

		int obj_temp = checkObject(objx - x, objy - i);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[objx - x][objy - i].x, tiles[objx - x][objy - i].y, 3);
				cur_pathX[t_n] = objx - x;
				cur_pathY[t_n] = objy - i;
				t_n++;
			}
			break;
		}

		DrawColorTile(tw, th, tiles[objx - x][objy - i].x, tiles[objx - x][objy - i].y, 2);
		cur_pathX[t_n] = objx - x;
		cur_pathY[t_n] = objy - i;
		t_n++;
	}
	cout << i << " <<< NUMERO 2\n";

	i = 0;
	t = 7 - objx;
	if ((objy + objx)<7) t = objy;
	for (int y = 1; y <= t; y++) {
		i++;

		int obj_temp = checkObject(objx + i, objy - y);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[objx + i][objy - y].x, tiles[objx + i][objy - y].y, 3);
				cur_pathX[t_n] = objx + i;
				cur_pathY[t_n] = objy - y;
				t_n++;
			}
			break;
		}

		DrawColorTile(tw, th, tiles[objx + i][objy - y].x, tiles[objx + i][objy - y].y, 2);
		cur_pathX[t_n] = objx + i;
		cur_pathY[t_n] = objy - y;
		t_n++;
	}
	cout << i << " <<< NUMERO 3\n";

	i = 0;
	t = 7 - objy;
	if ((objy + objx)<7) t = objx;
	for (int y = 1; y <= t; y++) {
		i++;

		int obj_temp = checkObject(objx - y, objy + i);
		if (obj_temp >= 0) {
			if (objs[obj_temp].player != cur_player) {
				DrawColorTile(tw, th, tiles[objx - y][objy + i].x, tiles[objx - y][objy + i].y, 3);
				cur_pathX[t_n] = objx - y;
				cur_pathY[t_n] = objy + i;
				t_n++;
			}
			break;
		}

		DrawColorTile(tw, th, tiles[objx - y][objy + i].x, tiles[objx - y][objy + i].y, 2);
		cur_pathX[t_n] = objx - y;
		cur_pathY[t_n] = objy + i;
		t_n++;
	}
	cout << i << " <<< NUMERO 4\n";
}

void DrawPosRainha() {
	DrawPosTorre();
	DrawPosBispo();
}

void DrawPos(){
	if (isSelect){
		switch (objs[obj_sel].type)
		{
		case OBJ_PEAO:
			DrawPosPeao();
			break;
		case OBJ_CAVALO:
			DrawPosCavalo();
			break;
		case OBJ_TORRE:
			t_n = 0;
			DrawPosTorre();
			break;
		case OBJ_BISPO:
			t_n = 0;
			DrawPosBispo();
			break;
		case OBJ_RAINHA:
			t_n = 0;
			DrawPosRainha();
			break;
		case OBJ_REI:
			DrawPosRei();
			break;

		default:
			break;
		}
	}
}

int MovePiece() {
	if (gameOver)
		return 0;
	for (int i = 0; i < 32; i++) {
		if (cur_pathX[i] < 0)
			break;

		if (objx == cur_pathX[i] && objy == cur_pathY[i]) {

			int obj_temp = checkObject(cur_pathX[i], cur_pathY[i]);
			if (obj_temp >= 0) {
				if (objs[obj_temp].isActive) {
					if (objs[obj_temp].type == OBJ_REI)
						gameOver = true;
					objs[obj_temp].isActive = 0;
					if (cur_player)
						p2_score++;
					else
						p1_score++;
				}
			}
			

			objs[obj_sel].x = tiles[objx][objy].x;
			objs[obj_sel].y = tiles[objx][objy].y;
			
			ResetPath();

			//if (obj_temp < 0) {
				if (cur_player)
					cur_player = 0;
				else
					cur_player = 1;
			//}

			isSelect = 0;
			rod++;
			
			return 1;
		}
	}
	isSelect = 0;
	checkState = false;
	return 0;
}

int checkTile() {
	if (gameOver)
		return 0;
	int objid = checkObject(objx, objy);
	if (isSelect) { //UM OBJETO ESTA SELECIONADO?
		if (selX == objx && selY == objy) { //O TILE SELECIONADO JA ESTIVA SELECIONADO?
			isSelect = 0;
			ResetPath();
			return 0; //FAZER NADA
		}
		else {
			if (objid >= 0) { //TEM OBJETO NO TILE?
				if(objs[objid].player == cur_player)
					isSelect = 0;
				else
					MovePiece();
				return 0; //FAZER NADA
			}
			else{
				MovePiece();
				return 2; //MUDAR POSIÇÃO DO OBJETO PARA O TILE
			}
		}
	}
	else {
		if (objid >= 0) { //TEM OBJETO NO TILE?
			if (objs[objid].player == cur_player) { // O DONO DO OBJETO SELECIONADO É O PLAYER ATUAL?
				if (objs[objid].isActive) { // O OBJETO ESTA ATIVO?
					obj_sel = objid;
					selX = objx;
					selY = objy;
					cout << "SELECIONEI\n";
					isSelect = 1;
				return 1; //SELECIONAR
				}
			}
			else{
				isSelect = 0;
				return 0; //FAZER NADA
			}
		}
		else{
			isSelect = 0;
			return 0; //FAZER NADA
		}
	}
	isSelect = 0;
	return 0;
}

bool sortByHeight(const Object &lhs, const Object &rhs) { return lhs.y < rhs.y; }

void UpdateGame() {
	checkTile();
	sort(objs.begin(), objs.end(), sortByHeight);
}

void DrawTable() {
	float px1 = 0.05f;
	float py1 = 0.8f;

	float p = 0.02f;

	float px2 = 0.8f;
	float py2 = 0.8;

	int count_p1 = 0;
	int count_p2 = 0;

	for (int i = 31; i >= 0; i--) {
		if(objs[i].isActive)
			DrawPiece(objs[i].texture, objs[i].w, objs[i].h, objs[i].x, objs[i].y - 0.02, objs[i].player);
		else {

			if (objs[i].player) {
				DrawPiece(objs[i].texture, objs[i].w/2, objs[i].h/2, px2, py2, objs[i].player);
				count_p1++;
				if (count_p1 == 8) {
					py2 -= 0.05;
					px2 = 0.8f;
				}
				px2 += p;
			}
			else {
				DrawPiece(objs[i].texture, objs[i].w/2, objs[i].h/2, px1, py1, objs[i].player);
				count_p2++;
				if (count_p2 == 8) {
					py1 -= 0.05;
					px1 = 0.05f;
				}
				px1 += p;
			}
			
		}
	}
	//DrawPiece(objs[0].texture, objs[0].w, objs[0].h, objs[0].x, objs[0].y - 0.02, objs[0].player);
	//cout << objs[21].y << "TXT \n";
}
float ang = 0;
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	DrawIsometricMap();
	DrawColorTile(tw, th, tiles[objx][objy].x, tiles[objx][objy].y,-1);
	DrawPos();
	DrawObject(0.01, 0.01, msx, msy);
	if (cur_player == 0) {
		DrawTxt("Player 1 turn", 15, 255, 255, 255, 0.05, 0.05);
		if (gameOver)
			DrawTxt("Player 2 wins", 15, 255, 255, 255, 0.05, 0.95);
	}
	else {
		DrawTxt("Player 2 turn", 15, 255, 255, 255, 0.05, 0.05);
		if (gameOver)
			DrawTxt("Player 1 wins", 15, 255, 255, 255, 0.05, 0.95);
	}

	DrawTable();
	

	glFlush();
}

void Mouse(int key, int state, int x, int y) {
	float xx = x / (float)winWidth;
	xx = xx * 1;
	float yy = (winHeight-y) / (float)winHeight;
	yy = yy * 1;

	if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		MouseMap(xx, yy);
		UpdateGame();
		msx = xx;
		msy = yy;
	}
}

void KeyPress(int key, int x, int y) {
	int temp_x, temp_y;
	switch (key) {
	case GLUT_KEY_DOWN:  down_k  = 1;  break;
	case GLUT_KEY_UP:	 up_k    = 1;  break;
	case GLUT_KEY_LEFT:  left_k  = 1;  break;
	case GLUT_KEY_RIGHT: right_k = 1;  break;
	}
	//glutPostRedisplay();
}

void KeyRelease(int key, int x, int y) {
	int temp_x, temp_y;
	switch (key) {
	case GLUT_KEY_DOWN:  down_k  = 0;  break;
	case GLUT_KEY_UP:	 up_k	 = 0;  break;
	case GLUT_KEY_LEFT:  left_k  = 0;  break;
	case GLUT_KEY_RIGHT: right_k = 0;  break;
	}
	//glutPostRedisplay();
}

void Timer(int iUnused) {
	//MoveObject();
	ang+=0.5;
	if (ang >= 360){
		ang = 0;
	}
	glutPostRedisplay();
	glutTimerFunc(30, Timer, 0);
}

//Reajuste da janela
void reshape(int w, int h) {
	if (h == 0)
		h = 1;
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, , 0, ortSize, -1.0, 1.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void init(void)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.4f, 0.0f, 1.0f);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(winWidth, winHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Xadrez 0.002 alpha");
	init();
	LoadTextures();
	LoadSys();
	sort(objs.begin(), objs.end(), sortByHeight);
	//Timer(0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(KeyPress);
	glutSpecialUpFunc(KeyRelease);
	glutMouseFunc(Mouse);
	glutMainLoop();
	return 0;
}