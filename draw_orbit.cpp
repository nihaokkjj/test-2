#include"draw_orbit.h"

void Orbit::drawOrbit(float radius, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        // ���㵱ǰ�ֶεĽǶ�
        float angle = 2.0f * M_PI * (float)i / (float)segments;
        // ���㵱ǰ�ֶε� z ����
        float x = radius * std::cos(angle);
        // ���㵱ǰ�ֶε� z ����
        float z = radius * std::sin(angle);
        // ָ����ǰ���������
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}