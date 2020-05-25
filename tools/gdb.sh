#!/bin/sh

set -e

JLinkGDBServer -select USB -device MIMXRT1062xxx5A -endian little -if SWD -speed auto -ir -LocalhostOnly
