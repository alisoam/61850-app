#!/bin/sh
set -e

BASEDIR=$(dirname "$0")

sh ${BASEDIR}/program.sh
sh ${BASEDIR}/gdb.sh

