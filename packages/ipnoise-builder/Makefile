IPNOISE_PACKAGES_TARGET=lin64
IPNOISE_PACKAGES_ARCH=x86_64

USER_ORIG=$(USER)

## hard disk params
HARD_DRIVE_ISO		= ./fedora-26-$(IPNOISE_PACKAGES_ARCH).qcow
HARD_DRIVE_SIZE		= 20G

## cdrom isos params
CDROM_ISO			= ./Fedora-Server-dvd-$(IPNOISE_PACKAGES_ARCH)-26-1.5.iso
CDROM_ORIG_ISO		= /mnt/multimedia/iso/Fedora-Server-dvd-$(IPNOISE_PACKAGES_ARCH)-26-1.5.iso
CDROM_CONTENT_DIR	= ./iso
CDROM_MOUNT_POINT	= ./cdrom
CDROM_LABEL			= $(shell blkid -o value -s LABEL $(CDROM_ORIG_ISO))
CDROM_LABEL_ESCAPED = $(shell blkid -o value -s LABEL $(CDROM_ORIG_ISO) | sed -e 's/ /\\\\\\x20/g')
CDROM_ROOTFS_DIR	= ./rootfs
CDROM_ROOTFS_FILES  = $(foreach dir, $(CDROM_ROOTFS_DIR), $(wildcard $(dir)/*))

MOUNT_DIR			= /mnt/ipnoise/
HDD_RPMS_DIR		= /root/rpms/

## qemu params
QEMU_MEM_SIZE		= 2047

QEMU_DIR  = ../out/$(IPNOISE_PACKAGES_TARGET)/ipnoise/pipe/
QEMU      = $(QEMU_DIR)/qemu

## start config {
CDROM=-cdrom $(CDROM_ISO)
HARD_DRIVE=-hda $(HARD_DRIVE_ISO)
ENABLE_KVM=-enable-kvm
# SMP=-smp 4
SMP=-smp 1
VNC_ARG=$(if $(VNC),-vnc $(VNC),)
## start config }

.PHONY: default 	\
	start 			\
	install_os 		\
	hard_drive_iso 	\
	cdrom_iso 		\
	$(CDROM_ISO) 	\
	mount			\
	umount			\
	copy_rpms		\
	lin32 lin64 win32 arm32 install

default:
	@echo "Usage:"
	@echo "  make start             - start OS from hard drive (with IPNoise net)"
	@echo "  make start_with_net    - start OS from hard drive (with common net)"
	@echo "  make install_os        - install OS from CD iso to hard drive"
	@echo "  make cdrom_iso         - to create install CD iso"
	@echo "  make hard_drive_iso    - to create hard drive iso"
	@echo "  make mount    			- mount virtual HDD disk to $(MOUNT_DIR)"
	@echo "  make umount			- umount virtual HDD disk"
	@echo "  make copy_rpms			- copy rpms to virtual HDD to $(HDD_RPMS_DIR)"
	@echo
	@echo "CDROM_LABEL:         '$(CDROM_LABEL)'"
	@echo "CDROM_LABEL_ESCAPED: '$(CDROM_LABEL_ESCAPED)'"
	@echo "CDROM_ORIG_ISO:      '$(CDROM_ORIG_ISO)'"
	@echo
	@echo "CDROM_ISO:           '$(CDROM_ISO)'"
	@echo "HARD_DRIVE_ISO:      '$(HARD_DRIVE_ISO)'"
	@echo
	@echo "VNC:					'$(VNC)'"
	@echo
	@echo "Examples:"
	@echo
	@echo "    VNC=0.0.0.0::5901 make start"
	@echo

## hard disk

$(HARD_DRIVE_ISO):
	@qemu-img create $@ $(HARD_DRIVE_SIZE)

hard_drive_iso: $(HARD_DRIVE_ISO)
	@echo "creating hard drive: '$?'"

## cdrom disk

$(CDROM_CONTENT_DIR):
	mkdir -p $(CDROM_MOUNT_POINT);
	mkdir -p $(CDROM_CONTENT_DIR);
	mount -o loop $(CDROM_ORIG_ISO) $(CDROM_MOUNT_POINT);
	cp -r $(CDROM_MOUNT_POINT)/* $(CDROM_CONTENT_DIR)/;
	umount ${CDROM_MOUNT_POINT};
	rm -rf ${CDROM_MOUNT_POINT};

$(CDROM_ISO): $(CDROM_CONTENT_DIR) $(CDROM_ROOTFS_FILES)
#	cp -f /usr/share/syslinux/isolinux.bin 	${CDROM_CONTENT_DIR}/isolinux/
#	cp -f /boot/extlinux/ldlinux.c32 		${CDROM_CONTENT_DIR}/isolinux/
#	cp -f /boot/extlinux/vesamenu.c32		${CDROM_CONTENT_DIR}/isolinux/
#	cp -f /boot/extlinux/libcom32.c32		${CDROM_CONTENT_DIR}/isolinux/
#	cp -f /boot/extlinux/libutil.c32		${CDROM_CONTENT_DIR}/isolinux/
	cp -rf $(CDROM_ROOTFS_DIR)/* ${CDROM_CONTENT_DIR}/;
	cat ${CDROM_CONTENT_DIR}/isolinux/isolinux.cfg					\
		| sed -e "s/%CDROM_LABEL%/$(CDROM_LABEL_ESCAPED)/g"			\
			> ${CDROM_CONTENT_DIR}/isolinux/isolinux.cfg.tmp;
	mv ${CDROM_CONTENT_DIR}/isolinux/isolinux.cfg.tmp 				\
		${CDROM_CONTENT_DIR}/isolinux/isolinux.cfg;
	mkisofs -o $@                									\
	    -r -V "$(CDROM_LABEL)" -cache-inodes -J -l  				\
	    -b isolinux/isolinux.bin                    				\
	    -c isolinux/boot.cat                        				\
	    -no-emul-boot                               				\
	    -boot-load-size 4                           				\
	    -boot-info-table                            				\
	    -input-charset utf8                         				\
    	${CDROM_CONTENT_DIR}

cdrom_iso: $(CDROM_ISO)

## install os

install_os: cdrom_iso hard_drive_iso
	$(QEMU)															\
		-L $(QEMU_DIR)												\
		$(ENABLE_KVM)												\
		$(SMP)														\
		$(HARD_DRIVE)												\
		$(CDROM)	 												\
		-boot order=cd												\
		-net none													\
		-m $(QEMU_MEM_SIZE)											\
		-localtime													\
		$(VNC_ARG)													\
		$(END_OF_ARGS)

start_with_net:
	$(QEMU)															\
		-L $(QEMU_DIR)												\
		$(ENABLE_KVM)												\
		$(SMP)														\
		$(HARD_DRIVE)												\
		-boot order=c												\
		-net nic -net user											\
		-m $(QEMU_MEM_SIZE)											\
		-localtime													\
		$(VNC_ARG)													\
		$(END_OF_ARGS)

start:
	$(QEMU)															\
		-L $(QEMU_DIR)												\
		$(ENABLE_KVM)												\
		$(SMP)														\
		$(HARD_DRIVE)												\
		$(CDROM)	 												\
		-boot order=c												\
		-net none													\
		-m $(QEMU_MEM_SIZE)											\
		-localtime													\
		$(VNC_ARG)													\
		$(END_OF_ARGS)

#		-virtfs local,path=/,mount_tag=host0,security_model=passthrough,id=host0	\

mount:
	sudo mkdir -p $(MOUNT_DIR)
	sudo guestmount -a $(HARD_DRIVE_ISO) -m /dev/sda2 $(MOUNT_DIR)

umount:
	sudo guestunmount $(MOUNT_DIR)
	sleep 5; ## wait until guestunmount finished, file owner will be changed on 'root' :(
	sudo chown $(USER_ORIG):$(USER_ORIG) $(HARD_DRIVE_ISO) ## change file owner back

copy_rpms: hard_drive_iso
	mkdir -p tmp/rpms
	find ~/rpmbuild/RPMS/$(IPNOISE_PACKAGES_ARCH)/ -name "*.rpm" | xargs -i cp -f {} tmp/rpms/
	$(MAKE) mount
	sudo mkdir -p $(MOUNT_DIR)$(HDD_RPMS_DIR)
	sudo cp -f ./tmp/rpms/*.rpm $(MOUNT_DIR)$(HDD_RPMS_DIR)
	$(MAKE) umount

## not used
lin32:
lin64:
win32:
arm32:
install:

