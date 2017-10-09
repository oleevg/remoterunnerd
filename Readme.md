# Project Title

Simple network service that provides remote and local execution of registered commands.

### Prerequisites
* CMake build system (>= 3.3) is used to compile the project. Compiling tested with gcc4.9.2 on Debian 8 system. 
* Compiler support for C++11 standard is required.
* boost (>= 1.64.0) is required to build the project.
* [zlog library](https://github.com/HardySimpson/zlog) might be used to collect the services' messages and redirect it to an ordinary file, syslog service etc.

### Getting Started

1. Download or clone the project's repository.
2. Install all required tools and libraries listed in 'Prerequisites' section.
3. Create build directory inside project folder and compile the sources. Building in source tree directory is forbidden.
    ```
    $ mkdir .build && cd .build
    $ cmake ..
    $ make
    ```
4. Getting help:
    ```
    $ cd .build/server
    $ ./runnerd_server -h
    Usage: ./runnerd_server [options]... 
    Options:
    -p [ --port ] arg (=12345)            The port number to listen.
    -c [ --config ] arg (=/etc/remote-runnerd.conf)
                                        The configuration file with supported 
                                        commands list.
    -s [ --socket ] arg (=/tmp/simple-telnetd)
                                        The Unix socket path.
    -t [ --timeout ] arg (=15)            Process execution wait time in ms.
    -u [ --unix ]                         Force to use Unix socket.
    -d [ --debug ]                        Interactive run without daemonizing.
    -h [ --help ]                         As it says.
    ```

### Usage

1. Prepare application's configuration file. 
Create the service configuration file that contains the list of allowed commands to execute. You need to provide only command name without any supported arguments. Each new command must be placed on the new line.
Example of configuration file creation:
    ```
    $ touch ~/remote-runnerd.conf
    $ echo "ls" >>  ~/remote-runnerd.conf
    $ echo "pwd" >>  ~/remote-runnerd.conf
    $ ./runnerd_server -c ~/remote-runnerd.conf
    ...
    ```
Application's default configuration file is '/etc/remote-runnerd.conf' - see application's help.

2. Listening the specified or default port (12345).
Default run mode is working with the specified or default numeric port:
    ```
    $ ./runnerd_server -t 54321
    ``` 

3. Working with local Unix sockets:
You have to provide '-u, --unix' flag to run the service with local Unix socket usage or explicitly provide local socket path through '-s, --socket' argument:
    ```
    $ ./runnerd_server -u
    $ ./runnerd_server -s /tmp/runnerd.sock
    ```

4. Run in interactive (debug) mode.
To run service for debug purposes without daemonizing use '-d, --debug' flag:
    ```
    $./runnerd_server -d
    ```
5. Supported signals.
* The service rereads configuration file and updates the list of allowed commands by receiving the SIGHUP signal.
* Use SIGTERM or SIGINT signals to stop the service.

6. Interaction with service through system tools:
    ```
    $ netcat localhost 12345
    $ telnet localhost 12345
    $ socat - UNIX-CONNECT:/tmp/simple-telnetd
    ```

7. The service internal commands.
The service accepts several internal commands:
* list - get the list of registered commands to execute including internal ones.
* exit - close client's connection (Might not work properly.) 

##$ Running the tests
CMake build framework might be used to run available unit tests. Just run 'ctest' inside build directory after building the sources.
    ```
    $ ctest
    $ ctest --verbose
    ``` 

##$ Authors
Oleg Fedorov, fedorov.ftf@gmail.com


##$ License

To be done...


