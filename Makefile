PIO := platformio
BOARD := esp12e

all:	build

build:	clean
	$(PIO) run --target buildfs --environment $(BOARD)

upload:	build
	$(PIO) run --target uploadfs --environment $(BOARD)
	$(PIO) device monitor

get_serial:
	$(PIO) device list --serial

monitor:
	$(PIO) device monitor

clean:
	$(PIO) run -t clean
