#!/bin/bash

# ^c$var^ = fg color
# ^b$var^ = bg color
export IDENTIFIER="unicode"
export SEP1=" "
export SEP2=""
interval=0

# load colors!
current_theme=$(cat ~/.dwm/config.h | grep themes | cut -d "/" -f2 | sed 's/"//g')
if [[ $current_theme == lollypop-light.h ]]; then
  . ~/.dwm/bar/themes/lollypop-light
else
  . ~/.dwm/bar/themes/lollypop
fi

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)

  printf "^c$black^ ^b$green^ CPU"
  printf "^c$white^ ^b$black^ $cpu_val $SEP1"
}

pkg_updates() {
  updates=$(checkupdates | wc -l)   # arch , needs pacman contrib

  if [ -z "$updates" ]; then
    printf "^c$green^Fully Updated"
  else
    printf "^c$white^^b$pink^  $updates"" updates ^b$black^$SEP1"
  fi
}

mem() {
  printf "^c$black^^b$green^ MEM "
  printf "^c$white^^b$black^ $(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)"
}

clock() {
  printf "^c$black^ ^b$green^ 󱑆 "
  printf "^c$white^^b$black^ $(date '+%a %H:%M') "
}

volume () {
  STATUS=$(amixer sget Master | tail -n1 | sed -r "s/.*\[(.*)\]/\1/")
    VOL=$(amixer get Master | tail -n1 | sed -r "s/.*\[(.*)%\].*/\1/")
    #printf "%s" "$SEP1"
    if [ "$IDENTIFIER" = "unicode" ]; then
      if [ "$STATUS" = "off" ]; then
              printf "^c#e78284^ 󰝟 Muted ^d^"
      else
        #removed this line becuase it may get confusing
          if [ "$VOL" -gt 0 ] && [ "$VOL" -le 10 ]; then
              printf "^c#e5c890^ 󰕿 %s%% ^d^" "$VOL"
          elif [ "$VOL" -gt 10 ] && [ "$VOL" -le 80 ]; then
              printf "^c#a6d189^ 󰖀 %s%% ^d^" "$VOL"
          else
              printf "^c#ef9f76^ 󰕾 %s%% ^d^" "$VOL"
          fi
    fi
    else
      if [ "$STATUS" = "off" ]; then
        printf "MUTE"
      else
          # removed this line because it may get confusing
          if [ "$VOL" -gt 0 ] && [ "$VOL" -le 33 ]; then
              printf "VOL %s%%" "$VOL"
          elif [ "$VOL" -gt 33 ] && [ "$VOL" -le 66 ]; then
              printf "VOL %s%%" "$VOL"
          else
              printf "VOL %s%%" "$VOL"
          fi
        fi
    fi
    printf "%s\n" "$SEP2"
}

music () {
    if ps -C spotify > /dev/null; then
        PLAYER="spotify"
    elif ps -C spotifyd > /dev/null; then
        PLAYER="spotifyd"
    fi

    if [ "$PLAYER" = "spotify" ] || [ "$PLAYER" = "spotifyd" ]; then
        ARTIST=$(playerctl metadata artist)
        TRACK=$(playerctl metadata title)
        POSITION=$(playerctl position | sed 's/..\{6\}$//')
        DURATION=$(playerctl metadata mpris:length | sed 's/.\{6\}$//')
        STATUS=$(playerctl status)
        SHUFFLE=$(playerctl shuffle)

        if [ "$IDENTIFIER" = "unicode" ]; then
            if [ "$STATUS" = "Playing" ]; then
                STATUS="▶"
            else
                STATUS="⏸"
            fi
            
            if [ "$SHUFFLE" = "On" ]; then
                SHUFFLE=" 🔀"
            else
                SHUFFLE=""
            fi
        else
            if [ "$STATUS" = "Playing" ]; then
                STATUS="PLA"
            else
                STATUS="PAU"
            fi

            if [ "$SHUFFLE" = "On" ]; then
                SHUFFLE=" S"
            else
                SHUFFLE=""
            fi
        fi
        
        if [ "$PLAYER" = "spotify" ]; then
            printf "%s%s %s - %s " "$SEP1" ^c$white^"$STATUS" ^c$blue^"$ARTIST" ^c$green^"$TRACK ^c$red^"
            printf "%0d:%02d $SEP1" $((DURATION%3600/60)) $((DURATION%60))
        else
            printf "%s%s %s - %s " "$SEP1" "$STATUS" "$ARTIST" "$TRACK"
            printf "%0d:%02d/" $((POSITION%3600/60)) $((POSITION%60))
            printf "%0d:%02d" $((DURATION%3600/60)) $((DURATION%60))
            printf "%s%s\n" "$SHUFFLE" "$SEP2"
        fi
    fi
}


while true; do
  [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  interval=$((interval + 1))

  sleep 1 && xsetroot -name "$(music) $updates $(cpu) $(mem) $(volume) $(clock)"
done
