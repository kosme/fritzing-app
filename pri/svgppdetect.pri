# Copyright (c) 2021 Fritzing GmbH

message("Using fritzing svgpp detect script.")

exists($$absolute_path($$_PRO_FILE_PWD_/../svgpp)) {
	SVGPPPATH = $$absolute_path($$_PRO_FILE_PWD_/../svgpp)
    message("found svgpp in $${SVGPPPATH}")
} else {
	error("Could not find svgpp in $$absolute_path($$_PRO_FILE_PWD_/../svgpp)")
}

message("including $$absolute_path($${SVGPPPATH}/include)")
INCLUDEPATH += $$absolute_path($${SVGPPPATH}/include)
