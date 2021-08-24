# App
## Requirements
- gcc for arm processors
``` sh
sudo apt install gcc-arm-none-eabi
```

- cmake
``` sh
sudo apt install cmake
```

- JLink
Download from [here](https://www.segger.com/downloads/jlink/)

## For the First Time
- create the Toolchain.cmake from Toolchain.sample.cmake
- run the following commands:
```
make init
```

## Development
### Build
``` sh
make
```

### Program
``` sh
make program
```

### Debug (GDB server)
To start the gdb server:
``` sh
make gdb-srv
```

To connect to the swo output of the gdb server:
```
make gdb-swo
```
