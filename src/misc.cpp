#include "misc.h"
#include <QString>
#include <vector>
#include <QFile>
#include <QTextStream>

#include <QIODevice>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>



using namespace std;

QString protect( const QString& string )
{
    QString s = string;
    s.replace( "&", "&amp;" );
    s.replace( ">", "&gt;" );
    s.replace( "<", "&lt;" );
    s.replace( "\"", "&quot;" );
    s.replace( "\'", "&apos;" );
    return s;
}

#ifndef NO_EXCEL
// fill a table in an xlsx sheet
bool fill_sheet (CWorkbook &book, QTableWidget *table, QString sheetname)
{

    // Get stored indexes
    int style_index_1 = book.m_styleList.main_style_index ;
    int style_index_2 = book.m_styleList.alter_style_inde ;

    CWorksheet &sheet = book.AddSheet(_T(sheetname.toStdString().c_str()));
    vector <vector<CellDataStr> >data;   // (data:style_index)

    vector<CellDataStr> headerdata; headerdata.clear();

    for( int c = 0; c < table->columnCount(); ++c )
    {
        CellDataStr celldata;
        celldata = table->horizontalHeaderItem(c)->data(Qt::DisplayRole).toString().toStdString().data();
        headerdata.push_back(celldata);
    }
    data.push_back(headerdata);


    for (int i=0; i< table->rowCount();i++)
    {
        vector<CellDataStr> linedata; linedata.clear();
        for (int j=0; j< table->columnCount();j++)
        {
            CellDataStr celldata;
            QString cell_string = table->item(i,j)->text();
            if (cell_string =="" ) cell_string = " ";
            cell_string = protect(cell_string);
            celldata = cell_string.toStdString().data();

            if (i % 2 == 0) celldata.style_id = style_index_1;
            else celldata.style_id = style_index_2;

            linedata.push_back(celldata);
        }
        data.push_back(linedata);
    }

    for (int i = 0; i < data.size(); i++)
    {
        sheet.AddRow(data[i]);
    }



}
#endif




bool send_by_email (QString body ,QString subject,QString fileName)
{
    QString UserName = run_cmd("whoami");

    // send by email
    QString command = "echo \" ";
    command += body;
    command += " \"  | mutt -s \"";
    command += subject;
    command += "\" -a ";
    command += fileName;
    command += " " +  UserName + "@synopsys.com";
    printf ("%s \n",command.toStdString().c_str());
    system (command.toStdString().c_str());
}


QString run_cmd(QString command) {
    // get execution directories
    FILE *fp = popen(command.toStdString().c_str(),"r");
    char buf[1024];
    QString result ;

    // get log dir cmp
    while (fgets(buf, 1024, fp)) {
      // do something with buf
        result += buf;
    }
    fclose(fp);
    return result.trimmed();

}

#ifndef NO_EXCEL
void init_workbook(CWorkbook &book)
{
    Style style;
    int style_index_1 = book.m_styleList.Add(style);


    int style_index_2 = book.m_styleList.Add(style);
    style.fill.patternType = PATTERN_SOLID;
    style.fill.fgColor   = "FFFFFFFF";
    style.font.attributes = FONT_NORMAL;
    style.font.attributes = FONT_ITALIC;
    style.fill.fgColor = "FFC9CDFF";
    //// WBY : disabled for better applying style in excel
    // int style_index_2 = book.m_styleList.Add(style);

    book.m_styleList.main_style_index = style_index_1;
    book.m_styleList.alter_style_inde = style_index_2;
}

#endif


#ifndef NO_EXCEL
// fill a table in an xlsx sheet
bool fill_visible_sheet (CWorkbook &book, QTableWidget *table, QString sheetname)
{

    // Get stored indexes
    int style_index_1 = book.m_styleList.main_style_index ;
    int style_index_2 = book.m_styleList.alter_style_inde ;

    CWorksheet &sheet = book.AddSheet(_T(sheetname.toStdString().c_str()));
    vector <vector<CellDataStr> >data;   // (data:style_index)

    vector<CellDataStr> headerdata; headerdata.clear();

    for( int c = 0; c < table->columnCount(); ++c )
    {
        CellDataStr celldata;
        celldata = table->horizontalHeaderItem(c)->data(Qt::DisplayRole).toString().toStdString().data();
        headerdata.push_back(celldata);
    }
    data.push_back(headerdata);


    for (int i=0; i< table->rowCount();i++)
    {
        if (table->isRowHidden(i) == false ) {
            vector<CellDataStr> linedata; linedata.clear();
            for (int j=0; j< table->columnCount();j++)
            {
                CellDataStr celldata;
                QString cell_string = table->item(i,j)->text();
                if (cell_string =="" ) cell_string = " ";
                cell_string = protect(cell_string);
                celldata = cell_string.toStdString().data();

                if (i % 2 == 0) celldata.style_id = style_index_1;
                else celldata.style_id = style_index_2;

                linedata.push_back(celldata);
            }
            data.push_back(linedata);
        }
    }

    for (int i = 0; i < data.size(); i++)
    {
        sheet.AddRow(data[i]);
    }



}
#endif

bool QStringToFile(QString FileName, QString myString)
{
    QFile file (FileName);
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream outstream(&file);
        outstream << myString;
        file.close();
    }
}


bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map) {
    QXmlStreamReader xmlReader(&device);
    QStringList elements;

    // Solange Ende nicht erreicht und kein Fehler aufgetreten ist
    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        // Nächsten Token lesen
        xmlReader.readNext();

        // Wenn Token ein Startelement
        if (xmlReader.isStartElement() && xmlReader.name() != "Settings") {
            // Element zur Liste hinzufügen
            elements.append(xmlReader.name().toString());
        // Wenn Token ein Endelement
        } else if (xmlReader.isEndElement()) {
            // Letztes Element löschen
            if(!elements.isEmpty()) elements.removeLast();
        // Wenn Token einen Wert enthält
        } else if (xmlReader.isCharacters() && !xmlReader.isWhitespace()) {
            QString key;

            // Elemente zu String hinzufügen
            for(int i = 0; i < elements.size(); i++) {
                if(i != 0) key += "/";
                key += elements.at(i);
            }

            // Wert in Map eintragen
            map[key] = xmlReader.text().toString();
        }
    }

    // Bei Fehler Warnung ausgeben
    if (xmlReader.hasError()) {
        qWarning() << xmlReader.errorString();
        return false;
    }

    return true;
}

bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map) {
    QXmlStreamWriter xmlWriter(&device);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Settings");

    QStringList prev_elements;
    QSettings::SettingsMap::ConstIterator map_i;

    // Alle Elemente der Map durchlaufen
    for (map_i = map.begin(); map_i != map.end(); map_i++) {

        QStringList elements = map_i.key().split("/");

        int x = 0;
        // Zu schließende Elemente ermitteln
        while(x < prev_elements.size() && elements.at(x) == prev_elements.at(x)) {
            x++;
        }

        // Elemente schließen
        for(int i = prev_elements.size() - 1; i >= x; i--) {
            xmlWriter.writeEndElement();
        }

        // Elemente öffnen
        for (int i = x; i < elements.size(); i++) {
            xmlWriter.writeStartElement(elements.at(i));
        }

        // Wert eintragen
        xmlWriter.writeCharacters(map_i.value().toString());

        prev_elements = elements;
    }

    // Noch offene Elemente schließen
    for(int i = 0; i < prev_elements.size(); i++) {
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return true;
}



