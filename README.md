# PALISADE example [![License MIT][badge-license]](LICENSE)

## Pre-requisites

To build this example, first, you need to have [PALISADE](https://gitlab.com/palisade/palisade-release) installed on your system. Installation instructions are provided [here](https://gitlab.com/palisade/palisade-release#build-instructions).

Then download or clone this PALISADE example repository e.g.,
```
git clone -b master https://gitlab.com/palisade/palisade-release.git
```

## Build this example

To build this example just type:
```
mkdir build && cd build
cmake -DPALISADE_DIR=/usr/local/ ..
make
```

The above command will generate a `mult-demo.out` inside the `build` directory.

If you have installed PALISADE in a custom directory, provide the that directory after the `-DPALISADE_DIR` command line argument.


## Run PALISADE example

Finally, to run the example type:
```
./mult-demo.out
```

## HElib example
Also, check out our [HElib example](https://github.com/TrustworthyComputing/helib_example).


[badge-license]: https://img.shields.io/badge/license-MIT-green.svg?style=flat-square
