#include <GL/glew.h> 
#include <GL/glut.h>  
#include <GLFW/glfw3.h>  
#include<iostream>
#include<vector>
#include<string>
#include<SOIL.h>
#include "add_music.h" //���ر�������
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<math.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include"creat_nebula.h"//����
#include"camera.h"//�ӽǱ䶯
#include"draw_orbit.h"


#define WIDTH 1000
#define HEIGHT 1000
#define M_PI 3.141592

 //����ͷ�ļ�
Planet creat_planet;
Cycle cycle;
Nebula nebula;
Camera camera;
Orbit orbit;
Music music;


unsigned int texture;
const int sun_texture_count = 5;
unsigned int sunTextures[sun_texture_count];
int currentSunTextureIndex = 0;
int nextSunTextureIndex = 1;
float gatherFactor = 0.0f;//���Ǿۼ�
std::vector<float> linePoints;  // ���ڴ洢�߶εĶ���
float linealpha = 0.8f;// ������͸����


// ��¼�����λ��
int clickX = 0;
int clickY = 0;
bool isClicked = false;

// ��������ṹ��
struct CelestialBody {
    float radius;  // ����뾶
    float orbitRadius;  // ����뾶
    float orbitCenterX; // ������� X ����
    float orbitCenterY; // ������� Y ����
    float orbitCenterZ; // ������� Z ����

    bool isSelected;    // �Ƿ�ѡ��
    std::string name;   // ��������
    std::string color;     // ������ɫ
   

    std::string description; // ����������Ϣ
    float diameter;          // ����ֱ��
};

// ���߽ṹ��
struct Ray {
    float originX, originY, originZ;
    float directionX, directionY, directionZ;
};

struct Star {
    float x, y, z; // ���ǵ�λ��
    bool isGathered; // �Ƿ��ھۼ�״̬
    float targetX, targetY, targetZ; // �ۼ�Ŀ��λ��
};
const int STAR_COUNT = 500;  // ���ǵ�����
Star stars[STAR_COUNT]; // ��������

// ��ʼ������

// ����
CelestialBody earth = { 0.32f, 5.4f, 0.0f, 0.0f, 0.0f, false, "Earth\n",
                        "blue\n","our home planet, full of life.\n", 12742.0f };

// ����
CelestialBody moon = { 0.1f, 0.55f, earth.orbitCenterX, earth.orbitCenterY, earth.orbitCenterZ, false, "Moon\n",
                       "yellow\n",  "Earth's natural satellite.\n", 3474.8f };

// ˮ��
CelestialBody mercury = { 0.15f, 2.0f, 0.0f, 0.0f, 0.0f, false, "Mercury\n",
                       " blue\n ","The closest planet to the Sun.\n", 4880.0f };

// ����
CelestialBody venus = { 0.3f, 4.0f, 0.0f, 0.0f, 0.0f, false, "Venus\n",
                       "orange\n","The second planet from the Sun, known for its thick atmosphere.\n", 12103.6f };

// ����
CelestialBody Saturn = { 0.6f, 9.5f, 0.0f, 0.0f, 0.0f, false, "Saturn\n",
                         "brown\n", "A gas giant with a prominent ring system.\n", 116464.0f };

// ����
CelestialBody mars = { 0.2f, 6.6f, 0.0f, 0.0f, 0.0f, false, "Mars\n",
                       "red\n","The fourth planet from the Sun, often called the 'Red Planet'.\n", 6779.0f };

// ľ��
CelestialBody Jupiter = { 0.7f, 7.9f, 0.0f, 0.0f, 0.0f, false, "Jupiter\n",
                        "oatmeal\n","The largest planet in our solar system.\n", 139822.0f };

// ������
CelestialBody Neptune = { 0.5f, 11.9f, 0.0f, 0.0f, 0.0f, false, "Neptune\n",
                         "blue\n","The eighth and farthest known planet from the Sun.\n", 49244.0f };

// ������
CelestialBody Uranus = { 0.45f, 10.9f, 0.0f, 0.0f, 0.0f, false, "Uranus\n",
                         "blue-black\n","The seventh planet from the Sun, known for its unique axial tilt.\n", 50724.0f };

// ȫ�ֱ���
float starX[STAR_COUNT];     // ���ǵ� x ����
float starY[STAR_COUNT];     // ���ǵ� y ����
float starZ[STAR_COUNT];     // ���ǵ� z ����
float mouseX = 0.0f;         // ���� x ����
float mouseY = 0.0f;
float blendFactor = 0.0f;  // ������ӣ���Χ�� 0 �� 1
const float blendStep = 0.99f;  // ÿ��֡����ʱ������ӵ�����

static GLfloat angle1 = 0.0f, angle4, angle5, angle6, angle7, angle8; // ����ת�Ƕ�
static GLfloat angle2 = 0.0f; // ���ǹ�ת�Ƕ�
static GLfloat angle3 = 0.0f; // �����Ƶ���ת�Ƕ�


// ���������
const float mouseSensitivity = 10.0f;

// С���Ǵ�
const int ASTEROID_COUNT = 200;
float asteroidX[ASTEROID_COUNT];
float asteroidY[ASTEROID_COUNT];
float asteroidZ[ASTEROID_COUNT];
float asteroidSize[ASTEROID_COUNT];
float asteroidAngle[ASTEROID_COUNT];

//��ǰ��ʾ��Ϣ������ͼ�ʱ��
CelestialBody* currentDisplayPlanet = NULL;
int displayTimer = 0;

//���Ƹ�������ƶ�������
void drawLine() {
    if (linePoints.size() >= 4) { //2���㹹��һ���߶�

        glDisable(GL_DEPTH_TEST);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix(); // ���浱ǰ����״̬
        glLoadIdentity();
        gluOrtho2D(-5.0, 5.0, -5.0, 5.0); // ��������ͶӰ
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glLineWidth(0.8f);
        glColor4f(0.2f, 0.6f, 1.0f, linealpha);  // �����߶���ɫΪ��ɫ
        glBegin(GL_LINE_STRIP);

        for (size_t i = 0; i < linePoints.size(); i += 2) {
            if (i + 2 < linePoints.size())
                glVertex2f(linePoints[i], linePoints[i + 1]);

        }

        glEnd();

        glPopMatrix(); 
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_DEPTH_TEST);

        if (linealpha >= 0.2f) linealpha -= 0.05f;
    }
}


////�ۼ����ѹ�Ӣ(ƽ��)
//void gatherStars(float centerX, float centerY, float centerZ) {
//
//    const int layers = 5; // �ѹ�Ӣ�Ĳ���
//    const float radius = 0.2f; // ÿ��İ뾶
//    const float angleStep = 2.0f * M_PI / (STAR_COUNT / layers); // ÿ��ĽǶȲ���
//
//    int starIndex = 0;
//    for (int layer = 0; layer < layers; ++layer) {
//        float currentRadius = radius * (layer + 1);
//        for (float angle = 0; angle < 2.0f * M_PI; angle += angleStep) {
//            if (starIndex >= STAR_COUNT) break;
//
//            stars[starIndex].targetX = centerX + currentRadius * cos(angle);
//            stars[starIndex].targetY = centerY + currentRadius * sin(angle);
//            stars[starIndex].targetZ = centerZ + currentRadius * sin(angle);
//
//            starIndex++;
//        }
//    }
//}
//�ۼ�����״�ѹ�Ӣ(����)
void gatherStars(float centerX, float centerY, float centerZ) {

    const int layers = 10; // �ѹ�Ӣ�Ĳ���
    const float radius = 0.1f; // ÿ��İ뾶

    int starIndex = 0;
    for (int layer =1; layer <= layers; ++layer) {
        float currentRadius = radius * layer;
        // ÿ�����������
        int starsPerLayer = STAR_COUNT / layers;

        for (int i = 0; i < starsPerLayer; ++i) {
            if (starIndex >= STAR_COUNT) break;

            // ���ɾ��ȷֲ��ļ��Ǻͷ�λ��
            float theta = acos(1 - 2 * (float)i / starsPerLayer); // ���ǣ���Χ�� 0 �� ��
            float phi = 2 * M_PI * (float)(rand() % 1000) / 1000.0f; // ��λ�ǣ���Χ�� 0 �� 2��

            // ��������ת��Ϊ�ѿ�������
            float x = currentRadius * sin(theta) * cos(phi);
            float y = currentRadius * sin(theta) * sin(phi);
            float z = currentRadius * cos(theta);

            // �������ǵ�Ŀ��λ��
            stars[starIndex].targetX = centerX + x;
            stars[starIndex].targetY = centerY + y;
            stars[starIndex].targetZ = centerZ + z;

            starIndex++;
        }
    }
}


//�������Ǿ�ɢ
void mouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
          
            // ��¼�����λ��
            clickX = x;
            clickY = y;
            isClicked = true;

            // �л����ǵľۼ�״̬
            for (int i = 0; i < STAR_COUNT; ++i) {

                stars[i].isGathered = !stars[i].isGathered;

                if (stars[i].isGathered) {
                    // ���þۼ�Ŀ��λ��
                    stars[i].targetX = (2.0f * x / WIDTH - 1.0f) * 5.0f; // ת��Ϊ OpenGL ����,�����λ����ʵ�ʶ���
                    stars[i].targetY = (1.0f - 2.0f * y / HEIGHT) * 5.0f;
                    stars[i].targetZ = (float)(rand() % 5) / 1.0f * 5.0f;
                    gatherStars(stars[i].targetX, stars[i].targetY, stars[i].targetZ);

                }
                else {
                    // ���÷�ɢĿ��λ�ã����λ�ã�
                    stars[i].targetX = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
                    stars[i].targetY = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
                    stars[i].targetZ = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
                }

            }
            // ��ʼ���߶ε����
            linePoints.clear();
            linePoints.push_back((2.0f * x / WIDTH - 1.0f) * 5.0f);
            linePoints.push_back((1.0f - 2.0f * y / HEIGHT) * 5.0f);
        }
        else if (state == GLUT_UP) {
            isClicked = false;
            //����ɿ�,�������
            linePoints.clear();
            linealpha = 1.0f;
        }
    }
}


//��궯̬
void mouseMotion(int x, int y) {

    if (isClicked) {
        // ��갴�²��ƶ�ʱ�������߶ε��յ�
        linePoints.push_back((2.0f * x / WIDTH - 1.0f) * 5.0f);
        linePoints.push_back((1.0f - 2.0f * y / HEIGHT )* 5.0f);
        if (linePoints.size() > 100) {
            // �����߶γ��Ȳ��䣬�Ƴ�����Ķ���
            linePoints.erase(linePoints.begin(), linePoints.begin() + 2);
        }
        glutPostRedisplay();  // ��Ǵ�����Ҫ�ػ�
    }
}

// ���Ʒ������Ϣ
void drawInfoBox(const CelestialBody& body) {
    if (!body.isSelected && &body != currentDisplayPlanet && displayTimer < 300) return;

    // ���浱ǰ��ͶӰ�����ģ����ͼ����
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    // ��������ͶӰ����Χ�� (0, 0) �� (WIDTH, HEIGHT)
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // ��Ϣ���λ��(���Ͻ�)
    float infoBoxX = WIDTH * 0.2f;
    float infoBoxY = HEIGHT * 0.7f;


    // ���ñ߿���ɫ
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(infoBoxX, infoBoxY);
    glVertex2f(infoBoxX + 550, infoBoxY);
    glVertex2f(infoBoxX + 550, infoBoxY + 100);
    glVertex2f(infoBoxX, infoBoxY + 100);
    glEnd();

    // ��ʾ������Ϣ
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(infoBoxX, infoBoxY + 80);
    for (char c : body.name) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(infoBoxX, infoBoxY + 50);
    for (char c : body.color) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glRasterPos2f(infoBoxX, infoBoxY + 20);
    for (char c : body.description) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // �ָ�֮ǰ�����ͶӰ�����ģ����ͼ����
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ���ƻ��̫��
void drawBlendedSun(float x, float y, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // ���Ƶ�ǰ����
    glBindTexture(GL_TEXTURE_2D, sunTextures[currentSunTextureIndex]);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - blendFactor);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();

    // ������һ����
    glBindTexture(GL_TEXTURE_2D, sunTextures[nextSunTextureIndex]);
    glColor4f(1.0f, 1.0f, 1.0f, blendFactor);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void initTextures() {
    glGenTextures(sun_texture_count, sunTextures);

    const char* filenames[sun_texture_count] = { "4.jpg",  "5.jpg", "6.png","7.jpg" ,"3.jpg" };

    for (int i = 0; i < sun_texture_count; ++i) {
        glBindTexture(GL_TEXTURE_2D, sunTextures[i]);
        // Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // ���ز���������
        int sun_width, sun_height, nrChannels;
        // stbi_set_flip_vertically_on_load(true); // ��ת�����Y��
        unsigned char* data = stbi_load(filenames[i], &sun_width, &sun_height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sun_width, sun_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
           
            std::cout << "Failed to load texture: " << filenames[i] << std::endl;

            exit(1); // �˳�����
        }
        stbi_image_free(data);
    }
}

// ��ͼλ��
void drawTexturedSquare(float x, float y, float width, float height, unsigned int texId) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

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


// ���̻ص�����
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 0.5f; // �ƶ��ٶ�

    switch (key) {
    case 'w': // ��ǰ�ƶ�
        camera.moveForward(moveSpeed);
        break;
    case 's': // ����ƶ�
        camera.moveBackward(moveSpeed);
        break;
    case 'a': // �����ƶ�
        camera.moveLeft(moveSpeed);
        break;
    case 'd': // �����ƶ�
        camera.moveRight(moveSpeed);
        break;
    case 'q': // �����ƶ�
        camera.moveUp(moveSpeed);
        break;
    case 'e': // �����ƶ�
        camera.moveDown(moveSpeed);
        break;

    case '1':
        currentDisplayPlanet = &mercury;
        displayTimer = 0;
        break;
    case '2':
        currentDisplayPlanet = &venus;
        displayTimer = 0;
        break;
    case '3':
        currentDisplayPlanet = &earth;
        displayTimer = 0;
        break;
    case '4':
        currentDisplayPlanet = &mars;
        displayTimer = 0;
        break;
    case '5':
        currentDisplayPlanet = &Jupiter;
        displayTimer = 0;
        break;
    case '6':
        currentDisplayPlanet = &Saturn;
        displayTimer = 0;
        break;
    case '7':
        currentDisplayPlanet = &Uranus;
        displayTimer = 0;
        break;
    case '8':
        currentDisplayPlanet = &Neptune;
        displayTimer = 0;
        break;
    }

    glutPostRedisplay(); // �����ػ�
}

//��ʾ���ǹ��
// radius: ����İ뾶
//segments: ����ķֶ������ֶ���Խ�࣬���Խƽ��


// �����ı�����
void drawText(std::string text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.5f, 0.5f, 1.0f); // �����ı���ɫΪ��ɫ
    glRasterPos2i(x, y);
    for (char c : text) {
       
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void myDisplay(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST); // ������Ȳ���
    // ����͸��Ч����ͼ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // ����ģ����ͼ����
    gluPerspective(120.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0f, 20.0f); // �������������� 1e6
    // 1���ӽǴ�С;    2����������ʾ�ڻ����ϵ�x��y�����ϵı��� ���ᰴʵ�ʷ�Ӧ�����
    //3���ֵԽ�󣬱�ʾ�۲������������ԽԶ
    //4����ɹ۲⵽���������Զ�������������ľ���
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // �������
    camera.update();

    glPushMatrix();
    //����̫��
    drawsun();

    // ����̫������
    drawSunRays();

    //����
    drawSunHalo();
    glPopMatrix();

    // ����̫��(��ͼ��ʽ)
    //glPushMatrix();
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, sunTextures[currentSunTextureIndex]);
    //float x = -1.15f;
    //float y = -1.15f;
    //float squareWidth = 2.38f;
    //float squareHeight = 2.38f;

    //drawBlendedSun(x, y, squareWidth, squareHeight);
    ////drawTexturedSquare(x, y, squareWidth, squareHeight, sunTextures[currentSunTextureIndex]);
    ////drawsun();

    //glDisable(GL_TEXTURE_2D);
    //glPopMatrix();

    // �������Ĳ��ʲ����Ƶ���
    GLfloat earth_mat_ambient[] = { 0.2f, 0.2f, 0.8f, 1.0f };
    GLfloat earth_mat_diffuse[] = { 0.2f, 0.2f, 0.5f, 1.0f };
    GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_shininess = 10.0f;

    // ��ʼ�������ı任״̬
        glPushMatrix();
        creat_planet.drawPlanet(0.32f, 5.4f, angle3, earth_mat_ambient, earth_mat_diffuse, earth_mat_specular, earth_mat_emission, earth_mat_shininess);
        glPopMatrix();

        // ���Ƶ�����
        glPushMatrix();
        glColor3f(0.0f, 0.1f, 0.9f);
        orbit.drawOrbit(earth.orbitRadius, 100);

        glPopMatrix();


        // ��������Ĳ��ʲ���������
        GLfloat moon_mat_ambient[] = { 0.5f, 0.5f, 0.0f, 1.0f };
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

        // ����������
        glPushMatrix();

        glColor3f(0.72f, 0.73f, 0.47f);

        float moonOrbitCenterX = earth.orbitRadius * cos(angle3 * M_PI / 180.0f);
        float moonOrbitCenterZ = earth.orbitRadius * sin(angle3 * M_PI / 180.0f);
        glTranslatef(moonOrbitCenterX, 0.0f, -moonOrbitCenterZ);
        orbit.drawOrbit(moon.orbitRadius, 100);
        glPopMatrix();

        // ������ǵĲ��ʲ����ƻ���
        GLfloat mars_mat_ambient[] = { 0.3f, 0.1f, 0.0f, 1.0f };
        GLfloat mars_mat_diffuse[] = { 0.8f, 0.4f, 0.2f, 1.0f };
        GLfloat mars_mat_specular[] = { 0.3f, 0.1f, 0.05f, 1.0f };
        GLfloat mars_mat_emission[] = { 0.05f, 0.02f, 0.01f, 1.0f };
        GLfloat mars_mat_shininess = 15.0f;

        creat_planet.drawPlanet(0.3f, 6.6f, angle4, mars_mat_ambient, mars_mat_diffuse, mars_mat_specular, mars_mat_emission, mars_mat_shininess);

        // ���ƻ��ǹ��
        glPushMatrix();

        glColor3f(0.3f, 0.1f, 0.0f);
        orbit.drawOrbit(mars.orbitRadius, 100);
        glPopMatrix();

        //����ˮ��
        GLfloat mercury_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };//û�й��յ�ʱ��: ǳ��ɫ
        GLfloat mercury_mat_diffuse[] = { 0.4f, 0.4f, 0.35f, 1.0f };//������: ǳ��ɫ
        GLfloat mercury_mat_specular[] = { 0.0f, 0.1f, 0.1f, 1.0f };//���淴ɫ����
        GLfloat mercury_mat_emission[] = { 0.2f, 0.2f, 0.2f, 0.1f };//��������
        GLfloat mercury_mat_shininess = 5.0f;//����ȵ�

        creat_planet.drawPlanet(0.15f, 2.8f, angle1, mercury_mat_ambient, mercury_mat_diffuse, mercury_mat_specular, mercury_mat_emission, mercury_mat_shininess);

        // ����ˮ�ǹ��
        glPushMatrix();
        glColor3f(0.25f, 0.25f, 0.25f);
        
        glColor3f(0.0f, 0.0f, 1.0f); // δѡ��ʱ�����ɫΪ��ɫ
        orbit.drawOrbit(mercury.orbitRadius, 100);
        glPopMatrix();
        //drawInfoBox(mercury);

        //���ƽ���
        GLfloat venus_mat_ambient[] = { 0.8f, 0.7f, 0.4f, 1.0f };//��ͻ�ɫ
        GLfloat venus_mat_diffuse[] = { 0.8f, 0.65f, 0.4f, 1.0f };//�����Ϊ����ɫ
        GLfloat venus_mat_specular[] = { 0.3f, 0.2f, 0.1f, 1.0f };
        GLfloat venus_mat_emission[] = { 0.2f, 0.16f, 0.1f, 1.0f };
        GLfloat venus_mat_shininess = 10.0f;

        creat_planet.drawPlanet(0.19f, 4.0f, angle2, venus_mat_ambient, venus_mat_diffuse, venus_mat_specular, venus_mat_emission, venus_mat_shininess);

        // ���ƽ��ǹ��
        glPushMatrix();
        glColor3f( 0.8f, 0.7f, 0.4f);
        orbit.drawOrbit(venus.orbitRadius, 100);
        glPopMatrix();
        //drawInfoBox(venus);

        // ��������
        GLfloat Saturn_mat_ambient[] = { 0.5f, 0.5f, 0.0f, 1.0f };
        GLfloat Saturn_mat_diffuse[] = { 0.8f, 0.81f, 0.27f, 1.0f };
        GLfloat Saturn_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
        GLfloat Saturn_mat_emission[] = { 0.1f, 0.1f, 0.0f, 1.0f };
        GLfloat Saturn_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.47f, 9.5f, angle6, Saturn_mat_ambient, Saturn_mat_diffuse, Saturn_mat_specular, Saturn_mat_emission, Saturn_mat_shininess);

        // �������ǹ��
        glPushMatrix();
         glColor3f(0.0f, 0.0f, 1.0f); // δѡ��ʱ�����ɫΪ��ɫ
        orbit.drawOrbit(Saturn.orbitRadius, 100);
        glPopMatrix();
       // drawInfoBox(Saturn);

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

        // ����ľ�ǹ��
        glColor3f(0.6f, 0.6f, 0.0f);
        orbit.drawOrbit(Jupiter.orbitRadius, 100);
        //drawInfoBox(Jupiter);

        // ����������
        GLfloat Uranus_mat_ambient[] = { 0.488f, 0.95f, 0.786f, 1.0f };
        GLfloat Uranus_mat_diffuse[] = { 0.388f, 0.9f, 0.686f, 1.0f };
        GLfloat Uranus_mat_specular[] = { 0.088f, 0.2f, 0.186f, 1.0f };
        GLfloat Uranus_mat_emission[] = { 0.1f, 0.2f, 0.15f, 1.0f };
        GLfloat Uranus_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.35f, 10.9f, angle7, Uranus_mat_ambient, Uranus_mat_diffuse, Uranus_mat_specular, Uranus_mat_emission, Uranus_mat_shininess);

        //// ���������ǹ��
         glColor3f(0.488f, 0.95f, 0.786f);
        orbit.drawOrbit (Uranus.orbitRadius, 100);

        //���ƺ�����
        GLfloat Neptune_mat_ambient[] = { 0.5f, 0.8f, 0.95f, 1.0f };
        GLfloat Neptune_mat_diffuse[] = { 0.458f, 0.756f, 0.921f, 1.0f };
        GLfloat Neptune_mat_specular[] = { 0.1f, 0.15f, 0.2f, 1.0f };
        GLfloat Neptune_mat_emission[] = { 0.1f, 0.1f, 0.2f, 1.0f };
        GLfloat Neptune_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.3f, 11.9f, angle8, Neptune_mat_ambient, Neptune_mat_diffuse, Neptune_mat_specular, Neptune_mat_emission, Neptune_mat_shininess);

        // ����sea���ǹ��
        glPushMatrix();
          glColor3f(0.5f, 0.8f, 0.95f);
          orbit.drawOrbit(Neptune.orbitRadius, 100);
        glPopMatrix();
        //drawInfoBox(Neptune);


    // ��������
    glPushMatrix();
    nebula.initnebula();
    nebula.drawnebula();
    glPopMatrix();

    // ����˵���ı�
    std::string instructions1 = "W: front";
    drawText(instructions1, 10, glutGet(GLUT_WINDOW_HEIGHT) - 20);
    std::string instructions2 = "S: back";
    drawText(instructions2, 10, glutGet(GLUT_WINDOW_HEIGHT) - 45);

    std::string instructions3 = "A: left";
    drawText(instructions3, 10, glutGet(GLUT_WINDOW_HEIGHT) - 70);
    std::string instructions4 = "D: right";
    drawText(instructions4, 10, glutGet(GLUT_WINDOW_HEIGHT) - 95);

    std::string instructions5 = "Q : up\n";
    drawText(instructions5, 10, glutGet(GLUT_WINDOW_HEIGHT) - 120);
    std::string instructions6 = "E : down";
    drawText(instructions6, 10, glutGet(GLUT_WINDOW_HEIGHT) - 145);

    //���򰴼�˵��
    std::string one = "Press the '1' key : Mercury";
    drawText(one, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 22);

    std::string two = "Press the '2' key : Venus";
    drawText(two, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 44);

    std::string three = "Press the '3' key : Earth";
    drawText(three, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 66);

    std::string four = "Press the '4' key : Mars";
    drawText(four, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 88);

    std::string five = "Press the '5' key : Jupiter";
    drawText(five, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 110);

    std::string six = "Press the '6' key : Saturn";
    drawText(six, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 132);

    std::string seven = "Press the '7' key : Uranus";
    drawText(seven, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 154);

    std::string eight = "Press the '8' key : Neptune";
    drawText(eight, glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) - 176);

    // ��������Ϣ��ȷ�������ϲ�
    if (currentDisplayPlanet != NULL)
    {
        drawInfoBox(earth);
        drawInfoBox(moon);
        drawInfoBox(mercury);
        drawInfoBox(venus);
        drawInfoBox(Saturn);
        drawInfoBox(mars);
        drawInfoBox(Jupiter);
        drawInfoBox(Neptune);
    }
    drawInfoBox(Uranus);

    // ����С���Ǵ�
    glColor3f(0.6f, 0.6f, 0.6f); // ����С���ǵ���ɫΪ��ɫ
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        glPushMatrix();
        glTranslatef(asteroidX[i], asteroidY[i], asteroidZ[i]);
        glRotatef(asteroidAngle[i], 0.0f, 1.0f, 0.0f); // �� Y ����ת
        glutSolidSphere(asteroidSize[i], 10, 10); // ����С����
        glPopMatrix();
    }

    // ��������
    glColor3f(1.0f, 1.0f, 1.0f); // ����������ɫΪ��ɫ
    glPointSize(2.0f); // �������Ǵ�С
    glBegin(GL_POINTS);
    for (int i = 0; i < STAR_COUNT; ++i) {
        float size = stars[i].isGathered ? 5.0f : 2.0f; // �ۼ�ʱ���Ǳ��
        glPointSize(size);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
    }
    glEnd();

    drawLine();//��������
    glutSwapBuffers();

}

void myIdle(void) {
    angle1 += 0.04f;
    angle2 += 0.0292f;
    angle3 += 0.0248f;
    angle4 += 0.02f;
    angle5 += 0.0108f;
    angle6 += 0.008f;
    angle7 += 0.0056f;
    angle8 += 0.0044f;

    if (angle1 >= 360.0f - 0.000001f) angle1 = 0.0f;
    if (angle2 >= 360.0f) angle2 = 0.0f;
    if (angle3 >= 360.0f) angle3 = 0.0f;

    // �������ǵ�λ��
    for (int i = 0; i < STAR_COUNT; ++i) {
        float speed = 0.01f; // �ƶ��ٶ�
        stars[i].x += (stars[i].targetX - stars[i].x) * speed;
        stars[i].y += (stars[i].targetY - stars[i].y) * speed;
        stars[i].z += (stars[i].targetZ - stars[i].z) * speed;
    }

    // ����С���Ǵ�����ת�Ƕ�
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        asteroidAngle[i] += 5.0f; // ������ת�ٶ�
        if (asteroidAngle[i] >= 360.0f) asteroidAngle[i] = 0.0f;
    }

    // ����̫����������
    //currentSunTextureIndex = (currentSunTextureIndex + 1) % sun_texture_count;
    // ���»������
    blendFactor += blendStep;
    if (blendFactor >= 1.0f) {
        blendFactor = 0.0f;
        currentSunTextureIndex = nextSunTextureIndex;
        nextSunTextureIndex = (nextSunTextureIndex + 1) % sun_texture_count;
    }

    // ���¼�ʱ��
    
        displayTimer++;
        if (displayTimer >= 300) { // ����֡��Ϊ 60fps������Ϊ 300 ֡
            currentDisplayPlanet = NULL;
            displayTimer = 0;
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
    
    //���ű�������
    music.initializeAudio();
    music.play_wav();

    init();//�����Դ
   // initTextures();
   
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

    // ��ʼ�����ǵ�λ��
    for (int i = 0; i < STAR_COUNT; ++i) {
        stars[i].x = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        stars[i].y = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        stars[i].z = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        stars[i].isGathered = false; // ��ʼ״̬Ϊ��ɢ
        stars[i].targetX = stars[i].x;
        stars[i].targetY = stars[i].y;
        stars[i].targetZ = stars[i].z;
    }

    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);

    //��������긽���ۼ�
    glutMotionFunc(mouseMotion);//���������ƶ��¼�

    //��������ƶ��鿴���
    //glutPassiveMotionFunc(mouseMotion);//������ڴ������ƶ���û�а����κ���갴��ʱ���ûص������ᱻ����
       glutMouseFunc(mouse);

    glutKeyboardFunc(keyboard); // ע����̻ص�����

    glGetString(GL_VERSION);
    const char* version = (const char*)glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << version << std::endl;


    glutMainLoop(); 
    music.deinit();
    return 0;
}
