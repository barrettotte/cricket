PIO := platformio

all:	build

build:	clean
	$(PIO) run

upload:	clean
	$(PIO) run --target upload

get_serial:
	$(PIO) device list --serial

clean:
	$(PIO) run -t clean
