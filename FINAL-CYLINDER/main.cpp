#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

static int num_faces = 5;
static int cam_offsetX = -2;
static int cam_offsetY = 0;
float x=0.0f,z=7.0f, y = 1.0f;
float lx=0.0f, lz=-3.0f, ly = 0.0f;
/* GLUT callback Handlers */

void exportTo(){
    ofstream output;
    output.open("prueba.txt");
    int indices[num_faces * 4];
    int i = 0, j;
    for (j = 0; j < num_faces; j++){
        float x_start = sin(M_PI * 2.0f * i / num_faces);
        float y_start = cos(M_PI * 2.0f * i / num_faces);
        float z_start = 0.0f;
        float x_end = sin(M_PI * 2.0f * (i + 1) / num_faces);
        float y_end = cos(M_PI * 2.0f * (i + 1) / num_faces);
        float z_end = -1;
        // Bottom left corner
        output << "v " << x_start << " " << y_start << " " << z_start << endl;
        indices[i] = i;
        i++;
        // Bottom right corner
        output << "v " << x_end << " " << y_end << " " << z_start << endl;
        indices[i] = i;
        i++;
        // Top left corner
        output << "v " << x_start << " " << y_start << " " << z_end << endl;
        indices[i] = i;
        i++;
        // Top right corner
        output << "v " << x_end << " " << y_end << " " << z_end << endl;
        indices[i] = i;
        i++;
    }
    output << "f " << indices[0] + 1 << " " << indices[1] + 1 << " " << indices[2] + 1 << endl;
    
    for(int i = 3; i < num_faces * 4; i++){
        output << "f " << indices[i - 2] + 1 << " " << indices[i - 1] + 1 << " " << indices[i] + 1 << endl;
    }
    output.close();
}

static void resize( int width, int height)
{
    const float ar = ( float ) width / ( float ) height;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display( void )
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    
    glPushMatrix();
        glTranslated(cam_offsetX,cam_offsetY,-6);
        glRotated(90,1,0,0);
        glRotated(a,0,0,1);
        glPushMatrix();
            // GL Line Loop for Bottom
            glBegin(GL_LINE_LOOP);
            float height = -1 * 1;
            for(int i = 0; i < num_faces; i++)
            {
                glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), 0.0f);
            }
            glEnd();
        glPopMatrix();
    
        glPushMatrix();
            // GL Line Loop for Top
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < num_faces; i++)
            {
                glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), height);
            }
            glEnd();
        glPopMatrix();
    
    
        glPushMatrix();
        // GL Line Loop for sides
        for(int i = 0; i < num_faces; i++)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), height);
            glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), 0.0f);
            glEnd();
        }
        glPopMatrix();
    
        glPushMatrix();
        // Dynamic face creation
        int j = 0, y = 1;
        for (int i = 0; i < num_faces; i++){
            float x_start = sin(M_PI * 2.0f * i / num_faces);
            float y_start = cos(M_PI * 2.0f * i / num_faces);
            float z_start = 0.0f;
            
            float x_end = sin(M_PI * 2.0f * (i + 1) / num_faces);
            float y_end = cos(M_PI * 2.0f * (i + 1) / num_faces);
            float z_end = height;
            
            glPushMatrix();
                float start = z_start;
                float offset = z_end * 1.0f;
                offset = (z_end * 1.0f);
                glBegin(GL_QUAD_STRIP);
                glNormal3f(0.0f, 0.0f, 1.0f);
                glColor3f(y,j,0);
                glTexCoord2f(3.0, 3.0); glVertex3f(x_start, y_start, start );
                glTexCoord2f(0.0, 3.0); glVertex3f(x_end, y_end, start);
                glTexCoord2f(0.0, 0.0); glVertex3f(x_start, y_start, offset);
                glTexCoord2f(3.0, 0.0); glVertex3f(x_end, y_end, offset);
                glEnd();
            if(j == 3){
                j = 0;
            } else {
                j++;
            }
                y+=0.2;
            
            glPopMatrix();
        }
        glPopMatrix();
    
    glPopMatrix();
    
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q' :
            exit (0);
            break ;
            
        case '+' :
            num_faces++;
            break ;
            
        case '-' :
            if (num_faces>3)
            {
                num_faces--;
            }
            break ;
        case 'l' :
            cam_offsetX -= 1;
            break;
        case 'r' :
            cam_offsetX += 1;
            break;
        case 'u' :
            cam_offsetY += 1;
            break;
        case 'd' :
            cam_offsetY -= 1;
            break;
        case 'm' :
            exportTo();
            break;
    }
    
    glutPostRedisplay();
}

void mouseMove(int x, int y) {
    // this will only be true when the left button is down
    ly += 1;
    lx += 2;
}

static void idle( void )
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main( int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    
    glutCreateWindow( "Infinite Menu" );
    gluLookAt(x, y, z,
              x + lx, y + ly, z + lz,
              0.0f, 1.0f, 0.0f);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    
    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    glutMotionFunc(mouseMove);
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
