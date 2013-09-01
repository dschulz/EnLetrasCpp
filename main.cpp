#include "numero.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QUuid>


void writeFile(){
    QFile archivo("numeros-cpp.txt");
    archivo.open(QFile::WriteOnly);
    QTextStream out(&archivo);
    out.setCodec("UTF-8");

    for (quint64 i=0; i<1200; i++){
        quint64 n = i;
        out << QString("%1: %2\n").arg(n).arg(Numero::enLetras(n));
    }

    for (quint64 i=0; i<1200; i++){
        quint64 n = (i*1000)+i;
        out << QString("%1: %2\n").arg(n).arg(Numero::enLetras(n));
    }

    for (quint64 i=0; i<1200; i++){
        quint64 n = (i*1000*1000)+i;
        out << QString("%1: %2\n").arg(n).arg(Numero::enLetras(n));
    }

    for (quint64 i=0; i<1200; i++){
        quint64 n = (i*1000*1000*1000)+i;
        out << QString("%1: %2\n").arg(n).arg(Numero::enLetras(n));
    }

    for (quint64 i=0; i<1200; i++){
        quint64 n = (i*1000*1000*1000*1000)+i;
        out << QString("%1: %2\n").arg(n).arg(Numero::enLetras(n));
    }

    for (quint64 i=0; i<1200; i++){
        quint64 n = (i*1000*1000*1000*1000*1000)+i;
        out << QString("%1: %2\n").arg(n).arg( Numero::enLetras(n));
    }

    out.flush();
    archivo.close();
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    // writeFile();


    // Pruebas

    QDateTime dt1 = QDateTime::currentDateTime();;

    int convs = 100000;

    for (int n = 0; n <= convs ; n++){
       // qDebug() <<
        QString("%1: %2\n").arg(n).arg( Numero::enLetras(n));
    }

    QDateTime dt2 = QDateTime::currentDateTime();

    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Paraguay));
    qDebug() << QString("%L1 (%2) conversiones en %L3 segundos (%4 milisegundos)")
                .arg(convs)
                .arg(Numero::enLetras(convs))
                .arg(dt1.msecsTo(dt2) / 1000.0)
                .arg(Numero::enLetras(dt1.msecsTo(dt2)) ) ;

    // Numero como QString
    qDebug() << QString("%1").arg(Numero::enLetras("2334234"));


    // Probar con algunos numeros aleatorios
    quint64 num=0;
    for (int i=0 ; i < 30;  i++){
        QUuid uuid = QUuid::createUuid();
        num = qHash(uuid.toByteArray(), QDateTime::currentMSecsSinceEpoch() );
        qDebug() << QString("%L1: %2").arg(num).arg(Numero::enLetras(num));
    }

    return 0;
}
