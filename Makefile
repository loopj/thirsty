APP_NAME = thirsty
PARTICLE_CLI ?= particle
DEVICE_TYPE ?= photon
FIRMWARE_TARGET ?= latest
BINARY_NAME = $(APP_NAME)-$(DEVICE_TYPE).bin

.PHONY: all install install-usb clean checkenv enter-dfu-mode

$(BINARY_NAME): clean
	$(PARTICLE_CLI) compile $(DEVICE_TYPE) --verbose --target $(FIRMWARE_TARGET) --saveTo $(BINARY_NAME)

install: checkenv $(BINARY_NAME) ## Build the firmware and flash it via the Particle Cloud
	$(PARTICLE_CLI) flash $(DEVICE_NAME) $(BINARY_NAME)

install-usb: $(FIRMWARE) enter-dfu-mode ## Build the firmware and flash it via USB
	$(PARTICLE_CLI) flash --usb $(BINARY_NAME)

clean: ## Delete all firmware files
	rm -f *.bin

enter-dfu-mode: ## Enter DFU mode on the attached USB device
	-stty -f /dev/tty.usbmodem1411 14400

checkenv:
ifndef DEVICE_NAME
	$(error DEVICE_NAME is not set)
endif

help:
	@awk -F ':|##' '/^[^\t].+?:.*?##/ {\
            printf "\033[36m%-20s\033[0m %s\n", $$1, $$NF \
         }' $(MAKEFILE_LIST)
