#!/bin/sh

set -e

BASEDIR=$(dirname "$0")

JLinkExe -exitonerror 1 -device MIMXRT1062xxx5A -if SWD -speed auto -autoconnect 1 -commanderscript ${BASEDIR}/program.jlink
