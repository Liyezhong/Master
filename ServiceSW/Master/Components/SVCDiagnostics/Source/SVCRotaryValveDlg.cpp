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
#include <QGraphicsEllipseItem>
#include "Core/Include/ServiceUtils.h"

namespace SVCDiagnostics {

CGraphicsPixmapItem::CGraphicsPixmapItem(QString file)
    : tag(0)
{
    pixmapEnable = QPixmap(file);
    pixmapDisable = QPixmap(pixmapEnable.size());
    pixmapDisable.fill(Qt::transparent);
    setPixmap(pixmapDisable);
}

QRectF CGraphicsPixmapItem::boundingRect() const
{
    QRect rect = this->pixmap().rect();
    return QRectF(0, 0, rect.width(), rect.width());
}

QPainterPath CGraphicsPixmapItem::shape() const
{
    QRectF rect = boundingRect();

    QPainterPath path;
    path.addRoundRect(rect, 0, 0);
    return path;
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

    // tube item init
    qreal angle = 0;
    for (int i = 0; i < 16; i++, angle += 11.25 * 2) {
        rotaryValvePos[i] = new CGraphicsPixmapItem(":/Images/RotaryValveTube.png");
        (void)connect(rotaryValvePos[i], SIGNAL(Clicked(int)), this, SLOT(RotaryValveClicked(int)));

        mp_Scene->addItem(rotaryValvePos[i]);
        rotaryValvePos[i]->setPos(171, 6);
        rotaryValvePos[i]->setTransform(QTransform().translate(29.5, 194).rotate(angle).translate(-29.5, -194));
        rotaryValvePos[i]->SetTag(i);
    }

    // sealing item init
    angle = 11.25;
    for (int i = 16; i < 32; i++, angle += 11.25 * 2) {
        rotaryValvePos[i] = new CGraphicsPixmapItem(":/Images/RotaryValveSealing.png");
        (void)connect(rotaryValvePos[i], SIGNAL(Clicked(int)), this, SLOT(RotaryValveClicked(int)));

        mp_Scene->addItem(rotaryValvePos[i]);
        rotaryValvePos[i]->setPos(186, 82);
        rotaryValvePos[i]->setTransform(QTransform().translate(14, 118).rotate(angle).translate(-14, -118));
        rotaryValvePos[i]->SetTag(i);
    }

    // pointer init
    rotaryValvePointer = mp_Scene->addPixmap(QPixmap(":/Images/RotaryValvePointer.png"));
    rotaryValvePointer->setPos(QPoint(mp_Scene->width()/2, mp_Scene->height()/2));
    rotaryValvePointer->setPos(QPoint(191,  53));  // P1
}

CSVCRotaryValveDlg::~CSVCRotaryValveDlg()
{
    try {
        for (int i = 0; i < 32; ++i)
            delete rotaryValvePos[i];
        delete mp_Scene;
        delete ui;
    }
    catch (...) {

    }
}

void CSVCRotaryValveDlg::RotaryValveClicked(int tag)
{
    static const qreal angle_table[32] = {
        22.5 * 0,
        22.5 * 1,
        22.5 * 2,
        22.5 * 3,
        22.5 * 4,
        22.5 * 5,
        22.5 * 6,
        22.5 * 7,
        22.5 * 8,
        22.5 * 9,
        22.5 * 10,
        22.5 * 11,
        22.5 * 12,
        22.5 * 13,
        22.5 * 14,
        22.5 * 15,

        11.25 + 22.5 * 0,
        11.25 + 22.5 * 1,
        11.25 + 22.5 * 2,
        11.25 + 22.5 * 3,
        11.25 + 22.5 * 4,
        11.25 + 22.5 * 5,
        11.25 + 22.5 * 6,
        11.25 + 22.5 * 7,
        11.25 + 22.5 * 8,
        11.25 + 22.5 * 8,
        11.25 + 22.5 * 10,
        11.25 + 22.5 * 11,
        11.25 + 22.5 * 12,
        11.25 + 22.5 * 13,
        11.25 + 22.5 * 14,
        11.25 + 22.5 * 15,
    };

    qDebug() << "tag: " << tag;
    for (int i = 0; i < 32; i++) {
        if (i != tag)
            rotaryValvePos[i]->SetState(false);
    }

    this->tag = tag;
    rotaryValvePointer->setTransform(
              QTransform().translate(rotaryValvePointer->pixmap().width()/2, rotaryValvePointer->pixmap().height() - 12)
             .rotate(angle_table[tag])
             .translate(-rotaryValvePointer->pixmap().width()/2, -rotaryValvePointer->pixmap().height() + 12)
    );
}

void CSVCRotaryValveDlg::SetPos(bool flag, qint32 position)
{
    static const qint32 tag_table[32] = {
        14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,  0, 15, // tube position tag
        29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 31, 30  // sealing position tag
    };
    RotaryValveClicked(flag ? tag_table[position - 1] : tag_table[position - 1 + 16]);

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

}//end of namespace SVCDiagnostics
