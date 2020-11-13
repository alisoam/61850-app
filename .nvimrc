set path+=./include
set path+=./include/usb
set path+=./board
set path+=./system/components/lan8720
set path+=./include/FreeRTOS
set path+=./modules/FreeRTOS/portable/GCC/ARM_CM4F

function Build()
  make -C build
endfunction

function Debug()
  call Build()
  GdbStart gdb-multiarch
  Ve!
  e term://./tools/debug.sh
endfunction

function Prog()
  !./tools/program.sh
endfunction

command Build call Build()
command Debug call Debug()
command Prog !./tools/program.sh
