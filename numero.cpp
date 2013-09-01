#include "numero.h"
#include <QString>
#include <QStringList>
#include <QTextCodec>

// á = \xC3\xA1
// é = \xC3\xA9
// í = \xC3\xAD
// ó = \xC3\xB3
// ú = \xC3\xBA

const QByteArray Numero::UNIDADES[] = {
    "cero", "uno", "dos", "tres",
    "cuatro", "cinco", "seis",
    "siete", "ocho", "nueve"};

const QByteArray Numero::DIECIS[] = {
    "diez", "once", "doce", "trece",
    "catorce", "quince", "diecis\xC3\xA9is",
    "diecisiete", "dieciocho", "diecinueve"};

const QByteArray Numero::VEINTIS[] = {
    "veinte", "veintiuno", "veintid\xC3\xB3s", "veintitr\xC3\xA9s",
    "veinticuatro", "veinticinco", "veintis\xC3\xA9is",
    "veintisiete", "veintiocho", "veintinueve"};

const QByteArray Numero::DECENAS[] = {
    "ERROR", "diez", "veinte", "treinta", "cuarenta",
    "cincuenta", "sesenta", "setenta", "ochenta", "noventa"
};

const QByteArray Numero::CENTENAS[] = {
    "ERROR", "ciento", "doscientos", "trescientos",
    "cuatrocientos", "quinientos", "seiscientos",
    "setecientos", "ochocientos", "novecientos"
};

const QByteArray Numero::PERIODOS[] = {
    "ERROR", "mi", "bi", "tri", "quatri", "quinti", "sexti",
    "septi", "octi", "noni", "deci", "undeci", "duodeci",
    "tredeci", "cuatrodeci", "quindeci", "hexadeci",
    "septendeci", "octodeci", "novendeci", "viginti",
    "unviginti", "duoviginti", "treviginti", "cuatroviginti",
    "quinviginti", "sexviginti", "septenviginti", "octoviginti",
    "novemviginti", "triginti", "untriginti", "duotriginti", "tretriginti"
};

const int Numero::NUM_PERIODOS = 32; // Ajustar esto si se modifica el arreglo PERIODOS
const short Numero::DIGITOS_POR_PERIODO = 6;
const QByteArray Numero::INVALIDO = "N\xC3\xBAmero inv\xC3\xA1lido";
const QByteArray Numero::MUY_GRANDE = "N\xC3\xBAmero demasiado grande (>= 1e+192)";
const QByteArray Numero::LLON = "ll\xC3\xB3n";
const QByteArray Numero::LLONES = "llones";
const QByteArray Numero::UN = "un";
const QByteArray Numero::VEINTIUN = "veinti\xC3\xBAn";
const QByteArray Numero::CIEN = "cien";
const QByteArray Numero::MIL = "mil";
const QByteArray Numero::Y = " y ";


QString Numero::enLetras(const QString &num, bool rellenar){

    QString digitos = num;

    if(0==digitos.length()){
        return QTextCodec::codecForName("UTF-8")->toUnicode(INVALIDO);
    }

    /// Numero entero
    if(QRegExp("[0-9]+").exactMatch(digitos))  {

        if(digitos.length()<2){
            bool ok;
            char entero = digitos.toInt(&ok, 10); /// base 10 explicito

            if(ok&& (entero==0)){
                QString cero = QTextCodec::codecForName("UTF-8")->toUnicode(UNIDADES[0]);
                cero[0]=cero[0].toUpper();
                return cero;
            }
        }

        if(digitos.length() > NUM_PERIODOS*DIGITOS_POR_PERIODO){
            return QTextCodec::codecForName("UTF-8")->toUnicode(MUY_GRANDE);
        }

        QChar sep('#');
        QStringList periodos = agrupar(digitos,sep, DIGITOS_POR_PERIODO).split(sep);
        QByteArray finalText;

        foreach(QByteArray parte, convertirPeriodos(periodos) ){
            // Omitir periodos vacios. Ej. 123.000000.123456
            if(parte.length()>0){
                finalText.append(parte);
                finalText.append(" ");
            }
        }

        QString finalStr = QTextCodec::codecForName("UTF-8")->toUnicode(finalText);
        finalStr=finalStr.simplified().toLower();
        finalStr[0]=finalStr[0].toUpper();

        return finalStr;

        /// Numero con parte decimal
    }else if(QRegExp("[0-9]{0,}(\\.[0-9]*){0,1}").exactMatch(digitos)){

        QStringList partes = digitos.split(QChar('.'));
        QString entero = partes.at(0);
        QString fraccion;
        QString padding;
        int padlen;
        bool ok;

        if(partes.size() == 2){
            fraccion = partes.at(1);
        }

        // Cero con..
        if(entero.length()==0){
            entero.append('0');
        }

        /// Remover todos los ceros a la derecha
        if(!rellenar){
            while(!fraccion.isEmpty() && fraccion.endsWith(QChar('0'))){
                fraccion.remove(fraccion.length()-1,1);
            }
        }

        /// Agregar solo uno, si se eliminaron todos
        if(fraccion.length()==0){
            fraccion.append('0');
        }

        /// Calcular esto *despues* de remover los ceros a la derecha
        padlen = fraccion.length();
        padding = QString().fill('0', padlen);

        /// Remover los ceros a la izquierda
        while((fraccion.length()>1) && fraccion.startsWith(QChar('0'))){
            fraccion.remove(0,1);
        }

        if(fraccion.length()<2){
            char valFraccion = fraccion.toInt(&ok, 10); /// Importante: base 10 explicito

            if(ok && (valFraccion==0L)){
                return enLetras(entero); /// Ignorar parte decimal si es cero
            }
        }

        return enLetras(entero).append(" con ").append(fraccion).append("/1").append(padding);

    }else{
        /// Numero invalido
        return QTextCodec::codecForName("UTF-8")->toUnicode(INVALIDO);
    }
}



QString Numero::enLetras(double numero, int precision){

    QString n = QString("%1").arg(numero, 0,'f',precision);

    return enLetras(n);
}

QString Numero::enLetras(quint64 numero){

    // Este caso conviene resolver aqui mismo
    if(numero == 0L){
        QString cero = QTextCodec::codecForName("UTF-8")->toUnicode(UNIDADES[0]);
        cero[0]=cero[0].toUpper();
        return cero;
    }

    QString digitos = QString("%1").arg(numero);

    return enLetras(digitos);
}

/// Agrupa digitos con el caracter cs cada dc
/// No apropiado para uso general
QString Numero::agrupar(const QString &numero, QChar cs, int dc){

    int orden=0;
    QString digitos;

    foreach(QChar c, reverse(numero)){
        digitos.append(c);
        orden++;

        if(((orden%dc)==0)&& (orden!=numero.length())){
            digitos.append(cs);
        }
    }

    return reverse(digitos);
}

QList<QByteArray> Numero::convertirPeriodos(const QStringList &periodos){

    int nper= periodos.length();
    QList<QByteArray> perEnTexto;

    foreach (QString periodo, periodos){
        perEnTexto.append(convertirPeriodo(periodo.toInt(), nper));
        nper--;
    }

    return perEnTexto;
}

/**
 *  Convierte un periodo numerico de 6 digitos a texto.
 *  Agrega el sufijo de periodo apropiadamente (millones, trillones, etc)
 */
QByteArray Numero::convertirPeriodo(quint32 per, int nper){

    // casos excepcionales
    if(nper>1){
        if(0==per){
            return QByteArray();
        }

        if(1==per){
            return QByteArray().append(UN).append(" ").append(PERIODOS[nper-1]).append(LLON);
        }
    }

    int clBaja, clAlta;
    clBaja = per - ((per / 1000) * 1000);
    clAlta = per / 1000;

    QByteArray perConv;

    if(clAlta>0){
        perConv.append(convertirClase(clAlta,false));
        perConv.append(" ");
        perConv.append(MIL);
    }

    if(clBaja>0){
        perConv.append(" ");
        perConv.append(convertirClase(clBaja,1==nper));
    }

    if(nper>1){
        perConv.append(" ");
        perConv.append(PERIODOS[nper-1]);
        perConv.append(LLONES);
    }

    return perConv.trimmed();
}

/**
 * Convierte una clase numerica de tres digitos a texto
 * Si es primera clase 1 es "UNO" y no "UN", 100 es "CIEN" y no "CIENTO", etc
 */
QByteArray Numero::convertirClase(quint16 m_numero, bool primeraClase){
    int cen = m_numero / 100;
    int dec = (m_numero - (m_numero / 100) * 100) / 10;
    int uni = m_numero - (m_numero / 10) * 10;

    QByteArray sb;

    // Caso excepcional
    if (100 == m_numero) {
        return CIEN;
    }

    // Empezar con las centenas
    if (cen > 0) {
        sb.append(CENTENAS[cen]);
    }

    if ((((10 * dec) + uni) < 10) && (((10 * dec) + uni) > 0)) {
        sb.append(" ");
        if (uni == 1) {
            if (primeraClase) {
                sb.append(UNIDADES[uni]);
            } else {
                sb.append(UN);
            }
        } else {
            sb.append(UNIDADES[uni]);
        }

    }

    if ((((10 * dec) + uni) > 9) && (((10 * dec) + uni) < 20)) {
        sb.append(" ");
        sb.append(DIECIS[uni]);
    }

    if ((((10 * dec) + uni) > 19) && (((10 * dec) + uni) < 30)) {
        sb.append(" ");
        if (uni == 1) {
            if (primeraClase) {
                sb.append(VEINTIS[uni]); // veintiuno
            } else {
                sb.append(VEINTIUN);
            }
        } else {
            sb.append(VEINTIS[uni]);
        }
    }

    if (dec > 2) {
        sb.append(" ");
        sb.append(DECENAS[dec]);
    }

    if ((dec > 2) && (uni > 0)) {
        sb.append(Y);
        if (uni == 1) {
            if (primeraClase) {
                sb.append(UNIDADES[uni]);
            } else {
                sb.append(UN);
            }
        } else {
            sb.append(UNIDADES[uni]);
        }
    }

    return sb.trimmed();
}

/**
 * Invierte un string
 */
//QString Numero::reverse(const QString &str){
//    if(str.length()==0)
//        return QString ();

//    QByteArray ba = str.toAscii();
//    char *d = ba.data();
//    std::reverse(d, d+str.length());
//    return QString(d);
//}

QString Numero::reverse(const QString &str){
    if(str.length()==0)
        return QString ();

    QString reversed;

    for (int i=0; i<str.length(); i++){
        reversed.prepend(str[i]);
    }

    return reversed;
}


