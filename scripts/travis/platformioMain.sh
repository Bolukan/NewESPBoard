#!/bin/sh -eux

platformio ci $PWD$EXAMPLE_FOLDER$EXAMPLE_NAME --lib="." --board=$BOARD
