#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

#include <GL/glut.h>

class Camera {
public:
    float posX, posY, posZ; // ���λ��
    float centerX, centerY, centerZ; // Ŀ���
    float upX, upY, upZ; // �Ϸ���

    Camera() : posX(0), posY(12), posZ(25), centerX(0), centerY(0), centerZ(0), upX(0), upY(1), upZ(0) {}

    // ��������ӽ�
    void update() {
        gluLookAt(posX, posY, posZ, centerX, centerY, centerZ, upX, upY, upZ);//ǰ���� ��ʾ�������������ϵ�е�λ��
    //�м�����������������ŵ���������ϵ�еĵ��λ�����꣬�������һ���λ�ڻ��������λ��
    //����������������
    // ����ĳ�������������������������ϵ�е�����㣬�������Ϊ��վ���������ͷ�ĳ�����������������������ϵ�е�����㣬�����������ϵ�ģ�ֻ���������巽��

    }

    // �����������
    void moveForward(float distance) { posZ -= distance; }
    void moveBackward(float distance) { posZ += distance; }
    void moveLeft(float distance) { posX -= distance; }
    void moveRight(float distance) { posX += distance; }
    void moveUp(float distance) { posY += distance; }
    void moveDown(float distance) { posY -= distance; }
};
#endif
