/*------------------------------------------------------------------------------
* Copyright (C) 2003-2006 Ben van Klinken and the CLucene Team
*
* Distributable under the terms of either the Apache License (Version 2.0) or
* the GNU Lesser General Public License, as specified in the COPYING file.
------------------------------------------------------------------------------*/
#include "pv_indexer_misc.h"


#include <iostream>


void DeleteFiles(const char* dir) {
    IndexReader* reader = IndexReader::open(dir);

    int32_t count = 0;
    for (int32_t i = 0; i < reader->maxDoc(); i++){
        reader->deleteDocument (i);
        count ++;
    }
    printf("Deleted %d files\n", count);
    reader->close();
    _CLDELETE(reader);

    //OPTIMIZE
    if ( IndexReader::indexExists(dir) ){
        lucene::analysis::SimpleAnalyzer an;
        if ( IndexReader::isLocked(dir) ){
            printf("Index was locked... unlocking it.\n");
            IndexReader::unlock(dir);
        }

        IndexWriter* writer = _CLNEW IndexWriter( dir, &an, false);
        writer->optimize();
        _CLDELETE(writer);
    }
}

Document* FileDocument(const char* f){
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

void indexDocs(IndexWriter* writer,const char* directory) {
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
                printf( "adding: %s\n", fl->d_name );

                Document* doc = FileDocument( path );
                writer->addDocument( doc );
                _CLDELETE(doc);
            }
        }
        fl = readdir(dir);

        }
        closedir(dir);
    }else{
            printf( "adding: %s\n", directory);

            Document* doc = FileDocument( directory );
            writer->addDocument( doc );
            _CLDELETE(doc);
    }
}

void IndexFiles(const char* path,const  char* target, const bool clearIndex){
    IndexWriter* writer = NULL;

    lucene::analysis::standard::StandardAnalyzer analayze;

    if ( !clearIndex && IndexReader::indexExists(target) ){
        if ( IndexReader::isLocked(target) ){
            printf("Index was locked... unlocking it.\n");
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
void SearchFiles(const char* index){
    //Searcher searcher(index);
    standard::StandardAnalyzer analyzer;
    char line[80];
    TCHAR tline[80];
    const TCHAR* buf;

    IndexSearcher s(index);
    while (true) {
        printf("Enter query string: ");
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

        printf("\n\nSearch took: %d ms.\n", srch);
        printf("Screen dump took: %d ms.\n\n", lucene::util::Misc::currentTimeMillis() - str);

        _CLDELETE(h);
        _CLDELETE(q);

    }
    s.close();
    //delete line;
}

void getStats(const char* directory){

    IndexReader* r = IndexReader::open(directory);
    _tprintf(_T("Statistics for %s\n"), directory);
    printf("==================================\n");

    printf("Max Docs: %d\n", r->maxDoc() );
    printf("Num Docs: %d\n", r->numDocs() );

    int64_t ver = r->getCurrentVersion(directory);
    TCHAR str[16];
    _i64tot(ver, str, 10);
    _tprintf(_T("Current Version: %s\n"), str );

    TermEnum* te = r->terms();
    int32_t nterms;
    for (nterms = 0; te->next() == true; nterms++) {
            /* empty */
    }
    printf("Term count: %d\n\n", nterms );
    _CLDELETE(te);

    r->close();
    _CLDELETE(r);
}

/*
int main( int32_t argc, char** argv ){
    //Dumper Debug
    #ifdef TR_LEAKS
    #ifdef _CLCOMPILER_MSVC
    #ifdef _DEBUG
        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );//| _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_CHECK_CRT_DF );
    #endif
    #endif
    #endif

    uint64_t str = lucene::util::Misc::currentTimeMillis();
    try{

        printf("Location of text files to be indexed: ");
        char files[250];
        fgets(files,250,stdin);
        files[strlen(files)-1] = 0;

        printf("Location to store the clucene index: ");
        char ndx[250];
        fgets(ndx,250,stdin);
        ndx[strlen(ndx)-1] = 0;

        IndexFiles(files,ndx,true);
        getStats(ndx);
        SearchFiles(ndx);
        DeleteFiles(ndx);

    }catch(CLuceneError& err){
        printf(err.what());
    }catch(...){
        printf("Unknown error");
    }

    _lucene_shutdown(); //clears all static memory
    //print lucenebase debug
#ifdef LUCENE_ENABLE_MEMLEAKTRACKING
    lucene::debug::LuceneBase::__cl_PrintUnclosedObjects();
   //clear memtracking memory (not the unclosed objects)
   lucene::debug::LuceneBase::__cl_ClearMemory();
#endif

    //Debuggin techniques:
    //For msvc, use this for breaking on memory leaks:
    //	_crtBreakAlloc
    //to break at this clucene item:
    //	_lucene_counter_break
    //run a memory check before deleting objects:
    //	_lucene_run_objectcheck
    //if LUCENE_ENABLE_CONSTRUCTOR_LOG is on, dont do log if this is true:
    //	_lucene_disable_debuglogging

    printf ("\n\nTime taken: %d\n\n",lucene::util::Misc::currentTimeMillis() - str);
    return 0;
}
*/
