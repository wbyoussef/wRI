#ifndef MISC_H
#define MISC_H
#include <QString>
#include <QTableWidget>
#include <QSettings>

#include "xlsx/Xlsx/Workbook.h"
#include "xlsx/Xlsx/Worksheet.h"
using namespace SimpleXlsx;


// convert nom xml characters to xml characters
QString protect( const QString& string );

#ifndef NO_EXCEL
void init_workbook(CWorkbook &book);

// fill a table in an xlsx sheet
bool fill_sheet (CWorkbook &book, QTableWidget *table, QString sheetname);

// fill visible part of a table in an xlsx sheet
bool fill_visible_sheet (CWorkbook &book, QTableWidget *table, QString sheetname);
#endif
// Send a file by email  :
bool send_by_email (QString body ,QString subject,QString fileName);

//run a fast unix command
QString run_cmd(QString command) ;

bool QStringToFile (QString FileName, QString myString);


bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map) ;
bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map) ;

#endif // MISC_H
