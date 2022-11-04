# cpu-tracker

Print CPU usage.

It's simple mulithreading program with 3 threads:
* `reader` reads data from `/proc/stat`
* `analyzer` analyzes the data
* `printer` prints output

## Build
```
cmake CMakeLists.txt
make
```

## Run

```
./cpu-tracker
```
