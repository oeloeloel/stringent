#!/bin/sh


cat ../plotter/data.ino | sed 's/SD\.open/fopen/;s/svgFile.close(/fclose(svgFile/;s/SD\.begin(10)/true/;s/svgFile\.read(/fgetc(svgFile/;s/svgFile\.seek(0)/fseek(svgFile, 0, SEEK_SET)/;s/svgFile\.position()/ftell(svgFile)/;s/svgFile\.seek(/tmp_seek(svgFile, /;s/"1.svg"/strdup("1.svg")/' > /tmp/data.ino


gcc -g -o sim sim.c -lm -std=c99
chmod +x sim
