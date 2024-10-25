#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //  INITIALIZE THE L, M COORDINATES FOR THE DISPLAY
    L = 100;
    M = size().height() - 100;

    // DECLARE ANGLE PHI AND ALPHA THAT HELP WITH THE 3D REPRESENTATION
    phi   = 30.0;
    alpha = 63.4;

    // CREATE THE OBJECT
    object = new Vertexes3D[16];
    assignObject();

    // SET THE OBJECT RED
    objectColor = QColor(255, 0, 0);

    // TRANSFORMATION VARIABLES
    tX = tY = tZ = 0;       // Translation
    rX = rY = rZ = false;   // Rotation controls
    // Coordinates of the center of the object for rotation porpuses
    cx1 = 50;
    cy1 = 50;
    cz1 = 50;
    // Angle for each rotation
    angleX = angleZ = 0.0;
    angleY = 90.0;

    // CHANGE THE VALUES OF THE OBJECT A LIL' BIT SO I DON'T FORM
    // TO FAR IN THE LEFT CORNER AND PLACE THE ROTATION IN 0
    ui->lEd_posX->setText ("20");   ui->lEd_posY->setText ("10");   ui->lEd_posZ->setText ("0");
}

Widget::~Widget()
{
    delete ui;
}

// RENDER METHOD(s)--------------------------------------------------------

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    initializePort();
    updateView();
}


void Widget::initializePort()
{
    mapp.window (0,0,size().width()-screenWidth, size().height());
    mapp.port   (0,0,size().width()-screenWidth, size().height());
}

void Widget::updateView()
{
    canvas = new QPainter ( this );
    canvas->setPen ( QPen( QColor ( 100, 100, 100 ), 2 ) );

    // UPDATE THE LINE EDIT VALUES
    // translation
    ui->lEd_posX->setText(QString::number(tX));
    ui->lEd_posY->setText(QString::number(tY));
    ui->lEd_posZ->setText(QString::number(tZ));
    // Rotation
    ui->lEd_rotZ->setText(QString::number(angleZ));
    ui->lEd_rotY->setText(QString::number(angleY));
    ui->lEd_rotX->setText(QString::number(angleX));

    // DIAGONAL, Z AXYS
    int zx1 = 0;
    int zy1 = 0;
    int zx2 = screenWidth * cos(radians(phi));
    int zy2 = screenWidth * sin(radians(phi));

    // MAP THE VALUES SO IT SHOWS CORRECTLY
    int x1, y1, x2, y2;
    mapp.map ( zx1, zy1, x1, y1, L, M );
    mapp.map ( zx2, zy2, x2, y2, L, M );
    // DRAW Z AXYS
    canvas->drawLine(x1,y1,x2,y2);

    // VERTICAL, Y AXYS
    zx1 = 0;
    zy1 = 0;
    zx2 = screenWidth * cos(radians(90));
    zy2 = screenWidth * sin(radians(90));

    // MAP THE VALUES SO IT SHOWS CORRECTL
    mapp.map ( zx1, zy1, x1, y1, L, M );
    mapp.map ( zx2, zy2, x2, y2, L, M );
    // DRAW Z AXYS
    canvas->drawLine(x1,y1,x2,y2);

    // HORIZONTAL, X AXYS
    zx1 = 0;
    zy1 = 0;
    zx2 = screenWidth * cos(radians(0));
    zy2 = screenWidth * sin(radians(0));

    // MAP THE VALUES SO IT SHOWS CORRECTL
    mapp.map ( zx1, zy1, x1, y1, L, M );
    mapp.map ( zx2, zy2, x2, y2, L, M );
    // DRAW Z AXYS
    canvas->drawLine(x1,y1,x2,y2);

    // 3D ROTATION ( JUST IN CASE IT'S CALLED )
    rotateObject(object, cx1, cy1, cz1, coordinates);

    // RENDER THE object
    canvas->setPen( QPen( objectColor, 7 ) );
    renderObject();
    canvas->end();
}

// OBJECT METHOD(s)--------------------------------------------------------

void Widget::assignObject()
{
    //                          x       y       z       Move/Draw Bool
     Vertexes3D baseObject[] = { { 0,     0,      0,      0 },
                             { 100,     0,      0,      1 },
                             { 100,     100,    0,      1 },
                             { 0,       100,    0,      1 },
                             { 0,       0,      0,      1 },
                             { 0,       0,      100,    1},
                             { 100,     0,      100,    1},
                             { 100,     100,    100,    1},
                             { 100,     100,    0,      1},
                             { 100,     0,      0,      0},
                             { 100,     0,      100,    1},
                             { 0,       0,      100,    0},
                             { 0,       100,    100,    1},
                             { 0,       100,    0,      1},
                             { 0,       100,    100,    0},
                             { 100,     100,    100,    1} };

    for ( int i = 0; i < 16; i++ )
        object[i] = baseObject[i];

    initializePort();
    screenWidth = this->size().width() - ui->widget->size().width();
}

void Widget::renderObject()
{
    for(int i = 0; i < 15; i++)
        canvas->drawLine ( coordinates[i].x(), coordinates[i].y(), coordinates[i+1].x(), coordinates[i+1].y());
}

void Widget::rotateObject(Vertexes3D *object, int cx, int cy, int cz, QPointF arr[])
{
    for ( int i = 0; i < 16; i++ ) {
        int x = object[i].x;
        int y = object[i].y;
        int z = object[i].z;

        if ( rX ) {
            x = object[i].x;
            y = qRound(cy + (object[i].y - cy ) * cos(radians(angleX)) + (object[i].z - cz) * sin(radians(angleX)));
            z = qRound(cz - (object[i].y - cy ) * sin(radians(angleX)) + (object[i].z - cz) * cos(radians(angleX)));
        }

        if ( rY ){
            y = object[i].y;
            x = qRound(cx + (object[i].x - cx ) * cos(radians(angleY)) - (object[i].z - cz) * sin(radians(angleY)));
            z = qRound(cz + (object[i].x - cx ) * sin(radians(angleY)) + (object[i].z - cz) * cos(radians(angleY)));
        }

        if ( rZ ) {
            z = object[i].z;
            x = qRound(cx + (object[i].x - cx ) * cos(radians(angleZ)) - (object[i].y - cy) * cos(radians(angleZ)));
            y = qRound(cy + (object[i].y - cy ) * cos(radians(angleZ)) + (object[i].x - cx) * sin(radians(angleZ)));
        }

        if(object[i].option)  //  0 = MOVETO, 1 = LINETO
            lineto3D ( x + tX, y + tY, z + tZ, radians(alpha), qDegreesToRadians(phi), i , arr);
        else
            moveto3D ( x + tX, y + tY, z + tZ, radians(alpha), qDegreesToRadians(phi), i , arr);
    }
}

void Widget::lineto3D(int x, int y, int z, float alpha, float phi, int i, QPointF arr[]) {
    int xp, yp;
    int length;
    float tanalpha = tan(alpha);

    if( tanalpha != 0)
        length = (int)(z/tanalpha);
    else
        length = 0;

    xp = x + length * cos ( phi );
    yp = y + length * sin ( phi );

    // WE MAP THE VALUES
    int x1, y1;
    mapp.map( xp, yp, x1, y1, L, M);

    // ASSIGN THE MAPPED COORDIANTES
    arr[i].setX(x1);
    arr[i].setY(y1);
}

void Widget::moveto3D(int x, int y, int z, float alpha, float phi, int i, QPointF arr[])
{
    int xp, yp;
    int length;
    float tanalpha = tan(alpha);

    if( tanalpha != 0)
        length = (int)(z/tanalpha);
    else
        length = 0;

    xp = x + length * cos ( phi );
    yp = y + length * sin ( phi );

    // WE MAP THE VALUES
    int x1, y1;
    mapp.map( xp, yp, x1, y1, L, M);

    // ASIGN THE MAPPED COORDINATES
    arr[i].setX(x1);
    arr[i].setY(y1);
}

//  KEY BINDS METHOD(s)--------------------------------------------------------

void Widget::keyPressEvent(QKeyEvent *event)
{
    if ( event->key() == Qt::Key_Escape ) this->close();

    // Translation
    if( event->modifiers() == Qt::ShiftModifier )
    {
        switch(event->key()){
        case (Qt::Key_W) :  tZ += 5;    break;
        case (Qt::Key_S) :  tZ -= 5;    break;
        }
    }
    else
    {
        if ( event->key() == Qt::Key_S )    tY-=5;
        if ( event->key() == Qt::Key_W )    tY+=5;
    }
    if ( event->key() == Qt::Key_D )    tX+=5;
    if ( event->key() == Qt::Key_A )    tX-=5;

    // Rotation
    switch( event->key() ){
        case ( Qt::Key_X ) :
            rZ = false;   rX = true;   rY = false;
            angleX += 10.0;
        break;

        case ( Qt::Key_Y ) :
            rZ = false;   rX = false;   rY = true;
            angleY += 10.0;
        break;

        case ( Qt::Key_Z ) :
            rZ = true;   rX = false;   rY = false;
            angleZ += 10.0;
        break;
    }

    update();
}

// UI METHOD(s) --------------------------------------------------------

void Widget::on_lEd_posX_textChanged(const QString &arg1)
{
    tX = arg1.toInt();
    update();
}


void Widget::on_lEd_posY_textChanged(const QString &arg1)
{
    tY = arg1.toInt();
    update();
}


void Widget::on_lEd_posZ_textChanged(const QString &arg1)
{
    tZ = arg1.toInt();
    update();
}


void Widget::on_lEd_rotX_textChanged(const QString &arg1)
{
    rX = true;  rY = false; rZ= false;
    angleX = arg1.toInt();
    update();
}


void Widget::on_lEd_rotY_textChanged(const QString &arg1)
{
    rX = false;  rY = true; rZ= false;
    angleY = arg1.toInt();
    update();
}


void Widget::on_lEd_rotZ_textChanged(const QString &arg1)
{
    rX = false;  rY = false; rZ= true;
    angleZ = arg1.toInt();
    update();
}


void Widget::on_dial_Phi_valueChanged(int value)
{
    phi = value;
    ui->label_12->setText(QString::number(value));
    update();
}


void Widget::on_slider_CameraX_valueChanged(int value)
{
    L = value;
    update();
}


void Widget::on_slider_CameraV_valueChanged(int value)
{
    M = size().height() - value;
    update();
}


void Widget::on_btn_ColorPicker_clicked()
{
    QColor selec = colorPicker->getColor(objectColor, this, "Select the object color");

    if ( selec.isValid() )
    {
        objectColor = selec;

        ui->lbl_colorShow->setStyleSheet( "background-color: " + objectColor.name() + ";" );
        update();
    }
}

