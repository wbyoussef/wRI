#ifndef PV_INDEXER_H
#define PV_INDEXER_H

#include <QWidget>
#include <stdio.h>
#include <QVector>
#include "mainwindow.h"


#include "CLucene/StdHeader.h"
#include "CLucene.h"
#include "CLucene/util/Reader.h"
#include "CLucene/util/Misc.h"
#include "CLucene/util/dirent.h"
#include "CLucene/index/Term.h"
#include "CLucene/index/SegmentTermEnum.h"
using namespace std;
using namespace lucene::index;
using namespace lucene::util;
using namespace lucene::store;
using namespace lucene::document;
using namespace lucene::analysis;
using namespace lucene::queryParser;
using namespace lucene::search;

void DeleteFiles(const char* dir) ;
Document* FileDocument(const char* f);
void indexDocs(IndexWriter* writer,const char* directory) ;
void IndexFiles(const char* path,const char* target, const bool clearIndex);
void SearchFiles(const char* index) ;
void getStats(const char* directory);

namespace Ui {
class PV_Indexer;
}

class PV_Indexer : public QWidget
{
    Q_OBJECT

public:
    explicit PV_Indexer(QWidget *parent, MainWindow *AppWindow);
    bool check_filters (QString filename);
    ~PV_Indexer();

private slots:
    void on_toolButton_select_directory_clicked();

    void on_toolButton_work_directory_clicked();

    void on_pushButton_gen_db_clicked();

    void on_lineEdit_search_string_returnPressed();

    void on_toolButton_filter_setup_add_clicked();

    void on_toolButton_filter_setup_remove_clicked();

    void on_toolButton_filter_setup_edit_clicked();

    void on_lineEdit_search_string_textChanged(const QString &arg1);

    void on_listWidget_results_doubleClicked(const QModelIndex &index);

private:
    Ui::PV_Indexer *ui;
    MainWindow *AppWindow;
    QStringList work_filters;

public :
//! Lucene APIs
    void DeleteFiles(const char* dir) ;
    Document* FileDocument(const char* f);
    void indexDocs(IndexWriter* writer,const char* directory) ;
    void IndexFiles(const char* path,const char* target, const bool clearIndex);
    void SearchFiles(const char* index) ;
    void getStats(const char* directory);

};

#endif // PV_INDEXER_H
