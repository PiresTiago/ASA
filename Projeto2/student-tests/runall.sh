#!/bin/bash

black=`tput setaf 0`
red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`
bold=`tput bold `



for ((i=1;i<=6;i++))
	do
		
		time ./caracol <T0$i.in >T0$i.out.me
		if diff T0$i.out T0$i.out.me >/dev/null ; then
  			echo ${bold}TEST $i${reset} "--" ${green}OK
		else
  			echo ${bold}TEST $i${reset} "--" ${red}NOK

		fi
		echo ${reset}

done
