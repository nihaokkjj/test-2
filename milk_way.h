#include <GL/glut.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include <ctime>
#include <cstdlib>
#include<iostream>

#define WIDTH 800
#define HEIGHT 800
#define M_PI 3.141593

// �����ǵ�ṹ��
//�������ӵ���������
float core_milk_big = 8.0f;//���ı߽�
float core_milk_small = 7.0f;
struct Star {
    float x, y, z;  // �ǵ�λ��
    float r, g, b;  // �ǵ���������ɫ
};

// �ǵ㼯����
class MilkWay {
public:
    static const int NUM_STARS = 8000;
    Star stars[NUM_STARS];

    // ��ʼ���ǵ�
    void init() {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < NUM_STARS; ++i) {
            stars[i].x = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;
            stars[i].y = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;
            stars[i].z = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;

            //// ��������ǵ���������ɫ
            //stars[i].r = static_cast<float>(rand()) / RAND_MAX;
            //stars[i].g = static_cast<float>(rand()) / RAND_MAX;
            //stars[i].b = static_cast<float>(rand()) / RAND_MAX;
        }
    }

    // �����ǵ�
    void draw() {
        glPointSize(1.4f);
        glBegin(GL_POINTS);
        //for (int i = 0; i < NUM_STARS; ++i) {
        //    //glColor3f(stars[i].r, stars[i].g, stars[i].b);
        //    glVertex3f(stars[i].x, stars[i].y, stars[i].z);
        //}
                for (int i = 0; i < NUM_STARS; i++) {
            float distance_milk = sqrt(stars[i].x * stars[i].x + stars[i].y * stars[i].y + stars[i].z * stars[i].z);
            //glColor3f(stars[i].r, stars[i].g, stars[i].b);
            if (distance_milk <= core_milk_big && distance_milk >= core_milk_small && stars[i].y * stars[i].y <= 5.0)
            {
                glVertex3f(stars[i].x, stars[i].y, stars[i].z);
            }
            /*else  if (distance_milk > core_milk_big && stars[i].y * stars[i].y <= 5.0)
            {
                if (rand() % 10 < 1)
                glVertex3f(stars[i].x, stars[i].y, stars[i].z);
            }
            else
            {
                if (rand() % 10 < 1 && stars[i].y * stars[i].y <= 5.0)
                    glVertex3f(stars[i].x, stars[i].y, stars[i].z);
            }*/
        }
        glEnd();
    }
};
// �����ǵ�ṹ��
//struct Star {
//    float x, y, z;
//    float r, g, b;  // �ǵ���������ɫ
//};
////�������ӵ���������
//float core_milk_big = 10.0f;//���ı߽�
//float core_milk_small = 3.0f;
//// ��������ϵ��
//class MilkWay {
//public:
//    static const int NUM_STARS = 5000;
//    Star stars[NUM_STARS];
//    void init() {
//        srand(time(0));
//        for (int i = 0; i < NUM_STARS; i++) {
//            stars[i].x = (rand() % 50 - 10) / 1.0f;
//            stars[i].y = ((rand() % 10 - 5) / 5.0f);
//            stars[i].z = (rand() % 50 - 10) / 1.0f;
//            // ��������ǵ���������ɫ
//            stars[i].r = ((rand() % 255) / 255) * 1.0f;
//            stars[i].g = ((rand() % 255) / 255) * 1.0f;
//            stars[i].b = ((rand() % 255) / 255) * 1.0f;
//        }
//    }
//    void draw() {
//        glPointSize(1.0f);  // �����ǵ��С
//        glBegin(GL_POINTS);
//        for (int i = 0; i < NUM_STARS; i++) {
//            float distance_milk = sqrt(stars[i].x * stars[i].x + stars[i].y * stars[i].y + stars[i].z * stars[i].z);
//            glColor3f(stars[i].r, stars[i].g, stars[i].b);
//            if (distance_milk <= core_milk_big && distance_milk >= core_milk_small)
//            {
//                glVertex3f(stars[i].x, stars[i].y, stars[i].z);
//            }
//            else  if (distance_milk > core_milk_big)
//            {
//                if (rand() % 10 < 4)
//                glVertex3f(stars[i].x, stars[i].y, stars[i].z);
//            }
//            else
//            {
//                if (rand() % 10 < 4)
//                    glVertex3f(stars[i].x, stars[i].y, stars[i].z);
//            }
//        }
//        glEnd();
//    }
//};