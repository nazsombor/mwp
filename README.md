# Map with People

A map project, where if you zoom enough, you can see people moving around.

The project consists of a map builder executable that generates map tiles and
generates a highway graph where people can travel, and a game executable where you can move
around in the map and see the people.

## Build on Debian

Install dependencies:
```
$ sudo apt install cmake libosmium2-dev libprotozero-dev libexpat1-dev libbz2-dev libcairo2-dev libz3-4
```

Build the **mapbuilder** executable with CMake:
```
cmake --build cmake-build-release --target mapbuilder -j $(nproc)
```

Build the game executable (**mwp**) with CMake:
```
cmake --build cmake-build-release --target mwp -j $(nproc)
```

Build tests (there are no currently):
```
cmake --build cmake-build-release --target tests -j $(nproc)
```

## Run

Download your country .osm.pbf from [Geofabrik downloads](https://download.geofabrik.de/).

```
cd cmake-build-release
mkdir <country name>
./mapbuilder <country>-latest.osm.pbf <country name>
```

The map builder executable generates the map image tiles, the highway graph and the society file in `<country name>` folder.

Use `./mwp` to run the game.