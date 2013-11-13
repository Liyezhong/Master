#include "Dashboard/Include/FavoriteProgramsPanelWidget.h"
#include "ui_FavoriteProgramsPanelWidget.h"
namespace Dashboard
{
    CFavoriteProgramsPanelWidget::CFavoriteProgramsPanelWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::CFavoriteProgramsPanelWidget)
    {
        ui->setupUi(this);
        SetButtonGroup();
        connect(ui->BtnProgram1, SIGNAL(clicked(bool)),
                    this, SLOT(on_pushButton_6_clicked()));
    }

    CFavoriteProgramsPanelWidget::~CFavoriteProgramsPanelWidget()
    {
        try {
                delete ui;
            }
            catch (...) {
                // to please Lint.
            }
    }

    void CFavoriteProgramsPanelWidget ::SetButtonGroup()
    {
        m_ButtonGroup.addButton(ui->BtnProgram1, 0);
        m_ButtonGroup.addButton(ui->BtnProgram2, 1);
        m_ButtonGroup.addButton(ui->BtnProgram3, 2);
        m_ButtonGroup.addButton(ui->BtnProgram4, 3);
        m_ButtonGroup.addButton(ui->BtnProgram5, 4);
    }

    void CFavoriteProgramsPanelWidget::on_pushButton_6_clicked()
    {

    }

}

