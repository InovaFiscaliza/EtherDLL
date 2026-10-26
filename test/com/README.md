# EtherDLL communication tests

`EtherDLLCommunicationTest.m` is a MATLAB black-box integration test for the
TCP protocol. Its request envelopes and CRLF terminator follow the existing
client and command examples in `test/TSMW/`.

`MessageQueueTest.cpp` is a standalone C++ test executable for queue, socket,
session, and configuration helpers. It checks the queue corrections, separate
client-session state and queues, worker completion signaling during exception
unwinding, continuous-worker stop/join, short-send handling, and that the
obsolete `bufferTTLMsgCount` is neither generated nor required while legacy
configurations remain valid.

## Requirements

- MATLAB with `matlab.unittest` and the `tcpclient` API.
- EtherDLL running and listening on the configured endpoint.
- For hardware-independent execution, run EtherDLL in DEMO mode. The test
  sends an IDN request and an unknown-command request; it does not start a
  measurement or access GPS hardware.

By default, the suite reads the host and port from
`test/TSMW/appConfig.json`. Set `ETHERDLL_TEST_HOST` and/or
`ETHERDLL_TEST_PORT` to override them.

Run from the repository root in MATLAB:

```matlab
results = runtests('test/com/EtherDLLCommunicationTest.m');
table(results)
```

The test checks a request split across TCP writes, two requests combined in a
single write, ACK and command/error responses, and acceptance of a new client
after disconnect. Rebuild EtherDLL before running it so the session-shutdown
changes are included. If the test fails, restart the service before another
run because the tested process may be left in a partially closed session.

It also sends a frame larger than the 1,000,000-byte limit and verifies that it
receives no response before its delimiter, is rejected once when complete,
and does not prevent the following valid frame from being processed.
The suite also sends the TSMW STOP command while idle to verify that command
code 3 passes validation without requiring a receiver.

For a manual shutdown check, run EtherDLL in a console, connect the MATLAB
client, and press Ctrl+C or Ctrl+Break. Verify that the process exits and that
the client session workers do not remain blocked. Repeat once with no client
connected to verify shutdown while the listener is waiting.

For a hardware-backed continuous-measurement check, send the TSMW
`cmdSetMeasurement.json` command (which sets `singleSweepMode` to `false`),
then `cmdRequestMeasurement.json`, followed by
`cmdRequestStopMeasurement.json`. Confirm that stop is acknowledged promptly,
measurement results cease, and a subsequent IDN request is still processed.

## Running the MessageQueue test

From a Visual Studio Developer Command Prompt at the repository root, build
and run it with:

```bat
cl /std:c++17 /EHsc /I src /I src\spdlog test\com\MessageQueueTest.cpp /Fe:test\com\MessageQueueTest.exe
test\com\MessageQueueTest.exe
```

The source uses the project's bundled `nlohmann/json` and `spdlog` headers; the
Winsock libraries are declared by `EtherDLLClient.hpp`.
