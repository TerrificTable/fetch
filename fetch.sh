#!/bin/bash

hostname=`uname -n`
ip=$(ip -o -4 addr show | awk '/inet 192.168.2./ {split($4,a,"/"); print a[1]; exit}')
disk_info=`df -Ph | awk '/\/dev\/mapper/ {print $2,$3,$5}'`
diskspacefree=$(echo "$disk_info" | awk '{print $2}')
diskspacetotal=$(echo "$disk_info" | awk '{print $1}')
diskspaceperc=$(echo "$disk_info" | awk '{print $3}')
memory_info=$(free -t -m)
memoryusage=$(echo "$memory_info" | awk '/Total/ {print $3}')
memorytotal=$(echo "$memory_info" | awk '/Total/ {print $2}')
cpuusage=$(mpstat 1 1 | awk '/Average:     all/ {print 100 - $NF}')
#cpuusage=$(ps -A -o pcpu | tail -n+2 | paste -sd+ | bc)

r="\033[0m"
g="\033[38;5;241m"
b="\033[38;5;105m"
w="\033[37m"
lb="\033[38;5;153m"
rd="\033[38;5;161m"

if [ -f "/etc/arch-release" ]; then
    os="$rd$lb arch"
elif [ -f "/etc/debian_version" ]; then
    os="$rd$lb debian"
elif [ -f "/etc/gentoo-release" ]; then
    os="$rd$lb gentoo"
elif [ -f "/etc/zorin-release" ]; then
    os="$rd$lb zorin"
elif [ -f "/etc/deepin-version" ]; then
    os="$rd$lb deepin"
elif [ -f "/etc/elementary_version" ]; then
    os="$rd$lb elementary"
elif [ -f "/etc/devuan_version" ]; then
    os="$rd$lb devuan"
elif [ -f "/etc/alpine-release" ]; then
    os="$rd$lb alpine"
elif [ -f "/etc/almalinux-release" ]; then
    os="$rd$lb almalinux"
elif [ -f "/etc/endeavouros-release" ]; then
    os="$rd$lb endeavour"
else
    os="$rd?$lb unknown"
fi

num=$((1+$RANDOM%3))
if [[ $num -eq 1 ]]; then
  l1="  ／l、     "
  l2=" ﾟ､ ｡ ７    "
  l3=" l  ~ ヽ    "
  l4=" じしf_,)ノ "
  l5="            "
elif [[ $num -eq 2 ]]; then
  l1="       "
  l2=" {\\_/} " 
  l3=" (●ᴗ●) "
  l4=" ( >🥕 "
  l5="       "
elif [[ $num -eq 3 ]]; then
  l1=" \\    /\\ "
  l2="  )  ( ')"
  l3=" (  /  ) "
  l4="  \\(__)| "
  l5="         "
fi

echo -e "
 $g$l1$b  cpu  $lb$cpuusage$w%
 $g$l2$b  mem  $lb$memoryusage$w/$lb$memorytotal$w MB
 $g$l3$b  dsk  $lb$diskspacefree$w/$lb$diskspacetotal $w($lb$diskspaceperc$w)
 $g$l4$b  ip   $lb$ip $w($lb$hostname$w)
 $l5$b  os   $os$r
"

# echo "
# ===========================================
#  - CPU Usage.....: $cpuusage%            
#  - Memory Usage..: $memoryusage/$memorytotal MB
#  - Disk Space....: $diskspacefree/$diskspacetotal ($diskspaceperc)
#  - IP............: $ip ($hostname)       
# ============================================
# "

