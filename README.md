# Word Hunt Solver

A iOS GamePigeon Word Hunt game solver, with a custom HTTP server with TCP transmission, custom C++ STL, and Next.JS frontend.

## Building

```
mkdir build
cd build
cmake ..
cmake --build .
```

## Running

From the build folder just made, run one of:
`./server`
to use port 5000 for the server, or specify a server port:
`./server <portNo>`


## Development Notes

If the server hogs the port after shutdown, you may have to kill it manually. In linux:

```
lsof -i :<port_number>
kill -9 <PID from running lsof above>
```
