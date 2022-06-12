#!/bin/bash

# ┏━━━┓╋╋┏┓╋╋╋┏━━━┓┏┓╋╋╋╋╋┏┓
# ┃┏━┓┃╋┏┛┗┓╋╋┃┏━┓┣┛┗┓╋╋╋┏┛┗┓
# ┃┃╋┃┣┓┣┓┏╋━━┫┗━━╋┓┏╋━━┳┻┓┏┛
# ┃┗━┛┃┃┃┃┃┃┏┓┣━━┓┃┃┃┃┏┓┃┏┫┃
# ┃┏━┓┃┗┛┃┗┫┗┛┃┗━┛┃┃┗┫┏┓┃┃┃┗┓
# ┗┛╋┗┻━━┻━┻━━┻━━━┛┗━┻┛┗┻┛┗━┛

### Start 2 monitors ###
xrandr --output HDMI1 --mode 1366x768 --pos 1600x0 --rotate normal --output VGA1 --primary --mode 1600x900 --pos 0x0 --rotate normal --output VIRTUAL1 --off

### Set keymap pt_br ###
setxkbmap -model abnt2 -layout br -variant abnt2

### Set custons cursors ###
xsetroot -cursor_name left_ptr &

### Do not turn off the screen ###
xset s off &
xset -dpms &

### Use nitrogen
nitrogen --restore &
### Use dunst ###
dunst &

polybar -c .config/bspwm/polybar/config.ini main &
