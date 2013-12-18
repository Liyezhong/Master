#ifndef PROGRAMPANELWIDGET_H
#define PROGRAMPANELWIDGET_H

#include "MainMenu/Include/PanelFrame.h"

class QDateTime;
namespace MainMenu
{
    class CMainWindow ;
}

namespace Core
{
    class CDataConnector;
}

namespace Dashboard {

namespace Ui {
    class CProgramPanelWidget;
}

class CProgramPanelWidget : public MainMenu::CPanelFrame
{
    Q_OBJECT
    
public:
    explicit CProgramPanelWidget(QWidget *parent = 0);
    ~CProgramPanelWidget();

    void SetPtrToMainWindow(MainMenu::CMainWindow *p_MainWindow, Core::CDataConnector *p_DataConnector);

signals:
    void AddItemsToFavoritePanel(bool bOnlyAddCleaningProgram);
    void PrepareSelectedProgramChecking(const QString& selectedProgramId);
    void OnSelectEndDateTime(const QDateTime &);
    void ProgramSelected(QString& ProgramId, int asapEndTime);
protected:
    void changeEvent(QEvent *p_Event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    void RetranslateUI();
    Ui::CProgramPanelWidget *ui;
};

}// end namespace Dashboard

#endif // PROGRAMPANELWIDGET_H
