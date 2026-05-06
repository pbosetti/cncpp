# CNCpp Python Bindings

This directory contains pybind11 wrappers for the C++ `Point`, `Block`,
`Program`, and `Machine` classes.

Build the extension in place with CMake:

```sh
cmake -S Python -B build/python -G Ninja
cmake --build build/python
PYTHONPATH=$PYTHONPATH:build/python python3 -c "import cncpp; print(cncpp.Point(1, 2, 3))"
```

Or install it as a Python package:

```sh
python3 -m pip install ./Python
```

The bindings preserve the C++ lifetime requirements: a Python `Block` keeps its
previous block and machine alive, and a Python `Program` keeps its machine alive.
