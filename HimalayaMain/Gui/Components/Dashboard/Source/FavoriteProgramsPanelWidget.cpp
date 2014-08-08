#include "Dashboard/Include/FavoriteProgramsPanelWidget.h"
#include "ui_FavoriteProgramsPanelWidget.h"
#include "Core/Include/DataConnector.h"
#include "HimalayaDataContainer/Containers/Programs/Include/DataProgramList.h"
#include "Global/Include/Utils.h"
#include "Dashboard/Include/DashboardDateTimeWidget.h"
#include "Core/Include/Startup.h"
#include "Core/Include/GlobalHelper.h"

#include "HimalayaDataContainer/Containers/Programs/Include/Program.h"


using namespace Dashboard;

QString CFavoriteProgramsPanelWidget::SELECTED_PROGRAM_NAME = tr("");

CFavoriteProgramsPanelWidget::CFavoriteProgramsPanelWidget(QWidget *p) :
    QWidget(p),
    ui(new Ui::CFavoriteProgramsPanelWidget),
    mp_DataConnector(NULL),
    m_ProcessRunning(false),
    m_LastSelectedButtonId(-1),
    m_LastCanBeSelectedButtonId(-1)
{
    ui->setupUi(this);
    SetButtonGroup();

    mp_wdgtDateTime = new Dashboard::CDashboardDateTimeWidget(this);
    mp_wdgtDateTime->setModal(true);
    CONNECTSIGNALSIGNAL(mp_wdgtDateTime, OnSelectDateTime(const QDateTime &), this, OnSelectEndDateTime(const QDateTime &));
    CONNECTSIGNALSIGNAL(mp_wdgtDateTime, RequestAsapDateTime(), this, RequestAsapDateTime());
    CONNECTSIGNALSLOT(this, SendAsapDateTime(int, bool), mp_wdgtDateTime, OnGetASAPDateTime(int, bool));
    CONNECTSIGNALSLOT(ui->BtnProgram1, clicked(bool), this, OnEndTimeButtonClicked());
    CONNECTSIGNALSLOT(ui->BtnProgram2, clicked(bool), this, OnEndTimeButtonClicked());
    CONNECTSIGNALSLOT(ui->BtnProgram3, clicked(bool), this, OnEndTimeButtonClicked());
    CONNECTSIGNALSLOT(ui->BtnProgram4, clicked(bool), this, OnEndTimeButtonClicked());
    CONNECTSIGNALSLOT(ui->BtnProgram5, clicked(bool), this, OnEndTimeButtonClicked());
    AddItemsToFavoritePanel(false);
}

CFavoriteProgramsPanelWidget::~CFavoriteProgramsPanelWidget()
{
    try {
            delete ui;
            delete mp_wdgtDateTime;
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
    (void)m_mapLabel.insert(0, ui->lblPrg1);
    (void)m_mapLabel.insert(1, ui->lblPrg2);
    (void)m_mapLabel.insert(2, ui->lblPrg3);
    (void)m_mapLabel.insert(3, ui->lblPrg4);
    (void)m_mapLabel.insert(4, ui->lblPrg5);
}

void CFavoriteProgramsPanelWidget::UpdateProgLabel()
{
    for (int i = 0; i < m_mapLabel.count(); ++ i) {
        CProgramLabel* label = m_mapLabel.value(i);
        label->setHighlight(false);
    }

    CProgramLabel* label = m_mapLabel.value(m_LastSelectedButtonId);
    label->setText(SELECTED_PROGRAM_NAME, true);
}

void CFavoriteProgramsPanelWidget::SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector)
{
    mp_wdgtDateTime->SetMainWindow(p_MainWindow);
    mp_DataConnector = p_DataConnector;
	mp_wdgtDateTime->SetUserSettings(p_DataConnector->SettingsInterface->GetUserSettings());
}

void CFavoriteProgramsPanelWidget::UpdateProgram(DataManager::CProgram &Program)
{
    qDebug() << "CFavoriteProgramsPanelWidget::UpdateProgram";

    for ( int j = 0; j < m_FavProgramIDs.count(); j++)
    {
        QString ProgramId = m_FavProgramIDs.at(j);

        if (ProgramId == Program.GetID()) {
            const QString name = Program.GetName();
            DataManager::CProgram *prog = const_cast<DataManager::CProgram *>(mp_ProgramList->GetProgram(ProgramId));
            prog->SetName(name);
            CProgramLabel* label = m_mapLabel.value(j);
            label->setText(name);
        }

        QString ProgramName = mp_ProgramList->GetProgram(ProgramId)->GetName();
        if (ProgramName == Program.GetName())
        {
            QString strIconName;
            if (Program.GetIcon().isEmpty())
            {
                strIconName = "";
            }
            else
            {
                strIconName = ":/HimalayaImages/Icons/Program/"+ Program.GetIcon() + ".png";
            }

            m_ButtonGroup.button(j)->setIcon(QIcon(strIconName));
            break;
        }
        else {
            Core::CGlobalHelper::UnselectProgram();
            emit UpdateFavProgram();
        }
    }
}

void CFavoriteProgramsPanelWidget::AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram)
{
    if (!bOnlyAddCleaningProgram && mp_DataConnector)
    {
        //check again
        QString strReagentIDOfLastStep = mp_DataConnector->SettingsInterface->GetUserSettings()->GetReagentIdOfLastStep();
        bOnlyAddCleaningProgram = strReagentIDOfLastStep != "";
    }

    UndoProgramSelection();
    //loop all program buttons
    for(int i = 0; i < 5; i++)
    {
        QAbstractButton* btn = m_ButtonGroup.button(i);
        if (btn->isEnabled())
        {
            btn->setEnabled(false);
            btn->setIcon(QIcon(""));

            CProgramLabel* label = m_mapLabel.value(i);
            label->setText("");
        }
    }

    if (!mp_DataConnector)
    return;

    m_FavProgramIDs.clear();
    mp_ProgramList = mp_DataConnector->ProgramList;
    if (bOnlyAddCleaningProgram)
    {
        m_FavProgramIDs.append("C01");
    }
    else
    {
        m_FavProgramIDs = mp_ProgramList->GetFavoriteProgramIDs(); // get five favorite Programs' ID
    }



    for ( int j = 0; j < m_FavProgramIDs.count(); j++)
    {
        QString ProgramId = m_FavProgramIDs.at(j);
        QString ProgramName = mp_ProgramList->GetProgram(ProgramId)->GetName();
        QString strIconName;
        if (mp_ProgramList->GetProgram(ProgramId)->GetIcon().isEmpty())
        {
            strIconName = "";
        }
        else
            strIconName = ":/HimalayaImages/Icons/Program/"+ mp_ProgramList->GetProgram(ProgramId)->GetIcon() + ".png";

        //enable this button and use it
        m_ButtonGroup.button(j)->setEnabled(true);
        m_ButtonGroup.button(j)->setIcon(QIcon(strIconName));
        CProgramLabel* label = m_mapLabel.value(j);
        label->setText(ProgramName);
        CONNECTSIGNALSLOT(m_ButtonGroup.button(j), toggled(bool), label, setHighlight(bool));
    }
}


void CFavoriteProgramsPanelWidget::OnProcessStateChanged()
{
   m_ProcessRunning = MainMenu::CMainWindow::GetProcessRunningStatus();
}

void CFavoriteProgramsPanelWidget::ProgramSelected(QString& programId, int asapEndTime, bool bProgramStartReady, bool bIsFirstStepFixation)
{
    Q_UNUSED(programId);
    Q_UNUSED(bProgramStartReady);
    emit SendAsapDateTime(asapEndTime, bIsFirstStepFixation);
}

void CFavoriteProgramsPanelWidget::UndoProgramSelection()
{
    m_ButtonGroup.setExclusive(false);
    int checkedID = m_ButtonGroup.checkedId();
    if (checkedID != -1)
    {
        m_ButtonGroup.button(checkedID)->setChecked(false);
    }
    m_ButtonGroup.setExclusive(true);
    m_LastSelectedButtonId = -1;
    m_LastCanBeSelectedButtonId = -1;
}

void CFavoriteProgramsPanelWidget::OnEndTimeButtonClicked()
{
    if ((m_LastSelectedButtonId == m_ButtonGroup.checkedId()) && (m_FavProgramIDs.at(0) != "C01"))
    {   //show Datetime dialog
        mp_wdgtDateTime->UpdateProgramName();
        mp_wdgtDateTime->show();
    }
    else
    {   //on selected a program
        m_LastSelectedButtonId = m_ButtonGroup.checkedId();
        m_NewSelectedProgramId = m_FavProgramIDs.at(m_LastSelectedButtonId);
        SELECTED_PROGRAM_NAME = mp_ProgramList->GetProgram(m_NewSelectedProgramId)->GetName();

        emit PrepareSelectedProgramChecking(m_NewSelectedProgramId);
    }
}

void CFavoriteProgramsPanelWidget::OnResetFocus(bool rst)
{
    if (rst) {
        m_ButtonGroup.setExclusive(false);
        int checkedID = m_ButtonGroup.checkedId();
        if (checkedID != -1)
        {
            m_ButtonGroup.button(checkedID)->setChecked(false);
        }
        m_ButtonGroup.setExclusive(true);

        if (m_LastCanBeSelectedButtonId != -1) {
            m_ButtonGroup.setExclusive(false);
            m_ButtonGroup.button(m_LastCanBeSelectedButtonId)->setChecked(true);
            m_ButtonGroup.setExclusive(true);
            m_LastSelectedButtonId = m_ButtonGroup.checkedId();
            m_NewSelectedProgramId = m_FavProgramIDs.at(m_LastSelectedButtonId);
            SELECTED_PROGRAM_NAME = mp_ProgramList->GetProgram(m_NewSelectedProgramId)->GetName();
        }
        else {
            m_LastSelectedButtonId = -1;
            m_LastCanBeSelectedButtonId = -1;
        }
    }
    else {
        m_LastCanBeSelectedButtonId = m_ButtonGroup.checkedId();
    }
}

