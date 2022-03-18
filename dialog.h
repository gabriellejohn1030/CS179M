#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QDir>
#include <QMessageBox>
#include <QButtonGroup>
#include <QCheckBox>
#include <QVector>
#include <QTimer>

#include "logger.h"
#include "ship.h"
#include "helper.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

enum currentPage {
    uploadManifestPage,
    balanceLoadPage,
    loadContainerPage,
    unloadContainerPage,
    loadingPage,
    viewStepPage,
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    void initializeUnloadCheckboxes();
    vector<vector<Container*>> Transpose(const vector<vector<Container*>> &);
    ~Dialog();

private slots:
    void on_loginTextEdit_returnPressed();

    void on_uploadManifestButton_clicked();

    void on_backButton_clicked();

    void on_balanceButton_clicked();

    void on_skipPageLoadingButton_clicked();

    void on_debugModeCheckBox_stateChanged(int arg1);

    void on_makeNoteButton_clicked();

    void on_nextStepButton_clicked();

    void on_loadButton_clicked();

    void on_loadContainerLineEdit_returnPressed();

    void on_unloadButton_clicked();

    void onBalanceFinished(bool success);
    void onContainersFound(int c);

    void on_doneLoadingButton_clicked();

private:
    Ui::Dialog *ui;
    QVector<QCheckBox*> checkBoxVector;
    QVector<QLabel*> labelChkVector;
    logger log;
    helper h;
    Ship* problem;
    QVector<Container*> loadOnContainers;

    bool debugMode;
    bool loggedIn;
    bool isFinished;
    int currStep;
    int totalSteps;

    QString currUser;
    QString manifestFileName;
};
#endif // DIALOG_H
