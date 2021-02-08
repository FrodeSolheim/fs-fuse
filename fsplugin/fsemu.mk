version = $(shell cat VERSION)
uname := $(shell uname -a)

glib_cflags = $(shell pkg-config --cflags glib-2.0)
glib_libs = $(shell pkg-config --libs glib-2.0)

fsemu_cppflags :=
fsemu_libs := -lpng -lSDL2_ttf -lsamplerate

ifneq ($(findstring Msys,$(uname)),)
os := Windows
arch := x86-64
exe := .exe
dll := .dll
opengl_libs := -lopengl32
# -lglew32
else
ifneq ($(findstring Darwin,$(uname)),)
os := macOS
arch := x86-64
exe :=
dll := .so
opengl_libs := -framework OpenGL
fsemu_libs += -framework IOKit -framework CoreFoundation
# -lGLEW
else
os := Linux
arch := x86-64
exe :=
dll := .so
opengl_libs := -lGL
fsemu_libs += -ldl
# -lGLEW
endif
endif

plugin_dir = build/${plugin}
data_dir = ${plugin_dir}/Data
licenses_dir = ${plugin_dir}/Licenses
locale_dir = ${plugin_dir}/Locale
os_arch_dir = ${plugin_dir}/${os}/${arch}

cppflags = -DFSGS -DFSEMU -DFSE ${glib_cflags} ${fsemu_cppflags}
libs = ${glib_libs} ${opengl_libs} ${fsemu_libs}

fsemu-all: fsemu-build
#	@echo Built ${version} for ${os}_${arch}

.PHONY: assemble bootstrap build clean configure distclean install strip \
	package plugin plugin-noclean rebuild

fsemu-assemble: assemble
fsemu-bootstrap: bootstrap
fsemu-build: build
fsemu-clean: clean
fsemu-configure: configure
fsemu-distclean: distclean

install: fsemu-install
strip: fsemu-strip
package: fsemu-package
plugin: fsemu-plugin
plugin-noclean: fsemu-plugin-noclean
rebuild: fsemu-rebuild

fsemu-rebuild: fsemu-bootstrap fsemu-configure fsemu-clean fsemu-build

fsemu-install: fsemu-plugin-noclean
	rm -Rf ../../OpenRetro/System/${plugin}
	mkdir -p ../../OpenRetro/System
	mv ${plugin_dir} ../../OpenRetro/System

fsemu-assemble-pre:
	rm -Rf ${plugin_dir}
	mkdir -p ${plugin_dir}
	echo "[plugin]" > ${plugin_dir}/Plugin.ini
	echo "name = ${plugin}" >> ${plugin_dir}/Plugin.ini
	echo "version = ${version}" >> ${plugin_dir}/Plugin.ini

fsemu-assemble-wrap: fsemu-assemble-pre fsemu-assemble

fsemu-plugin-noclean: fsemu-plugin-prep fsemu-build fsemu-assemble-wrap \
		fsemu-strip fsemu-package
ifeq (${os},Linux)
#	if [ -d ${os_arch_dir} ]; then cp ${os_arch_dir}/*.so* .; fi
endif

fsemu-plugin-prep:
	# Remove locally installed shared libraries (used for development)
	# before building plugin, to avoid getting stale libraries included
	# in the dist.
	rm -f *.so*

fsemu-plugin: fsemu-bootstrap fsemu-configure fsemu-clean \
		fsemu-plugin-noclean

# fsemu-plugin-clean:
# 	rm -Rf dist/${plugin}

fsemu-package:
	mkdir -p dist
	cd build && tar cfJ \
		../dist/${plugin}_${version}_${os}_${arch}.tar.xz \
		${plugin}
	@echo Packaged ${version} for ${os}-${arch}

fsemu-strip:
	python3 standalone.py ${os_arch_dir}
ifeq (${os},macOS)
ifneq (${executable},)
	sh appify.sh ${os_arch_dir} ${plugin} ${executable} ${appid}
	python3 sign.py ${os_arch_dir}/${plugin}.app
	python3 notarize.py ${os_arch_dir}/${plugin}.app ${appid}
endif
endif
