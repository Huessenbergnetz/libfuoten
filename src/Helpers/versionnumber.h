/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016 Buschtrommel / Matthias Fehring
 * https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/
 * https://github.com/Buschtrommel/libfuoten
 *
 * helpers/versionnumber.h
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef VERSIONNUMBER_H
#define VERSIONNUMBER_H

#include <QSharedDataPointer>
#include <QString>
#include "../fuoten_global.h"

namespace Fuoten {

class VersionNumberData : public QSharedData
{
public:
    VersionNumberData() :
        maj(0),
        min(0),
        mic(0)
    {}

    VersionNumberData(const VersionNumberData &other) :
        QSharedData(other), maj(other.maj), min(other.min), mic(other.mic)
    {}

    ~VersionNumberData() {}

    int maj;
    int min;
    int mic;
};

/*!
 * \brief Contains a version number.
 *
 * This is a helper class for Qt versions prior to Qt 5.6. Qt 5.6 provides QVersionNumber that
 * makes this class obsolete.
 */
class FUOTENSHARED_EXPORT VersionNumber
{
public:
    /*!
     * \brief Constructs a null version.
     */
    VersionNumber();

    /*!
     * \brief Constructs a new version from the given arguments.
     */
    VersionNumber(int maj, int min, int mic);

    /*!
     * \brief Constructs a new version from a string.
     *
     * String has to be in format "0.2.4".
     */
    VersionNumber(const QString &versionString);

    /*!
     * \brief Constructs a copy of \a other.
     */
    VersionNumber(const VersionNumber &other);

    /*!
     * \brief Sets the major version number.
     */
    void setMaj(int maj);

    /*!
     * \brief Sets the minor version number.
     */
    void setMin(int min);

    /*!
     * \brief Sets the micro version number.
     */
    void setMic(int mic);

    /*!
     * \brief Returns the major version number.
     */
    int maj() const;

    /*!
     * \brief Returns the minor version number.
     */
    int min() const;

    /*!
     * \brief Returns the micro version number.
     */
    int mic() const;

    /*!
     * \brief Returns true if this version is lower than the arguments.
     */
    bool lowerThan(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is lower than \a other.
     */
    bool lowerThan(const VersionNumber &other) const;

    /*!
     * \brief Returns true if this version is equal to the arguments.
     */
    bool equalTo(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is equal to \a other.
     */
    bool equalTo(const VersionNumber &other) const;

    /*!
     * \brief Returns true if this version is greater than the arguments.
     */
    bool greaterThan(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is greater than \a other.
     */
    bool greaterThan(const VersionNumber &other) const;

    /*!
     * \brief Returns true if this version is lower or equal to the arguments.
     */
    bool lowerThanOrEqualTo(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is lower or equal to \a other.
     */
    bool lowerThanOrEqualTo(const VersionNumber &other) const;

    /*!
     * \brief Returns true if this version is greater or equal to the arguments.
     */
    bool greaterThanOrEqualTo(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is greater or equal to \a other.
     */
    bool greaterThanOrEqualTo(const VersionNumber &other) const;

    /*!
     * \brief Returns true all parts of the version are 0.
     */
    bool isNull() const;

    /*!
     * \brief Loads the version data from a string.
     */
    void loadFromString(const QString &versionString);

    /*!
     * \brief Returns a string with all of the segments delimited by a '.'.
     */
    QString toString() const;

private:
    QSharedDataPointer<VersionNumberData> d;
};

}

#endif // VERSIONNUMBER_H
