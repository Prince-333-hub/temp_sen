savedcmd_temp_driver.mod := printf '%s\n'   temp_driver.o | awk '!x[$$0]++ { print("./"$$0) }' > temp_driver.mod
