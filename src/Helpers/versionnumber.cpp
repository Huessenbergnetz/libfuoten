#include "versionnumber.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include <QtDebug>

using namespace Fuoten;

VersionNumber::VersionNumber()
{
    d = new VersionNumberData;
}



VersionNumber::VersionNumber(int maj, int min, int mic)
{
    d = new VersionNumberData;
    setMaj(maj);
    setMin(min);
    setMic(mic);
}



VersionNumber::VersionNumber(const QString &versionString)
{
    d = new VersionNumberData;
    loadFromString(versionString);
}



VersionNumber::VersionNumber(const VersionNumber &other) : d(other.d)
{

}



void VersionNumber::loadFromString(const QString &versionString)
{
    if (versionString.isEmpty()) {
        setMaj(0);
        setMin(0);
        setMic(0);
    }

    QRegularExpression re(QStringLiteral("\\d+"));
    QRegularExpressionMatchIterator i = re.globalMatch(versionString);

    if (i.hasNext()) {
        setMaj(i.next().captured(0).toInt());

        if (i.hasNext()) {
            setMin(i.next().captured(0).toInt());

            if (i.hasNext()) {
                setMic(i.next().captured(0).toInt());
            } else {
                setMic(0);
            }
        } else {
            setMin(0);
            setMic(0);
        }
    } else {
        setMaj(0);
        setMin(0);
        setMic(0);
    }
}





void VersionNumber::setMaj(int maj) { d->maj = maj; }

void VersionNumber::setMin(int min) { d->min = min; }

void VersionNumber::setMic(int mic) { d->mic = mic; }


int VersionNumber::maj() const { return d->maj; }

int VersionNumber::min() const { return d->min; }

int VersionNumber::mic() const { return d->mic; }




bool VersionNumber::lowerThan(int maj, int min, int mic) const
{
    if (d->maj < maj) {
        return true;
    } else if (d->maj > maj) {
        return false;
    } else {
        if (d->min < min) {
            return true;
        } else if (d->min > min) {
            return false;
        } else {
            if (d->mic < mic) {
                return true;
            } else {
                return false;
            }
        }
    }
}

bool VersionNumber::lowerThan(const VersionNumber &other) const
{
    return lowerThan(other.maj(), other.min(), other.mic());
}


bool VersionNumber::equalTo(int maj, int min, int mic) const
{
    return (d->maj == maj && d->min == min && d->mic == mic);
}


bool VersionNumber::equalTo(const VersionNumber &other) const
{
    return equalTo(other.maj(), other.min(), other.mic());
}


bool VersionNumber::greaterThan(int maj, int min, int mic) const
{
    if (d->maj > maj) {
        return true;
    } else if (d->maj < maj) {
        return false;
    } else {
        if (d->min > min) {
            return true;
        } else if (d->min < min) {
            return false;
        } else {
            if (d->mic > mic) {
                return true;
            } else {
                return false;
            }
        }
    }
}


bool VersionNumber::greaterThan(const VersionNumber &other) const
{
    return greaterThan(other.maj(), other.min(), other.mic());
}



bool VersionNumber::lowerThanOrEqualTo(int maj, int min, int mic) const
{
    return (lowerThan(maj, min, mic) || equalTo(maj, min, mic));
}

bool VersionNumber::lowerThanOrEqualTo(const VersionNumber &other) const
{
    return lowerThanOrEqualTo(other.maj(), other.min(), other.mic());
}


bool VersionNumber::greaterThanOrEqualTo(int maj, int min, int mic) const
{
    return (greaterThan(maj, min, mic) || equalTo(maj, min, mic));
}


bool VersionNumber::greaterThanOrEqualTo(const VersionNumber &other) const
{
    return greaterThanOrEqualTo(other.maj(), other.min(), other.mic());
}



bool VersionNumber::isNull() const
{
    return (d->maj == 0 && d->min == 0 && d->mic == 0);
}


QString VersionNumber::toString() const
{
    return QStringLiteral("%1.%2.%3").arg(QString::number(maj()), QString::number(min()), QString::number(mic()));
}
