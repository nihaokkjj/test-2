#include"drawcycle.h"
#define M_PI  3.141593
void Cycle::draw_cycle(float innerRadius, float outerRadius, int numSegments) {
    GLfloat theta, x, z;

    // ����ľ�ǻ��Ĳ���
    GLfloat ring_mat_ambient[] = { 0.2f, 0.4f, 0.6f, 0.1f }; // �������ɫ
    GLfloat ring_mat_diffuse[] = { 0.4f, 0.7f, 0.9f, 0.1f };//0.5����͸����
    GLfloat ring_mat_specular[] = { 0.6f, 0.8f, 1.0f, 1.0f };
    GLfloat ring_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat ring_mat_shininess = 5.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ring_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ring_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ring_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, ring_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, ring_mat_shininess);

    // ����ľ�ǻ�
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numSegments; i++) {
        theta = 2.0f * M_PI * i / numSegments; // ��ǰ�Ƕ�
        x = cos(theta); // x ����
        z = sin(theta); // z ����

        // ��Ȧ����
        glVertex3f(innerRadius * x, 0.0f, innerRadius * z);
        // ��Ȧ����
        glVertex3f(outerRadius * x, 0.0f, outerRadius * z);
    }
    glEnd();
}