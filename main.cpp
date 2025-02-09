#include <GL/glew.h> 
#include <GL/glut.h>  
#include <GLFW/glfw3.h>  
#include<iostream>
#include<SOIL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<math.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include"milk_way.h"//�Ǻ�
#include"creat_nebula.h"//����

#define WIDTH 1000
#define HEIGHT 1000
unsigned int texture;

// ȫ�ֱ���
const int STAR_COUNT = 100;  // ���ǵ�����
float starX[STAR_COUNT];     // ���ǵ� x ����
float starY[STAR_COUNT];     // ���ǵ� y ����
float starZ[STAR_COUNT];     // ���ǵ� z ����
float mouseX = 0.0f;         // ���� x ����
float mouseY = 0.0f; 

static GLfloat angle1 = 0.0f, angle4, angle5, angle6, angle7, angle8; // ����ת�Ƕ�
static GLfloat angle2 = 0.0f; // ���ǹ�ת�Ƕ�
static GLfloat angle3 = 0.0f; // �����Ƶ���ת�Ƕ�

// ����ӽǲ���
float cameraX = 0.0f;
float cameraY = 12.0f;
float cameraZ = -20.0f;
float centerX = 0.0f;
float centerY = 0.0f;
float centerZ = 0.0f;
float upX = 0.0f;
float upY = 10.5f;
float upZ = 0.0f;

// �����һλ��
int lastMouseX = 0;
int lastMouseY = 0;

// ���������
const float mouseSensitivity = 0.005f;

// С���Ǵ�
const int ASTEROID_COUNT = 200;
float asteroidX[ASTEROID_COUNT];
float asteroidY[ASTEROID_COUNT];
float asteroidZ[ASTEROID_COUNT];
float asteroidSize[ASTEROID_COUNT];
float asteroidAngle[ASTEROID_COUNT];

Planet creat_planet;
Cycle cycle;
MilkWay milkyWay;
Nebula nebula;

void initTextures() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // ���ز���������
    int sun_width, sun_height, nrChannels;
   // stbi_set_flip_vertically_on_load(true); // ��ת�����Y��
    unsigned char* data = stbi_load("sun.jpg", &sun_width, &sun_height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sun_width, sun_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
        exit(1); // �˳�����
    }
    stbi_image_free(data);
}

// ��ͼλ��
void drawTexturedSquare(float x, float y, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    // ���½�
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    // ���½�
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    // ���Ͻ�
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    // ���Ͻ�
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void mouseMotion(int x, int y) {
   //ƫ����
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;
    // ��������ӽ�
    cameraX += dx * mouseSensitivity;
    cameraY -= dy * mouseSensitivity;

    // ���������һλ��
    lastMouseX = x;
    lastMouseY = y;

    float aspect = (float)WIDTH / (float)HEIGHT;
    mouseX = (2.0f * x / WIDTH - 1.0f) * aspect;
    mouseY = 1.0f - 2.0f * y / HEIGHT;
}

void myDisplay(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ����͸��Ч����ͼ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120.0f, 1.0f, 1.0f, 20.0f); // �������������� 1e6
    // 1���ӽǴ�С;    2����������ʾ�ڻ����ϵ�x��y�����ϵı��� ,��Ϊ1���ᰴʵ�ʷ�Ӧ�����
    //3���ֵԽ�󣬱�ʾ�۲������������ԽԶ
    //4����ɹ۲⵽���������Զ�������������ľ���
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, centerX, centerY, centerZ, upX, upY, upZ);//ǰ���� ��ʾ�������������ϵ�е�λ��
    //�м�����������������ŵ���������ϵ�еĵ��λ�����꣬�������һ���λ�ڻ��������λ��
    //����������������
    // ����ĳ�������������������������ϵ�е�����㣬�������Ϊ��վ���������ͷ�ĳ�����������������������ϵ�е�����㣬�����������ϵ�ģ�ֻ���������巽��

    
    drawsun();
    // ����̫��
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    float x = -1.15f;
    float y = -1.15f;
    float squareWidth = 2.38f;
    float squareHeight = 2.38f;
    
    drawTexturedSquare(x, y, squareWidth, squareHeight);
    //drawsun();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    // �������Ĳ��ʲ����Ƶ���
    GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_shininess = 30.0f;

    glPushMatrix(); // ��ʼ�������ı任״̬
    {
        creat_planet.drawPlanet(0.32f, 5.4f, angle3, earth_mat_ambient, earth_mat_diffuse, earth_mat_specular, earth_mat_emission, earth_mat_shininess);

        // ��������Ĳ��ʲ���������
        GLfloat moon_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_diffuse[] = { 0.72f, 0.73f, 0.47f, 1.0f };
        GLfloat moon_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_emission[] = { 0.72f, 0.73f, 0.47f, 1.0f };
        GLfloat moon_mat_shininess = 10.0f;

        // ������ת��ͬʱ���������Ƶ���ת
        glPushMatrix(); // ���浱ǰ�任���󣬼�����ı任����
        {
            glRotatef(angle3, 0.0f, 1.0f, 0.0f);
            glTranslatef(5.4f, 0.0f, 0.0f);
            glRotatef(angle2, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.55f, 0.0f, 0.0f);
            glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
            glMaterialf(GL_FRONT, GL_SHININESS, moon_mat_shininess);
            glutSolidSphere(0.06f, 40, 32);
        }
        glPopMatrix(); // �ָ�����ı任����

        // ������ǵĲ��ʲ����ƻ���
        GLfloat mars_mat_ambient[] = { 0.3f, 0.0f, 0.0f, 1.0f };
        GLfloat mars_mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
        GLfloat mars_mat_specular[] = { 0.6f, 0.0f, 0.0f, 1.0f };
        GLfloat mars_mat_emission[] = { 0.3f, 0.0f, 0.0f, 1.0f };
        GLfloat mars_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.3f, 6.6f, angle4, mars_mat_ambient, mars_mat_diffuse, mars_mat_specular, mars_mat_emission, mars_mat_shininess);

        //����ˮ��
        GLfloat mercury_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };//û�й��յ�ʱ��: ǳ��ɫ
        GLfloat mercury_mat_diffuse[] = { 0.4f, 0.4f, 0.35f, 1.0f };//������: ǳ��ɫ
        GLfloat mercury_mat_specular[] = { 0.0f, 0.1f, 0.1f, 1.0f };//���淴ɫ����
        GLfloat mercury_mat_emission[] = { 0.2f, 0.2f, 0.2f, 0.1f };//��������
        GLfloat mercury_mat_shininess = 5.0f;//����ȵ�

        creat_planet.drawPlanet(0.15f, 2.8f, angle1, mercury_mat_ambient, mercury_mat_diffuse, mercury_mat_specular, mercury_mat_emission, mercury_mat_shininess);

        //���ƽ���
        GLfloat venus_mat_ambient[] = { 0.8f, 0.7f, 0.4f, 1.0f };//��ͻ�ɫ
        GLfloat venus_mat_diffuse[] = { 0.8f, 0.65f, 0.4f, 1.0f };//�����Ϊ����ɫ
        GLfloat venus_mat_specular[] = { 0.3f, 0.2f, 0.1f, 1.0f };
        GLfloat venus_mat_emission[] = { 0.2f, 0.16f, 0.1f, 1.0f };
        GLfloat venus_mat_shininess = 10.0f;

        creat_planet.drawPlanet(0.19f, 4.0f, angle2, venus_mat_ambient, venus_mat_diffuse, venus_mat_specular, venus_mat_emission, venus_mat_shininess);

        // ��������
        GLfloat Saturn_mat_ambient[] = { 0.5f, 0.5f, 0.0f, 1.0f };
        GLfloat Saturn_mat_diffuse[] = { 0.8f, 0.81f, 0.27f, 1.0f };
        GLfloat Saturn_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
        GLfloat Saturn_mat_emission[] = { 0.1f, 0.1f, 0.0f, 1.0f };
        GLfloat Saturn_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.47f, 9.5f, angle6, Saturn_mat_ambient, Saturn_mat_diffuse, Saturn_mat_specular, Saturn_mat_emission, Saturn_mat_shininess);

        // ���ǵĻ�
        glPushMatrix();
        glRotatef(angle6, 0.0f, 1.0f, 0.0f);
        glTranslatef(9.5f, 0.0f, 0.0f);
        cycle.draw_cycle(0.6f, 0.9f, 100);//Ȧ��, Ȧ��
        glPopMatrix();

        // ����ľ��
        GLfloat Jupiter_mat_ambient[] = { 0.6f, 0.6f, 0.0f, 1.0f };
        GLfloat Jupiter_mat_diffuse[] = { 0.82f, 0.87f, 0.27f, 1.0f };
        GLfloat Jupiter_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
        GLfloat Jupiter_mat_emission[] = { 0.1f, 0.1f, 0.0f, 1.0f };
        GLfloat Jupiter_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.5f, 7.9f, angle5, Jupiter_mat_ambient, Jupiter_mat_diffuse, Jupiter_mat_specular, Jupiter_mat_emission, Jupiter_mat_shininess);

        // ����������
        GLfloat Uranus_mat_ambient[] = { 0.488f, 0.95f, 0.786f, 1.0f };
        GLfloat Uranus_mat_diffuse[] = { 0.388f, 0.9f, 0.686f, 1.0f };
        GLfloat Uranus_mat_specular[] = { 0.088f, 0.2f, 0.186f, 1.0f };
        GLfloat Uranus_mat_emission[] = { 0.1f, 0.2f, 0.15f, 1.0f };
        GLfloat Uranus_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.35f, 10.9f, angle7, Uranus_mat_ambient, Uranus_mat_diffuse, Uranus_mat_specular, Uranus_mat_emission, Uranus_mat_shininess);
    
        //���ƺ�����
        GLfloat Neptune_mat_ambient[] = { 0.5f, 0.8f, 0.95f, 1.0f };
        GLfloat Neptune_mat_diffuse[] = { 0.458f, 0.756f, 0.921f, 1.0f };
        GLfloat Neptune_mat_specular[] = { 0.1f, 0.15f, 0.2f, 1.0f };
        GLfloat Neptune_mat_emission[] = { 0.1f, 0.1f, 0.2f, 1.0f };
        GLfloat Neptune_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.3f, 11.9f, angle8, Neptune_mat_ambient, Neptune_mat_diffuse, Neptune_mat_specular, Neptune_mat_emission, Neptune_mat_shininess);

    }
    glPopMatrix();

    // ��������ϵ�ǵ�
    glPushMatrix();
    milkyWay.init();
    milkyWay.draw();
    glPopMatrix();

    // ��������
    glPushMatrix();
    nebula.initnebula();
    nebula.drawnebula();
    glPopMatrix();

    // ��������
    glColor3f(1.0f, 1.0f, 1.0f);  
    glPointSize(1.0f);           
    glBegin(GL_POINTS);
    for (int i = 0; i < STAR_COUNT; ++i) {
       
        float size = 2.0f - (starZ[i] + 20.0f) / 40.0f;
        if (size < 0.5f) size = 0.2f; // ȷ����С��С

        float alpha = 1.0f - (starZ[i] + 20.0f) / 40.0f;
        if (alpha < 0.2f) alpha = 0.2f;

        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        glPointSize(size);
        glBegin(GL_POINTS);
        glVertex3f(starX[i], starY[i], starZ[i]);
        glEnd();
    }
    glEnd();

    // ����С���Ǵ�
    glColor3f(0.6f, 0.6f, 0.6f); // ����С���ǵ���ɫΪ��ɫ
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        glPushMatrix();
        glTranslatef(asteroidX[i], asteroidY[i], asteroidZ[i]);
        glRotatef(asteroidAngle[i], 0.0f, 1.0f, 0.0f); // �� Y ����ת
        glutSolidSphere(asteroidSize[i], 10, 10); // ����С����
        glPopMatrix();
    }
    glutSwapBuffers();
}

void myIdle(void) {
    angle1 += 0.02f;
    angle2 += 0.0146f;
    angle3 += 0.0124f;
    angle4 += 0.01f;
    angle5 += 0.0054f;
    angle6 += 0.004f;
    angle7 += 0.0028f;
    angle8 += 0.0022f;

    if (angle1 >= 360.0f) angle1 = 0.0f;
    if (angle2 >= 360.0f) angle2 = 0.0f;
    if (angle3 >= 360.0f) angle3 = 0.0f;
    // �������ǵ�λ�ã������������λ�þۼ�
    for (int i = 0; i < STAR_COUNT; ++i) {
        float dx = mouseX - starX[i];
        float dy = mouseY - starY[i];
        float dz = 0.0f - starZ[i];
        float speedFactor = 0.00001f + (starZ[i] + 20.0f) / 40.0f * 0.001f;

        starX[i] += dx * speedFactor;
        starY[i] += dy * speedFactor;
        starZ[i] += dz * speedFactor;
    }

    // ����С���Ǵ�����ת�Ƕ�
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        asteroidAngle[i] += 0.05f; // ������ת�ٶ�
        if (asteroidAngle[i] >= 360.0f) asteroidAngle[i] = 0.0f;
    }

    glutPostRedisplay(); // �����ػ�����
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(400, 200);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL������ʾ");

    glewInit();
    init();
    initTextures();

    // ��ʼ�����ǵ�λ��
    for (int i = 0; i < STAR_COUNT; ++i) {
        // ����������ǵ�λ�ã���Χ���Ը�����Ҫ����
        starX[i] = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        starY[i] = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        starZ[i] = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
    }

    // ��ʼ��С���Ǵ�
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        // �������С���ǵĽǶ�
        float angle = (float)(rand() % 360) * M_PI / 180.0f;
        // �������С���ǵ����ĵľ��룬ģ�⻷�ηֲ�
        float distance = (float)(rand() % 100) / 100.0f * 1.0f + 6.7f; // ���뷶Χ�� 6.6�� 7.9 ֮��,������ľ��
        asteroidX[i] = cos(angle) * distance;
        asteroidY[i] = 0.0f; // ����С���Ǵ��� XY ƽ����
        asteroidZ[i] = sin(angle) * distance;

        // �������С���ǵĴ�С
        asteroidSize[i] = (float)(rand() % 10) / 1000.0f * 5.0f + 0.001f; // ��С��Χ�� 0.001 �� 0.006 ֮��

        // ��ʼ����ת�Ƕ�
        asteroidAngle[i] = 0.0f;
    }

    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);

    //�¹���:��������긽���ۼ�
    glutPassiveMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
