central: source
	west build --pristine -p -d ncbuild/ -b nice_nano_v2

central_cp: central
	cp ncbuild/zephyr/zmk.uf2 /Volumes/NICENANO/

left: source
	west build  --pristine -d nlbuild/ -b tree -- -DSHIELD=tree_left

right: source
	west build  --pristine -d nrbuild/ -b tree -- -DSHIELD=tree_right

source:
	source ../zephyr/zephyr-env.sh
