#ifndef FAVORITEPROGRAMSPANELWIDGET_H
#define FAVORITEPROGRAMSPANELWIDGET_H

#include <QWidget>
#include <QButtonGroup>

namespace Dashboard {
    namespace Ui {
    class CFavoriteProgramsPanelWidget;
    }

    class CFavoriteProgramsPanelWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit CFavoriteProgramsPanelWidget(QWidget *parent = 0);
        ~CFavoriteProgramsPanelWidget();

    private slots:
        void on_pushButton_6_clicked();

    private:

        void SetButtonGroup();
        Ui::CFavoriteProgramsPanelWidget *ui;
        QButtonGroup m_ButtonGroup;
    };
}
#endif // FAVORITEPROGRAMSPANELWIDGET_H
