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

    connect(&h, &helper::containersFound, this, &Dialog::onContainersFound);
    connect(&h, &helper::balanceFinished, this, &Dialog::onBalanceFinished);
    connect(&h, &helper::loadAndUnloadFinished, this, &Dialog::onLoadAndUnloadFinished);
    ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
    this->setWindowTitle("");
    QFont custom_font = QFont("Arial", 16, QFont::Bold);
//    custom_font.setWeight(18);
//    QApplicationsetFont(custom_font, "QLabel")
    QApplication::setFont(custom_font, "QLabel");
    QApplication::setFont(custom_font, "QPushButton");
    ui->loginTextEdit->setPlaceholderText("Enter login username");
    ui->enterContainersLabel->setText("Enter containers in format: {mass, container name} followed by return key.\n"
                                      "Click 'Done' when finished.");
    ui->enterContainersLabel->setAlignment(Qt::AlignLeft);

    QList<QPushButton *> allButtons = ui->stackedWidget->findChildren<QPushButton *>();
    ui->backButton->setVisible(false);
    ui->backButton->setFocusPolicy(Qt::NoFocus);
    for (auto it: allButtons) {
        it->setFocusPolicy(Qt::NoFocus);
    }

    ui->debugModeCheckBox->setChecked(debugMode);
    ui->skipPageLoadingButton->setVisible(debugMode);
    ui->debugModeCheckBox->setVisible(debugMode);
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
        ui->currUserLabel->setAlignment(Qt::AlignCenter);
        ui->loginTextEdit->clear();
    }
}

void Dialog::on_uploadManifestButton_clicked() {
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
            qDebug() << "Opened " << fileName << " manifest file.";
            log.WriteManifestName(fileName);
            manifestFileName = fileName;
            problem = h.loadManifest(fileName.toStdString());
            initializeUnloadCheckboxes();
            // gray out checkboxes
            ui->stackedWidget->setCurrentIndex(currentPage::balanceLoadPage);
            ui->backButton->setVisible(true);
        }
    }
}

vector<vector<Container*>> Dialog::Transpose(const vector<vector<Container*>> &g) {
    vector<vector<Container*>> res(g[0].size(), vector<Container*>());
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            res[j].push_back(g[i][j]);
        }
    }
    return res;
}

void Dialog::initializeUnloadCheckboxes() {
    qDeleteAll(checkBoxVector);
    checkBoxVector.clear();
    int size = 30;
    vector<vector<Container*>> grid = Transpose(problem->getGrid());
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            checkBoxVector.append(new QCheckBox(ui->unloadContainerPage));
            if (grid[i][j]->contents == "NAN" || grid[i][j]->contents == "UNUSED") {
                checkBoxVector.last()->setEnabled(false);
            }
            checkBoxVector.last()->setGeometry(((i * size)+125)*SCALE_FACTOR, (((j * size)+10))*SCALE_FACTOR, size*SCALE_FACTOR, size*SCALE_FACTOR);
        }
    }
    QLabel selectUnloadContainers("Select Containers to Unload", ui->unloadContainerPage);
    selectUnloadContainers.setGeometry(100*SCALE_FACTOR, 100*SCALE_FACTOR, 300*SCALE_FACTOR, 100*SCALE_FACTOR);

    for (auto it: checkBoxVector) {
        it->setFocusPolicy(Qt::NoFocus);
    }
}

void Dialog::on_backButton_clicked() {
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
    if (ui->stackedWidget->currentIndex() == currentPage::uploadManifestPage) {
        ui->backButton->setVisible(false);
    }
}

void Dialog::on_balanceButton_clicked() {
    ui->stackedWidget->setCurrentIndex(currentPage::loadingPage);
    ui->backButton->setVisible(false);
    if (!debugMode) {
        QTimer::singleShot(10, [this]() {
            h.balance(problem);
        });
    }
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
        ui->nextStepButton->setText("Next");
        isFinished = false;
        ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
//        ui->backButton->setVisible(true);
        h.updateManifest(h.getGoal(), manifestFileName.toStdString());

        QMessageBox note;
        QString temp = manifestFileName;
        temp.chop(4);
        note.setText(temp + "OUTBOUND.txt was downloaded");
        log.WriteToLog("Finished a Cycle. Manifest " + temp + " was written"
                       + " to desktop, and a reminder pop-up to operator to send file was displayed.");
        note.exec();
//        ui->backButton->setVisible(true);
    } else if (currStep == totalSteps-1) {
        ui->moveContainerLabel->setText("Finished all moves");
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);
        ui->nextStepButton->setText("Finish");
        currStep = 0;
        totalSteps = 0;
        isFinished = true;
    } else {
        currStep++;
        QString currMove = QString::fromStdString(h.getMoves()[currStep]);
        if (currMove.contains("Container from buffer/truck")) {
            log.WriteToLog("Container is onloaded.");
        } else if (currMove.contains("to a truck.")) {
            log.WriteToLog("Container is offloaded.");
        }
        ui->moveContainerLabel->setText(currMove);
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);
        ui->currStepPageLabel->setText("Step " + QString::number(currStep + 1) + "/" + QString::number(totalSteps));
        ui->estimatedTimeLabel->setText("Estimated Time For Move: " + QString::number(timeEstimates.at(currStep)) + " Minutes");
    }
    qDebug() << "Current step: " << currStep << " out of " << totalSteps << "total Steps";
}

void Dialog::on_loadButton_clicked() {
    ui->stackedWidget->setCurrentIndex(currentPage::loadContainerPage);
    // load/unload function
}

void Dialog::on_loadContainerLineEdit_returnPressed() {
    QString input = ui->loadContainerLineEdit->text();
    int weight;
    QString name;
    QString temp;
    bool cFound = false;
    bool fail = false;
    for (int j = 0; j < input.length(); j++) {
        if (input[j] == ',') {
            cFound = true;
        } else if (input[j].digitValue() == -1 && !cFound) {
            fail = true;
        }
    }
    if (!cFound || fail) {
        QMessageBox note;
        note.setText("Please input in correct format. Ensure mass is greater than zero.");
        note.exec();
        fail = true;
    }
    int i = 0;
    if (!input.isEmpty() && !fail) {
        while (input[i] != ',' && i < input.length()) {
            temp.push_back(input[i]);
            i++;
        }
        weight = temp.toInt();
        i += 2;
        for (i; i < input.length(); i++) {
            name.push_back(input[i]);
        }
        qDebug() << "User asked to load container " << name << " with weight " << weight;
        loadOnContainers.push_back(new Container(weight, name.toStdString()));
        ui->loadContainerLineEdit->clear();
    }
}

void Dialog::on_unloadButton_clicked() {
    // iterate through checkboxes and see which ones are checked
    // jump to loading page
    ui->backButton->setVisible(false);
    ui->stackedWidget->setCurrentIndex(currentPage::loadingPage);

    for (int i = 0; i < 96; i++) {
        if (checkBoxVector[i]->isChecked()) {
            unloadContainers.push_back(pair<int,int>(i%8,i/8));
            qDebug() << "box is checked at: " << i%8 << " " << i/8 << " with i of: " << i;
        }
    }
    qDebug() << "Containers being unloaded:";
    for (auto it: unloadContainers) {
        qDebug() << it.first << " " << it.second;
    }
    qDebug() << "Containers being loaded:";
    for (auto it: loadOnContainers) {
        qDebug() << QString::fromStdString(it->contents) << " " << it->weight;
    }
    h.unloadAndLoadAlgorithm(unloadContainers, problem, loadOnContainers);

    // call unloading/loading function
    // 23 = 7,2 maps to (23+1) / (2+1)

}

void Dialog::onBalanceFinished(bool success) {
    if (success) {
        ui->stackedWidget->setCurrentIndex(currentPage::viewStepPage);
        ui->moveContainerLabel->setText(QString::fromStdString(h.getMoves()[0]));
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);

        totalSteps = h.getMoves().size();
        qDebug() << "Total steps: " << totalSteps;
        ui->currStepPageLabel->setText("Step " + QString::number(currStep + 1) + "/" + QString::number(totalSteps));
        if (problem->check_SIFT()) {
            qDebug() << "Check Sift returns true";
            timeEstimates = h.estimated_time_SIFT(problem);
            ui->estimatedTimeLabel->setText("Estimated Time For Move: " + QString::number(timeEstimates.at(0)) +
                                            " Minutes");
        } else {
            timeEstimates = h.getTimes();
            qDebug() << timeEstimates.size();
            ui->estimatedTimeLabel->setText("Estimated time For Move: "  + QString::number(timeEstimates.at(0)) + " Minutes");
        }
        vector<string> moves = h.getMoves();
        for (auto it: moves) {
            qDebug() << QString::fromStdString(it);
        }
    } else {
        qDebug() << "Balancing failed.";
        ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
    }
}

void Dialog::onLoadAndUnloadFinished(bool success) {
    if (success) {
        ui->moveContainerLabel->setText(QString::fromStdString(h.getMoves()[0]));
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);

        totalSteps = h.getMoves().size();
        timeEstimates = h.getTimes();
        qDebug() << "Total steps: " << totalSteps;
        ui->currStepPageLabel->setText("Step " + QString::number(currStep + 1) + "/" + QString::number(totalSteps));
        ui->estimatedTimeLabel->setText("Estimated time For Move: "  + QString::number(timeEstimates.at(0)) + " Minutes");
        vector<string> moves = h.getMoves();
        for (auto it: moves) {
            qDebug() << QString::fromStdString(it);
        }
        ui->stackedWidget->setCurrentIndex(currentPage::viewStepPage);
    } else {
        qDebug() << "Failure";
        ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
    }
}

void Dialog::onContainersFound(int containers) {
    log.WriteToLog("Manifest " + manifestFileName + " is opened, there are " + QString::number(containers) + " containers on the ship.");
}

void Dialog::on_doneLoadingButton_clicked() {
    // put the containers in the proper way
    ui->stackedWidget->setCurrentIndex(currentPage::unloadContainerPage);
}

