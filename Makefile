all:
	@make -f RaylibMake.mk
	@cd nrf-controller/app && make


flash:
	@make -f RaylibMake.mk
	@cd nrf-controller/app && make flash-3
	@cd bin/Debug && ./CE346Final