#ifndef NUMERO_H
#define NUMERO_H
#include <QtGlobal>
#include <QList>
class QString;
class QStringList;
class QChar;


class Numero {

public:
    static QString enLetras(quint64 numero);
    static QString enLetras(double numero, int precision);
    static QString enLetras(const QString &numero, bool rellenar=true);


private:
    Numero(); // no se puede instanciar

    static QList<QByteArray> convertirPeriodos(const QStringList &);

    static QString reverse(const QString &);
    static QString agrupar(const QString &numero, QChar cs, int dc);
    static QByteArray convertirPeriodo(quint32,int);
    static QByteArray convertirClase(quint16, bool);

    static const int NUM_PERIODOS;
    static const short DIGITOS_POR_PERIODO;
    static const QByteArray MUY_GRANDE;
    static const QByteArray LLON;
    static const QByteArray LLONES;
    static const QByteArray UN;
    static const QByteArray Y;
    static const QByteArray VEINTIUN;
    static const QByteArray CIEN;
    static const QByteArray MIL;
    static const QByteArray INVALIDO;

    static const QByteArray UNIDADES[];
    static const QByteArray DIECIS[];
    static const QByteArray VEINTIS[];
    static const QByteArray DECENAS[];
    static const QByteArray CENTENAS[];
    static const QByteArray PERIODOS[];

};



#endif // NUMERO_H
