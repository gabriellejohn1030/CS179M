#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog) {
    ui->setupUi(this);

    debugMode = false;
    loggedIn = false;
    isFinished = false;

    currStep = 0;
    totalSteps = 0;

    connect(&h, &helper::balanceFinished, this, &Dialog::onBalanceFinished);

    ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
    this->setWindowTitle("");

    ui->loginTextEdit->setPlaceholderText("Enter login username");

    QList<QPushButton *> allButtons = ui->stackedWidget->findChildren<QPushButton *>();

    for (auto it: allButtons) {
        it->setFocusPolicy(Qt::NoFocus);
    }

    // Checkboxes position works as 0-7 down, then 8-15, etc
    int size = 30;
    for(int x = 0; x < 12; ++x){
        for(int y = 0; y < 8; ++y){
            checkBoxVector.append(new QCheckBox(ui->unloadContainerPage));
            checkBoxVector.last()->setGeometry((x * size)+125, ((y * size)+25), size, size);
        }
    }

    for (auto it: checkBoxVector) {
        it->setFocusPolicy(Qt::NoFocus);
    }
    ui->debugModeCheckBox->setChecked(debugMode);
    ui->backButton->setVisible(false);
    ui->skipPageLoadingButton->setVisible(debugMode);
//    ui->debugModeCheckBox->setVisible(debugMode);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::on_loginTextEdit_returnPressed() {
    if (!ui->loginTextEdit->text().isEmpty()) {
        if (loggedIn) {
            log.WriteToLog(currUser + " signs out");
        } else {
            loggedIn = true;
        }
        currUser = ui->loginTextEdit->text();
        log.WriteToLog(currUser + " signs in");
        ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
        QString userLabel = "Current User: " + currUser;
        ui->currUserLabel->setText(userLabel);
        ui->loginTextEdit->clear();
    }
}

void Dialog::on_uploadManifestButton_clicked() {
    isFinished = false;
    if (debugMode) {
        ui->stackedWidget->setCurrentIndex(currentPage::balanceLoadPage);
    }
    else if (!loggedIn) {
        QMessageBox note;
        note.setText("Please login first.");
        note.exec();
    } else {
        QString fileName = QFileDialog::getOpenFileName(
                    this,
                    "Select Manifest",
                    QDir::currentPath(),
                    "Text files (*.txt)");
        if (!fileName.isNull()) {
            // check how many containers there are in ship here
//             QString containers = to_QString(CountContainers());
            QString containers = "12";
            log.WriteManifestName(fileName);
            problem = h.loadManifest(fileName.toStdString());

            log.WriteToLog("Manifest " + fileName + " is opened, there are " + containers + " containers on the ship");
            ui->stackedWidget->setCurrentIndex(currentPage::balanceLoadPage);
        }
    }
}

void Dialog::on_backButton_clicked() {
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}

void Dialog::on_balanceButton_clicked() {
    ui->stackedWidget->setCurrentIndex(currentPage::loadingPage);
    QTimer::singleShot(2000, [this]() {
        h.balance(problem);
    });
    // set step labels text
}

void Dialog::on_skipPageLoadingButton_clicked() {
    ui->stackedWidget->setCurrentIndex(currentPage::viewStepPage);
}

void Dialog::on_debugModeCheckBox_stateChanged(int arg1) {
    debugMode = (arg1 > 0);
    int idx = ui->stackedWidget->currentIndex();
    bool isVisible = (idx == currentPage::balanceLoadPage || idx == currentPage::loadContainerPage
                      || idx == currentPage::unloadContainerPage);
    ui->backButton->setVisible(isVisible);
    if (debugMode) {
        ui->skipPageLoadingButton->setVisible(ui->skipPageLoadingButton->isVisible() ? false : true);
    }
}

void Dialog::on_makeNoteButton_clicked() {
    bool ok;
    QString text = QInputDialog::getText(this, tr(""),
                                            tr("Make Note"), QLineEdit::Normal,
                                            "", &ok);
    if (!text.isEmpty()) {
        log.WriteToLog(text);
    }
}

void Dialog::on_nextStepButton_clicked() {
    if (isFinished) {
        ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
    }
    currStep++;
    if (currStep >= totalSteps) {
        ui->nextStepButton->setText("Finish");
        isFinished = true;
    }
}

void Dialog::on_loadButton_clicked() {
    ui->stackedWidget->setCurrentIndex(currentPage::loadContainerPage);
    // load/unload function
}

void Dialog::on_loadContainerLineEdit_returnPressed() {
    QString containers = ui->loadContainerLineEdit->text();
    // parse through QString to get containers needed
    // also validate that the input is in the correct format
    // if (inputValid(containers)) {
    //  ...
    // }
    //
    ui->stackedWidget->setCurrentIndex(currentPage::unloadContainerPage);
}

void Dialog::on_unloadButton_clicked() {
    // iterate through checkboxes and see which ones are checked
    // jump to loading page

    ui->stackedWidget->setCurrentIndex(currentPage::loadingPage);
    // call unloading/loading function
    for (int i = 0; i < 96; i++) {
        if (checkBoxVector[i]->isChecked()) {
            qDebug() << "box is checked at: " << i;
        }
    }
}

void Dialog::onBalanceFinished(bool success) {
    if (success) {
        qDebug() << "Balancing successfull yay";
        ui->stackedWidget->setCurrentIndex(currentPage::viewStepPage);
    } else {
        qDebug() << "balancing successful sad";
    }
}

