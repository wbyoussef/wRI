QT       += core gui xml
QT           += sql

CONFIG      +=  qscintilla2




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui




HEADERS = mainwindow.h \
    xlsx/xmlwriter.h \
    xlsx/tchar.h \
    xlsx/Xlsx/XlsxHeaders.h \
    xlsx/Xlsx/Worksheet.h \
    xlsx/Xlsx/Workbook.h \
    xlsx/Xlsx/SimpleXlsxDef.h \
    xlsx/Xlsx/Chartsheet.h \
    xlsx/Zip/zip.h \
    misc.h \
    domitem.h \
    dommodel.h \
    qxmlputget.h \
    wri_desk_settings.h \
    wri_project.h \
    wri_project_doc.h \
    wri_project_doc_info.h \
    wri_project_doc_rtlgroup.h \
    wri_project_doc_includes.h


SOURCES = mainwindow.cpp \
    main.cpp \
    xlsx/Xlsx/XlsxHeaders.cpp \
    xlsx/Xlsx/Worksheet.cpp \
    xlsx/Xlsx/Workbook.cpp \
    xlsx/Xlsx/Chartsheet.cpp \
    xlsx/Zip/zip.cpp \
    misc.cpp \
    domitem.cpp \
    dommodel.cpp \
    qxmlputget.cpp \
    wri_desk_settings.cpp \
    wri_project.cpp \
    wri_project_doc.cpp \
    wri_project_doc_info.cpp \
    wri_project_doc_rtlgroup.cpp


RESOURCES = mdi.qrc
FORMS += \
    mainwindow.ui \
    wri_desk_settings.ui \
    wri_project.ui


CONFIG += static
ICON = ressources/qa_desk.png

######################################################
# OS dependeant stuff
######################################################

### UNIX

# unix :HEADERS +=     pv_indexer.h \
#    pv_indexer_misc.h \
# unix:FORMS+= pv_indexer.ui
# unix:SOURCES+=     pv_indexer.cpp \
#     pv_indexer_misc.cpp \
# unix:LIBS += -lclucene

unix:DEFINES += NO_EXCEL
unix:DEFINES += NO_LUCENE
unix:SOURCES -=     xlsx/Xlsx/XlsxHeaders.cpp \
    xlsx/Xlsx/Worksheet.cpp \
    xlsx/Xlsx/Workbook.cpp \
    xlsx/Xlsx/Chartsheet.cpp \
    xlsx/Zip/zip.cpp \
    pv_indexer.cpp \
    pv_indexer_misc.cpp \

unix:FORMS -=     pv_indexer.ui


### Windows
win32:DEFINES += NO_EXCEL
win32:DEFINES += NO_LUCENE
win32:SOURCES -=     xlsx/Xlsx/XlsxHeaders.cpp \
    xlsx/Xlsx/Worksheet.cpp \
    xlsx/Xlsx/Workbook.cpp \
    xlsx/Xlsx/Chartsheet.cpp \
    xlsx/Zip/zip.cpp \
    pv_indexer.cpp \
    pv_indexer_misc.cpp \

win32:FORMS -=     pv_indexer.ui
