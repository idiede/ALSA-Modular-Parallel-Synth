#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <signal.h>
#include <QApplication>
#include <QCloseEvent>
#include <QMainWindow>
#include <QMenu>
#include <QSocketNotifier>
#include <QStringList>

#include "modularsynth.h"
#include "msoptions.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

  static int pipeFd[2];
  static void sighandler(int);

private:
    int rcFd;
    QString fileName;
    QStringList recentFiles;
    ModularSynth *modularSynth;
    QMenu* fileRecentlyOpenedFiles;
    bool saveFile();
    void newFile();
    void openFile(const QString&);
    void chooseFile();
    bool isSave();
    int querySaveChanges();
    bool isModified();
    void addRecentlyOpenedFile(const QString&, QStringList&);
    void appendRecentlyOpenedFile(const QString&, QStringList&);

private slots:
    void unixSignal(int fd);
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void updateWindowTitle();
    void helpAboutQt();
    void recentFileActivated(QAction*);
    void setupRecentFilesMenu();


public:
  MainWindow(const ModularSynthOptions&);
  virtual ~MainWindow();

protected:
  void closeEvent(QCloseEvent *e);
  void readConfig();
  void writeConfig();

};

#endif
