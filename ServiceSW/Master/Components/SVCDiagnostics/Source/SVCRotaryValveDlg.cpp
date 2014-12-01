/****************************************************************************/
/*! \file SVCRotaryValveDlg.cpp
 *
 *  \brief Implementation of rotary value detail dialog.
 *
 *   $Version: $ 0.1
 *   $Date:    $ 2014-11-15
 *   $Author:  $ Arthur Li
 *
 *  \b Company:
 *
 *       Leica Biosystems R&D Center Shanghai.
 *
 *  (C) Copyright 2014 by LBS R&D Center Shanghai. All rights reserved.
 *  This is unpublished proprietary source code of Leica. The copyright notice
 *  does not evidence any actual or intended publication.
 *
 */
/****************************************************************************/

#include "SVCDiagnostics/Include/SVCRotaryValveDlg.h"

#include "Global/Include/Utils.h"
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QPainter>
#include "Core/Include/ServiceUtils.h"

namespace SVCDiagnostics {

CGraphicsPixmapItem::CGraphicsPixmapItem(QString enableState, QString disableState)
    : tag(0)
{
    pixmapEnable = QPixmap(enableState);

    if (!disableState.length()) {
        pixmapDisable = QPixmap(pixmapEnable.size());
        pixmapDisable.fill(Qt::transparent);
    } else {
        pixmapDisable = QPixmap(disableState);
    }

    setPixmap(pixmapDisable);
}

QRectF CGraphicsPixmapItem::boundingRect() const
{
    QRect rect = this->pixmap().rect();
    return QRectF(0, 0, rect.width(), rect.height());
}

QPainterPath CGraphicsPixmapItem::shape() const
{
    QRectF rect = boundingRect();

    QPainterPath path;
    path.addRoundRect(rect, 0, 0);
    return path;
}

void CGraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
           QWidget *)
{
    QPixmap pixmap = this->pixmap();
    QRect rect = pixmap.rect();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPixmap(rect, pixmap);
}

void CGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    SetState(true);
    emit Clicked(tag);
    QGraphicsItem::mousePressEvent(event);
}

void CGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update(boundingRect());
    QGraphicsItem::mouseReleaseEvent(event);
}

void CGraphicsPixmapItem::SetState(bool status)
{
    if (status)
        setPixmap(pixmapEnable);
    else
        setPixmap(pixmapDisable);
}

void CGraphicsPixmapItem::SetTag(int tag)
{
    this->tag = tag;
}

CSVCRotaryValveDlg::CSVCRotaryValveDlg(QWidget* p_Parent):
    MainMenu::CDialogFrame(p_Parent),
    ui(new Ui::CRotaryValveDlg)
{
    ui->setupUi(GetContentFrame());
    setModal(true);

    (void)connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(accept()));
    (void)connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));

    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    mp_Scene = new QGraphicsScene(ui->graphicsView);
    mp_Scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    mp_Scene->setSceneRect(0, 0, 400, 400);
    mp_Scene->setBackgroundBrush(QImage(":/Images/RotaryValveDetail.png"));
    ui->graphicsView->setScene(mp_Scene);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

    // tube item init
    qreal angle = 0;
    for (int i = 0; i < 16; i++, angle += 11.25 * 2) {
        rotaryValvePos[i] = new CGraphicsPixmapItem(":/Images/RotaryValveTube.png");
        (void)connect(rotaryValvePos[i], SIGNAL(Clicked(int)), this, SLOT(RotaryValveClicked(int)));

        mp_Scene->addItem(rotaryValvePos[i]);

        rotaryValvePos[i]->setPos(200 - rotaryValvePos[i]->pixmap().width()/2, 4); //
        rotaryValvePos[i]->setTransform(
                    QTransform().translate(rotaryValvePos[i]->pixmap().width()/2, 196)
                    .rotate(angle)
                    .translate(-rotaryValvePos[i]->pixmap().width()/2, -196));
        rotaryValvePos[i]->SetTag(i);
    }

    {
        rotaryValvePos[0]->setPos(166, -1); // 166.000000, y: -1.000000
        rotaryValvePos[1]->setPos(162, -1); // 162.000000, y: -1.000000
        rotaryValvePos[2]->setPos(166, -4); // 166.000000, y: -3.000000
        rotaryValvePos[3]->setPos(169.5, -0.5); //x: 169.500000, y: -0.500000
        rotaryValvePos[4]->setPos(169.5, 5.5); //x: 169.500000, y: 5.500000
        rotaryValvePos[5]->setPos(170, 2.5); //x: 170.000000, y: 2.500000
        rotaryValvePos[6]->setPos(170, 5.5); //x: 170.000000, y: 5.500000
        rotaryValvePos[7]->setPos(163, 6); //x: 163.000000, y: 6.000000
        rotaryValvePos[8]->setPos(165, 6); //x: 165.000000, y: 6.000000
        rotaryValvePos[9]->setPos(168, 8); //x: 168.000000, y: 8.000000
        rotaryValvePos[10]->setPos(166.5, 6); //x: 166.500000, y: 6.000000
        rotaryValvePos[11]->setPos(166, 6); //x: 166.000000, y: 6.000000
        rotaryValvePos[12]->setPos(165, 3.5); //x: 165.000000, y: 3.500000
        rotaryValvePos[13]->setPos(162, 5); //x: 162.500000, y: 5.500000
        rotaryValvePos[14]->setPos(165.5, 1.5); //x: 165.500000, y: 1.500000
        rotaryValvePos[15]->setPos(163, 4); //x: 163.000000, y: 4.000000
    }

    // sealing item init
    angle = 11.25;
    for (int i = 16; i < 32; i++, angle += 11.25 * 2) {
        rotaryValvePos[i] = new CGraphicsPixmapItem(":/Images/RotaryValveSealing.png", ":/Images/RotaryValveSealingDisable.png");
        (void)connect(rotaryValvePos[i], SIGNAL(Clicked(int)), this, SLOT(RotaryValveClicked(int)));

        mp_Scene->addItem(rotaryValvePos[i]);
        rotaryValvePos[i]->setPos(200 - rotaryValvePos[i]->pixmap().width()/2, 200 - rotaryValvePos[i]->pixmap().height() - 98 + 15); //
        rotaryValvePos[i]->setTransform(
                    QTransform().translate(rotaryValvePos[i]->pixmap().width()/2, rotaryValvePos[i]->pixmap().height() + 98 - 15)
                    .rotate(angle)
                    .translate(-rotaryValvePos[i]->pixmap().width()/2, -rotaryValvePos[i]->pixmap().height() - 98 + 15));
        rotaryValvePos[i]->SetTag(i);
    }

    // pointer init
    rotaryValvePointer = new CGraphicsPixmapItem(":/Images/RotaryValvePointer.png", ":/Images/RotaryValvePointer.png");
    mp_Scene->addItem(rotaryValvePointer);
    rotaryValvePointer->setPos(QPoint(200 - rotaryValvePointer->pixmap().width()/2, 200 - rotaryValvePointer->pixmap().height() + 12)); // P1

// just for debug & test
//    for (int i = 0; i < 32; i++) {
//        QGraphicsLineItem *line = mp_Scene->addLine(0, 0, mp_Scene->width(), mp_Scene->height(), QPen(Qt::cyan));
//        line->setTransform(QTransform().translate(200, 200).rotate(11.25 * i).translate(-200, -200));
//    }

//    buttonPosX_inc = new QPushButton("x+");
//    buttonPosX_dec = new QPushButton("x-");

//    buttonPosY_inc = new QPushButton("y+");
//    buttonPosY_dec = new QPushButton("y-");

//    EditPos = new QLineEdit;
//    editIndex = new QLineEdit;

//    mp_Scene->addWidget(buttonPosX_inc)->setPos(200, 0);
//    mp_Scene->addWidget(buttonPosX_dec)->setPos(200, 50);
//    mp_Scene->addWidget(buttonPosY_inc)->setPos(200, 100);

//    mp_Scene->addWidget(buttonPosY_dec)->setPos(200, 150);
//    mp_Scene->addWidget(EditPos)->setPos(200, 250);
//    mp_Scene->addWidget(editIndex)->setPos(200, 300);

//    (void)connect(buttonPosX_inc, SIGNAL(clicked()), this, SLOT(setPosIncrease_x()));
//    (void)connect(buttonPosX_dec, SIGNAL(clicked()), this, SLOT(setPosDecrease_x()));
//    (void)connect(buttonPosY_inc, SIGNAL(clicked()), this, SLOT(setPosIncrease_y()));
//    (void)connect(buttonPosY_dec, SIGNAL(clicked()), this, SLOT(setPosDecrease_y()));
}

CSVCRotaryValveDlg::~CSVCRotaryValveDlg()
{
    try {
        for (int i = 0; i < 32; ++i)
            delete rotaryValvePos[i];
        delete rotaryValvePointer;
        delete mp_Scene;
        delete ui;
    } catch (...) {

    }
}

void CSVCRotaryValveDlg::RotaryValveClicked(int tag)
{
    qreal angle;

    if (tag < 16)
        angle = 22.5 * tag;
    else
        angle = 11.25 + 22.5 * (tag - 16);

    qDebug() << "tag: " << tag;
    for (int i = 0; i < 32; i++) {
        if (i != tag)
            rotaryValvePos[i]->SetState(false);
    }

    this->tag = tag;
    rotaryValvePointer->setTransform(
              QTransform().translate(rotaryValvePointer->pixmap().width()/2, rotaryValvePointer->pixmap().height() - 12)
             .rotate(angle)
             .translate(-rotaryValvePointer->pixmap().width()/2, -rotaryValvePointer->pixmap().height() + 12)
    );
}

void CSVCRotaryValveDlg::SetPos(bool flag, qint32 position)
{
    static const qint32 tag_table[32] = {
        14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,  0, 15, // tube position tag
        29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 31, 30  // sealing position tag
    };
    tag = flag ? tag_table[position - 1] : tag_table[position - 1 + 16];
    RotaryValveClicked(tag);
    rotaryValvePos[tag]->SetState(true);

    QString text(tr("Rotary valve current position :  "));
    if (flag == false)
        text += tr("X");
    if (position == 14)
        text += tr("P1");
    else if (position == 15)
        text += tr("P2");
    else if (position == 16)
        text += tr("P3");
    else
        text += tr("%1").arg(position);

    ui->labelCurrentPosition->setText(text);
}

void CSVCRotaryValveDlg::GetPos(bool &flag, qint32 &position)
{
    static const qint32 pos_table[32] = {
        15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 16, // tube position
        14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 16, 15  // sealing position
    };

    if (tag < 16)
        flag = true;
    else
        flag = false;
    position = pos_table[tag];
}

//void CSVCRotaryValveDlg::setPosIncrease_x()
//{
//    int i = editIndex->text().toInt();
//    if (i < 0 || i > 32)
//        return;
//    QPointF p = rotaryValvePos[i]->pos();
//    rotaryValvePos[i]->setPos(p.x() + 0.5, p.y());
//    EditPos->setText(QString().sprintf("x: %f, y: %f", p.x() + 0.5, p.y()));
//}

//void CSVCRotaryValveDlg::setPosDecrease_x()
//{
//    int i = editIndex->text().toInt();
//    if (i < 0 || i > 32)
//        return;
//    QPointF p = rotaryValvePos[i]->pos();
//    rotaryValvePos[i]->setPos(p.x() - 0.5, p.y());
//    EditPos->setText(QString().sprintf("x: %f, y: %f", p.x() - 0.5, p.y()));
//}

//void CSVCRotaryValveDlg::setPosIncrease_y()
//{
//    int i = editIndex->text().toInt();
//    if (i < 0 || i > 32)
//        return;
//    QPointF p = rotaryValvePos[i]->pos();
//    rotaryValvePos[i]->setPos(p.x(), p.y() + 0.5);
//    EditPos->setText(QString().sprintf("x: %f, y: %f", p.x(), p.y() + 0.5));
//}

//void CSVCRotaryValveDlg::setPosDecrease_y()
//{
//    int i = editIndex->text().toInt();
//    if (i < 0 || i > 32)
//        return;
//    QPointF p = rotaryValvePos[i]->pos();
//    rotaryValvePos[i]->setPos(p.x(), p.y() - 0.5);
//    EditPos->setText(QString().sprintf("x: %f, y: %f", p.x(), p.y() - 0.5));
//}

}//end of namespace SVCDiagnostics
