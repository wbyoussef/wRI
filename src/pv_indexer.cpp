#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>

#include "pv_indexer.h"
#include "ui_pv_indexer.h"

PV_Indexer::PV_Indexer(QWidget *parent, MainWindow *AppWindow) :
    QWidget(parent),
    ui(new Ui::PV_Indexer)
{
    ui->setupUi(this);
    this->AppWindow = AppWindow;


    //! Defaul value for search folder
    this->ui->lineEdit_src_folder->setText(QDir::currentPath());
    this->ui->lineEdit_work_folder->setText(QDir::currentPath());
    this->ui->lineEdit_work_folder_query->setText(QDir::currentPath());

}

bool PV_Indexer::check_filters(QString filename)
{
    QFileInfo fi (filename);
    foreach (const QString &str, this->work_filters) {
        QRegExp regex (str);
        regex.setPatternSyntax( QRegExp::Wildcard);
        if (filename.contains(regex))
            // result += str;
            return true;
    }
    return false;

}

PV_Indexer::~PV_Indexer()
{
    delete ui;
}

void PV_Indexer::on_toolButton_select_directory_clicked()
{

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                this->ui->lineEdit_src_folder->text(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);


    this->ui->lineEdit_src_folder->setText(dir);
}


void PV_Indexer::DeleteFiles(const char* dir) {
    IndexReader* reader = IndexReader::open(dir);

    int32_t count = 0;
    for (int32_t i = 0; i < reader->maxDoc(); i++){
        reader->deleteDocument (i);
        count ++;
    }
    this->AppWindow->Log("Deleted %d files"+ count, MainWindow::LOG_INFO);
    reader->close();
    _CLDELETE(reader);

    //OPTIMIZE
    if ( IndexReader::indexExists(dir) ){
        lucene::analysis::SimpleAnalyzer an;
        if ( IndexReader::isLocked(dir) ){
            this->AppWindow->Log("Index was locked... unlocking it.", MainWindow::LOG_INFO);
            IndexReader::unlock(dir);
        }

        IndexWriter* writer = _CLNEW IndexWriter( dir, &an, false);
        writer->optimize();
        _CLDELETE(writer);
    }
}

Document* PV_Indexer::FileDocument(const char* f){
    // make a new, empty document
    Document* doc = _CLNEW Document();

    // Add the path of the file as a field named "path".  Use a Tex t field, so
    // that the index stores the path, and so that the path is searchable
   TCHAR tf[CL_MAX_DIR];
   STRCPY_AtoT(tf,f,CL_MAX_DIR);
   doc->add( *_CLNEW Field(_T("path"), tf, Field::STORE_YES | Field::INDEX_UNTOKENIZED ) );

    // Add the last modified date of the file a field named "modified".  Use a
    // Keyword field, so that it's searchable, but so that no attempt is made
    // to tokenize the field into words.
    //doc->add( *Field.Keyword("modified", DateField.timeToString(f->lastModified())));

    // Add the contents of the file a field named "contents".  Use a Text
    // field, specifying a Reader, so that the text of the file is tokenized.

    //read the data without any encoding. if you want to use special encoding
    //see the contrib/jstreams - they contain various types of stream readers
    FILE* fh = fopen(f,"r");
    if ( fh != NULL ){
        StringBuffer str;
        // use fstat for portability
        int fn = fileno(fh);
        struct stat filestat;
        fstat(fn, &filestat);
        str.reserve(filestat.st_size);
        //str.reserve(fileSize(fh->_file));
        char abuf[1024];
        TCHAR tbuf[1024];
        size_t r;
        do{
            r = fread(abuf,1,1023,fh);
            abuf[r]=0;
            STRCPY_AtoT(tbuf,abuf,r);
            tbuf[r]=0;
            str.append(tbuf);
        }while(r>0);
        fclose(fh);

        doc->add( *_CLNEW Field(_T("contents"),str.getBuffer(), Field::STORE_YES | Field::INDEX_TOKENIZED) );
    }

    //_tprintf(_T("%s\n"),doc->toString());
    // return the document
    return doc;
}

void PV_Indexer::indexDocs(IndexWriter* writer,const char* directory) {
    DIR* dir = opendir(directory);
    if ( dir != NULL ){
        struct dirent* fl;

        struct fileStat buf;

        char path[CL_MAX_DIR];
        strcpy(path,directory);
        strcat(path,PATH_DELIMITERA);
        char* pathP = path + strlen(path);

        fl = readdir(dir);
        while ( fl != NULL ){
            if ( (strcmp(fl->d_name, ".")) && (strcmp(fl->d_name, "..")) ) {
            pathP[0]=0;
            strcat(pathP,fl->d_name);
            int32_t ret = fileStat(path,&buf);
            if ( buf.st_mode & S_IFDIR ) {
                indexDocs(writer, path );
            }else{
                // check file extension
                if (this->check_filters(fl->d_name)){
                    this->AppWindow->Log( QString ("adding: ") + QString (fl->d_name), MainWindow::LOG_INFO );
                    Document* doc = FileDocument( path );
                    writer->addDocument( doc );
                    _CLDELETE(doc);
                }
                else
                    this->AppWindow->Log( QString ("Excluding: ") + QString (fl->d_name), MainWindow::LOG_WARNING );

            }
        }
        fl = readdir(dir);

        }
        closedir(dir);
    }else{
            this->AppWindow->Log( QString ("adding: %s\n") + QString (directory),  MainWindow::LOG_INFO);

            Document* doc = FileDocument( directory );
            writer->addDocument( doc );
            _CLDELETE(doc);
    }
}

void PV_Indexer::IndexFiles(const char* path,const  char* target, const bool clearIndex){
    IndexWriter* writer = NULL;

    lucene::analysis::standard::StandardAnalyzer analayze;

    if ( !clearIndex && IndexReader::indexExists(target) ){
        if ( IndexReader::isLocked(target) ){
            this->AppWindow->Log(QString ("Index was locked... unlocking it.\n"), MainWindow::LOG_INFO);
            IndexReader::unlock(target);
        }

        writer = _CLNEW IndexWriter( target, &analayze, false);
    }else{
        writer = _CLNEW IndexWriter( target ,&analayze, true);
    }
    writer->setMaxFieldLength(IndexWriter::DEFAULT_MAX_FIELD_LENGTH);

    indexDocs(writer, path);
    writer->optimize();
    writer->close();
    _CLDELETE(writer);

}

void PV_Indexer::SearchFiles(const char* index){
    //Searcher searcher(index);
    standard::StandardAnalyzer analyzer;
    char line[80];
    TCHAR tline[80];
    const TCHAR* buf;

    IndexSearcher s(index);
    while (true) {
        this->AppWindow->Log(QString ("Enter query string: "), MainWindow::LOG_INFO);
        fgets(line,80,stdin);
        line[strlen(line)-1]=0;

        if ( strlen(line) == 0 )
            break;
       STRCPY_AtoT(tline,line,80);
        Query* q = QueryParser::parse(tline,_T("contents"),&analyzer);

        buf = q->toString(_T("contents"));
        _tprintf(_T("Searching for: %s\n\n"), buf);
        _CLDELETE_CARRAY(buf);

        uint64_t str = lucene::util::Misc::currentTimeMillis();
        Hits* h = s.search(q);
        uint64_t srch = lucene::util::Misc::currentTimeMillis() - str;
        str = lucene::util::Misc::currentTimeMillis();

        for ( int32_t i=0;i<h->length();i++ ){
            Document* doc = &h->doc(i);
            //const TCHAR* buf = doc.get(_T("contents"));
            _tprintf(_T("%d. %s - %f\n"), i, doc->get(_T("path")), h->score(i));
            //delete doc;
        }


        _CLDELETE(h);
        _CLDELETE(q);

    }
    s.close();
    //delete line;
}

void PV_Indexer::getStats(const char* directory){

    IndexReader* r = IndexReader::open(directory);
    _tprintf(_T("Statistics for %s\n"), directory);
    this->AppWindow->Log("==================================\n", MainWindow::LOG_INFO);

    this->AppWindow->Log(QString ("Max Docs: ")+ QString::number ( r->maxDoc()) , MainWindow::LOG_INFO);
    this->AppWindow->Log(QString ("Num Docs: ")+ QString::number  (r->numDocs()) , MainWindow::LOG_INFO);

    int64_t ver = r->getCurrentVersion(directory);
    TCHAR str[16];
    _i64tot(ver, str, 10);
    _tprintf(_T("Current Version: %s\n"), str );

    TermEnum* te = r->terms();
    int32_t nterms;
    for (nterms = 0; te->next() == true; nterms++) {
            /* empty */
    }
    this->AppWindow->Log(QString ("Term count: ")+ QString::number  ( nterms), MainWindow::LOG_INFO );
    _CLDELETE(te);

    r->close();
    _CLDELETE(r);
}

void PV_Indexer::on_toolButton_work_directory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                this->ui->lineEdit_work_folder->text(),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);


    this->ui->lineEdit_work_folder->setText(dir);

}

void PV_Indexer::on_pushButton_gen_db_clicked()
{
    this->AppWindow->Log("Building database ...", MainWindow::LOG_WARNING);
    this->work_filters.clear();
    // get list of filters
    for(int row = 0; row < this->ui->listWidget_filters->count(); row++)
    {
             QListWidgetItem *item = this->ui->listWidget_filters->item(row);
             this->work_filters.append(item->text());
             this->AppWindow->Log(item->text(), MainWindow::LOG_INFO);
    }

    this->IndexFiles(this->ui->lineEdit_src_folder->text().toLatin1(),this->ui->lineEdit_work_folder->text().toLatin1(),true);
    this->getStats(this->ui->lineEdit_work_folder->text().toLatin1());
}

void PV_Indexer::on_lineEdit_search_string_returnPressed()
{
    if (!ui->lineEdit_search_string->text().isEmpty()){

    //Searcher searcher(index);
    standard::StandardAnalyzer analyzer;

    this->ui->listWidget_results->clear();
    TCHAR tline[80];

    IndexSearcher searcher(this->ui->lineEdit_work_folder_query->text().toLatin1());
        this->AppWindow->Log(QString ("Query result : "), MainWindow::LOG_INFO);
    const char *line = this->ui->lineEdit_search_string->text().toLatin1().data();

        STRCPY_AtoT(tline,line,80);
        Query* query = QueryParser::parse(tline,_T("contents"),&analyzer);

        Hits* hits = searcher.search(query);

        for ( int32_t i=0;i<hits->length();i++ ){
            Document* doc = &hits->doc(i);
            QString x = QString::fromWCharArray( doc->get(_T("path"))) ;
            QListWidgetItem* item = new QListWidgetItem();
            item->setText(x);
            item->setFlags (item->flags () | Qt::ItemIsEditable);
            this->AppWindow->Log(x,MainWindow::LOG_INFO);
            this->ui->listWidget_results->addItem(item);

        }
        _CLDELETE(hits);
        _CLDELETE(query);

    searcher.close();
    //delete line;
    }
}

void PV_Indexer::on_toolButton_filter_setup_add_clicked()
{
    QListWidgetItem *item = new QListWidgetItem("New Filter");
    item->setFlags (item->flags () | Qt::ItemIsEditable);
    this->ui->listWidget_filters->addItem(item);
}

void PV_Indexer::on_toolButton_filter_setup_remove_clicked()
{
    if (this->ui->listWidget_filters->selectedItems().length() > 0)
    {
        this->ui->listWidget_filters->takeItem(this->ui->listWidget_filters->row(this->ui->listWidget_filters->selectedItems().first()));
    }
}

void PV_Indexer::on_toolButton_filter_setup_edit_clicked()
{
    this->ui->listWidget_filters->editItem(this->ui->listWidget_filters->item(0));
}

void PV_Indexer::on_lineEdit_search_string_textChanged(const QString &arg1)
{
    if (this->ui->checkBox_on_fly->isChecked())
        on_lineEdit_search_string_returnPressed();
}

void PV_Indexer::on_listWidget_results_doubleClicked(const QModelIndex &index)
{
    QString fname = this->ui->listWidget_results->selectedItems().first()->text();
    QFile file(fname);

    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream ReadFile(&file);
    this->ui->plainTextEdit_viewer->document()->setPlainText (ReadFile.readAll());
    this->ui->tabWidget_main->setCurrentIndex(2);
}
