#include <GL/glu.h>

#include <QDebug>
#include <QTimer>
#include <QKeyEvent>
#include "GLWindow.h"
#define vis 0.005f//0.005f

vertex vertices[N][N];
GLWindow::GLWindow(QWidget *parent):QGLWidget(parent)
{
    this->resize(800,600);
    frames=0;
    time=0;
    rotX=rotY=rotZ=0.0f;
    speedX=speedY=speedZ=1.0f;
    drop=true;

    p=&A;
    n=&B;
    pos[0]=0.0f;
    pos[1]=0.0f;
    pos[2]=-1.0f;
    pos[3]=0.0f;
    memset(vertices,0,sizeof(vertices));
    memset(&A,0,sizeof(A));
    memset(&B,0,sizeof(B));
    for(unsigned int i=0;i<N;++i)
    {
        for(unsigned int j=0;j<N;++j)
        {
            vertices[i][j].coo[0]=-1.0f+2.0f*i/(N-1);
            vertices[i][j].coo[1]=-1.0f+2.0f*j/(N-1);
            vertices[i][j].nor[2]=-4.0f/(N-1);
        }
    }

    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeStep()));



//    QGLFormat frmt;
//    frmt.setSwapInterval(1);
//    setFormat(frmt);
}
GLWindow::~GLWindow(){}
void GLWindow::paintGL(void)
{
    if(frames==0)
    {
        time=clock();
    }


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //glLightfv(GL_LIGHT0, GL_POSITION,pos);
    glTranslatef (0.0f, 0.3f, 0.0f);
    glTranslatef (0.0f, 0.0f, -1.5f);
    glRotatef(145.0f,1.0f,0.0f,0.0f);
    glLightfv(GL_LIGHT2, GL_POSITION,pos);


    glRotatef(rotX,1.0f,0.0f,0.0f);
    glRotatef(rotY,0.0f,1.0f,0.0f);
    glRotatef(rotZ,0.0f,0.0f,1.0f);
    drawWater();

    seconds=(float)(clock()-time)/CLOCKS_PER_SEC;
    frames++;
    if(seconds>5.0)
    {
        printf("%u frames in %f seconds=%f FPS\n",frames,1.0*seconds,1.0*frames/seconds);
        frames=time=0;
    }


}
void GLWindow::initializeGL(void)
{

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

//    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_LINEAR);
//    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_LINEAR);
    loadTextures();
    //objectDot=glGenLists(SIZE_PICTURE);
    //objectPairWheels=glGenLists(SIZE_PICTURE);

    glClearColor(0.0f,0.0f,0.0f,0.5f);
    glClearDepth(2.0);//зустановка буфера глубины
    glDepthFunc(GL_LESS);//(GL_LEQUAL) (GL_LESS);//тип теста глубины
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    //glEnable(GL_AUTO_NORMAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_LIGHT2);

}
void GLWindow::resizeGL(int w, int h)
{

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);
    //glOrtho(-1.2,1.2,-1.2,1.2,-3.0,3.0);
    //gluOrtho2D(5.0,5.0,-1.0,-1.0);
    //gluPerspective(90,(w+.1)/(h+.1),0.2f,100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
void GLWindow::keyPressEvent(QKeyEvent *press)
{

    switch(press->key())
    {
        case Qt::Key_X:glRotX();break;
        case Qt::Key_Y:glRotY();break;
        case Qt::Key_Z:glRotZ();break;
        case Qt::Key_D:drop=!drop;break;
        case Qt::Key_C:changeSigle();break;
        case Qt::Key_S:timer->stop();break;
        case Qt::Key_T:timer->start(10);break;
        case Qt::Key_Escape:this->close();break;
    }
    updateGL();
}

GLvoid GLWindow::loadTextures(GLvoid)
{
    QImage texture1;
    //texture1.load(":/Sith.jpeg");
    //texture1.load(":/sith_2.png");
    texture1.load(":/background.png");
    //texture1.load(":/phong.jpg");

    texture1=QGLWidget::convertToGLFormat(texture1);
    glGenTextures(1,&texture[0]);


//    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//    glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RGB_S3TC_DXT1_EXT,(GLsizei)texture1.width(),(GLsizei)texture1.height(),
//                 0, GL_RGB,GL_UNSIGNED_BYTE,  texture1.bits());

    //создание текстуры по соседним пикселям
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    //GL_TEXTURE_MIN_FILTE изображение меньше чем оригинал текстуры
    //GL_TEXTURE_MAG_FILTER изображение больше чем оригинал текстуры
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//GL_LINEAR узнал что это билинейная фильтрация
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //    glTexImage2D(ГоворGLvoidим что 2d текстурка, уровень детолизации обычно ставять 0,
    //       число компонент цветовых данных у нас 3 цвета
    // далее размеры, (GLsizei)texture1.width(), (GLsizei)texture1.height(),
    //0--это бордюр обычно отсается нулем, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());
    glTexImage2D(GL_TEXTURE_2D, 0, 4, (GLsizei)texture1.width(), (GLsizei)texture1.height(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE, texture1.bits());
}


GLvoid GLWindow::glRotX(GLvoid)
{
    rotX+=speedX;
    if(rotX>=360.0)
    {
        rotX=0.0f;
    }
}

GLvoid GLWindow::glRotY(GLvoid)
{
    rotY+=speedY;
    if(rotY>=360.0)
    {
        rotY=0.0f;
    }
}

GLvoid GLWindow::glRotZ(GLvoid)
{
    rotZ+=speedZ;
    if(rotZ>=360.0)
    {
        rotZ=0.0f;
    }
}

GLvoid GLWindow::changeSigle(GLvoid)
{
    speedX=-speedX;
    speedY=-speedY;
    speedZ=-speedZ;
}

GLvoid GLWindow::drawWater(GLvoid)
{
    int i,j;
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    for(i=1;i<N-1;i++)
    {
        glBegin(GL_TRIANGLE_STRIP);//(GL_TRIANGLE_STRIP);
        {
            for(j=1;j<N-1;j++)
            {
                glNormal3fv(vertices[i][j].nor);
                glVertex3fv(vertices[i][j].coo);
                glNormal3fv(vertices[i+1][j].nor);
                glVertex3fv(vertices[i+1][j].coo);

            }
        }glEnd();
    }

}

void GLWindow::timeStep(void)
{

    int i,i1,j,j1;
    //srand(time(0));
    i1=rand()%(N-8);
    j1=rand()%(N-8);
    //qDebug()<<i1<<j1<<(rand()&(N-1));

    if(drop)
    {
        if(1.0*(rand()%(N-1))/(N)>0.5)
        {
            for(i=-3;i<4;i++)
            {
                for(j=-3;j<4;j++)
                {
                    GLfloat v=4.5f-i*i-j*j;
                    if(v<0.0f){v=0.0f;}
                    n->U[i+i1+3][j+j1+3]-=v*0.006f;
                }
            }
        }
    }

    for(i=1;i<N-1;i++)
    {
        for(j=1;j<N-1;j++)
        {
            vertices[i][j].coo[2]=n->U[i][j];
            vertices[i][j].nor[0]=n->U[i-1][j]-n->U[i+1][j];
            vertices[i][j].nor[1]=n->U[i][j-1]-n->U[i][j+1];

            float laplas=(n->U[i-1][j]+n->U[i+1][j]+n->U[i][j+1]+n->U[i][j-1])*0.25f-n->U[i][j];
            p->U[i][j]=((2.0f-vis)*n->U[i][j]-p->U[i][j]*(1.0f-vis)+laplas);
        }
    }
    field *sw=p;p=n;n=sw;
    updateGL();
}

