/* libfuoten - Qt based library to access the ownCloud/Nextcloud News App API
 * Copyright (C) 2016-2019 Hüssenbergnetz/Matthias Fehring
 * https://github.com/Huessenbergnetz/libfuoten
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

#ifndef FUOTENCOMPONENT_H
#define FUOTENCOMPONENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QSslError>
#include <QUrlQuery>
#include <QJsonDocument>
#include "../Helpers/abstractnotificator.h"
#include "../fuoten_global.h"

class QNetworkReply;

namespace Fuoten {

class ComponentPrivate;
class Error;
class AbstractConfiguration;
class AbstractStorage;
class AbstractNamFactory;
class AbstractNotificator;

/*!
 * \brief Base class for all API requests.
 *
 * This is the base class for all other classes that implement API operations. Most
 * important property to set when using this class or derived classes is the
 * \link Component::configuration configuration \endlink property. This can either be
 * done per instance direclty via the property or on a global scope via setDefaultConfiguration().
 * The \link Component::storage storage \endlink property can be set on a global scope
 * via setDefaultStorage(). Pointers to configuration and storage that are set directly to
 * an instance of a Component subclass via the property setter functions setStorage() and
 * setConfiguration() will take precedence over the default ones.
 *
 * To modify the QNetworkAccessManager that will be used to perform the network requests,
 * create a subclass of AbstractNamFactory and set it via Component::setNetworkAccessManagerFactory().
 * This will than be used to create new QNetworkAccessManager instances on the fly that will
 * be children of the Component object.
 *
 * When creating a subclass of Component, you have to reimplement successCallback(),
 * extractError() and checkOutput(). Optionally you should reimplement checkInput()
 * if your derived class provides own input properties that should be checked before
 * starting the network request.
 *
 * When reimplementing checkOutput() and checkInput() you have to call the
 * implementations of the class from which you derive to also include their checks and
 * perform some basic operations. In the function that starts the request, set
 * setInOperation() to \c true and call sendRequest(). In your error handling and
 * successCallback() functions you should afterwards set inOpeartion back to \c false.
 *
 * In your reimplementation of successCallback() you should work on the content
 * requested from the News App API that can be obtained by jsonResult(). The content
 * returned by jsonResult() will be set by Component::checkOutput().
 *
 * In the constructor of your class you should set the API route to use and the
 * expected result, that will be checked by Component::checkOutput().
 *
 * Component and its subclasses in libfuoten using a
 * <a href="https://techbase.kde.org/Policies/Library_Code_Policy/Shared_D-Pointer_Example">shared D-pointer</a>.
 *
 * \par Subclassing exmaple
 *
 * \code{.cpp}
 *
 * #include <Fuoten/component.h>
 * #include <Fuoten/error.h>
 *
 * class MyClass : public Fuoten::Component
 * {
 *     Q_OBJECT
 * public:
 *     MyClass(QObject *parent = nullptr);
 *
 *     void get();
 *
 * protected:
 *     void successCallback() override;
 *     void extractError(QNetworkReply *reply) override;
 *     bool checkOutput() override;
 *
 * private:
 *     Q_DISABLE_COPY(MyClass)
 * };
 *
 *
 * MyClass::MyClass(QObject *parent) : Fuoten::Component(parent)
 * {
 *     setApiRoute(QStringLiteral("/route"));
 *     setExpectedJSONType(Component::Object);
 *     setNetworkOperation(QNetworkAccessManager::GetOperation);
 * };
 *
 * void MyClass::execute()
 * {
 *     if (inOperation()) {
 *         return;
 *     }
 *
 *     setInOperation(true);
 *
 *     sendRequest();
 * }
 *
 *
 * void MyClass::successCallback()
 * {
 *     const QJsonDocument r = jsonResult();
 *
 *     // operate on the result
 *     // or use a reimplementation of AbstractStorage
 *
 *     setInOperation(false);
 *
 *     emit succeeded(r);
 * }
 *
 *
 * void MyClass::extractError(QNetworkReply *reply)
 * {
 *     setError(new Fuoten::Error(reply, this));
 *     emit failed(error());
 * }
 *
 *
 * bool MyClass::checkOutput()
 * {
 *     const QJsonObject o = jsonResult().object();
 *
 *     if (Fuoten::Component::checkOutput()) {
 *
 *         if (!o.contains(QStringLiteral("my_important_value"))) {
 *             setError(new Fuoten::Error(Fuoten::Error::OutputError, Fuoten::Error::Critical, tr("Can not find that really important value in the server reply."), QString(), this));
 *             emit failed(error());
 *             return false;
 *         } else {
 *             return true;
 *         }
 *
 *     } else {
 *         return false;
 *     }
 * }
 *
 *
 * \endcode
 *
 * \headerfile "" <Fuoten/API/Component>
 */
class FUOTENSHARED_EXPORT Component : public QObject
{
    Q_OBJECT
    /*!
     * \brief Returns true while the request is in operation.
     *
     * \sa setInOperation()
     *
     * \par Access functions:
     * <TABLE><TR><TD>bool</TD><TD>inOperation() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>inOperationChanged(bool inOperation)</TD></TR></TABLE>
     */
    Q_PROPERTY(bool inOperation READ inOperation NOTIFY inOperationChanged)
    /*!
     * \brief Timeout in seconds for network requests.
     *
     * If you set the timeout to 0, it wil be disabled. Default value: \a 120 \a seconds
     *
     * \par Access functions:
     * <TABLE><TR><TD>quint8</TD><TD>requestTimeout() const</TD></TR><TR><TD>void</TD><TD>setRequestTimeout(quint8 nRequestTimeout)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>requestTimeoutChanged(quint8 requestTimeout)</TD></TR></TABLE>
     */
    Q_PROPERTY(quint16 requestTimeout READ requestTimeout WRITE setRequestTimeout NOTIFY requestTimeoutChanged)
    /*!
     * \brief Pointer to an error object, if any error occurred.
     *
     * If no error occurred, it will return a \c nullptr. The error is set internally by setError().
     *
     * \par Access functions:
     * <TABLE><TR><TD>Error*</TD><TD>error() const</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>errorChanged(Error *error)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::Error *error READ error NOTIFY errorChanged)
    /*!
     * \brief Pointer to an AbstractConfiguration derived object.
     *
     * It is mandatory for all calls to set this property to a valid object that provides the authentication information for the server.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * If no configuration object has been set via setConfiguration(), the one set via Component::setDefaultConfiguration() will be the
     * value of this property (if a default one has been set). Configuration objects that are set per instance via setConfiguration()
     * will take precedence over the default ones set via Component::setDefaultConfiguration().
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractConfiguration*</TD><TD>configuration() const</TD></TR><TR><TD>void</TD><TD>setConfiguration(AbstractConfiguration *nAbstractConfiguration)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>configurationChanged(AbstractConfiguration *configuration)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::AbstractConfiguration *configuration READ configuration WRITE setConfiguration NOTIFY configurationChanged)
    /*!
     * \brief Pointer to an AbstractStorage derived object.
     *
     * Set a storage handler to store the results of API requests. You have to derive your own storage from AbstractStorage.
     * Have a look at SQLiteStorage for an example implementation.
     *
     * This property can not be changed while Component::inOperation() returns \c true.
     *
     * If no storage object has been set via setStorage() the one set via Component::setDefaultStorage() will be the value of this property.
     * So if you do not set a storage object through one the methods, this property will hold a \c nullptr. If a storage object ist set
     * to an instance of this class via setStorage(), this storage object will take precedence over the default storage object (if any set).
     *
     * Together with the \link Component::useStorage useStorage \endlink property, this can be used in the successCallback() function
     * to determine if the API request result should be processed through the local storage object or not.
     *
     * \par Access functions:
     * <TABLE><TR><TD>AbstractStorage*</TD><TD>storage() const</TD></TR><TR><TD>void</TD><TD>setStorage(AbstractStorage *nStorageHandler)</TD></TR></TABLE>
     * \par Notifier signal:
     * <TABLE><TR><TD>void</TD><TD>storageChanged(AbstractStorage *storage)</TD></TR></TABLE>
     */
    Q_PROPERTY(Fuoten::AbstractStorage *storage READ storage WRITE setStorage NOTIFY storageChanged)
    /*!
     * \brief If \c true (the default), a local storage should be used in the successCallback() function to further process the request results.
     *
     * Setting this property to \c false will omit possible usage of local storage objects in the successCallback() function. Even if a defaultStorage()
     * has been set through setDefaultStorage(), it will not be used if this propery ist \c false.
     *
     * \par Access functions:
     * \li bool isUseStorageEnabled() const
     * \li void setUseStorage(bool useStorage)
     *
     * \par Notifier signal:
     * void useStorageChanged(bool useStorage)
     */
    Q_PROPERTY(bool useStorage READ isUseStorageEnabled WRITE setUseStorage NOTIFY useStorageChanged)
    /*!
     * \brief Pointer to an object derived from AbstractNotificator.
     *
     * Set a notificator to notify users about errors and events. This is not mandatory. You have to derive your own notificator that uses
     * the notification system of the target platform.
     *
     * If no notificator has been set via setNotificator(), the one set via Component::setDefaultConfigurator() will be used - if any has
     * been set. If you do not set a notificator either per instance or global, this property will hold a \c nullptr. If a notificator is
     * set to an instance of this class via setNotificator(), this notificator will take precedence over the global default notificator
     * object (if any set).
     *
     * \par Access functions:
     * \li AbstractNotificator* notificator() const
     * \li void setNotificator(AbstractNotificator *notificator);
     *
     * \par Notifier signal:
     * \li void notificatorChanged(AbstractNotificator *notificator);
     *
     * \sa notify()
     */
    Q_PROPERTY(Fuoten::AbstractNotificator *notificator READ notificator WRITE setNotificator NOTIFY notificatorChanged)
public:
    /*!
     * \brief Constructs a component with the given \a parent.
     */
    explicit Component(QObject *parent = nullptr);

    /*!
     * \brief Destroys the component.
     */
    ~Component();

    /*!
     * \brief Defines the expected JSON type.
     */
    enum ExpectedJSONType : quint8 {
        Empty   = 0,    /**< Expects an empty body in the reply. */
        Array   = 1,    /**< Expects a JSON array in the reply body. */
        Object  = 2     /**< Expects a JSON object in the reply body. */
    };

    /*!
     * \brief Executes the API request.
     *
     * Reimplement this in a subclass to prepare the request and start it by calling sendRequest().
     *
     * \par Implementation example
     *
     * \code{.cpp}
     * void RenameFolder::execute()
     * {
     *     if (inOperation()) {
     *         qWarning("Operation is still running.");
     *         return;
     *     }
     *
     *     setInOperation(true);
     *
     *     QStringList routeParts;
     *     routeParts << QStringLiteral("folders") << QString::number(folderId());
     *     setApiRoute(routeParts);
     *
     *     QJsonObject jsonPayload;
     *     jsonPayload.insert(QStringLiteral("name"), newFolderName());
     *     setPayload(jsonPayload);
     *
     *     sendRequest();
     * }
     * \endcode
     */
    Q_INVOKABLE virtual void execute() = 0;

    /*!
     * \brief Returns true while the API request is running.
     *
     * This might also include the local storage operation, if Component::storage is set to a valid AbstractStorage sublcass.
     * \sa inOperation
     */
    bool inOperation() const;

    /*!
     * \brief Returns the currently set request timeout.
     * \sa requestTimeout
     */
    quint16 requestTimeout() const;

    /*!
     * \brief Returns a pointer to an Error object, if any error occurred.
     *
     * Will return a \c nullptr if no error occurred.
     *
     * \sa error
     */
    Error *error() const;

    /*!
     * \brief Returns a pointer to the AbstractConfiguration that is currently set.
     *
     * \sa configuration
     */
    AbstractConfiguration *configuration() const;

    /*!
     * \brief Getter function for the \link Component::storage storage \endlink property.
     * \sa setStorage(), setDefaultStorage(), storageChanged()
     */
    AbstractStorage *storage() const;

    /*!
     * \brief Getter function for the \link Component::useStorage useStorage \endlink property.
     * \sa setUseStorage(), useStorageChanged()
     */
    bool isUseStorageEnabled() const;

    /*!
     * \brief Getter function for the \link Component::notificator notificator \endlink property.
     * \sa setNotificator(), notificatorChanged()
     */
    AbstractNotificator *notificator() const;

    /*!
     * \brief Sets the timeout for the API request in seconds.
     *
     * \sa requestTimeout
     */
    void setRequestTimeout(quint16 seconds);

    /*!
     * \brief Sets a pointer to a AbstractConfiguration to use for the API request.
     *
     * \sa configuration
     */
    void setConfiguration(AbstractConfiguration *nAbstractConfiguration);

    /*!
     * \brief Setter function for the \link Component::storage storage \endlink property.
     * \sa storage(), setDefaultStorage(), storageChanged()
     */
    void setStorage(AbstractStorage *localStorage);

    /*!
     * \brief Setter function for the \link Component::useStorage useStorage \endlink property.
     * \sa isUseStorageEnabled(), useStorageChanged()
     */
    void setUseStorage(bool useStorage);

    /*!
     * \brief Setter function for the \link Component::notificator notificator \endlink property.
     * \sa notificator(), notificatorChanged()
     */
    void setNotificator(AbstractNotificator *notificator);

    /*!
     * \brief Sets the global default configuration.
     * \sa defaultConfiguration()
     */
    static void setDefaultConfiguration(AbstractConfiguration *config);

    /*!
     * \brief Returns the global default configuration.
     * \sa setDefaultConfiguration()
     */
    static AbstractConfiguration *defaultConfiguration();

    /*!
     * \brief Sets the global default storage.
     * \sa defaultStorage()
     */
    static void setDefaultStorage(AbstractStorage *storage);

    /*!
     * \brief Returns the global default storage.
     * \sa setDefaultStorage()
     */
    static AbstractStorage *defaultStorage();

    /*!
     * \brief Sets the network access manager \a factory.
     * The factory will be used to create QNetworkAccessManager objects on demand.
     * If no factory is set, a default QNetworkAccessManager object will be created.
     * The Component class will take ownership of the created QNetworkAccessManager.
     * \sa networkAccessManagerFactory()
     */
    static void setNetworkAccessManagerFactory(AbstractNamFactory *factory);

    /*!
     * \brief Returns the currently set network access manager factory.
     * \sa setNetworkAccessManagerFactory()
     */
    static AbstractNamFactory *networkAccessManagerFactory();

    /*!
     * \brief Sets the global default notificator.
     * \sa defaultNotificator()
     */
    static void setDefaultNotificator(AbstractNotificator *notificator);

    /*!
     * \brief Returns the global default notificator.
     * \sa setDefaultNotificator()
     */
    static AbstractNotificator *defaultNotificator();

Q_SIGNALS:
    /*!
     * \brief This signal is emitted when the in operation status changes.
     * \sa inOperation
     */
    void inOperationChanged(bool inOperation);

    /*!
     * \brief This signal is emitte when the timeout for the request changes.
     * \sa requestTimeout
     */
    void requestTimeoutChanged(quint16 requestTimeout);

    /*!
     * \brief This signal is emitted when the pointer to the Error object changes.
     * \a error will be a nullptr if no error occurred or the current error has been reset.
     * \sa error
     */
    void errorChanged(Fuoten::Error *error);

    /*!
     * \brief This signal is emitted when the pointer to the AbstractConfiguration object changes.
     * \sa configuration
     */
    void configurationChanged(Fuoten::AbstractConfiguration *configuration);

    /*!
     * \brief Notifier signal for the \link Component::storage storage \endlink property.
     * \sa storage(), setDefaultStorage(), setStorage()
     */
    void storageChanged(Fuoten::AbstractStorage *storage);

    /*!
     * \brief Notifier signal for the \link Component::useStorage useStorage \endlink property.
     * \sa setUseStorage(), isUseStorageEnabled()
     */
    void useStorageChanged(bool useStorage);

    /*!
     * \brief Notifier signal for the \link Component::notificator notificator \endlink property.
     * \sa setNotificator(), notificator()
     */
    void notificatorChanged(Fuoten::AbstractNotificator *notificator);

    /*!
     * \brief This signal is emitted if the SSL/TLS session encountered errors during the set up.
     *
     * Will only be emitted if AbstractConfiguration::getIgnoreSSLErrors() returns \c false (the default).
     *
     * \param reply     the network reply that cause the SSL error
     * \param errors    list of occured SSL errors
     */
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);

    /*!
     * \brief Emit this signal in a subclass when the request was successful.
     * \param result JSON data replied by the server as result of an API request
     */
    void succeeded(const QJsonDocument &result);

    /*!
     * \brief Emit this signal in a subclass when the request failed for some reason.
     * \sa error
     * \param error pointer to an Error object
     */
    void failed(Fuoten::Error *error);

protected:
    const QScopedPointer<ComponentPrivate> d_ptr;
    Component(ComponentPrivate &dd, QObject *parent = nullptr);

    /*!
     * \brief Sets the value of the \link Component::inOperation inOperation \endlink property.
     *
     * Use this in subclasses of Component to indicate, that the request is still running or has been finished.
     * \sa inOperation
     */
    void setInOperation(bool nInOperation);

    /*!
     * \brief Sets the pointer of the \link Component::error error \endlink property.
     *
     * If there was already an error set, the old Error will be deleted.
     * \sa error
     */
    void setError(Error *nError);

    /*!
     * \brief Sets the expected JSON type for initial output check.
     *
     * Default: Empty
     *
     * \sa expetedJSONType()
     */
    void setExpectedJSONType(ExpectedJSONType type);

    /*!
     * \brief Sets the API route.
     * \sa apiRoute()
     * \param route complete API route starting with \c "/"
     */
    void setApiRoute(const QString &route);

    /*!
     * \brief Sets the API route constructed from a route part list.
     *
     * The parts in the QStringList will be joind with a '/' into the API route path.
     *
     * \sa apiRoute()
     *
     * \param routeParts list of route parts that will be joined with a '/'.
     */
    void setApiRoute(const QStringList &routeParts);

    /*!
     * \brief Returns the JSON result document.
     */
    QJsonDocument jsonResult() const;

    /*!
     * \brief Performs basic input checks.
     *
     * Reimplement this in a subclass and call the parent's class implementation from there.
     * The basic implementation checks for valid \link Component::configuration configuration property \endlink and basic server and user settings.
     */
    virtual bool checkInput();

    /*!
     * \brief Performs basic output checks.
     *
     * Reimplement this in a subclass and call the parent's class implementation from there.
     * The basic implementation extracts the JSON body, if there is data expected (setExpectedJSONType() is not set to Empty) and checks if
     * the expected JSON data type can be found.
     */
    virtual bool checkOutput();

    /*!
     * \brief Sets the operation the network manager should perform for this call.
     *
     * Default: QNetworkAccessManager::GetOperation
     *
     * \param operation the type of the nework operation
     */
    void setNetworkOperation(QNetworkAccessManager::Operation operation);

    /*!
     * \brief Returns the currently set HTTP headers for the request.
     *
     * \sa setRequestHeaders(), addRequestHeader(), addRequestHeaders()
     */
    QHash<QByteArray, QByteArray> requestHeaders() const;

    /*!
     * \brief Sets the headers to use for the HTTP request.
     *
     * \sa requestHeaders(), addRequestHeader(), addRequestHeaders()
     *
     * \param headers dictionary with \a key as header name and \a value as header value
     */
    void setRequestHeaders(const QHash<QByteArray, QByteArray> &headers);

    /*!
     * \brief Adds a header to the HTTP request.
     *
     * \sa addRequestHeaders(), setRequestHeaders(), requestHeaders()
     *
     * \param headerName    the name of the header entry
     * \param headerValue   the value of the header entry
     */
    void addRequestHeader(const QByteArray & headerName, const QByteArray &headerValue);

    /*!
     * \brief Adds headers to the HTTP request.
     *
     * \sa addRequestHeader(), setRequestHeaders(), requestHeaders()
     *
     * \param headers dictionary with \a key as header name and \a value as header value
     */
    void addRequestHeaders(const QHash<QByteArray, QByteArray> &headers);

    /*!
     * \brief Sets the payload for the request.
     *
     * \param payload the JSON payload to send with the request
     */
    void setPayload(const QByteArray &payload);

    /*!
     * \brief Sets the payload for the request.
     *
     * \param payload the JSON payload to send with the request
     */
    void setPayload(const QJsonObject &payload);

    /*!
     * \brief Sets the URL query for the request.
     *
     * \param query the query to append to the request URL
     */
    void setUrlQuery(const QUrlQuery &query);

    /*!
     * \brief Processes the request data if the request was successful.
     *
     * Reimplement this in a subclass to extract the data from the JSON reply and optionally
     * use the AbtractStorage provided by Component::storage property to store the requested data.
     *
     * Setting the storage is optional, so you should check if it contains a valid pointer.
     *
     * In the successCallback you should also set inOperation to false and emit the succeeded() signal
     * (or a custom succeeded signal).
     *
     * \par Implementation example
     *
     * \code{.cpp}
     *
     * void RenameFolder::successCallback()
     * {
     *     if (isUseStorageEnabled() && storage()) {
     *         storage()->folderRenamed(folderId(), newName());
     *     }
     *
     *     setInOperation(false);
     *
     *     Q_EMIT succeeded(folderId(), newName());
     * }
     *
     * \endcode
     */
    virtual void successCallback() = 0;

    /*!
     * \brief Sends the request to the server.
     */
    void sendRequest();

    /*!
     * \brief Extracts error data from the network reply.
     *
     * Reimplement this in a subclass to extract errors from the request result. The simplest implementation is to create
     * a new Error object from the QNetworkReply and set it to the Component::error property. You should than also
     * emit the failed() signal.
     *
     * The basic implementation uses the overloaded Error constructor to extract reply errors, sets the new Error to the error
     * property, sets inOperation to \c false and emits the failed() signal.
     *
     * \par Implementation example
     *
     * \code{.cpp}
     * void GetStatus::extractError(QNetworkReply *reply)
     * {
     *     setError(new Error(reply, this));
     *     setInOperation(false);
     *     Q_EMIT failed(error());
     * }
     * \endcode
     */
    virtual void extractError(QNetworkReply *reply);

    /*!
     * \brief Set this to true if the request requires authentication.
     *
     * Default: true
     */
    void setRequiresAuth(bool reqAuth);

    /*!
     * \brief Checks if a \link Component::notificator notificator \endlink has been set and will use it to notify about an occured error.
     * \sa AbstractNotificator::notify(const Error *e)
     */
    void notify(const Error *e) const;

    /*!
     * \brief Checks if a \link Component::notificator notificator \endlink has been set and will use it to notify the user.
     * \sa AbstractNotificator::notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data)
     */
    void notify(AbstractNotificator::Type type, QtMsgType severity, const QVariant &data) const;

private Q_SLOTS:
    void _requestFinished();
    void _requestTimedOut();
    void _ignoreSSLErrors(QNetworkReply *reply, const QList<QSslError> &errors);

private:
    Q_DISABLE_COPY(Component)
    Q_DECLARE_PRIVATE(Component)

};

}


#endif // FUOTENCOMPONENT_H
