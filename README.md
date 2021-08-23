# App

## Requirements
- gcc for arm processors
``` sh
sudo apt install gcc-arm-none-eabi
```

## For the First Time
- create the Toolchain.cmake from Toolchain.sample.cmake
- run the following commands:
```
git submodule init
git submodule update
cd modules/lwip; git  apply ../patch/lwip.patch; cd ...
cd modules/libiec61850; git apply ../patch/libiec61850.patch; cd ...
mkdir build; cd build; cmake ..; cd ..
```

## Development
### Build
```
make -C build
```

### Program
```
./tools/program.sh
```

### Debug
```
./tools/debug.sh
```
