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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QProcess>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QSettings;
class wRI_Desk_settings;

QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    enum MessageLevel {LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_DEBUG};

    QToolBar * GetChildToolBar ();
    QSettings *mSettings ;
    QMdiArea *mdiArea;

    /*! show in log !*/
    void Log (QString, MessageLevel) ;
    /*! run a shell command !*/
    void run_fast_cmd(QString cmd);
    void run_full_cmd(QString cmd);


protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    void open_file(QString filename);
    void save();
    void saveAs();
    void cut();
    void copy();
    void paste();
    void toggle_deskbar();
    void zoomin();
    void zoomout();
    void clear();
    void wri_desk_settings();
    void PVIndexer();

    void about();
    void updateMenus();
    void updateWindowMenu();
    void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);
    void openRecentFile();

// TC actions
    void compile ();
    void run ();
    void clean ();
    void upload();
    void download();
    void apply();
    void analyze();

    void on_textBrowser_copyAvailable(bool b);

    void on_lineEdit_editingFinished();

    void on_lineEdit_shell_cmd_returnPressed();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    QMdiSubWindow *findMdiChild(const QString &fileName);
    void updateRecentFileActions();


    QSignalMapper *windowMapper;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *toolsMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *ChildToolBar;
    QToolBar *runToolbar;
    QToolBar *toolsToolbar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *SettingsAct;
    QAction *CampainAnalysisAct;
    QAction *PVIndexAct;
    QAction *pasteAct;
    QAction *ToggleDeskBarAct;
    QAction *zoominAct;
    QAction *zoomoutAct;
    QAction *clearAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QSettings *settings;
    wRI_Desk_settings* app_settings;

    QAction *compileAct;
    QAction *runAct;
    QAction *cleanAct;
    QAction *uploadAct;
    QAction *downloadAct;
    QAction *applyAct;
    QAction *AnalyzeAct;
    QProcess *fast_terminal;


    // size of listof recent files :
    enum { MaxRecentFiles = 20 };
     QAction *recentFileActs[MaxRecentFiles];

private:
    Ui::MainWindow *ui;

public slots :
    void fast_readFromStdout();


};

#endif
