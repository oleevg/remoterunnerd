# Class Diagrams

## 1. Core Module

```mermaid
classDiagram
    class std_exception {
        <<interface>>
        +what() const char*
    }
    class BaseException {
        -string errorMessage_
        +BaseException(string errorMessage)
        +what() const char*
    }
    class SystemException {
        -int errorCode_
        +SystemException(string errorMessage, int errorCode)
        +getErrorCode() int
    }
    class ThreadHelper {
        +threadIdToString()$ string
        +threadIdToInt()$ long
    }

    std_exception <|-- BaseException
    BaseException <|-- SystemException
```

---

## 2. Network Module

```mermaid
classDiagram
    class IAsyncConnection {
        <<interface>>
        +close() void
        +readAsync(IOBuffer, ReadCompleteHandler, IOHandler) void
        +writeAsync(IOBuffer, IOHandler) void
    }
    class IAsyncListener {
        <<interface>>
        +listenAsync(AcceptHandler) void
        +wait() void
        +stop() void
        #acceptAsync(AcceptHandler) void
    }
    class AsyncBaseConnection~Socket~ {
        -Socket socket_
        -bool started_
        +AsyncBaseConnection(io_context)
        +getSocket() Socket
        +isStarted() bool
        +close() void
        +readAsync(IOBuffer, ReadCompleteHandler, IOHandler) void
        +writeAsync(IOBuffer, IOHandler) void
    }
    class AsyncBaseListener~Acceptor, Socket~ {
        -io_context service_
        -Acceptor acceptor_
        -size_t threadPoolSize_
        -vector~thread~ threadPool_
        +wait() void
        +stop() void
        +listenAsync(AcceptHandler) void
        #acceptAsync(AcceptHandler) void
        #getIoService() io_context
    }
    class AsyncListener {
        +AsyncListener(size_t threadPoolSize, int port)
    }
    class AsyncLocalListener {
        -string unixSocketPath_
        +AsyncLocalListener(size_t threadPoolSize, string socketPath)
        +~AsyncLocalListener()
    }

    IAsyncConnection <|-- AsyncBaseConnection : implements
    IAsyncListener <|-- AsyncBaseListener : implements
    AsyncBaseListener <|-- AsyncListener : tcp specialization
    AsyncBaseListener <|-- AsyncLocalListener : unix specialization
```

**Type aliases defined in `IAsyncConnection.hpp`:**

| Alias | Type |
|---|---|
| `Byte` | `uint8_t` |
| `IOBuffer` | `vector<Byte>` |
| `IOHandler` | `function<void(error_code, size_t)>` |
| `ReadCompleteHandler` | `function<size_t(error_code, size_t)>` |
| `AcceptHandler` | `function<void(IAsyncConnection::Ptr, error_code)>` |

---

## 3. Common Module

```mermaid
classDiagram
    class CommandStore {
        -unordered_set~string~ commands_
        -mutex mtx_
        +CommandStore(size_t sizeHint)
        +registerCommand(string) bool
        +isRegistered(string) bool
        +getAllCommands() CommandCollection
        +setAllCommands(CommandCollection) void
    }
    class ProcessExecutorImpl {
        <<pimpl>>
    }
    class ProcessExecutor {
        -unique_ptr~ProcessExecutorImpl~ impl
        +ProcessExecutor()
        +executeProcess(string execName, Arguments, milliseconds timeout) string
    }
    class ProcessRunnerProtocol {
        -milliseconds processExecutionTimeout_
        -CommandHandlers commandHandlers_
        -CommandStore commandStore_
        -IAsyncConnection connection_
        -IOBuffer readBuffer_
        -string commandPrompt_
        +ProcessRunnerProtocol(IAsyncConnection, CommandStore, milliseconds)
        +start() void
        +close() void
        +getProcessExecutionTimeout() milliseconds
        -startReadTaskAsync() void
        -startWriteTaskAsync(string) void
        -handleRequest() string
        -exitHandler() string
        -listHandler() string
    }
    class TextConfigurationParser {
        -string filePath_
        +TextConfigurationParser(string filePath)
        +readByLine() ConfigurationContent
    }
    class UnixService {
        <<singleton>>
        -mutex mtx$
        -condition_variable conditionVariable$
        +daemonize() void
        +setSignalHandler() void
        +waitForSignalsSync() void
        +registerSignalHandlerFlag(int signal, SignalHandlerFlag) void
        +unregisterSignalHandlerFlag(int signal) bool
        -signalHandler(int signal)$ void
    }
    class SignalHandlerFlag {
        +atomic~bool~ flag
        +SignalHandlerFlag(bool value)
    }

    ProcessExecutor *-- ProcessExecutorImpl : pimpl
    ProcessRunnerProtocol --> CommandStore : queries
    ProcessRunnerProtocol --> ProcessExecutor : creates per request
    ProcessRunnerProtocol --> IAsyncConnection : reads/writes
    UnixService *-- SignalHandlerFlag : manages map of
```

---

## 4. Server Module

```mermaid
classDiagram
    class Application {
        -shared_ptr~ApplicationService~ appService
        +run(int argc, char argv) int
        -parseArguments(int argc, char argv) void
    }
    class ApplicationService {
        -TextConfigurationParser parser_
        -CommandStore commandStore_
        -IAsyncListener asyncListener_
        -SignalHandlerFlag termFlag_
        -SignalHandlerFlag intFlag_
        -SignalHandlerFlag hupFlag_
        -milliseconds processExecutionTimeout_
        -bool daemonized_
        +ApplicationService(int port, milliseconds timeout, size_t threads, parser, commandStore, bool daemonize)
        +ApplicationService(string socketPath, milliseconds timeout, size_t threads, parser, commandStore, bool daemonize)
        +isDaemonized() bool
        +run() int
        -initialize() void
        -signalHandlerTask() void
    }

    Application *-- ApplicationService : owns
    ApplicationService *-- TextConfigurationParser : owns
    ApplicationService *-- CommandStore : owns
    ApplicationService o-- IAsyncListener : owns
    ApplicationService --> UnixService : uses singleton
    ApplicationService --> SignalHandlerFlag : registers with UnixService
```

---

## 5. Full System — Cross-Module Relationships

```mermaid
classDiagram
    %% Server
    class Application
    class ApplicationService

    %% Common
    class CommandStore
    class TextConfigurationParser
    class ProcessRunnerProtocol
    class ProcessExecutor
    class UnixService

    %% Network
    class IAsyncListener {
        <<interface>>
    }
    class IAsyncConnection {
        <<interface>>
    }
    class AsyncListener
    class AsyncLocalListener
    class AsyncBaseListener~Acceptor, Socket~
    class AsyncBaseConnection~Socket~

    %% Core
    class BaseException
    class SystemException

    %% Server relationships
    Application *-- ApplicationService
    ApplicationService *-- CommandStore
    ApplicationService *-- TextConfigurationParser
    ApplicationService o-- IAsyncListener
    ApplicationService --> UnixService

    %% Protocol created per accepted connection
    ApplicationService ..> ProcessRunnerProtocol : creates on accept

    %% Common relationships
    ProcessRunnerProtocol --> CommandStore
    ProcessRunnerProtocol --> IAsyncConnection
    ProcessRunnerProtocol --> ProcessExecutor

    %% Network hierarchy
    IAsyncListener <|-- AsyncBaseListener
    AsyncBaseListener <|-- AsyncListener
    AsyncBaseListener <|-- AsyncLocalListener
    IAsyncConnection <|-- AsyncBaseConnection

    %% Core hierarchy
    BaseException <|-- SystemException
```
