#pragma once
#include<GL/glut.h>
class Planet
{
public:

    void drawPlanet(GLfloat radius, GLfloat distance, GLfloat angle, GLfloat mat_ambient[], GLfloat mat_diffuse[], GLfloat mat_specular[], GLfloat mat_emission[], GLfloat mat_shininess);
    //�뾶,  ���о���,  �Ƕ�
    //�ⷴ��, ������, ���淴��, �Է�������, �����

    void setupLights(void);
};
