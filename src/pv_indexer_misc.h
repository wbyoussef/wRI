#ifndef PV_INDEXER_MISC
#define PV_INDEXER_MISC

#include <stdio.h>
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

#endif // PV_INDEXER_MISC

