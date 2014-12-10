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
    QPixmap _pixmap = this->pixmap();
    QRect rect = _pixmap.rect();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPixmap(rect, _pixmap);
}

void CGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *_event)
{
    SetState(true);
    emit Clicked(tag);
    QGraphicsItem::mousePressEvent(_event);
}

void CGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *_event)
{
    update(boundingRect());
    QGraphicsItem::mouseReleaseEvent(_event);
}

void CGraphicsPixmapItem::SetState(bool status)
{
    if (status)
        setPixmap(pixmapEnable);
    else
        setPixmap(pixmapDisable);
}

void CGraphicsPixmapItem::SetTag(int _tag)
{
    this->tag = _tag;
}

CSVCRotaryValveDlg::CSVCRotaryValveDlg(QWidget* p_Parent)
    : MainMenu::CDialogFrame(p_Parent)
    , ui(new Ui::CRotaryValveDlg)
    , tag(0)
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

    QPen pen;
    pen.setColor(QColor(82, 103,111));
    pen.setWidth(3);
    for (int i = 0; i < 16; i++) {
        QGraphicsLineItem *line = mp_Scene->addLine(200, 56, 200, 90, pen);
        line->setTransform(QTransform().translate(200, 200).rotate(22.5 * i).translate(-200, -200));
    }

    // tube item init
    for (int i = 0; i < 16; ++i) {
        rotaryValvePos[i] = new CGraphicsPixmapItem(":/Images/RotaryValveTube.png");
        (void)connect(rotaryValvePos[i], SIGNAL(Clicked(int)), this, SLOT(RotaryValveClicked(int)));

        mp_Scene->addItem(rotaryValvePos[i]);

        rotaryValvePos[i]->setPos(200 - rotaryValvePos[i]->pixmap().width()/2, 4); //
        rotaryValvePos[i]->setTransform(
                    QTransform().translate(rotaryValvePos[i]->pixmap().width()/2, 196)
                    .rotate(22.5 * i)
                    .translate(-rotaryValvePos[i]->pixmap().width()/2, -196));
        rotaryValvePos[i]->SetTag(i);
    }

    // position calibration
    {
        rotaryValvePos[0]->setPos(166, -1); // 166.000000, y: -1.000000
        rotaryValvePos[1]->setPos(162, -1); // 162.000000, y: -1.000000
        rotaryValvePos[2]->setPos(166, -4); // 166.000000, y: -4.000000
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
        rotaryValvePos[13]->setPos(162.5, 5.5); //x: 162.500000, y: 5.500000
        rotaryValvePos[14]->setPos(165.5, 1.5); //x: 165.500000, y: 1.500000
        rotaryValvePos[15]->setPos(163, 4); //x: 163.000000, y: 4.000000
    }

    // sealing item init
    for (int i = 16; i < 32; ++i) {
        rotaryValvePos[i] = new CGraphicsPixmapItem(":/Images/RotaryValveSealing.png", ":/Images/RotaryValveSealingDisable.png");
        (void)connect(rotaryValvePos[i], SIGNAL(Clicked(int)), this, SLOT(RotaryValveClicked(int)));

        mp_Scene->addItem(rotaryValvePos[i]);
        rotaryValvePos[i]->setPos(200 - rotaryValvePos[i]->pixmap().width()/2, 200 - rotaryValvePos[i]->pixmap().height() - 98 + 15); //
        rotaryValvePos[i]->setTransform(
                    QTransform().translate(rotaryValvePos[i]->pixmap().width()/2, rotaryValvePos[i]->pixmap().height() + 98 - 15)
                    .rotate(11.25 + 22.5 * (i - 16))
                    .translate(-rotaryValvePos[i]->pixmap().width()/2, -rotaryValvePos[i]->pixmap().height() - 98 + 15));
        rotaryValvePos[i]->SetTag(i);
    }

    // pointer init
    rotaryValvePointer = new CGraphicsPixmapItem(":/Images/RotaryValvePointer.png", ":/Images/RotaryValvePointer.png");
    mp_Scene->addItem(rotaryValvePointer);
    rotaryValvePointer->setPos(QPoint(200 - rotaryValvePointer->pixmap().width()/2, 200 - rotaryValvePointer->pixmap().height() + 12)); // P1

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

void CSVCRotaryValveDlg::RotaryValveClicked(int _tag)
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

    flag = tag < 16;
    position = pos_table[tag];
}

}//end of namespace SVCDiagnostics
