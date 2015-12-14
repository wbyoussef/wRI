QT       += core gui xml
QT           += sql

CONFIG      +=  qscintilla2




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets gui
win32:DEFINES += NO_EXCEL
win32:DEFINES += NO_LUCENE

# QMAKE_LFLAGS += -static
#QMAKE_CXXFLAGS += -stati
# QMAKE_LFLAGS += -static-libstdc++ $(QTDIR)/lib/libclucene.a
unix:LIBS += -lclucene



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

unix :HEADERS +=     pv_indexer.h \
   pv_indexer_misc.h \

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

unix:SOURCES+=     pv_indexer.cpp \
    pv_indexer_misc.cpp \

RESOURCES = mdi.qrc
FORMS += \
    mainwindow.ui \
    wri_desk_settings.ui \
    wri_project.ui

unix:FORMS+= pv_indexer.ui

CONFIG += static
ICON = ressources/qa_desk.png

win32:SOURCES -=     xlsx/Xlsx/XlsxHeaders.cpp \
    xlsx/Xlsx/Worksheet.cpp \
    xlsx/Xlsx/Workbook.cpp \
    xlsx/Xlsx/Chartsheet.cpp \
    xlsx/Zip/zip.cpp \
    pv_indexer.cpp \
    pv_indexer_misc.cpp \

win32:FORMS -=     pv_indexer.ui

#
