# /*******************************************************************
# Part of the Fritzing project - http://fritzing.org
# Copyright (c) 2016 Fritzing
# Fritzing is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# Fritzing is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with Fritzing. If not, see <http://www.gnu.org/licenses/>.
# ********************************************************************

# Windows still uses the old variant, libgit 0.28, static linked
# Linux uses 1.7.1 dynamic linked (for working with openssl 3)
# macOS uses 1.7.1 static linked


LIBGITPATH = $$absolute_path($$_PRO_FILE_PWD_/../libgit2)

unix:!macx {
	LIBGIT_STATIC = false
} else {
	LIBGIT_STATIC = true
}

LIBGIT2LIB = $$LIBGITPATH/lib

win32 {
	if ($$LIBGIT_STATIC) {
		LIBGIT2INCLUDE = "$$_PRO_FILE_PWD_/../libgit2/include"

		exists($$LIBGIT2INCLUDE/git2.h) {
			message("found libgit2 include path at $$LIBGIT2INCLUDE")
		} else {
			message("Fritzing requires libgit2")
			message("Build it from the repo at https://github.com/libgit2")
			message("See https://github.com/fritzing/fritzing-app/wiki for details.")

			error("libgit2 include path not found in $$LIBGIT2INCLUDE")
		}
	}

	exists($$LIBGIT2LIB/libgit2.dll) {
        message("found libgit2 library in $$LIBGIT2LIB")
    } else {
        error("libgit2 library not found in $$LIBGIT2LIB")
    }

	INCLUDEPATH += $$LIBGIT2INCLUDE
    LIBS += -L$$LIBGIT2LIB -lgit2
}

unix {
	LIBGIT2LIB = $$LIBGITPATH/lib
	if ($$LIBGIT_STATIC) {

		exists($$LIBGIT2LIB/libgit2.a) {
			message("found libgit2 library in $$LIBGIT2LIB")
		} else {
			error("static libgit2 library not found in $$LIBGIT2LIB")
		}
		INCLUDEPATH += $$LIBGITPATH/include
		macx {
			LIBS += $$LIBGIT2LIB/libgit2.a -framework Security
		} else {
			LIBS += $$LIBGIT2LIB/libgit2.a -lssl -lcrypto
		}
	} else {
		exists($$LIBGIT2LIB) {
			message("Enabled dynamic linking of libgit2")
			INCLUDEPATH += $$LIBGITPATH/include
				message("libgit: $${INCLUDEPATH}")
			LIBS += -L$$LIBGIT2LIB -lgit2
			!macx {
				QMAKE_RPATHDIR += $$LIBGIT2LIB
			}
		} else {
			error("Could not find $$LIBGIT2LIB")
		}
	}
}

