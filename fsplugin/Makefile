plugin = FS-Fuse
executable = fs-fuse
appid = dev.solheim.fs-fuse

include fsemu.mk

current_dir = $(shell pwd)
install_dir = ${current_dir}/build

ifeq (${os},macOS)
# Workaround for stupid macOS bug where /libxml2 is missing after /include
xml_cflags = $(shell xml2-config --cflags | sed s./include./include/libxml2. | sed s./libxml2/libxml2./libxml2. )
xml_libs = $(shell xml2-config --libs)
else
# Leaving blank to let the system find these via pkg-config as normal
xml_cflags =
xml_libs =
endif

bootstrap:
	cd ../libspectrum && ./autogen.sh
	cd .. && ./autogen.sh

configure:
	echo ${install_dir}
	cd ../libspectrum && ./configure \
		--disable-silent-rules \
		--prefix=${install_dir} \
		--without-libgcrypt \
		--without-libaudiofile
	cd ../libspectrum && make all install
ifeq (${os},Windows)
    # For running the development version from source dir
	cp ${install_dir}/bin/libspectrum-8.dll ..
endif
	cd .. && ./configure \
		CPPFLAGS="${cppflags}" \
		XML_CFLAGS="${xml_cflags}" \
		XML_LIBS="${xml_libs}" \
		LIBS="${libs}" \
		LIBSPECTRUM_CFLAGS="-I${install_dir}/include" \
		LIBSPECTRUM_LIBS="-L${install_dir}/lib -L${install_dir}/lib64 -lspectrum" \
		--prefix=${install_dir} \
		--disable-silent-rules \
		--without-gtk \
		--without-alsa \
		--without-win32
#
#		--with-sdl \
#

build-fuse:
	make -C .. all

ifeq (${os},Windows)
../${executable}${exe}: ../.libs/fuse${exe}
	cp -p ../.libs/fuse${exe} ../${executable}${exe}
else
../${executable}${exe}: ../fuse${exe}
	cp -p ../fuse${exe} ../${executable}${exe}
endif

build: build-fuse ../${executable}${exe}
#	make -C .. all
#ifeq (${os},Windows)
#	cp ../.libs/fuse${exe} ../${executable}${exe}
#else
#	cp ../fuse${exe} ../${executable}${exe}
#endif

assemble:
	rm -Rf build/share
	make -C .. install

	mkdir -p ${plugin_dir}
	echo ${version} > ${plugin_dir}/Version.txt
	cp ../README.md ${plugin_dir}/ReadMe.txt

	mkdir -p ${os_arch_dir}
	echo ${version} > ${os_arch_dir}/Version.txt
	cp ../${executable}${exe} ${os_arch_dir}/
ifeq (${os},Windows)
	cp ${install_dir}/bin/libspectrum-8.dll ${os_arch_dir}/
endif

	mkdir -p ${licenses_dir}
	cp ../COPYING ${licenses_dir}/Fuse.txt

	mkdir -p ${data_dir}
	cp -a ../fsemu/data/* ${data_dir}/
	cp ${install_dir}/share/fuse/* ${data_dir}/

clean:
	make -C .. clean
	# Do not remove build here (because of libspectrum?)
	rm -Rf dist
	rm -Rf ../${executable}${exe}

distclean: clean
	cd libspectrum && make distclean
	make -C distclean
