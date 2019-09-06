#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QGLWidget>
#define N 512
typedef struct
{
    GLfloat coo[3];
    GLfloat nor[3];
}vertex;

typedef struct
{
    GLfloat U[N][N];
}field;

class QTimer;
class QKeyEvent;
class GLWindow : public QGLWidget
{
    Q_OBJECT
    public:
        GLWindow(QWidget *parent = 0);
        ~GLWindow();
    private:
        bool drop;
        clock_t time;
        float seconds;

        field A,B;
        field *p,*n;


        QTimer *timer;

        GLfloat rotX;
        GLfloat rotY;
        GLfloat rotZ;

        GLfloat speedX;
        GLfloat speedY;
        GLfloat speedZ;

        GLuint frames;

        GLuint texture[1];

        GLfloat pos[4];

        GLvoid glRotX(GLvoid);
        GLvoid glRotY(GLvoid);
        GLvoid glRotZ(GLvoid);
        GLvoid changeSigle(GLvoid);
        GLvoid loadTextures(GLvoid);


        GLvoid drawWater(GLvoid);

    protected:
        virtual void paintGL(void);
        virtual void initializeGL(void);
        virtual void resizeGL(int w,int h);
        virtual void keyPressEvent(QKeyEvent *press);
    private slots:
        void timeStep(void);
};

#endif // GLWINDOW_H

