# libfuoten
A [Qt](https://www.qt.io/) based library to provide access to the ownCloud / Nextcloud [News App](https://github.com/nextcloud/news) API.

libfuoten is currently in development and not all features of the News App API are implemented, also interfaces of libfuoten may be changed.
Have a look at the [roadmap](https://github.com/Buschtrommel/Fuoten/milestones) for further information about the development plans. Find out
more in the [API documentation](https://www.buschmann23.de/docs/libfuoten) of libfuoten.

## Features
* account validation (request version, status and user)
* folder management (synchronization, adding, deleting, renaming, marking as read)
* folder list model and filtering
* SQLite storage class

## Goals
* implement complete News App API
* provide convenience functions for synchronization
* provide data models representing feeds, folders and items
* provide local storage classes
* be extensible

## Contributing
The source code is available on [Github](https://github.com/Buschtrommel/libfuoten), feel free to clone or branch according to the LGPLv3.
Translation is done on [Transifex](https://www.transifex.com/buschtrommel/libfuoten).

## Building libfuoten
Either clone the git repository or download one of the [release](https://github.com/Buschtrommel/libfuoten/releases) tarballs. Qt 5.2 or newer
is required to build libfuoten as well as the following Qt modules: Qt5Core, Qt5Network and Qt5Sql.

### Install dependencies
Install gcc, g++, qmake, make, Qt5Core, Qt5Network and Qt5Sql.

#### openSUSE

```
sudo zypper in gcc-++ make libqt5-qtbase-common-devel libqt5-qtbase-devel libQt5Core-devel libQt5Network-devel libQt5Sql-devel
```
    
### Available qmake keywords
* INSTALL_LIB_DIR - defaults to QT_INSTALL_LIBS
* INSTALL_TRANSLATIONS_DIR - defaults to QT_INSTALL_TRANSLATIONS
* INSTALL_HEADERS_DIR - defaults to QT_INSTALL_HEADERS

You can show the default values with `qmake -query`.

### Get libfuoten and build it
On openSUSE qmake for qt5 is named qmake-qt5.

```
git clone https://github.com/Buschtrommel/libfuoten.git
cd libfuoten
mkdir build && cd build
qmake -r ..
make
sudo make install
```
    
## License
libfuoten - Qt based library to access the ownCloud/Nextcloud News App API<br>
Copyright (C) 2016 Buschtrommel / Matthias Fehring<br>
https://www.buschmann23.de/entwicklung/bibliotheken/libfuoten/<br>
https://github.com/Buschtrommel/libfuoten

This library is free software: you can redistribute it and/or<br>
modify it under the terms of the GNU Lesser General Public<br>
License as published by the Free Software Foundation; either<br>
version 3 of the License, or (at your option) any later version.<br>

This library is distributed in the hope that it will be useful,<br>
but WITHOUT ANY WARRANTY; without even the implied warranty of<br>
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU<br>
Lesser General Public License for more details.<br>

You should have received a copy of the GNU Lesser General Public<br>
License along with this library.  If not, see <http://www.gnu.org/licenses/>.
