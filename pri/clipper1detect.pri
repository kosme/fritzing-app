# Copyright (c) 2023 Fritzing GmbH

message("Using fritzing Clipper 1 detect script.")

message("including Clipper1 library")
exists($$absolute_path($$PWD/../../Clipper1-6.4.2)) {
          CLIPPER1 = $$absolute_path($$PWD/../../Clipper1-6.4.2)
	    message("found Clipper1 in $${CLIPPER1}")
} else {
     error("No Clipper1 found")
}

message("including $$absolute_path($${CLIPPER1}/cpp)")
INCLUDEPATH += $$absolute_path($${CLIPPER1}/cpp)

LIBS += -L$$absolute_path($${CLIPPER1}/cpp/lib) -lpolyclipping
QMAKE_RPATHDIR += $$absolute_path($${CLIPPER1}/cpp/lib)
