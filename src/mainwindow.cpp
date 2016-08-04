/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QMdiArea>
#include <QFileDialog>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QToolBar>
#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wri_desk_settings.h"
#include "wri_project.h"


MainWindow::MainWindow():
        QMainWindow(),
        ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize app_settings
    app_settings = new wRI_Desk_settings(this);


    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget *)),
            this, SLOT(setActiveSubWindow(QWidget *)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    readSettings();

    setWindowTitle(tr("QA Desk"));
    setUnifiedTitleAndToolBarOnMac(true);

    //! configure Docks
    this->ui->dockWidget->setVisible(true);
    this->ui->dockWidget_tree->setVisible(true);

    this->ui->gridLayout->setSpacing(0);
    this->ui->gridLayout_2->setSpacing(0);
    this->ui->gridLayout_3->setSpacing(0);
    this->ui->gridLayout_4->setSpacing(0);
    this->ui->gridLayout_5->setSpacing(0);

    this->ui->gridLayout->setContentsMargins(0,0,0,0);
    this->ui->gridLayout_2->setContentsMargins(0,0,0,0);
    this->ui->gridLayout_3->setContentsMargins(0,0,0,0);
    this->ui->gridLayout_4->setContentsMargins(0,0,0,0);
    this->ui->gridLayout_5->setContentsMargins(0,0,0,0);
    this->ui->gridLayout_6->setContentsMargins(0,0,0,0);

    //! Setup terminal :
    // Fast  terminal
    fast_terminal = new QProcess(this);
    fast_terminal->setWorkingDirectory ("~");
    fast_terminal->setProcessChannelMode(QProcess::MergedChannels);
    fast_terminal->start("bash ");

    connect( fast_terminal, SIGNAL(readyReadStandardOutput()),
      this, SLOT(fast_readFromStdout()) );
    this->run_fast_cmd("source ~/.bashrc");

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    /// Save window state
    this->writeSettings();

    // Brutal Exit
    exit(0);

}

void MainWindow::newFile()
{
    wRI_Project* child = new wRI_Project();
    mdiArea->addSubWindow(child);
    child->showMaximized();
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr(" (*.qat)"));
    if (!fileName.isEmpty()) {
        open_file(fileName);
    }
}

void MainWindow::open_file(QString filename)
{
        QSettings settings("EVE", "QA_DESK");
        QStringList files = settings.value("recentFileList").toStringList();
        files.removeAll(filename);
        files.prepend(filename);
        while (files.size() > MaxRecentFiles)
            files.removeLast();

        settings.setValue("recentFileList", files);

}
void MainWindow::save()
{
}

void MainWindow::saveAs()
{
}

void MainWindow::cut()
{
}

void MainWindow::copy()
{
}

void MainWindow::paste()
{
}
void MainWindow::toggle_deskbar()
{
    this->ui->dockWidget->setVisible (ToggleDeskBarAct->isChecked());
}


void MainWindow::toggle_explorerbar()
{
    this->ui->dockWidget_tree->setVisible (ToggleExplorerAct->isChecked());
}

void MainWindow::zoomin()
{
}

void MainWindow::zoomout()
{
}

void MainWindow::clear()
{
}

void MainWindow::about()
{
   QString Message = QString ("") ;
   Message += "For any issue, please contact : walid.benyoussef@gmail.com . <br>";
   Message += QString ("Build : ") + __DATE__ ;
   QMessageBox::about(this, tr("About wRI DESK V1.0"),
            Message
            );
}

void MainWindow::updateMenus()
{
    bool has_qa_campaign_child = true;
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    pasteAct->setEnabled(true);
    ToggleExplorerAct->setEnabled(true);
    ToggleDeskBarAct->setEnabled(true);
    zoominAct->setEnabled(true);
    zoomoutAct->setEnabled(true);
    clearAct->setEnabled(true);
    closeAct->setEnabled(true|has_qa_campaign_child);
    closeAllAct->setEnabled(true|has_qa_campaign_child);
    tileAct->setEnabled(true|has_qa_campaign_child);
    cascadeAct->setEnabled(true|has_qa_campaign_child);
    nextAct->setEnabled(true|has_qa_campaign_child);
    previousAct->setEnabled(true|has_qa_campaign_child);
    separatorAct->setVisible(true|has_qa_campaign_child);

    bool hasSelection ; /// = (activeMdiChild() &&
                        /// activeMdiChild()->textCursor().hasSelection());
    cutAct->setEnabled(hasSelection);
    SettingsAct->setEnabled(true);

    copyAct->setEnabled(hasSelection);


    compileAct->setEnabled(true);
    runAct->setEnabled(true);
    cleanAct->setEnabled(true);
    uploadAct->setEnabled(true);
    downloadAct->setEnabled(true);
    applyAct->setEnabled(true);
    AnalyzeAct->setEnabled(true);



}

void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);
    windowMenu->addSeparator();
    windowMenu->addAction(zoominAct);
    windowMenu->addAction(zoomoutAct);
    windowMenu->addAction(clearAct);
    windowMenu->addSeparator();
    windowMenu->addAction(ToggleDeskBarAct);
    windowMenu->addAction (ToggleExplorerAct);
}


void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New Project"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open Project..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(QIcon(":/images/save.png"),tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

//! [0]
    exitAct = new QAction(QIcon(":/images/application_exit.png"),tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
//! [0]

    cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    SettingsAct = new QAction(QIcon(":/images/settings1.png"), tr("&Settings"), this);
    SettingsAct->setStatusTip(tr("QA Desk settings"));
    connect(SettingsAct, SIGNAL(triggered()), this, SLOT(wri_desk_settings()));



    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    zoominAct = new QAction(QIcon(":/images/viewmag+.png"), tr("Zoom &In"), this);
    zoominAct->setShortcuts(QKeySequence::Paste);
    zoominAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(zoominAct, SIGNAL(triggered()), this, SLOT(zoomin()));


    ToggleDeskBarAct = new QAction(QIcon(":/images/deskbar.png"), tr("Toggle Deskbar"), this);
    ToggleDeskBarAct->setShortcuts(QKeySequence::Paste);
    ToggleDeskBarAct->setStatusTip(tr("Toggle Deskbar"));
    ToggleDeskBarAct->setCheckable(true);
    ToggleDeskBarAct->setChecked(true);
    connect(ToggleDeskBarAct, SIGNAL(triggered()), this, SLOT(toggle_deskbar()));

    ToggleExplorerAct = new QAction(QIcon(":/images/cascade.png"), tr("Toggle Project Explorer"), this);
    ToggleExplorerAct->setStatusTip(tr("Toggle Project Explorer"));
    ToggleExplorerAct->setCheckable(true);
    ToggleExplorerAct->setChecked(true);
    connect(ToggleExplorerAct, SIGNAL(triggered()), this, SLOT(toggle_explorerbar()));



    zoomoutAct = new QAction(QIcon(":/images/viewmag-.png"), tr("Zoom &Out"), this);
    zoomoutAct->setShortcuts(QKeySequence::Paste);
    zoomoutAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(zoomoutAct, SIGNAL(triggered()), this, SLOT(zoomout()));

    clearAct = new QAction(QIcon(":/images/view_close.png"), tr("&Clear"), this);
    clearAct->setShortcuts(QKeySequence::Paste);
    clearAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setShortcut(tr("Ctrl+F4"));
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()),
            mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    aboutAct = new QAction(QIcon(":/images/help_about.png"),tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));


    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    AnalyzeAct = new QAction(QIcon(":/images/view_tree.png"), tr("&Analyze"), this);
    AnalyzeAct->setStatusTip(tr("Analyze"
                              "selection"));
    connect(AnalyzeAct, SIGNAL(triggered()), this, SLOT(analyze()));



    compileAct = new QAction(QIcon(":/images/run_build_file.png"), tr("&compile"), this);
    compileAct->setStatusTip(tr("compile"
                                  "selection"));
    connect(compileAct, SIGNAL(triggered()), this, SLOT(compile()));

    runAct = new QAction(QIcon(":/images/system_run.png"), tr("&run"), this);
    runAct->setStatusTip(tr("run"
                              "selection"));
    connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

    cleanAct = new QAction(QIcon(":/images/edit_clear_list.png"), tr("&clean"), this);
    cleanAct->setStatusTip(tr("clean"
                              "selection"));
    connect(cleanAct, SIGNAL(triggered()), this, SLOT(clean()));

    uploadAct = new QAction(QIcon(":/images/up.png"), tr("&upload"), this);
    uploadAct->setStatusTip(tr("upload"
                              "selection"));
    connect(uploadAct, SIGNAL(triggered()), this, SLOT(upload()));

    downloadAct = new QAction(QIcon(":/images/media_playlist_refresh.png"), tr("&download"), this);
    downloadAct->setStatusTip(tr("download"
                              "selection"));
    connect(downloadAct, SIGNAL(triggered()), this, SLOT(download()));

    applyAct= new QAction(QIcon(":/images/apply.png"), tr("&apply"), this);
    applyAct->setStatusTip(tr("apply"
                              "selection"));
    connect(applyAct, SIGNAL(triggered()), this, SLOT(apply()));


}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    updateRecentFileActions();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);

    fileMenu->addSeparator();

    QAction *action = fileMenu->addAction(tr("Switch layout direction"));
    connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));

    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Tools"));

    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    editMenu->addAction(SettingsAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();


    toolsMenu = menuBar()->addMenu(tr("&Actions"));
    toolsMenu->addAction(compileAct);
    toolsMenu->addAction(runAct);
    toolsMenu->addAction(cleanAct);
    toolsMenu->addAction(uploadAct);
    toolsMenu->addAction(downloadAct);
    toolsMenu->addAction(applyAct);
    toolsMenu->addAction(AnalyzeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    // help_about

}

void MainWindow::createToolBars()
{
    toolsToolbar = addToolBar(tr("Tools"));


    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    toolsToolbar->addAction(SettingsAct);


    ChildToolBar = addToolBar(tr("View"));
    ChildToolBar->addAction(ToggleDeskBarAct);
    ChildToolBar->addAction(ToggleExplorerAct);
    ChildToolBar->addAction(zoominAct);
    ChildToolBar->addAction(zoomoutAct);
    ChildToolBar->addAction(clearAct);

    // this->ui->mainToolBar = addToolBar("Main");

    //this->ui->runToolbar->tr()
    runToolbar = addToolBar("Run");
    runToolbar->addAction(compileAct);
    runToolbar->addAction(runAct);
    runToolbar->addAction(cleanAct);
    runToolbar->addAction(uploadAct);
    runToolbar->addAction(downloadAct);
    runToolbar->addAction(applyAct);
    runToolbar->addAction(AnalyzeAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::readSettings()
{
    QSettings AppSettings ("EVE", "APP");

    if (AppSettings.contains("Window/Geometry"))
        this->restoreGeometry(AppSettings.value("Window/Geometry").toByteArray());
    if (AppSettings.contains("Window/State"))
        this->restoreState(AppSettings.value("Window/State").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings AppSettings ("EVE", "APP");
    AppSettings.beginGroup("Window");
    AppSettings.setValue("Geometry", this->saveGeometry());
    AppSettings.setValue("State", this->saveState());
    AppSettings.endGroup();
}




QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
}

void MainWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}


QToolBar *MainWindow::GetChildToolBar ()
{
    // return this->ui->child_toolBar;
    return NULL;
}

void MainWindow::Log(QString log, MessageLevel level)
{
    /*!      Display in Log Dock !*/

    QListWidgetItem* item = new QListWidgetItem();
    item->setBackgroundColor(Qt::white);
    switch(level){
    case MainWindow::LOG_INFO  :
        item->setBackgroundColor(Qt::white);
        item->setForeground(Qt::green);
        this->ui->listWidget_Log->addItem(item);
       break;
    case MainWindow::LOG_WARNING  :
        item->setBackgroundColor(Qt::green);
        item->setForeground(Qt::black);
        this->ui->listWidget_Log->addItem(item);
       break;
    case MainWindow::LOG_ERROR  :
        item->setBackgroundColor(Qt::red);
        item->setForeground(Qt::black);
        this->ui->listWidget_Log->addItem(item);
       break;
    case MainWindow::LOG_DEBUG  :
//!        item->setBackgroundColor(Qt::gray);
//!        item->setForeground(Qt::black);
       break;

        default :
        item->setBackgroundColor(Qt::white);
        //this->ui->listWidget_Log->addItem(item);
    }
    item->setText(log);
    this->ui->listWidget_Log->scrollToBottom();

}


void MainWindow::wri_desk_settings()
{
    app_settings->show ();
}






void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action){
        open_file (action->data().toString());
        printf ("loadFile(action->data().toString());");
    }


}

void MainWindow::updateRecentFileActions()
{
    QSettings settings("EVE", "QA_DESK");
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(QFileInfo((files[i])).fileName());
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

void MainWindow::fast_readFromStdout()
{
    //
    QString x= fast_terminal->readAll();
    QStringList str = x.split("\n");
    foreach (const QString &iter, str)
    {
        this->ui->listWidget_shell_log->addItem(iter);
        this->ui->listWidget_shell_log->scrollToBottom();

    }

}

void MainWindow::run_fast_cmd(QString cmd)
{

    QListWidgetItem *item = new QListWidgetItem();
    QFont font = item->font();
    font.setBold(true);
    item->setFont(font);
    item->setText(cmd);
    this->ui->listWidget_shell_log->addItem(item);
    fast_terminal->write(cmd.toStdString().c_str());
    fast_terminal->write("\n");


}


void MainWindow::run_full_cmd(QString cmd)
{

    QListWidgetItem *item = new QListWidgetItem();
    QFont font = item->font();
    font.setBold(true);
    item->setFont(font);
    item->setText(cmd);
    this->ui->listWidget_shell_log->addItem(item);
    fast_terminal->write(QString (cmd + ";echo [DONE]") .toStdString().c_str());
    fast_terminal->write("\n");


}


void MainWindow::on_textBrowser_copyAvailable(bool b)
{
    // exit (0);
}

void MainWindow::on_lineEdit_editingFinished()
{
    exit (0);
}


void MainWindow::compile()
{
}


void MainWindow::run()
{
}


void MainWindow::clean()
{
}


void MainWindow::upload()
{
}


void MainWindow::download()
{
}


void MainWindow::apply()
{
}



void MainWindow::analyze()
{
}


void MainWindow::on_lineEdit_shell_cmd_returnPressed()
{
    this->run_fast_cmd(this->ui->lineEdit_shell_cmd->text());

    this->ui->lineEdit_shell_cmd->clear();
}
