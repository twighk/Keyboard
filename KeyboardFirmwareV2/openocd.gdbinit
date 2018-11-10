target remote | openocd -f "interface/stlink.cfg" -f "target/nrf52.cfg" -c "gdb_port pipe; log_output openocd.log"
monitor reset halt

