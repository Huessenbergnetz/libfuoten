#ifndef NEWSAPPVERSION_H
#define NEWSAPPVERSION_H

#include <QSharedDataPointer>
#include <QString>

namespace Fuoten {

class NewsAppVersionData : public QSharedData
{
public:
    NewsAppVersionData() :
        maj(0),
        min(0),
        mic(0)
    {}

    NewsAppVersionData(const NewsAppVersionData &other) :
        QSharedData(other), maj(other.maj), min(other.min), mic(other.mic)
    {}

    ~NewsAppVersionData() {}

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
class NewsAppVersion
{
public:
    /*!
     * \brief Constructs a null version.
     */
    NewsAppVersion();

    NewsAppVersion(int maj, int min, int mic);

    NewsAppVersion(const QString &versionString);

    NewsAppVersion(const NewsAppVersion &other);

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
    bool lowerThan(const NewsAppVersion &other) const;

    /*!
     * \brief Returns true if this version is equal to the arguments.
     */
    bool equalTo(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is equal to \a other.
     */
    bool equalTo(const NewsAppVersion &other) const;

    /*!
     * \brief Returns true if this version is greater than the arguments.
     */
    bool greaterThan(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is greater than \a other.
     */
    bool greaterThan(const NewsAppVersion &other) const;

    /*!
     * \brief Returns true if this version is lower or equal to the arguments.
     */
    bool lowerThanOrEqualTo(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is lower or equal to \a other.
     */
    bool lowerThanOrEqualTo(const NewsAppVersion &other) const;

    /*!
     * \brief Returns true if this version is greater or equal to the arguments.
     */
    bool greaterThanOrEqualTo(int maj, int min = 0, int mic = 0) const;

    /*!
     * \brief Returns true if this version is greater or equal to \a other.
     */
    bool greaterThanOrEqualTo(const NewsAppVersion &other) const;

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
    QSharedDataPointer<NewsAppVersionData> d;
};

}

#endif // NEWSAPPVERSION_H
