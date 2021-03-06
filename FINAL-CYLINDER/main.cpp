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

GLuint tex;
static int num_faces = 4;
static float cam_offsetX = 0;
static float cam_offsetY = -0.50;
float x=0.0f,z=7.0f, y = 1.0f;
float lx=0.0f, lz=-3.0f, ly = 0.0f;
float angle = 90.0f;
float deltaAngle = 0.0f;
int xOrigin = 1;
float offs1 = 0;
float offs2 = 0;
float rati = 90.0;
float rotation;
bool tru = true;
/* GLUT callback Handlers */


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

void make_tex(void)
{
    unsigned char data[256][256][3];
    for (int y = 0; y < 255; y++) {
        for (int x = 0; x < 255; x++) {
            unsigned char *p = data[y][x];
            p[0] = p[1] = p[2] = (x ^ y) & 8 ? 255 : 0;
        }
    }
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *) data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

static void display( void )
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*rati;
    if(tru){
        rotation = a;
    } else {
        rotation = rotation;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,1,1);
    
    glPushMatrix();
        glTranslated(cam_offsetX,cam_offsetY,-3);
        glRotated(90,1,0,0);
        glRotated(rotation,0,0,1);
        glPushMatrix();
            glBegin(GL_LINE_LOOP);
            float height = -1 * 1;
            for(int i = 0; i < num_faces; i++)
            {
                glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), 0.0f);
            }
            glEnd();
        glPopMatrix();
    
        glPushMatrix();
            glBegin(GL_LINE_LOOP);
            for(int i = 0; i < num_faces; i++)
            {
                glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), height);
            }
            glEnd();
        glPopMatrix();
    
    
        glPushMatrix();
        for(int i = 0; i < num_faces; i++)
        {
            glBegin(GL_LINE_LOOP);
            glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), height);
            glVertex3f(sin(M_PI * 2.0f * i / num_faces), cos(M_PI * 2.0f * i / num_faces), 0.0f);
            glEnd();
        }
        glPopMatrix();
    
        glPushMatrix();
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
                glBindTexture(GL_TEXTURE_2D, tex);
                glColor3f(255.0,255.0,255.0);
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'a');
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

void exportTo(){
    ofstream output;
    output.open("IWall.obj");
    int indices[num_faces * 4];
    int i = 0, j;
    for (j = 0; j < num_faces; j++){
        float x_start = sin(M_PI * 2.0f * i / num_faces);
        float y_start = cos(M_PI * 2.0f * i / num_faces);
        float z_start = 0.0f;
        float x_end = sin(M_PI * 2.0f * (i + 1) / num_faces);
        float y_end = cos(M_PI * 2.0f * (i + 1) / num_faces);
        float z_end = -1;
        // bottom left corner
        output << "v " << x_start << " " << y_start << " " << z_start << endl;
        indices[i] = i;
        i++;
        // bottom right corner
        output << "v " << x_end << " " << y_end << " " << z_start << endl;
        indices[i] = i;
        i++;
        // top left corner
        output << "v " << x_start << " " << y_start << " " << z_end << endl;
        indices[i] = i;
        i++;
        // top right corner
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
            cam_offsetX -= 0.2;
            break;
        case 'r' :
            cam_offsetX += 0.2;
            break;
        case 'u' :
            cam_offsetY += 0.2;
            break;
        case 'd' :
            cam_offsetY -= 0.2;
            break;
        case 'm' :
            exportTo();
            break;
        case 'k' :
            lx += 1;
            ly -= 1;
            break;
        case 's' :
            tru = !tru;
    }
    
    glutPostRedisplay();
}


void mouseMove(int x, int y) {
    // this will only be true when the left button is down
    cam_offsetX += x * .0001f;
    cam_offsetY += y * .0001f;
}

void mouseButton(int button, int state, int x, int y) {
    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            cam_offsetX += offs1 + .00001f;
            cam_offsetY += offs2 + .00001f;
        }
        else  {// state = GLUT_DOWN
            cam_offsetX = x;
            cam_offsetY = y;
        }
    }
}

void init(void)
{
    glEnable(GL_DEPTH_TEST);
    make_tex();
    glEnable(GL_TEXTURE_2D);
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
    init();
    glutMainLoop();
    
    return EXIT_SUCCESS;
}
