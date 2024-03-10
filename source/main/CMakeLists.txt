# source
set( srcs
	"spi.c"
	"wifi.c"
	"sntp.c"
	"cgi.c"
	"http.c"
	"staticip.c"
	"segdisplay.c"
	"timer.c"
	"main.c"
)

# build
idf_component_register(
	SRCS ${srcs}
	INCLUDE_DIRS "."
)
