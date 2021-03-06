APPNAME	= 	nginx
APPDIR 	= 	./nginx-1.5.8

STORE_FILES_TO	= .files
STORE_RPMS_TO	= .rpms
ARCHES			= lin32 lin64
HASH_PATHS		= $(APPNAME)/

## DESTDIR must be only absolute path
DESTDIR ?= $(PWD)/$(STORE_FILES_TO)

.PHONY:		nginx-prepare-lin32		\
			nginx-make-lin32		\
			nginx-install-lin32		\
			nginx-clean-lin32		\
									\
			nginx-prepare-lin64		\
			nginx-make-lin64		\
			nginx-install-lin64		\
			nginx-clean-lin64		\
									\
			nginx-platform-changed	\
			nginx-rpms				\

include ../Makefile.mk

$(APPNAME)/Makefile:
	cd $(APPNAME) 										\
		&& ./configure 									\
			--with-cc-opt=-I$(PWD)/../					\
			--prefix=/etc/nginx/						\
			--sbin-path=/usr/sbin/nginx					\
			--conf-path=/etc/nginx/nginx.conf			\
			--with-http_ssl_module						\
			--with-pcre									\
			--with-ipv6									\
			--error-log-path=/var/log/nginx/error.log	\
			--http-log-path=/var/log/nginx/access.log	\
			--pid-path=/var/run/nginx.pid

## lin32

nginx-make-lin32: $(APPNAME)/Makefile
	$(MAKE) -C $(APPNAME)

nginx-install-lin32:
	cd $(APPNAME)	\
		&& make install DESTDIR=$(DESTDIR)
	install -m 0755 -d $(DESTDIR)/etc/
	install -m 0755 -d $(DESTDIR)/etc/nginx/
	install -m 0755 -d $(DESTDIR)/etc/nginx/conf.d/
	install -m 0755 -d $(DESTDIR)/lib/
	install -m 0755 -d $(DESTDIR)/lib/systemd/
	install -m 0755 -d $(DESTDIR)/lib/systemd/system/
	install -m 0755 rootfs/etc/nginx/nginx.conf	$(DESTDIR)/etc/nginx/nginx.conf
	install -m 0755 rootfs/etc/nginx/conf.d/ipnoise.conf $(DESTDIR)/etc/nginx/conf.d/ipnoise.conf
	install -m 0755 rootfs/lib/systemd/system/ipnoise-nginx.service $(DESTDIR)/lib/systemd/system/ipnoise-nginx.service

nginx-clean-lin32:
	rm -rf $(DESTDIR)/*
	if [ -r "$(APPNAME)/Makefile" ];then	\
		make -C $(APPNAME) clean;			\
	fi

## lin64

nginx-make-lin64: $(APPNAME)/Makefile
	$(MAKE) -C $(APPNAME)

nginx-install-lin64:
	cd $(APPNAME)	\
		&& make install DESTDIR=$(DESTDIR)
	install -m 0755 -d $(DESTDIR)/etc/
	install -m 0755 -d $(DESTDIR)/etc/nginx/
	install -m 0755 -d $(DESTDIR)/etc/nginx/conf.d/
	install -m 0755 -d $(DESTDIR)/lib/
	install -m 0755 -d $(DESTDIR)/lib/systemd/
	install -m 0755 -d $(DESTDIR)/lib/systemd/system/
	install -m 0755 rootfs/etc/nginx/nginx.conf	$(DESTDIR)/etc/nginx/nginx.conf
	install -m 0755 rootfs/etc/nginx/conf.d/ipnoise.conf $(DESTDIR)/etc/nginx/conf.d/ipnoise.conf
	install -m 0755 rootfs/lib/systemd/system/ipnoise-nginx.service $(DESTDIR)/lib/systemd/system/ipnoise-nginx.service

nginx-clean-lin64:
	rm -rf $(DESTDIR)/*
	if [ -r "$(APPNAME)/Makefile" ];then	\
		make -C $(APPNAME) clean;			\
	fi

## not used
nginx-prepare-lin32:
nginx-prepare-lin64:
nginx-platform-changed:
nginx-rpms:

