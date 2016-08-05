#include "misc.h"
#include <QString>
#include <vector>
#include <QFile>
#include <QTextStream>

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
