#pragma once
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

// �����������ӽṹ��
struct NebulaParticle {
    float x, y, z;  // ����λ��
    float r, g, b;  // ������ɫ
    float alpha;    // ����͸����
};

// ������
class Nebula {
public:
    static const int NUM_PARTICLES = 1000;  // ��������
    NebulaParticle particles[NUM_PARTICLES];

    // ��ʼ����������
    void initnebula() {
        srand(static_cast<unsigned int>(time(nullptr)));
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            // �����������λ��
            particles[i].x = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
            particles[i].y = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;
            particles[i].z = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 10.0f;

            //// �������������ɫ������ɫ��Ϊ��ģ�����ƣ�
            //particles[i].r = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
            //particles[i].g = 0.0f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
            //particles[i].b = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f;

            //// �����������͸����
            //particles[i].alpha = static_cast<float>(rand()) / RAND_MAX * 0.3f;
        }
    }

    // ��������
    void drawnebula() {

        glBegin(GL_POINTS);
        for (int i = 0; i < NUM_PARTICLES; ++i) {
            //glColor4f(particles[i].r, particles[i].g, particles[i].b, particles[i].alpha);
            glVertex3f(particles[i].x, particles[i].y, particles[i].z);
        }
        glEnd();

    }
};
