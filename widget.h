#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include <QKeyEvent>
#include <QColorDialog>

#include "mapping.h"

struct Vertexes3D {
    int x, y, z; // Coordiantes
    bool option; // Line to / Move to
};

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define radians(g)    ((double)(g) * M_PI / 180.0)

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent     ( QPaintEvent *event);

    void assignObject     ();
    void renderObject     ();
    void rotateObject     (Vertexes3D *object, int cx, int cy, int cz, QPointF arr[]);

    void updateView     ();
    void initializePort ();    

    void lineto3D ( int x, int y, int z, float alpha, float phi, int i, QPointF arr[] );
    void moveto3D ( int x, int y, int z, float alpha, float phi, int i, QPointF arr[] );

    void keyPressEvent ( QKeyEvent *event );

private slots:
    void on_lEd_posX_textChanged(const QString &arg1);

    void on_lEd_posY_textChanged(const QString &arg1);

    void on_lEd_posZ_textChanged(const QString &arg1);

    void on_lEd_rotX_textChanged(const QString &arg1);

    void on_lEd_rotY_textChanged(const QString &arg1);

    void on_lEd_rotZ_textChanged(const QString &arg1);

    void on_dial_Phi_valueChanged(int value);

    void on_slider_CameraX_valueChanged(int value);

    void on_slider_CameraV_valueChanged(int value);

    void on_btn_ColorPicker_clicked();

private:
    Ui::Widget *ui;
    QPainter *canvas;

    // OBJECT POINT STRUCTURE
    QPointF coordinates[16];

    // MAPPING
    Mapping mapp;
    int L, M;
    int screenWidth;

    // TRANSFORMATIONS
    int     tX, tY, tZ;
    int     cx1, cy1, cz1;
    bool    rX, rY, rZ;

    // RENDERING
    float   alpha, phi;
    double  angleY, angleX, angleZ;

    // OBJECT
    Vertexes3D *object;
    QColorDialog *colorPicker;
    QColor objectColor;

    // TRACK CURSOR MOVEMENT
    QPoint cursorSta, cursorEnd;

};
#endif // WIDGET_H
