#include "numero.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QUuid>


void writeFile(const QString dest){

    QFile archivo(dest);
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
        quint64 n = (i*1000*1000*1000*1000)+(i*1000*1000*1000)+i;
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

     //writeFile("numeros-texto.txt");


    // Pruebas

    QDateTime dt1 = QDateTime::currentDateTime();;

    int convs = 100000;

    for (int n = 0; n <= convs ; n++){
        Numero::enLetras(n);
    }

    QDateTime dt2 = QDateTime::currentDateTime();
    quint64 ms = dt1.msecsTo(dt2);

    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Paraguay));
    qDebug() << QString("%L1 (%2) conversiones en %L3 segundos (%4 milisegundos)")
                .arg(convs)
                .arg(Numero::enLetras(convs))
                .arg(ms / 1000.0)
                .arg(Numero::enLetras(ms) ) ;


    quint64 convspersec = convs/ms*1000;
    double msperconv = ms / (double) convs;
    qDebug() << QString("%L1 (%2) conversiones por segundo (%4 milisegundos/conversion)")
                .arg(convspersec)
                .arg(Numero::enLetras(convspersec))
                .arg(msperconv);


    // Numero como QString
    qDebug() << QString("%1").arg(Numero::enLetras("2334234"));


    // Probar con algunos numeros aleatorios
    quint64 num=0;
    for (int i=0 ; i < 100;  i++){
        QUuid uuid = QUuid::createUuid();
        num = qHash(uuid.toByteArray(), QDateTime::currentMSecsSinceEpoch() );
        qDebug() << QString("%L1: %2").arg(num).arg(Numero::enLetras(num));
    }

    return 0;
}
