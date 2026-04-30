# Copyright (c) 2021 Fritzing GmbH

message("Using fritzing svgpp detect script.")
SVGPPPATH = $$absolute_path($$_PRO_FILE_PWD_/../svgpp)

exists($$absolute_path($$SVGPPPATH)) {
    message("found svgpp in $${SVGPPPATH}")
} else {
	error("Could not find svgpp in $$absolute_path($$SVGPPPATH)")
}

message("including $$absolute_path($${SVGPPPATH}/include)")
INCLUDEPATH += $$absolute_path($${SVGPPPATH}/include)

