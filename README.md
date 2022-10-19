# HTTP Server in C

## Description
An HTTP Server for Linux/Unix machines.

This server only takes GET requests.
This server responds with 200, 400, 404, 418, or 500 responses only.

## How To Use This Server

Note: Only works on Linux or Unix based machines

This server uses port 8080. If you have any Docker containers, or other processes using 8080, this program will fail to bind.

1. clone this git repository to a local directory
```bash
git clone https://github.com/billvanleeuwen424/HTTPServer.git
```

2. Compile the code
```bash
gcc server.c -o server
```

3. Run the code
```bash
./server
```

4. To test locally, using your browser of choice, enter the loopback address into your browser
```
127.0.0.1
```


## Potential Improvements
- Support for POST and HEAD requests,
- logging connections and requests to an external file,
- Command line arguments to serve from a specific directory
- Support for all HTTP responses.


## What I learned
- C Programming
- Systems Programming on UNIX with C
- Network Programming
- TCP Sockets
- HTTP