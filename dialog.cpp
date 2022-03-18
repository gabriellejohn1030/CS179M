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
    ui->stackedWidget->setCurrentIndex(currentPage::uploadManifestPage);
    this->setWindowTitle("");

    ui->loginTextEdit->setPlaceholderText("Enter login username");
    ui->enterContainersLabel->setText("Enter containers in format: {mass, container name}\n"
                                      "Click 'Done' when finished.");
    ui->enterContainersLabel->setAlignment(Qt::AlignLeft);

    QList<QPushButton *> allButtons = ui->stackedWidget->findChildren<QPushButton *>();

    for (auto it: allButtons) {
        it->setFocusPolicy(Qt::NoFocus);
    }

    // Checkboxes position works as 0-7 down, then 8-15, etc
//    int size = 30;
//    for(int x = 0; x < 12; ++x){
//        for(int y = 0; y < 8; ++y){
//            checkBoxVector.append(new QCheckBox(ui->unloadContainerPage));
//            checkBoxVector.last()->setGeometry((x * size)+125, ((y * size)+10), size, size);
//        }
//    }

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
    checkBoxVector.clear();
    int size = 30;
    vector<vector<Container*>> grid = Transpose(problem->getGrid());

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            checkBoxVector.append(new QCheckBox(ui->unloadContainerPage));
            if (grid[i][j]->contents == "NAN") {
                checkBoxVector.last()->setEnabled(false);
            } /*else if (grid[i][j]->contents != "UNUSED") {
                checkBoxVector.last()->setText(QString::fromStdString(grid[i][j]->contents));
            }*/
//            int a = checkBoxVector.last()->size()
            checkBoxVector.last()->setGeometry((i * size)+125, ((j * size)+10), size, size);
        }
    }
    QLabel selectUnloadContainers("Select Containers to Unload", ui->unloadContainerPage);
    selectUnloadContainers.setGeometry(100, 100, 300, 100);
    labelChkVector.append(new QLabel("Column", ui->unloadContainerPage));
    labelChkVector.last()->setGeometry(checkBoxVector.first()->geometry().x(),
                                       checkBoxVector.first()->geometry().y() - 55, 100, 100);
    labelChkVector.append(new QLabel("Row", ui->unloadContainerPage));
    labelChkVector.last()->setGeometry(checkBoxVector.first()->geometry().x() - 35,
                                       checkBoxVector.first()->geometry().y() - 33, 100, 100);

    for (auto it: checkBoxVector) {
        it->setFocusPolicy(Qt::NoFocus);
    }
}

void Dialog::on_backButton_clicked() {
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);
}

void Dialog::on_balanceButton_clicked() {
    ui->stackedWidget->setCurrentIndex(currentPage::loadingPage);
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
    } else if (currStep == totalSteps-1) {
        ui->moveContainerLabel->setText("Finished all moves");
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);
        ui->nextStepButton->setText("Finish");
        currStep = 0;
        totalSteps = 0;
        isFinished = true;
    } else {
        currStep++;
        ui->moveContainerLabel->setText(QString::fromStdString(h.getMoves()[currStep]));
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);
        ui->currStepPageLabel->setText("Step " + QString::number(currStep + 1) + "/" + QString::number(totalSteps));
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
        note.setText("Please input in correct format.");
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
        ui->stackedWidget->setCurrentIndex(currentPage::viewStepPage);
        ui->moveContainerLabel->setText(QString::fromStdString(h.getMoves()[0]));
        ui->moveContainerLabel->setAlignment(Qt::AlignCenter);

        totalSteps = h.getMoves().size();
        qDebug() << "Total steps: " << totalSteps;
        ui->currStepPageLabel->setText("Step " + QString::number(currStep + 1) + "/" + QString::number(totalSteps));

        vector<string> moves = h.getMoves();
        for (auto it: moves) {
            qDebug() << QString::fromStdString(it);
        }
    } else {
        qDebug() << "Balancing failed.";
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

