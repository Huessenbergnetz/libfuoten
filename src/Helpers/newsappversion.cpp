#include "newsappversion.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include <QtDebug>

using namespace Fuoten;

NewsAppVersion::NewsAppVersion()
{
    d = new NewsAppVersionData;
}



NewsAppVersion::NewsAppVersion(int maj, int min, int mic)
{
    d = new NewsAppVersionData;
    setMaj(maj);
    setMin(min);
    setMic(mic);
}



NewsAppVersion::NewsAppVersion(const QString &versionString)
{
    d = new NewsAppVersionData;
    loadFromString(versionString);
}



NewsAppVersion::NewsAppVersion(const NewsAppVersion &other) : d(other.d)
{

}



void NewsAppVersion::loadFromString(const QString &versionString)
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





void NewsAppVersion::setMaj(int maj) { d->maj = maj; }

void NewsAppVersion::setMin(int min) { d->min = min; }

void NewsAppVersion::setMic(int mic) { d->mic = mic; }


int NewsAppVersion::maj() const { return d->maj; }

int NewsAppVersion::min() const { return d->min; }

int NewsAppVersion::mic() const { return d->mic; }




bool NewsAppVersion::lowerThan(int maj, int min, int mic) const
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


bool NewsAppVersion::equalTo(int maj, int min, int mic) const
{
    return (d->maj == maj && d->min == min && d->mic == mic);
}



bool NewsAppVersion::greaterThan(int maj, int min, int mic) const
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



bool NewsAppVersion::lowerThanOrEqualTo(int maj, int min, int mic) const
{
    return (lowerThan(maj, min, mic) || equalTo(maj, min, mic));
}


bool NewsAppVersion::greaterThanOrEqualTo(int maj, int min, int mic) const
{
    return (greaterThan(maj, min, mic) || equalTo(maj, min, mic));
}



bool NewsAppVersion::isNull() const
{
    return (d->maj == 0 && d->min == 0 && d->mic == 0);
}


QString NewsAppVersion::toString() const
{
    return QStringLiteral("%1.%2.%3").arg(QString::number(maj()), QString::number(min()), QString::number(mic()));
}
