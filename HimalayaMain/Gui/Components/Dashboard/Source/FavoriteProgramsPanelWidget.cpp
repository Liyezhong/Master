#include "Dashboard/Include/FavoriteProgramsPanelWidget.h"
#include "ui_FavoriteProgramsPanelWidget.h"
#include "Core/Include/DataConnector.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include <QLabel>

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

        m_mapLabel.clear();
        m_mapLabel.insert(0, ui->lblPrg1);
        m_mapLabel.insert(1, ui->lblPrg2);
        m_mapLabel.insert(2, ui->lblPrg3);
        m_mapLabel.insert(3, ui->lblPrg4);
        m_mapLabel.insert(4, ui->lblPrg5);

        m_mapButtonProgramId.clear();
        for(int i = 0; i < 5; i++)
        {
            m_mapButtonProgramId.insert(i, "");
        }

    }

    void CFavoriteProgramsPanelWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector)
    {
        Q_UNUSED(p_MainWindow);
        mp_DataConnector = p_DataConnector;
    }

    void CFavoriteProgramsPanelWidget::AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram)
    {
        if (!mp_DataConnector)
        return;

        QStringList favProgramIDs;
        mp_ProgramList = mp_DataConnector->ProgramList;
        if (bOnlyAddCleaningProgram)
        {
            favProgramIDs.append("C01");
        }
        else
        {
            favProgramIDs = mp_ProgramList->GetFavoriteProgramIDs(); // get five favorite Programs' ID
        }

        //loop all program buttons
        QMapIterator<int, QString> iter(m_mapButtonProgramId);
        while (iter.hasNext()) {
            iter.next();
            QString val = iter.value();
            if (favProgramIDs.contains(val))//in favProgramIDs
            {
               favProgramIDs.removeOne(iter.value());
            }
            else//not in favProgramIDs
            {
                QAbstractButton* btn = m_ButtonGroup.button(iter.key());
                btn->setEnabled(false);
                btn->setIcon(QIcon(""));

                QLabel* label = m_mapLabel.value(iter.key());
                label->setText("");
                m_mapButtonProgramId[iter.key()] = "";
            }
        }

        for ( int i = 0; i < favProgramIDs.count(); i++)
        {
            QString ProgramId = favProgramIDs.at(i);
            QString ProgramName = mp_ProgramList->GetProgram(ProgramId)->GetName();
            QString strIconName;
            if (mp_ProgramList->GetProgram(ProgramId)->GetIcon().isEmpty())
            {
                strIconName = ":/HimalayaImages/Icons/Program/IconEmpty.png";
            }
            else
                strIconName = ":/HimalayaImages/Icons/Program/"+ mp_ProgramList->GetProgram(ProgramId)->GetIcon() + ".png";

            for (int j = 0; j < 5; j++)
            {
                if(!m_ButtonGroup.button(j)->isEnabled())
                {
                    //enable this button and use it
                    m_ButtonGroup.button(j)->setEnabled(true);
                    m_ButtonGroup.button(j)->setIcon(QIcon(strIconName));
                    QLabel* label = m_mapLabel.value(j);
                    label->setText(ProgramName);
                    m_mapButtonProgramId[j] = ProgramId;
                    break;
                }
            }
        }
    }

    void CFavoriteProgramsPanelWidget::on_pushButton_6_clicked()
    {

    }

}

