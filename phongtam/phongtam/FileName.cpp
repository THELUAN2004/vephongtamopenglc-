
#include <iostream> 
#include "glut.h"
#define GL_SILENCE_DEPRECATION
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>

void drawAxes() {
    glDisable(GL_LIGHTING); // Tắt ánh sáng để màu sắc hiển thị chính xác
    glLineWidth(2.0);       // Đặt độ dày của đường

    glBegin(GL_LINES);
    // Trục X (màu đỏ)
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.01, 0.0);
    glVertex3f(10.0, 0.01, 0.0);
    // Trục Y (màu xanh lá)
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 10.0, 0.0);
    // Trục Z (màu xanh dương)
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.01, 0.0);
    glVertex3f(0.0, 0.01, 10.0);
    glEnd();

    glEnable(GL_LIGHTING); // Bật lại ánh sáng nếu cần cho các vật thể khác
}
// Global state for lighting toggles and camera
GLboolean switchOne = false, switchTwo = false, switchLamp = false;
GLboolean amb1 = true, diff1 = true, spec1 = true;
GLboolean amb2 = true, diff2 = true, spec2 = true;
GLboolean amb3 = true, diff3 = true, spec3 = true;
GLboolean switchDirLight = false;  // bật/tắt đèn định hướng
double eyeX = -4.0, eyeY = 7.0, eyeZ = 18.0;
double refX = 0.0, refY = 0.0, refZ = -20.0;
double windowHeight = 600.0, windowWidth = 600.0;

// Vertices for a unit cube (0..3 in each axis)
static GLfloat v_cube[8][3] = {
    {0.0, 0.0, 0.0}, {0.0, 0.0, 3.0}, {3.0, 0.0, 3.0}, {3.0, 0.0, 0.0},
    {0.0, 3.0, 0.0}, {0.0, 3.0, 3.0}, {3.0, 3.0, 3.0}, {3.0, 3.0, 0.0}
};
static GLubyte quadIndices[6][4] = {
    {0, 1, 2, 3}, // bottom
    {4, 5, 6, 7}, // top
    {5, 1, 2, 6}, // front
    {0, 4, 7, 3}, // back
    {2, 3, 7, 6}, // right
    {1, 5, 4, 0}  // left
};

/* Compute and set normal for a quad given three vertices */
static void getNormal3p(
    GLfloat x1, GLfloat y1, GLfloat z1,
    GLfloat x2, GLfloat y2, GLfloat z2,
    GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux = x2 - x1, Uy = y2 - y1, Uz = z2 - z1;
    GLfloat Vx = x3 - x1, Vy = y3 - y1, Vz = z3 - z1;
    GLfloat Nx = Uy * Vz - Uz * Vy;
    GLfloat Ny = Uz * Vx - Ux * Vz;
    GLfloat Nz = Ux * Vy - Uy * Vx;
    glNormal3f(Nx, Ny, Nz);
}

/* Draw a cube (all six faces) with material properties */
void drawCube1(GLfloat difX, GLfloat difY, GLfloat difZ,
    GLfloat ambX = 0, GLfloat ambY = 0, GLfloat ambZ = 0,
    GLfloat shine = 50)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { shine };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++)
    {
        // Compute normal for the face
        getNormal3p(
            v_cube[quadIndices[i][0]][0], v_cube[quadIndices[i][0]][1], v_cube[quadIndices[i][0]][2],
            v_cube[quadIndices[i][1]][0], v_cube[quadIndices[i][1]][1], v_cube[quadIndices[i][1]][2],
            v_cube[quadIndices[i][2]][0], v_cube[quadIndices[i][2]][1], v_cube[quadIndices[i][2]][2]);
        // Draw the face (4 vertices)
        glVertex3fv(v_cube[quadIndices[i][0]]);
        glVertex3fv(v_cube[quadIndices[i][1]]);
        glVertex3fv(v_cube[quadIndices[i][2]]);
        glVertex3fv(v_cube[quadIndices[i][3]]);
    }
    glEnd();
}

/* Draw the bathroom room: floor, three walls, ceiling */
void drawBathroom()
{

    // Left wall 
    glPushMatrix();
    glTranslatef(-6.0, 0.0, -6.0);
    glScalef(-0.1333, 2.7967, 5.0);
    drawCube1(0.6, 0.8, 0.9,
        0.3, 0.4, 0.45);
    glPopMatrix();

    // Right wall
    glPushMatrix();
    glTranslatef(6.0, 0.0, -6.0);
    glScalef(0.1333, 2.7967, 5.0);
    drawCube1(0.6, 0.8, 0.9,
        0.3, 0.4, 0.45);
    glPopMatrix();

    // Back wall 
    glPushMatrix();
    glTranslatef(-6.0, 0.0, -6.0);
    glScalef(4.0, 2.7967, 0.0333);
    drawCube1(0.6, 0.8, 0.9,
        0.3, 0.4, 0.45);
    glPopMatrix();

    // Ceiling 
    glPushMatrix();
    glTranslatef(-6.0, 8.0, -6.0);
    glScalef(4.0, 0.1333, 5.0);
    drawCube1(1.0, 1.0, 1.0,
        0.5, 0.5, 0.5);
    glPopMatrix();
}

// vẽ sàn nhà caro
void drawCheckerboard() {
    // Kích thước sàn theo trục X (chiều dài) và Z (chiều sâu)
    float floorWidth = 12.0f;   // từ -6 đến 6
    float floorDepth = 15.0f;   // từ -6 đến 9

    // Số ô vuông theo mỗi chiều
    int numSquaresX = 12;       // mỗi ô có kích thước 1 đơn vị theo X
    int numSquaresZ = 15;       // mỗi ô có kích thước 1 đơn vị theo Z

    // Kích thước của mỗi khối lập phương
    float cubeWidth = 1.0f;     // Chiều rộng
    float cubeHeight = 0.3f;    // Chiều cao
    float cubeDepth = 1.0f;     // Chiều sâu

    // Điểm bắt đầu (tương đương với dịch chuyển -6 theo X và Z)
    float startX = -6.0f;
    float startZ = -6.0f;

    // Offset theo trục Y để đặt các khối xuống dưới y = 0
    float yOffset = -cubeHeight; // Đặt toàn bộ khối xuống dưới, phần trên tại y = -0.5

    // Vẽ các khối lập phương
    for (int i = 0; i < numSquaresX; ++i) {
        for (int j = 0; j < numSquaresZ; ++j) {
            // Tính toán vị trí của khối lập phương
            float x = startX + i * cubeWidth;
            float z = startZ + j * cubeDepth;
            float y = yOffset; // Đặt tại vị trí y âm

            // Chọn màu: nếu (i+j) chẵn thì trắng, lẻ thì đen
            float color = (i + j) % 2 == 0 ? 1.0f : 0.0f;

            // Vẽ khối lập phương
            glPushMatrix();
            glTranslatef(x, y, z);
            // Scale theo các trục: X và Z giữ nguyên 1.0, Y điều chỉnh cho chiều cao 0.5
            glScalef(cubeWidth / 3.0f, cubeHeight / 3.0f, cubeDepth / 3.0f);
            drawCube1(color, color, color, color * 0.5f, color * 0.5f, color * 0.5f, 50);
            glPopMatrix();
        }
    }
}
//Khai báo một mảng toàn cục để lưu vector hướng của LIGHT3
GLfloat dirLightDirection[4] = { -1.0f, -1.0f, -1.0f, 0.0f };

// hàm cấu hình đèn định hướng
void dirLight() {
    glPushMatrix();

    GLfloat light_amb[] = { 0.2f, 0.1f, 0.1f, 1.0f };
    GLfloat light_diff[] = { 0.8f, 0.4f, 0.2f, 1.0f };
    GLfloat light_spec[] = { 0.9f, 0.5f, 0.3f, 1.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, dirLightDirection);
    glLightfv(GL_LIGHT3, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT3, GL_SPECULAR, light_spec);
    glPopMatrix();
}

/* First light source (GL_LIGHT0) */
void lightOne()
{
    glPushMatrix();
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
    // Position for light one (above right-front)
    GLfloat light_position[] = { 8.0, 8.0, -6.0, 1.0 };

    if (amb1)  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    else       glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
    if (diff1) glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    else       glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);
    if (spec1) glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    else       glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
}

/* Second light source (GL_LIGHT1) */
void lightTwo()
{
    glPushMatrix();
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
    // Position for light two (above left-front)
    GLfloat light_position[] = { -8.0, 8.0, -6.0, 1.0 };

    if (amb2)  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    else       glLightfv(GL_LIGHT1, GL_AMBIENT, no_light);
    if (diff2) glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    else       glLightfv(GL_LIGHT1, GL_DIFFUSE, no_light);
    if (spec2) glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    else       glLightfv(GL_LIGHT1, GL_SPECULAR, no_light);

    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glPopMatrix();
}

/* Third light source (GL_LIGHT2) - e.g., a ceiling lamp (spotlight) */
void lampLight()
{
    glPushMatrix();
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    // Position for lamp light (ceiling center)
    GLfloat light_position[] = { 0.0, 7.5, 0.0, 1.0 };
    // Spot direction (pointing downwards)
    GLfloat spot_direction[] = { 0.0, -1.0, 0.0 };

    if (amb3)  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    else       glLightfv(GL_LIGHT2, GL_AMBIENT, no_light);
    if (diff3) glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    else       glLightfv(GL_LIGHT2, GL_DIFFUSE, no_light);
    if (spec3) glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    else       glLightfv(GL_LIGHT2, GL_SPECULAR, no_light);

    glLightfv(GL_LIGHT2, GL_POSITION, light_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 40.0);
    glPopMatrix();
}
// Hàm vẽ bóng đèn cho GL_LIGHT0
void drawLightBulb0(bool isOn) {
    glPushMatrix();
    glTranslatef(6.0, 7.5, -4.0); // Vị trí GL_LIGHT0
    if (isOn) {
        GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 }; // Sáng trắng
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        GLfloat no_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // Tắt
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }
    glutSolidSphere(0.4, 16, 16); // Bóng đèn nhỏ
    glPopMatrix();
}

// Hàm vẽ bóng đèn cho GL_LIGHT1
void drawLightBulb1(bool isOn) {
    glPushMatrix();
    glTranslatef(-6.0, 7.5, -4.0); // Vị trí GL_LIGHT1
    if (isOn) {
        GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 }; // Sáng trắng
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        GLfloat no_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // Tắt
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }
    glutSolidSphere(0.4, 16, 16); // Bóng đèn nhỏ
    glPopMatrix();
}

// Hàm vẽ bóng đèn cho GL_LIGHT2 (đèn trần)
void drawLightBulb2(bool isOn) {
    glPushMatrix();
    glTranslatef(2.0, 8.0, 2.5); // Vị trí GL_LIGHT2
    if (isOn) {
        GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 }; // Sáng trắng
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        GLfloat no_emission[] = { 0.0, 0.0, 0.0, 1.0 }; // Tắt
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }
    glutSolidSphere(0.4, 16, 16); // Bóng đèn lớn hơn cho đèn trần
    glPopMatrix();
}


void drawDoor() {
    glPushMatrix();
 
    glTranslatef(6.2f, 0.0f, 6.0f);
    // Xoay -90° 
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    // --- PHẦN DƯỚI CỬA (nhựa, tông ấm) ---
    glPushMatrix();

    glScalef(1.0f, 1.0f, 0.0667f);

    drawCube1(0.8f, 0.7f, 0.5f, 0.4f, 0.3f, 0.2f, 50);
    glPopMatrix();

    // --- PHẦN TRÊN CỬA (kinh) ---
    glPushMatrix();
 
    glTranslatef(0.0f, 3.0f, 0.0f);
    glScalef(1.0f, 1.0f, 0.0667f);
    // Chọn màu kính
    drawCube1(0.9f, 0.9f, 0.95f, 0.4f, 0.4f, 0.45f, 150);
    glPopMatrix();

    // --- TAY NẮM CỬA ---
    glPushMatrix();

    glTranslatef(0.1f, 2.75f, 0.05f);
    // Tỉ lệ tay nắm:
    glScalef(0.1f, 0.1f, 0.1f);
    // Vẽ tay nắm với màu brass: diffuse (0.8, 0.6, 0.0), ambient (0.4, 0.3, 0.0)
    // và shininess 80
    drawCube1(0.8f, 0.6f, 0.0f, 0.4f, 0.3f, 0.0f, 80);
    glPopMatrix();

    glPopMatrix();
}

// Hàm vẽ bồn rửa mặt
void drawSmallSinkCabinet() {
    glPushMatrix();
    glTranslatef(6.0f, 0.15f, 1.0f);
    // Xoay tủ 
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    // 1. Vẽ thân tủ (cabinet body)
    glPushMatrix();
    glScalef(1.0f, 0.6667f, 0.3333f);  // 3×(1.0, 0.6667, 0.3333) = (3, 2, 1)
    // Chọn vật liệu gỗ tông ấm
    drawCube1(0.55f, 0.27f, 0.07f, 0.3f, 0.15f, 0.05f, 30);
    glPopMatrix();
    // 2. Vẽ mặt bàn (countertop)
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);  // Nằm ngay trên tủ
    // Scale: (1.0, 0.05, 0.3333) → (3, 0.15, 1)
    glScalef(1.0f, 0.05f, 0.3333f);
    drawCube1(0.7f, 0.4f, 0.1f, 0.4f, 0.2f, 0.05f, 50);
    glPopMatrix();
    // Vẽ vòi nước 
    glPushMatrix();
    // Đặt gốc của vòi tại trung tâm của bồn rửa:
    glTranslatef(1.5f, 3.0f, 0.01f);
    // --- Vẽ phần stem của vòi ---
    glPushMatrix();
    glScalef(0.05f, 0.05f, 0.15f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 80);
    glPopMatrix();
    // --- Vẽ phần spout của vòi (cong xuống) ---
    glPushMatrix();
    // Dịch đến đầu của stem: 0.45 
    glTranslatef(0.0f, 0.1f, 0.45f);
    // Xoay 30° quanh trục x để tạo hiệu ứng vòi cong xuống
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    // Dùng scale(0.05, 0.05, 0.10) cho spout (kích thước thật: 0.15, 0.15, 0.30)
    glScalef(0.05f, 0.05f, 0.10f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 80);
    glPopMatrix();
    glPopMatrix();

    // ===========================
    // 3. Vẽ 4 chân tủ
    float legScale = 0.05f;
    // Chân trước - trái (front left):
    glPushMatrix();
    glTranslatef(0.1f, -0.15f, 1.0f - 0.1f); // tại z ≈ 0.9
    glScalef(legScale, legScale, legScale);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 20);
    glPopMatrix();
    // Chân trước - phải (front right):
    glPushMatrix();
    glTranslatef(3.0f - 0.1f, -0.15f, 1.0f - 0.1f);
    glScalef(legScale, legScale, legScale);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 20);
    glPopMatrix();
    // Chân sau - trái (back left):
    glPushMatrix();
    glTranslatef(0.1f, -0.15f, 0.1f);
    glScalef(legScale, legScale, legScale);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 20);
    glPopMatrix();
    // Chân sau - phải (back right):
    glPushMatrix();
    glTranslatef(3.0f - 0.1f, -0.15f, 0.1f);
    glScalef(legScale, legScale, legScale);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 20);
    glPopMatrix();

    // ===========================
    // 5. Vẽ 2 cánh cửa tủ với khoảng cách giữa chúng
    float doorScaleX = 0.4667f;          // 1.4/3
    float doorScaleY = 0.6667f;          // 2/3
    float doorThickness = 0.01667f;      // 0.05/3
    float handleScale = 0.05f;           // kích thước tay cầm (0.15/3)

    // --- Cánh cửa bên trái ---
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.01f);
    glPushMatrix();
    glScalef(doorScaleX, doorScaleY, doorThickness);
    // Vật liệu cửa: diffuse (0.6, 0.4, 0.2), ambient (0.3, 0.2, 0.1)
    drawCube1(0.6f, 0.4f, 0.2f, 0.3f, 0.2f, 0.1f, 40);
    glPopMatrix();
    // Vẽ tay cầm cho cửa bên trái:
    glPushMatrix();
    glTranslatef(0.2f, 1.0f, 0.02f);
    glScalef(handleScale, handleScale, handleScale);
    drawCube1(0.8f, 0.6f, 0.0f, 0.4f, 0.3f, 0.0f, 80);
    glPopMatrix();
    glPopMatrix();
    // --- Cánh cửa bên phải ---
    glPushMatrix();
    // Dịch để đặt cánh cửa bên phải
    glTranslatef(1.6f, 0.0f, 1.01f);
    glPushMatrix();
    glScalef(doorScaleX, doorScaleY, doorThickness);
    drawCube1(0.6f, 0.4f, 0.2f, 0.3f, 0.2f, 0.1f, 40);
    glPopMatrix();
    // Vẽ tay cầm cho cửa bên phải: đặt tại tâm của panel (0.7, 1.0)
    glPushMatrix();
    glTranslatef(0.2f, 1.0f, 0.02f);
    glScalef(handleScale, handleScale, handleScale);
    drawCube1(0.8f, 0.6f, 0.0f, 0.4f, 0.3f, 0.0f, 80);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

// vẽ chậu rửa mặt
void drawToiletryTray2() {
    glPushMatrix();
    glTranslatef(6.0f, 2.3f, 1.43f);
    glScalef(2, 4, 2);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    // --- Vẽ đáy khay (base) ---
    glPushMatrix();
    glScalef(1.2f / 3.0f, 0.05f / 3.0f, 0.5f / 3.0f);
    // Vật liệu: chọn màu xám nhạt
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();

    // --- Vẽ các thành viền (rim) của khay ---
    // 1. Thành bên trái:
    glPushMatrix();
    // Vị trí: Ở bên trái, tức x = 0.
    glTranslatef(0.0f, 0.05f, 0.0f);
    // Scale cho thành trái
    glScalef(0.03f / 3.0f, 0.07f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 2. Thành bên phải:
    glPushMatrix();
    // Vị trí: bên phải
    glTranslatef(1.17f, 0.05f, 0.0f);
    glScalef(0.03f / 3.0f, 0.07f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 3. Thành mặt trước:
    glPushMatrix();
    // Vị trí: mặt trước, dán sát cạnh trước của khay.
    // Cạnh trước ở z 
    glTranslatef(0.0f, 0.05f, 0.47f);
    glScalef(1.2f / 3.0f, 0.07f / 3.0f, 0.03f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 4. Thành mặt sau:
    glPushMatrix();
    // Vị trí: mặt sau, tại z = 0.
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(1.2f / 3.0f, 0.07f / 3.0f, 0.03f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    glPopMatrix();
}
//hàm vẽ gương
void drawMirror() {
    glPushMatrix();
    glTranslatef(6.0f, 3.4f, 1.0f);
    // xoay -90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
  
    glScalef(1.0f, 0.6f, 0.00333f);
    drawCube1(0.9f, 0.9f, 1.0f, 0.4f, 0.4f, 0.45f, 200);
    glPopMatrix();
}

// Hàm vẽ thanh treo khăn
void drawTowelRack() {
    glPushMatrix();
    glTranslatef(5.9f, 4.0f, -3.0f);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    // 1. Vẽ chân trái (vertical left leg)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(0.01f, 0.0333f, 0.01f);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 50);
    glPopMatrix();
    // 2. Vẽ chân phải 
    glPushMatrix();
    glTranslatef(2.97f, 0.0f, 0.0f);
    glScalef(0.01f, 0.0333f, 0.01f);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 50);
    glPopMatrix();
    // 3. Vẽ thanh nối dưới
    glPushMatrix();
    glTranslatef(0.03f, 0.0f, 0.0f);
    glScalef(0.98f, 0.01f, 0.01f);
    drawCube1(0.3f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 50);
    glPopMatrix();
    glPopMatrix();
}

// Hàm vẽ bồn cầu (toilet) 
void drawToilet() {
    glPushMatrix();
  
    glTranslatef(3.2f, 0.0f, -4.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    // 1. Thân bồn cầu
    glPushMatrix();
    glScalef(0.5f, 0.4f, 0.6667f);
    drawCube1(0.9f, 0.9f, 0.9f, 0.5f, 0.5f, 0.5f, 150);
    glPopMatrix();

    // 2. Bể nước
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 2.0f);
    glScalef(0.5f, 0.7f, 0.2f);
    drawCube1(0.9f, 0.9f, 0.9f, 0.5f, 0.5f, 0.5f, 150);
    glPopMatrix();
    // 3. Nắp bồn cầu
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.1f); 
    glScalef(0.5f, 0.2f, 0.6f); 
    drawCube1(0.9f, 0.9f, 0.9f, 0.5f, 0.5f, 0.5f, 150);
    glPopMatrix();
    // 4. Ống dẫn nước 
    glPushMatrix();
    glTranslatef(-0.5f, 0.5f, 2.6f); 
    glScalef(0.0333f, 0.3f, 0.0333f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 80); 
    glPopMatrix();
    glPopMatrix();
}
// Hàm vẽ chậu tắm màu trắng sứ 
void drawBathtub() {
    glPushMatrix();

    glTranslatef(-0.95f, 0.0f, 6.5f);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.1f, 0.0f, 0.1f);
    glScalef(2.3f / 3.0f, 0.1f / 3.0f, 4.8f / 3.0f);
    // Dùng màu nền trắng sứ  
    drawCube1(0.95f, 0.95f, 0.98f, 0.5f, 0.5f, 0.55f, 200);
    glPopMatrix();
    // 2. Back Wall (tường sau)
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);
    glScalef(2.5f / 3.0f, 1.75f / 3.0f, 0.1f / 3.0f);
    drawCube1(0.95f, 0.95f, 0.98f, 0.5f, 0.5f, 0.55f, 200);
    glPopMatrix();
    // 3. Left Wall (tường bên trái)
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);
    glScalef(0.1f / 3.0f, 1.75f / 3.0f, 5.0f / 3.0f);
    drawCube1(0.95f, 0.95f, 0.98f, 0.5f, 0.5f, 0.55f, 200);
    glPopMatrix();
    // 4. Right Wall (tường bên phải)
    glPushMatrix();
    glTranslatef(2.4f, 0.1f, 0.0f);
    glScalef(0.1f / 3.0f, 1.75f / 3.0f, 5.0f / 3.0f);
    drawCube1(0.95f, 0.95f, 0.98f, 0.5f, 0.5f, 0.55f, 200);
    glPopMatrix();
    // 5. Front Rim (viền trước)
    glPushMatrix();
    glTranslatef(0.0f, 0.1f, (5.0f - 0.1f));
    glScalef(2.5f / 3.0f, 1.75f / 3.0f, 0.1f / 3.0f);
    drawCube1(0.95f, 0.95f, 0.98f, 0.5f, 0.5f, 0.55f, 200);
    glPopMatrix();
    glPopMatrix();
}

// Hàm vẽ 3 vòi nước để xả vào chậu tắm.
void drawBathtubFaucets() {
    glPushMatrix();
    glTranslatef(2.45f, 1.9f, 0.0f);
    float faucetZPos[3] = { 7.0f , 8.5f };

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(-8.0f, 0.3f, faucetZPos[i]);
        // xoay 90
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        // --- Vẽ phần Stem 
        glPushMatrix();
        glScalef(0.05f, 0.05f, 0.15f);
        drawCube1(0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 80);
        glPopMatrix();
        // --- Vẽ phần Spout của vòi
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.15f);
        glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.05f, 0.05f, 0.10f);
        drawCube1(0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 80);
        glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}
// vòi hoa sen trên bồn tắm
void drawShowerSetEnhanced0() {


    glPushMatrix();
    glTranslatef(-6.0, 2.2f, 8.0f);
    // xoay 90
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    // ===== PHẦN A: Vẽ các nút vặn 
    glPushMatrix();
    glTranslatef(-0.5, 0.0f, 0.0f);
    glScalef(0.5f, 0.05f, 0.05f);
    // Vẽ nút với màu tối
    drawCube1(0.3f, 0.3f, 0.3f, 0.15f, 0.15f, 0.15f, 40);
    glPopMatrix();
    // ===== PHẦN B: Vẽ Stem 
    glPushMatrix();
    glTranslatef(0.0f, 0.06f, 0.0f);
    glScalef(0.03f, 1.0f, 0.03f);
    drawCube1(0.7f, 0.7f, 0.7f, 0.4f, 0.4f, 0.4f, 80);
    glPopMatrix();
    // ===== PHẦN C: Vẽ Faucet Head
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);
    // Xoay 45° 
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glScalef(0.05f, 0.05f, 0.20f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();

    // ===== PHẦN D: Vẽ Drain Outlet
    glPushMatrix();
    glTranslatef(-0.2f, -0.3f, 0.6f);
    glScalef(0.2f, 0.2f, 0.01f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 50);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}
// Hàm vẽ hệ vòi hoa sen 
void drawShowerSetEnhanced() {
    glPushMatrix();
    glTranslatef(-5, 3.0f, -5.8f);
    // ===== PHẦN A: Vẽ các nút vặn 
    glPushMatrix();
    glTranslatef(-1.0, 0.0f, 0.0f);
    glScalef(0.630f, 0.05f, 0.05f);
    drawCube1(0.3f, 0.3f, 0.3f, 0.15f, 0.15f, 0.15f, 40);
    glPopMatrix();

    // ===== PHẦN B: Vẽ Stem
    glPushMatrix();
    glTranslatef(0.0f, 0.06f, 0.0f);
    glScalef(0.03f, 1.0f, 0.03f);
    drawCube1(0.7f, 0.7f, 0.7f, 0.4f, 0.4f, 0.4f, 80);
    glPopMatrix();
    // ===== PHẦN C: Vẽ vòi dài
    glPushMatrix();
    glTranslatef(0.0f, 3.0f, 0.0f);
    // Xoay 45° 
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glScalef(0.05f, 0.05f, 0.20f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // ===== PHẦN D: Vẽ chỗ xả nước
    glPushMatrix();
    glTranslatef(-0.2f, -0.3f, 0.6f);
    glScalef(0.2f, 0.2f, 0.01f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 50);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// Hàm vẽ khay đựng các đồ vệ sinh gần vòi hoa sen
void drawToiletryTray() {
    glPushMatrix();
    glTranslatef(-4.0f, 3.0f, -5.8f);

    // --- Vẽ đáy khay (base) 
    glPushMatrix();
    glScalef(1.2f / 3.0f, 0.05f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();

    // --- Vẽ các thành viền (rim) của khay ---
    // 1.Thành bên trái:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(0.03f / 3.0f, 0.07f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 2. Thành bên phải:
    glPushMatrix();
    glTranslatef(1.17f, 0.05f, 0.0f);
    glScalef(0.03f / 3.0f, 0.07f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 3. Thành mặt trước:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.47f);
    // Scale: width = toàn bộ width của khay = 1.2, height = 0.07, thickness = 0.03.
    glScalef(1.2f / 3.0f, 0.07f / 3.0f, 0.03f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 4. Thành mặt sau:
    glPushMatrix();
    // Vị trí: mặt sau, tại z = 0.
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(1.2f / 3.0f, 0.07f / 3.0f, 0.03f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    glPopMatrix();
}
// Hàm vẽ hai lọ dầu gội màu đỏ
void drawShampooBottles() {
    glPushMatrix();
    glTranslatef(-3.5f, 3.01f, -5.6f);
    // --- Vẽ lọ dầu gội thứ nhất ---
    glPushMatrix();
    glTranslatef(-0.3f, 0.0f, 0.0f);
    glScalef(0.2f / 3.0f, 0.5f / 3.0f, 0.2f / 3.0f);
    drawCube1(1.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 80);
    glPopMatrix();
    // --- Vẽ lọ dầu gội thứ hai ---
    glPushMatrix();
    glTranslatef(0.3f, 0.0f, 0.0f);
    glScalef(0.2f / 3.0f, 0.5f / 3.0f, 0.2f / 3.0f);
    drawCube1(1.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 80);
    glPopMatrix();
    glPopMatrix();
}
// Hàm vẽ khay đựng các đồ vệ sinh (toiletry tray) ở gần bồn tắm.
void drawToiletryTray1() {
    glPushMatrix();
    glTranslatef(-5.5f, 2.0f, 4.6f);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    // --- Vẽ đáy khay
    glPushMatrix();
    glScalef(1.2f / 3.0f, 0.05f / 3.0f, 0.5f / 3.0f);
    // Vật liệu: chọn màu xám nhạt 
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // --- Vẽ các thành viền (rim) của khay ---
    // 1. Thành bên trái:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(0.03f / 3.0f, 0.07f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 2. Thành bên phải:
    glPushMatrix();
    glTranslatef(1.17f, 0.05f, 0.0f);
    glScalef(0.03f / 3.0f, 0.07f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 3. Thành mặt trước:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.47f);
    glScalef(1.2f / 3.0f, 0.07f / 3.0f, 0.03f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    // 4. Thành mặt sau:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(1.2f / 3.0f, 0.07f / 3.0f, 0.03f / 3.0f);
    drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 100);
    glPopMatrix();
    glPopMatrix();
}

// Hàm vẽ các chai dầu gội đặt trên khay đựng đồ vệ sinh (toiletry tray).
void drawShampooBottlesOnTray() {
    glPushMatrix();
    glTranslatef(-6.0f, 2.01f, 5.9f);
    // xoay 90
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    float bottleX[3] = { 0.2f, 0.6f, 1.0f };
    float bottleZ = 0.25f;

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        // Dịch các chai đến vị trí mong muốn trên khay.
        glTranslatef(bottleX[i], 0.0f, bottleZ);

        // --- Vẽ thân chai (bottle body) ---
        glPushMatrix();
        glScalef(0.05f, 0.13333f, 0.05f);
        drawCube1(0.0f, 0.7f, 0.0f, 0.0f, 0.3f, 0.0f, 50);
        glPopMatrix();
        // --- Vẽ nắp chai (cap) ---
        glPushMatrix();
        glTranslatef(0.0f, 0.13333f, 0.0f);
        glScalef(0.05f, 0.01667f, 0.05f);
        // Vẽ nắp chai 
        drawCube1(0.7f, 0.7f, 0.7f, 0.4f, 0.4f, 0.4f, 80);
        glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}
// Hàm vẽ công thoát nước 
void drawFloorDrain() {
    glPushMatrix();
    glTranslatef(-5.8f, 0.001f, -5.8f);
    glScalef(0.4f / 3.0f, 0.05f / 3.0f, 0.4f / 3.0f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 50);
    glPopMatrix();
}

void drawWaterHeater() {
    glPushMatrix();
    glTranslatef(3.0f, 6.5f, -5.8f);
    glScalef(0.8f, 0.4f, 0.2f);
    // Vẽ bình 
    drawCube1(1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.8f, 50);
    glPopMatrix();
}

//hàm vẽ ổ điện
void odien() {
    // ổ điện
    glPushMatrix();
    glTranslatef(6.0f, 3.2f, 5.0f);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.2f, 0.1f, 0.00333f);
    drawCube1(0.9f, 0.9f, 1.0f, 0.4f, 0.4f, 0.45f, 200);
    glPopMatrix();
    //nút bấm
    glPushMatrix();
    glTranslatef(5.99777f, 3.2555f, 5.2f);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.05f, 0.05f, 0.00333f);
    drawCube1(0.0f, 0.0f, 0.0f, 0.4f, 0.4f, 0.45f, 200);
    glPopMatrix();
}
// vẽ thảm lau chân
void drawDoormat() {
    glPushMatrix();
    glTranslatef(4.0f, 0.01f, 6.25f);
    glScalef(0.5f, 0.00667f, 0.9f);
    drawCube1(0.6f, 0.1f, 0.1f, 0.4f, 0.2f, 0.2f, 70);
    glPopMatrix();
}
// Hàm vẽ hệ thống móc treo quần áo
void drawClothesHangerSystem() {
    glPushMatrix();
    glTranslatef(-5.8f, 5.0f, 3.0f);
    // xoay 90
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
    // --- Vẽ thanh ngang
    glPushMatrix();
    glScalef(0.5f, 0.01667f, 0.03333f);
    glTranslatef(-1.5f, 0.0f, 0.0f);
    // Dùng vật liệu kim loại xám
    drawCube1(0.7f, 0.7f, 0.7f, 0.3f, 0.3f, 0.3f, 80);
    glPopMatrix();
    // --- Vẽ các móc treo quần áo dưới thanh ngang ---
    int numHooks = 5;
    float barWidth = 1.5f;  
    float spacing = barWidth / (numHooks + 1);
    for (int i = 0; i < numHooks; i++) {
        glPushMatrix();
        float hookX = -barWidth / 2 + spacing * (i + 1);
        glTranslatef(hookX, 0.0f, 0.0f);

        // --- Vẽ phần stem
        glPushMatrix();
        glTranslatef(0.0f, -0.05f, 0.0f);
        glScalef(0.02f / 3.0f, 0.15f / 3.0f, 0.02f / 3.0f);
        drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 80);
        glPopMatrix();
        // --- Vẽ phần đầu móc
        glPushMatrix();
        glTranslatef(0.0f, -0.15f, 0.0f);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.03f / 3.0f, 0.1f / 3.0f, 0.03f / 3.0f);
        drawCube1(0.8f, 0.8f, 0.8f, 0.4f, 0.4f, 0.4f, 80);
        glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}

// Hàm vẽ quạt hút mùi 
void drawExhaustFanStatic() {
    glPushMatrix();
    glTranslatef(-5.7f, 6.5f, -6.0f);

    // --- Vẽ Vỏ Quạt
    glPushMatrix();
    glScalef(1.3f / 3.0f, 1.30f / 3.0f, 0.5f / 3.0f);
    drawCube1(0.9f, 0.9f, 0.9f, 0.5f, 0.5f, 0.5f, 100);
    glPopMatrix();

    // --- Vẽ Hub và các Cánh Quạt
    glPushMatrix();
    glTranslatef(0.65f, 0.65f, 1.05f);

    // --- Vẽ Fan Hub
    glPushMatrix();
    glScalef(0.1f / 3.0f, 0.1f / 3.0f, 0.1f / 3.0f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 150);
    glPopMatrix();

    //// --- Vẽ các Cánh Quạt Tĩnh ---
    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        // xoay để phân bố 3 cánh
        glRotatef(i * 120.0f, 0.2f, 0.2f, 1.0f);
        // đặt cánh cách tâm quạt
        glTranslatef(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLES);
        glColor3f(0.2f, 0.6f, 0.9f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        // đỉnh mở rộng dài hơn
        glVertex3f(0.5f, 0.1f, 0.0f);
        glVertex3f(0.5f, -0.1f, 0.0f);
        glEnd();
        glPopMatrix();
    }
 
    glPopMatrix();
    glPopMatrix();
}
// Hàm vẽ thùng rác 
void drawTrashCan() {
    glPushMatrix();
     glTranslatef(5.3f, 0.0f, 4.2f);

    // ----- Vẽ Đáy Thùng -----
    glPushMatrix();
    glScalef(0.6f / 3.0f, 0.05f / 3.0f, 0.6f / 3.0f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 80);
    glPopMatrix();

    // ----- Vẽ Các Thành (trừ mặt trên, để tạo phần mở) -----
    // + Thành bên trái:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(0.05f / 3.0f, 0.95f / 3.0f, 0.6f / 3.0f);
    // Vẽ thành với màu đồng nhất với đáy.
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 80);
    glPopMatrix();
    // + Thành bên phải:
    glPushMatrix();
    glTranslatef(0.55f, 0.05f, 0.0f);
    glScalef(0.05f / 3.0f, 0.95f / 3.0f, 0.6f / 3.0f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 80);
    glPopMatrix();
    // + Thành mặt sau:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.0f);
    glScalef(0.6f / 3.0f, 0.95f / 3.0f, 0.05f / 3.0f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 80);
    glPopMatrix();
    // + Thành mặt trước:
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, 0.55f);
    glScalef(0.6f / 3.0f, 0.95f / 3.0f, 0.05f / 3.0f);
    drawCube1(0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 80);
    glPopMatrix();

    glPopMatrix();
}

// Hàm vẽ 2 vòi nước dán tường
void drawFaucet() {
    glPushMatrix();
    glTranslatef(2.45f, 1.5f, -7.0f);

    // Chọn 2 vị trí dọc theo trục z để đặt 2 vòi
    float faucetZPos[2] = { 7.9f , 8.5f };

    for (int i = 0; i < 2; i++) {
        glPushMatrix();
        glTranslatef(-8.0f, 0.3f, faucetZPos[i]);
        // xoay 90
        glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
        // --- Vẽ phần Stem của vòi ---
        glPushMatrix();
        glScalef(0.05f, 0.05f, 0.15f);
        drawCube1(0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 80);
        glPopMatrix();
        // --- Vẽ phần Spout của vòi (cong xuống) ---
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.15f);
        glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.05f, 0.05f, 0.10f);
        drawCube1(0.5f, 0.5f, 0.5f, 0.2f, 0.2f, 0.2f, 80);
        glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}

/* Display callback */
void display(void)
{
    glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up camera perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    gluPerspective(60.0, windowWidth / windowHeight, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, refX, refY, refZ, 0.0, 1.0, 0.0);

    // Enable lighting and call light setup
    glEnable(GL_LIGHTING);
    if (switchOne)  glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    if (switchTwo)  glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
    if (switchLamp) glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);
    lightOne();
    lightTwo();
    lampLight();
    // Gọi các hàm vẽ bóng đèn
    drawLightBulb0(switchOne);
    drawLightBulb1(switchTwo);
    drawLightBulb2(switchLamp);
    if (switchDirLight) dirLight();
    // Nếu đèn directional đang bật ⇒ bóng đèn phát sáng
    if (switchDirLight) {
        // Vật liệu tự phát (emission) để mô phỏng bóng đèn đang sáng
        GLfloat mat_emission[] = { 1.0f, 0.8f, 0.2f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    }
    else {
        // Trở về vật liệu bình thường (no emission)
        GLfloat no_emission[] = { 0,0,0,1 };
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }
    // Vẽ một sphere nhỏ tại vị trí đèn trần
    glPushMatrix();
    glTranslatef(-2.0f, 8.0f, 2.5f); // trùng với light2 hoặc giữa trần
    glutSolidSphere(0.5, 16, 16);
    glPopMatrix();
    // Sau khi vẽ xong, reset emission về zero để không ảnh hưởng vật thể khác
    {
        GLfloat no_emission[] = { 0,0,0,1 };
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    }

    // Vẽ nền caro
    drawCheckerboard();
    // Draw the bathroom (walls, floor, ceiling)
    drawBathroom();
    // hàm vẽ cửa
    drawDoor();
    //gọi hàm vẽ tủ 
    drawSmallSinkCabinet();
    //gọi hàm vẽ gương
    drawMirror();
    // hàm vẽ treo khăn mặt
    drawTowelRack();
    // hàm vẽ toilet
    drawToilet();
    // hàm vẽ chậu tắm
    drawBathtub();
    // vẽ vòi hoa sen trên bồn
    drawShowerSetEnhanced0();
    // hàm vẽ vòi chậu tắm
    drawBathtubFaucets();
    // hàm vẽ vòi hoa sen
    drawShowerSetEnhanced();
    // khay dung do gần vòi hoa sen
    drawToiletryTray();
    // vẽ dàu gội gần vòi hoa sen
    drawShampooBottles();
    // khay2
    drawToiletryTray1();
    drawShampooBottlesOnTray();
    // vẽ cống thoát nước
    drawFloorDrain();
    // vẽ chậu rửa mặt
    drawToiletryTray2();
    // nóng lạnh
    drawWaterHeater();
    // ổ điện
    odien();
    // tham lau chan
    drawDoormat();
    //vẽ móc treo qao
    drawClothesHangerSystem();
    // hàm vẽ quạt hút mùi
    drawExhaustFanStatic();
    // thùng rác
    drawTrashCan();
    // vòi nước dán tường
    drawFaucet();

    // Vẽ trục tọa độ
 /*   drawAxes();*/
    glDisable(GL_LIGHTING);
    glutSwapBuffers();

}

/* Keyboard callback */
void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        // Camera controls (eye position)
    case 'w': eyeY += 1.0; break;
    case 's': eyeY -= 1.0; break;
    case 'a': eyeX -= 1.0; break;
    case 'd': eyeX += 1.0; break;
    case 'i': eyeZ += 1.0; break; // zoom in
    case 'o': eyeZ -= 1.0; break; // zoom out
    case 'q': // reset camera
        eyeX = -4.0; eyeY = 7.0; eyeZ = 18.0;
        refX = 0.0; refY = 0.0; refZ = -20.0;
        break;
        // Reference point controls
    case 'j': refY += 1.0; break;
    case 'n': refY -= 1.0; break;
    case 'b': refX -= 1.0; break;
    case 'm': refX += 1.0; break;
    case 'k': refZ += 1.0; break;
    case 'l': refZ -= 1.0; break;

        // Light 1 controls
    case '0':  // phím '0' để bật/tắt directional light
        switchDirLight = !switchDirLight;
        if (switchDirLight) glEnable(GL_LIGHT3);
        else                glDisable(GL_LIGHT3);
        break;
    case '1':
        switchOne = !switchOne;
        if (switchOne) { amb1 = diff1 = spec1 = true; }
        break;
    case '4': amb1 = !amb1; break;
    case '5': diff1 = !diff1; break;
    case '6': spec1 = !spec1; break;

        // Light 2 controls
    case '2':
        switchTwo = !switchTwo;
        if (switchTwo) { amb2 = diff2 = spec2 = true; }
        break;
    case '7': amb2 = !amb2; break;
    case '8': diff2 = !diff2; break;
    case '9': spec2 = !spec2; break;

        // Lamp (spot) controls
    case '3':
        switchLamp = !switchLamp;
        if (switchLamp) { amb3 = diff3 = spec3 = true; }
        break;
    case 'e': amb3 = !amb3; break;
    case 'r': diff3 = !diff3; break;
    case 't': spec3 = !spec3; break;
        // các phím chỉnh hướng đèn light3
    case ',': dirLightDirection[0] += 0.1f; break;   // Tăng x
    case '.': dirLightDirection[0] -= 0.1f; break;   // Giảm x
    case '[': dirLightDirection[1] += 0.1f; break;   // Tăng y
    case ']': dirLightDirection[1] -= 0.1f; break;  // Giảm y
    case '+': dirLightDirection[2] += 0.1f; break;   // Tăng z
    case '-': dirLightDirection[2] -= 0.1f; break;  // Giảm z
    case 27: // ESC key
        exit(0);
        break;

    }
    glutPostRedisplay();
}
void printInstructions() {
    std::cout << "Hướng dẫn sử dụng phím điều khiển:" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Điều khiển camera (vị trí mắt):" << std::endl;
    std::cout << "  'w': Di chuyển camera lên (tăng eyeY)" << std::endl;
    std::cout << "  's': Di chuyển camera xuống (giảm eyeY)" << std::endl;
    std::cout << "  'a': Di chuyển camera sang trái (giảm eyeX)" << std::endl;
    std::cout << "  'd': Di chuyển camera sang phải (tăng eyeX)" << std::endl;
    std::cout << "  'i': Phóng to (tăng eyeZ)" << std::endl;
    std::cout << "  'o': Thu nhỏ (giảm eyeZ)" << std::endl;
    std::cout << "  'q': Đặt lại camera về vị trí mặc định" << std::endl;
    std::cout << "Điều khiển điểm tham chiếu:" << std::endl;
    std::cout << "  'j': Di chuyển điểm tham chiếu lên (tăng refY)" << std::endl;
    std::cout << "  'n': Di chuyển điểm tham chiếu xuống (giảm refY)" << std::endl;
    std::cout << "  'b': Di chuyển điểm tham chiếu sang trái (giảm refX)" << std::endl;
    std::cout << "  'm': Di chuyển điểm tham chiếu sang phải (tăng refX)" << std::endl;
    std::cout << "  'k': Di chuyển điểm tham chiếu về phía trước (tăng refZ)" << std::endl;
    std::cout << "  'l': Di chuyển điểm tham chiếu về phía sau (giảm refZ)" << std::endl;
    std::cout << "Điều khiển đèn:" << std::endl;
    std::cout << "  '0': Bật/tắt đèn định hướng (GL_LIGHT3)" << std::endl;
    std::cout << "  '1': Bật/tắt đèn 1 (GL_LIGHT0)" << std::endl;
    std::cout << "  '4': Bật/tắt ambient của đèn 1" << std::endl;
    std::cout << "  '5': Bật/tắt diffuse của đèn 1" << std::endl;
    std::cout << "  '6': Bật/tắt specular của đèn 1" << std::endl;
    std::cout << "  '2': Bật/tắt đèn 2 (GL_LIGHT1)" << std::endl;
    std::cout << "  '7': Bật/tắt ambient của đèn 2" << std::endl;
    std::cout << "  '8': Bật/tắt diffuse của đèn 2" << std::endl;
    std::cout << "  '9': Bật/tắt specular của đèn 2" << std::endl;
    std::cout << "  '3': Bật/tắt đèn chiếu (GL_LIGHT2)" << std::endl;
    std::cout << "  'e': Bật/tắt ambient của đèn chiếu" << std::endl;
    std::cout << "  'r': Bật/tắt diffuse của đèn chiếu" << std::endl;
    std::cout << "  't': Bật/tắt specular của đèn chiếu" << std::endl;
    std::cout << "Điều khiển hướng đèn định hướng:" << std::endl;
    std::cout << "  ',': Tăng hướng X" << std::endl;
    std::cout << "  '.': Giảm hướng X" << std::endl;
    std::cout << "  '[': Tăng hướng Y" << std::endl;
    std::cout << "  ']': Giảm hướng Y" << std::endl;
    std::cout << "  '+': Tăng hướng Z" << std::endl;
    std::cout << "  '-': Giảm hướng Z" << std::endl;
    std::cout << "Thoát chương trình:" << std::endl;
    std::cout << "  ESC: Thoát" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Luân Vinh Duy <3" << std::endl;
}
/* Window reshape callback */
void reshape(int w, int h)
{
    if (h == 0) return;
    windowWidth = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}
/* Main function */
int main(int argc, char** argv)
{
    // Initialize GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(500, 500);
    glutCreateWindow("Bathroom Scene");
    // gọi hàm hiển thị hướng dẫn
    printInstructions();
    // Set up rendering state
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT3);
    // Register callbacks
    glutDisplayFunc(display);

    glutReshapeFunc(reshape);
    glutKeyboardFunc(myKeyboardFunc);

    // Enter main loop
    glutMainLoop();
    return 0;
}

