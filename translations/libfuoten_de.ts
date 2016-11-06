<?xml version="1.0" ?><!DOCTYPE TS><TS language="de" sourcelanguage="en" version="2.1">
<context>
    <name/>
    <message id="invalid-api-url">
        <location filename="../Fuoten/API/component.cpp" line="+94"/>
        <source>Invalid API URL</source>
        <translation>Ungültige API-URL</translation>
    </message>
    <message id="err-conn-timeout" numerus="yes">
        <location line="+116"/>
        <source>The connection to the server timed out after %n second(s).</source>
        <translation><numerusform>Die Verbindung zum Server hat die Zeitbegrenzung von %n Sekunde überschritten.</numerusform><numerusform>Die Verbindung zum Server hat die Zeitbegrenzung von %n Sekunden überschritten.</numerusform></translation>
    </message>
    <message id="id-err-no-config">
        <location line="+24"/>
        <location filename="../Fuoten/Helpers/accountvalidator.cpp" line="+68"/>
        <source>No configuration available.</source>
        <translation>Keine Konfiguration verfügbar.</translation>
    </message>
    <message id="err-username-pass-missing">
        <location line="+7"/>
        <source>You have to specify a username and a password.</source>
        <translation>Sie müssen einen Benutzernamen und ein Passwort angeben.</translation>
    </message>
    <message id="err-no-host">
        <location line="+7"/>
        <source>No host specified</source>
        <translation>Keine Server-Adresse angegeben.</translation>
    </message>
    <message id="err-no-route">
        <location line="+7"/>
        <source>No API route specified.</source>
        <translation>Keine API-Route angegeben.</translation>
    </message>
    <message id="err-no-payloud">
        <location line="+7"/>
        <source>Empty payload when trying to perform a PUT or POST network operation.</source>
        <translation>Leerer Inhalt beim Durchführen einer PUT- oder POST-Operation.</translation>
    </message>
    <message id="err-empty-answer">
        <location line="+26"/>
        <source>The request replied an empty answer, but there was content expected.</source>
        <translation>Die Anfrage hat eine leere Antwort erhalten obwohl Inhalt erwartet wurde.</translation>
    </message>
    <message id="err-no-json-array">
        <location line="+7"/>
        <source>It was expected that the request returns a JSON array, but it returned something else.</source>
        <translation>In der Antwort wurde ein JSON-Array erwarte, aber es wurde etwas anderes geliefert.</translation>
    </message>
    <message id="err-no-json-object">
        <location line="+7"/>
        <source>It was expected that the request returns a JSON object, but it returned something else.</source>
        <translation>In der Antwort wurde ein JSON-Objekt erwartet, aber es wurde etwas anderes geliefert.</translation>
    </message>
    <message id="err-net-reply-1">
        <location filename="../Fuoten/error.cpp" line="+68"/>
        <source>The remote server at %1 refused the connection.</source>
        <translation>Der Server unter %1 hat die Verbindung verweigert.</translation>
    </message>
    <message id="err-net-reply-2">
        <location line="+4"/>
        <source>The remote server at %1 closed the connection prematurely, before the entire reply was received and processed.</source>
        <translation>Der Server unter %1 hat die Verbindung vorzeitig beendet, bevor die komplette Antwort erhalten wurde.</translation>
    </message>
    <message id="err-net-reply-3">
        <location line="+4"/>
        <source>The remote host name %1 was not found.</source>
        <translation>Der Server unter %1 wurde nicht gefunden.</translation>
    </message>
    <message id="err-net-reply-4">
        <location line="+4"/>
        <source>The connection to the server at %1 timed out.</source>
        <translation>Die Verbindung zum Server hat die Zeitbegrenzung überschritten.</translation>
    </message>
    <message id="err-net-reply-5">
        <location line="+4"/>
        <source>The operation was canceled before it was finished.</source>
        <translation>Die Operation wurde abgebrochen bevor sie fertig war.</translation>
    </message>
    <message id="err-net-reply-6">
        <location line="+4"/>
        <source>The SSL/TLS handshake failed and the encrypted channel could not be established.</source>
        <translation>Der SSL/TLS-Handschlag ist fehlgeschlagen und es konnte keine verschlüsselte Verbindung aufgebaut werden.</translation>
    </message>
    <message id="err-net-reply-7">
        <location line="+4"/>
        <source>The connection was broken due to disconnection from the network.</source>
        <translation>Die Verbindung wurde durch Abtrennen vom Netzwerk unterbrochen.</translation>
    </message>
    <message id="err-net-reply-8">
        <location line="+4"/>
        <source>The connection was broken due to disconnection from the network or failure to start the network.</source>
        <translation>Die Verbindung wurde unterbrochen weil das Gerät vom Netzwerk getrennt wurde oder das Netzwerk nicht gestartet werden konnte.</translation>
    </message>
    <message id="err-net-reply-9">
        <location line="+4"/>
        <source>The background request is not currently allowed due to platform policy.</source>
        <translation>Die Hintergrundanfrage ist auf Grund von Plattformeinschränkungen zur Zeit nicht erlaubt.</translation>
    </message>
    <message id="err-net-reply-10">
        <location line="+5"/>
        <source>While following redirects, the maximum limit was reached.</source>
        <translation>Es wurde die maximale Anzahl an Umleitungen überschritten.</translation>
    </message>
    <message id="err-net-reply-11">
        <location line="+4"/>
        <source>While following redirects, the network access API detected a redirect from an encrypted protocol (https) to an unencrypted one (http).</source>
        <translation>Beim Folgen einer Umleitung wurde eine Umleitung von einem verschlüsselten Protokoll (https) auf ein unverschlüsseltes (http) entdeckt.</translation>
    </message>
    <message id="err-net-reply-101">
        <location line="+5"/>
        <source>The connection to the proxy server was refused (the proxy server is not accepting requests).</source>
        <translation>Die Verbindung wurde vom Proxy-Server abgelehnt (der Proxy-Server akzeptiert keine Anfragen).</translation>
    </message>
    <message id="err-net-reply-102">
        <location line="+4"/>
        <source>The proxy server closed the connection prematurely, before the entire reply was received and processed.</source>
        <translation>Der Proxy-Server hat die Verbindung vorzeitig beendet, bevor die komplette Antwort empfangen werden konnte.</translation>
    </message>
    <message id="err-net-reply-103">
        <location line="+4"/>
        <source>The proxy host name was not found (invalid proxy hostname).</source>
        <translation>Der Proxy-Server konnte nicht gefunden werden (ungültiger Proxy-Host-Name).</translation>
    </message>
    <message id="err-net-reply-104">
        <location line="+4"/>
        <source>The connection to the proxy timed out or the proxy did not reply in time to the request sent</source>
        <translation>Die Verbindung zum Proxy hat die Zeitbegrenzung überschritten oder der Proxy hat nicht rechtzeitig geantwortet.</translation>
    </message>
    <message id="err-net-reply-105">
        <location line="+4"/>
        <source>The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any).</source>
        <translation>Der Proxy verlangt nach einer Authentifizierung, lehnt die Anmeldeinformationen aber ab.</translation>
    </message>
    <message id="err-net-reply-201">
        <location line="+4"/>
        <source>The access to the remote content was denied.</source>
        <translation>Der Zugriff auf den Server wurde verweigert.</translation>
    </message>
    <message id="err-net-reply-202">
        <location line="+4"/>
        <source>The operation requested on the remote content is not permitted.</source>
        <translation>Die auf die entfernten Inhalte anzuwendende Operation ist nicht erlaubt.</translation>
    </message>
    <message id="err-net-reply-203">
        <location line="+4"/>
        <source>The remote content was not found at the server.</source>
        <translation>Der verlangte Inhalt wurde auf dem Server nicht gefunden.</translation>
    </message>
    <message id="err-net-reply-204">
        <location line="+4"/>
        <source>The remote server requires authentication to serve the content but the credentials provided were not accepted (if any).</source>
        <translation>Der Server verlangt für die Auslieferung der Inhalte nach einer Authentifizierung, die bereitgestellten Anmeldeinformationen wurden allerdings nicht akzeptiert.</translation>
    </message>
    <message id="err-net-reply-205">
        <location line="+4"/>
        <source>The request needed to be sent again, but this failed for example because the upload data could not be read a second time.</source>
        <translation>Die Anfrage musste noch einmal gesendet werden, was aber fehlschlug, bspw. weil die Daten kein zweites mal gelesen werden konnten.</translation>
    </message>
    <message id="err-net-reply-206">
        <location line="+5"/>
        <source>The request could not be completed due to a conflict with the current state of the resource.</source>
        <translation>Die Anfrage konnte auf Grund eines Konflikts mit dem aktuellen Zustand der Ressource nicht vervollständigt werden.</translation>
    </message>
    <message id="err-net-reply-207">
        <location line="+4"/>
        <source>The requested resource is no longer available at the server.</source>
        <translation>Die angefragte Ressource ist auf dem Server nicht mehr verfügbar.</translation>
    </message>
    <message id="err-net-reply-401">
        <location line="+4"/>
        <source>The server encountered an unexpected condition which prevented it from fulfilling the request.</source>
        <translation>Auf dem Server ist ein unerwarteter Fehler aufgetreten, der die Verarbeitung der Anfrage verhinderte.</translation>
    </message>
    <message id="err-net-reply-402">
        <location line="+4"/>
        <source>The server does not support the functionality required to fulfill the request.</source>
        <translation>Der Server unterstützt die Funktion zur Beantwortung der Anfrage nicht.</translation>
    </message>
    <message id="err-net-reply-403">
        <location line="+4"/>
        <source>The server is unable to handle the request at this time.</source>
        <translation>Der Server kann die Anfrage zur Zeit nicht verarbeiten.</translation>
    </message>
    <message id="err-net-reply-301">
        <location line="+5"/>
        <source>The Network Access API cannot honor the request because the protocol is not known.</source>
        <translation>Die Anfrage kann nicht verarbeitet werden, da das Protokoll nicht bekannt ist.</translation>
    </message>
    <message id="err-net-reply-302">
        <location line="+4"/>
        <source>The requested operation is invalid for this protocol.</source>
        <translation>Diese Operation ist mit diesem Protokoll nicht möglich.</translation>
    </message>
    <message id="err-net-reply-99">
        <location line="+4"/>
        <source>An unknown network-related error was detected.</source>
        <translation>Es ist ein unbekannter Netzwerkfehler aufgetreten.</translation>
    </message>
    <message id="err-net-reply-199">
        <location line="+4"/>
        <source>An unknown proxy-related error was detected.</source>
        <translation>Es ist ein unbekannter Proxy-Fehler aufgetreten.</translation>
    </message>
    <message id="err-net-reply-299">
        <location line="+4"/>
        <source>An unknown error related to the remote content was detected.</source>
        <translation>Es wurde ein unbekannter Fehler mit dem entfernten Inhalt festgestellt.</translation>
    </message>
    <message id="err-net-reply-399">
        <location line="+4"/>
        <source>A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.).</source>
        <translation>Es wurde ein Ausfall im Protokoll festgestellt (Parsing-Fehler, ungültige oder unerwartete Antworten, usw.).</translation>
    </message>
    <message id="err-net-reply-499">
        <location line="+5"/>
        <location line="+5"/>
        <source>An unknown error related to the server response was detected.</source>
        <translation>Es wurde ein unbekannter Fehler im Zusammenhang mit der Server-Antwort festgestellt.</translation>
    </message>
    <message id="id-err-version-low-status-user">
        <location filename="../Fuoten/Helpers/accountvalidator.cpp" line="+21"/>
        <source>The version of your News App is lower than 5.2.4. Status and user information can not be queried.</source>
        <translation>Die Version Ihrer News App ist kleiner als 5.2.4. Status und Benutzerinformationen können nicht abgefragt werden.</translation>
    </message>
    <message id="id-err-version-low-user">
        <location line="+22"/>
        <source>The version of your News App is lower than 6.0.5. User information can not be queried.</source>
        <translation>Die Version Ihrer News App ist kleiner als 6.0.5. Benutzerinformationen können nicht abgefragt werden.</translation>
    </message>
    <message id="err-version-not-found">
        <location filename="../Fuoten/API/getstatus.cpp" line="+102"/>
        <location filename="../Fuoten/API/getversion.cpp" line="+99"/>
        <source>Can not find the version information in the server reply.</source>
        <translation>Die Server-Antwort enthält keine Versionsinformation.</translation>
    </message>
    <message id="err-warnings-not-found">
        <location line="+7"/>
        <source>Can not find the warnings information in the server reply.</source>
        <translation>Die Server-Antwort enthält keine Warnhinweise.</translation>
    </message>
    <message id="err-displayname-not-found">
        <location filename="../Fuoten/API/getuser.cpp" line="+107"/>
        <source>Can not find the user&apos;s display name in the server reply.</source>
        <translation>Die Server-Antwort enthält keinen Anzeigenamen des Benutzers.</translation>
    </message>
    <message id="libfuoten-err-failed-open-db">
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="+69"/>
        <location line="+254"/>
        <source>Failed to open the SQLite database.</source>
        <translation>Die SQLite-Datenbank konnte nicht geöffnet werden.</translation>
    </message>
    <message id="fuoten-error-failed-execute-query">
        <location line="-242"/>
        <location line="+16"/>
        <location line="+11"/>
        <location line="+6"/>
        <location line="+20"/>
        <location line="+23"/>
        <location line="+6"/>
        <location line="+6"/>
        <location line="+6"/>
        <location line="+10"/>
        <location line="+10"/>
        <location line="+10"/>
        <location line="+10"/>
        <location line="+10"/>
        <location line="+11"/>
        <location line="+12"/>
        <location line="+16"/>
        <location line="+7"/>
        <location line="+31"/>
        <location line="+10"/>
        <location line="+65"/>
        <location line="+16"/>
        <location line="+136"/>
        <location line="+24"/>
        <location line="+26"/>
        <location line="+69"/>
        <location line="+45"/>
        <location line="+61"/>
        <location line="+49"/>
        <location line="+25"/>
        <location line="+69"/>
        <location line="+49"/>
        <location line="+83"/>
        <location line="+44"/>
        <location line="+70"/>
        <location line="+44"/>
        <location line="+25"/>
        <location line="+80"/>
        <location line="+37"/>
        <location line="+44"/>
        <location line="+44"/>
        <location line="+44"/>
        <location line="+32"/>
        <location line="+137"/>
        <location line="+60"/>
        <location line="+97"/>
        <location line="+88"/>
        <location line="+23"/>
        <location line="+67"/>
        <location line="+39"/>
        <location line="+15"/>
        <location line="+10"/>
        <source>Failed to execute database query.</source>
        <translation>Datenbankabfrage konnte nicht durchgeführt werden.</translation>
    </message>
    <message id="fuoten-error-transaction-begin">
        <location line="-1942"/>
        <location line="+430"/>
        <location line="+524"/>
        <location line="+62"/>
        <location line="+807"/>
        <source>Failed to begin a database transaction.</source>
        <translation>Datenbanktransaktion konnte nicht begonnen werden.</translation>
    </message>
    <message id="fuoten-error-transaction-commit">
        <location line="-1656"/>
        <location line="+335"/>
        <location line="+493"/>
        <location line="+139"/>
        <location line="+792"/>
        <source>Failed to commit a database transaction.</source>
        <translation>Datenbanktransaktion konnte nicht abgeschlossen werden.</translation>
    </message>
    <message id="libfuoten-err-sqlite-db-not-ready">
        <location line="-1675"/>
        <location line="+75"/>
        <location line="+249"/>
        <location line="+117"/>
        <location line="+206"/>
        <location line="+253"/>
        <location line="+67"/>
        <location line="+37"/>
        <location line="+43"/>
        <location line="+45"/>
        <location line="+619"/>
        <source>SQLite database not ready. Can not process requested data.</source>
        <translation>SQLite-Datenbank nicht verfügbar. Daten können nicht verarbeitet werden.</translation>
    </message>
    <message id="libfuoten-err-invalid-feed-id">
        <location filename="../Fuoten/API/deletefeed.cpp" line="+91"/>
        <location filename="../Fuoten/API/markfeedread.cpp" line="+79"/>
        <location filename="../Fuoten/API/movefeed.cpp" line="+80"/>
        <location filename="../Fuoten/API/renamefeed.cpp" line="+114"/>
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="-1699"/>
        <location line="+961"/>
        <location line="+37"/>
        <location line="+49"/>
        <location line="+39"/>
        <source>The feed ID is not valid.</source>
        <translation>Die Feed-ID ist ungültig.</translation>
    </message>
    <message id="libfuoten-err-empty-feed-name">
        <location filename="../Fuoten/API/renamefeed.cpp" line="+8"/>
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="-45"/>
        <source>The feed name can not be empty.</source>
        <translation>Der Fee-Titel darf nicht leer sein.</translation>
    </message>
    <message id="libfuoten-err-feed-not-exists">
        <location filename="../Fuoten/API/deletefeed.cpp" line="+38"/>
        <location filename="../Fuoten/API/markfeedread.cpp" line="+44"/>
        <location filename="../Fuoten/API/movefeed.cpp" line="+30"/>
        <location filename="../Fuoten/API/renamefeed.cpp" line="+42"/>
        <source>The feed was not found on the server.</source>
        <translation>Der Feed wurde auf dem Server nicht gefunden.</translation>
    </message>
    <message id="fuoten-failed-query-folders">
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="-948"/>
        <source>Failed to query the folders from the local database.</source>
        <translation>Ordner konnten nicht aus der lokalen Datenbank abgefragt werden.</translation>
    </message>
    <message id="fuoten-error-failed-prepare-query">
        <location line="-58"/>
        <location line="+159"/>
        <location line="+26"/>
        <location line="+69"/>
        <location line="+45"/>
        <location line="+111"/>
        <location line="+25"/>
        <location line="+118"/>
        <location line="+116"/>
        <location line="+70"/>
        <location line="+47"/>
        <location line="+99"/>
        <location line="+51"/>
        <location line="+43"/>
        <location line="+44"/>
        <location line="+44"/>
        <location line="+33"/>
        <location line="+462"/>
        <location line="+35"/>
        <source>Failed to prepare database query.</source>
        <translation>Datenbankabfrage konnte nicht vorbereitet werden.</translation>
    </message>
    <message id="libfuoten-err-invalid-feed-url">
        <location filename="../Fuoten/API/createfeed.cpp" line="+81"/>
        <source>The URL of the feed is not valid.</source>
        <translation>Die URL des Feeds ist ungültig.</translation>
    </message>
    <message id="libfuoten-err-invalid-folder-id">
        <location line="+8"/>
        <location filename="../Fuoten/API/deletefolder.cpp" line="+75"/>
        <location filename="../Fuoten/API/markfolderread.cpp" line="+81"/>
        <location filename="../Fuoten/API/movefeed.cpp" line="-22"/>
        <location filename="../Fuoten/API/renamefolder.cpp" line="+81"/>
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="-1638"/>
        <location line="+330"/>
        <location line="+111"/>
        <location line="+569"/>
        <source>The folder ID is not valid.</source>
        <oldsource>Failed to rename the folder. The folder ID is not valid.</oldsource>
        <translation>Die Ordner-ID ist ungültig.</translation>
    </message>
    <message id="libfuoten-err-feed-exists">
        <location line="+45"/>
        <source>The feed does already exist on the server.</source>
        <translation>Der Feed existiert bereits auf dem Server.</translation>
    </message>
    <message id="libfuoten-err-feed-unreadable">
        <location line="+4"/>
        <source>The feed can not be read (most likely contains errors).</source>
        <translation>Der Feed kann nicht gelesen werden (enthält wahrscheinlich Fehler).</translation>
    </message>
    <message id="libfuoten-err-empty-folder-name">
        <location filename="../Fuoten/API/createfolder.cpp" line="+79"/>
        <location filename="../Fuoten/API/renamefolder.cpp" line="+8"/>
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="-686"/>
        <source>The folder name can not be empty.</source>
        <oldsource>Failed to rename the folder. The new folder name is empty.</oldsource>
        <translation>Der Ordnername darf nicht leer sein.</translation>
    </message>
    <message id="libfuoten-err-no-folders-array-in-reply">
        <location line="+22"/>
        <location filename="../Fuoten/API/getfolders.cpp" line="+98"/>
        <source>The data the server replied does not contain a &quot;folders&quot; array.</source>
        <translation>Die vom Server erhaltenen Daten enthalten kein &quot;folders&quot; Array.</translation>
    </message>
    <message id="libfuoten-err-folder-name-exists">
        <location line="+23"/>
        <location filename="../Fuoten/API/renamefolder.cpp" line="+42"/>
        <source>The folder name does already exist on the server.</source>
        <oldsource>Failed to rename the folder. The new folder name does already exist on the server.</oldsource>
        <translation>Der Ordnername existiert auf dem Server bereits.</translation>
    </message>
    <message id="libfuoten-err-folder-not-exists">
        <location filename="../Fuoten/API/deletefolder.cpp" line="+38"/>
        <location filename="../Fuoten/API/markfolderread.cpp" line="+44"/>
        <location filename="../Fuoten/API/renamefolder.cpp" line="+4"/>
        <source>The folder was not found on the server.</source>
        <oldsource>Failed to rename the folder. The folder to rename was not found on the server.</oldsource>
        <translation>Der Ordner wurde auf dem Server nicht gefunden.</translation>
    </message>
    <message id="libfuoten-err-folder-invalid-name">
        <location filename="../Fuoten/API/createfolder.cpp" line="+4"/>
        <location filename="../Fuoten/API/renamefolder.cpp" line="+4"/>
        <source>The folder name is invalid (for instance empty).</source>
        <oldsource>Failed to rename the folder. The new folder name is invalid (for instance empty).</oldsource>
        <translation>Der Ordnername ist ungültig (bspw. leer).</translation>
    </message>
    <message id="libfuoten-err-invalid-item-id">
        <location filename="../Fuoten/API/markfeedread.cpp" line="-36"/>
        <location filename="../Fuoten/API/markfolderread.cpp" line="-36"/>
        <location filename="../Fuoten/Storage/sqlitestorage.cpp" line="+774"/>
        <source>The item ID is not valid.</source>
        <translation>Die Artikel-ID ist ungültig.</translation>
    </message>
    <message id="libfuoten-err-no-feeds-array-in-reply">
        <location filename="../Fuoten/API/createfeed.cpp" line="-27"/>
        <location filename="../Fuoten/API/getfeeds.cpp" line="+95"/>
        <source>The data the server replied does not contain a &quot;feeds&quot; array.</source>
        <translation>Die vom Server gelieferten Daten enthalten kein Feeds-Array.</translation>
    </message>
    <message id="libfuoten-err-no-items-array-in-reply">
        <location filename="../Fuoten/API/getitems.cpp" line="+121"/>
        <location filename="../Fuoten/API/getupdateditems.cpp" line="+110"/>
        <source>The data the server replied does not contain an &quot;items&quot; array.</source>
        <translation>Die vom Server erhaltenen Daten enthalten kein &quot;items&quot; Array.</translation>
    </message>
    <message id="libfuoten-error-invalid-id">
        <location line="+24"/>
        <location filename="../Fuoten/API/getupdateditems.cpp" line="+32"/>
        <source>Invalid ID</source>
        <translation>Ungültige ID</translation>
    </message>
    <message id="libfuoten-error-invalid-time">
        <location filename="../Fuoten/API/getupdateditems.cpp" line="-8"/>
        <source>Invalid last modified time.</source>
        <translation>Ungültiger Zeitstempel der Zeit der letzten Änderung.</translation>
    </message>
    <message id="libfuoten-today-dateteime">
        <location filename="../Fuoten/Models/articlelistmodel_p.h" line="+39"/>
        <source>Today, %1</source>
        <translation>Heute, %1</translation>
    </message>
    <message id="libfuoten-today-datetime">
        <location filename="../Fuoten/article_p.h" line="+85"/>
        <source>Today, %1</source>
        <translation>Heute, %1</translation>
    </message>
    <message id="libfuoten-time-format">
        <location line="+2"/>
        <location line="+6"/>
        <location filename="../Fuoten/Models/articlelistmodel_p.h" line="+2"/>
        <source>hh:mm</source>
        <translation>hh:mm</translation>
    </message>
    <message id="libfuoten-yesterday-datetime">
        <location line="-2"/>
        <source>Yesterday, %1</source>
        <translation>Gestern, %1</translation>
    </message>
    <message id="libfuoten-day-time-format">
        <location line="+6"/>
        <source>dddd, hh:mm</source>
        <translation>dddd, hh:mm</translation>
    </message>
    <message id="libfuoten-short-datetime">
        <location line="+3"/>
        <source>d. MMMM, hh:mm</source>
        <oldsource>d.MMMM, hh:mm</oldsource>
        <translation>d. MMMM, hh:mm</translation>
    </message>
    <message id="libfuoten-date-time-format">
        <location line="+3"/>
        <source>d. MMM yyyy, hh:mm</source>
        <oldsource>d.MMM yyyy, hh:mm</oldsource>
        <translation>d. MMM yyyy, hh:mm</translation>
    </message>
</context>
</TS>