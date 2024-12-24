central:
	west build --pristine -S zmk-usb-logging -S studio-rpc-usb-uart  -d ncbuild/ -b nice_nano_v2 -- -DSHIELD=tree_central_nano

central_cp: central
	cp ncbuild/zephyr/zmk.uf2 /Volumes/NICENANO/

left: 
	west build  --pristine -d nlbuild/ -b tree -- -DSHIELD=tree_left

right:
	west build  --pristine -d nrbuild/ -b tree -- -DSHIELD=tree_right

all: central left right central_cp
