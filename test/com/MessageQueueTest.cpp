#include "../../src/EtherDLLClient.hpp"

#include <future>
#include <climits>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <spdlog/sinks/null_sink.h>

spdlog::logger* loggerPtr = nullptr;

// EtherDLLClient.hpp references this function from ClientConn::pingClient.
// The queue tests do not exercise demo payload generation.
json buildDemoData() {
	return json::object();
}

namespace {

struct InterruptWaiterOnExit {
	MessageQueue& queue;
	std::atomic<edll::INT_CODE>& interruptionCode;

	~InterruptWaiterOnExit() {
		interruptionCode.store(edll::Code::CTRL_C_INTERRUPT, std::memory_order_release);
		queue.notify();
	}
};

struct StopSessionOnExit {
	MessageQueue& queue;
	std::atomic<bool>& sessionStop;

	~StopSessionOnExit() {
		sessionStop.store(true, std::memory_order_release);
		queue.notify();
	}
};

void require(bool condition, const char* message) {
	if (!condition) {
		throw std::runtime_error(message);
	}
}

template<typename Predicate>
bool waitUntil(Predicate predicate, std::chrono::milliseconds timeout) {
	const auto deadline = std::chrono::steady_clock::now() + timeout;
	while (std::chrono::steady_clock::now() < deadline) {
		if (predicate()) {
			return true;
		}
		std::this_thread::yield();
	}
	return predicate();
}

void testPushAndWaitTracksItsOwnMessage() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };

	const auto earlierId = queue.push(json::object(), "MessageQueueTest");
	auto waiter = std::async(std::launch::async, [&] {
		return queue.pushAndWait(json::object(), "MessageQueueTest", interruptionCode);
	});
	InterruptWaiterOnExit cleanup{ queue, interruptionCode };

	require(waitUntil([&] { return queue.size() == 2; }, std::chrono::seconds(2)),
		"pushAndWait did not enqueue its message");
	require(waiter.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout,
		"pushAndWait returned before its message was popped");

	const json earlier = queue.pop("MessageQueueTest");
	require(earlier.value(taskKeys::QueueId::VALUE, ULONG_MAX) == earlierId,
		"The queue did not preserve FIFO order");
	require(waiter.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout,
		"Popping an earlier message incorrectly released pushAndWait");

	const json waitedFor = queue.pop("MessageQueueTest");
	require(waitedFor.value(taskKeys::QueueId::VALUE, ULONG_MAX) == earlierId + 1,
		"The waited-for message was not the next FIFO item");
	require(waiter.wait_for(std::chrono::seconds(2)) == std::future_status::ready,
		"pushAndWait did not return after its message was popped");
	require(waiter.get() == earlierId + 1,
		"pushAndWait returned the wrong public queue ID");
}

void testPushAndWaitUnblocksOnInterruption() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };

	auto waiter = std::async(std::launch::async, [&] {
		return queue.pushAndWait(json::object(), "MessageQueueTest", interruptionCode);
	});
	InterruptWaiterOnExit cleanup{ queue, interruptionCode };

	require(waitUntil([&] { return queue.size() == 1; }, std::chrono::seconds(2)),
		"pushAndWait did not enqueue its message before interruption");
	require(waiter.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout,
		"pushAndWait returned before pop or interruption");

	interruptionCode.store(edll::Code::CTRL_C_INTERRUPT, std::memory_order_release);
	queue.notify();
	require(waiter.wait_for(std::chrono::seconds(2)) == std::future_status::ready,
		"pushAndWait did not unblock after interruption notification");
	(void)waiter.get();
}

void testWaitActionWakesWhenQueueChanges() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };
	std::promise<void> waitStarted;
	auto started = waitStarted.get_future();

	auto waiter = std::async(std::launch::async, [&] {
		waitStarted.set_value();
		return queue.waitAction(interruptionCode, "MessageQueueTest", 5000);
	});
	InterruptWaiterOnExit cleanup{ queue, interruptionCode };
	started.wait();

	queue.push(json::object(), "MessageQueueTest");
	require(waiter.wait_for(std::chrono::seconds(1)) == std::future_status::ready,
		"waitAction did not wake promptly after a push");
	require(waiter.get(), "waitAction did not report the queue change");
}

void testWaitActionReturnsFalseOnTimeout() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };

	require(!queue.waitAction(interruptionCode, "MessageQueueTest", 20),
		"waitAction should return false when its timeout expires without a push");
}

void testWaitActionUnblocksOnInterruption() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };
	std::promise<void> waitStarted;
	auto started = waitStarted.get_future();

	auto waiter = std::async(std::launch::async, [&] {
		waitStarted.set_value();
		return queue.waitAction(interruptionCode, "MessageQueueTest", 5000);
	});
	InterruptWaiterOnExit cleanup{ queue, interruptionCode };
	started.wait();

	interruptionCode.store(edll::Code::CTRL_C_INTERRUPT, std::memory_order_release);
	queue.notify();
	require(waiter.wait_for(std::chrono::seconds(1)) == std::future_status::ready,
		"waitAction did not unblock after interruption notification");
	require(!waiter.get(), "waitAction should not report an interruption as a queue push");
}

void testWaitAndPopHasSingleConsumerForEachItem() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };
	std::atomic<int> consumersStarted{ 0 };

	auto consume = [&] {
		consumersStarted.fetch_add(1, std::memory_order_release);
		return queue.waitAndPop(interruptionCode, "MessageQueueTest");
	};
	auto firstConsumer = std::async(std::launch::async, consume);
	auto secondConsumer = std::async(std::launch::async, consume);
	InterruptWaiterOnExit cleanup{ queue, interruptionCode };

	require(waitUntil([&] { return consumersStarted.load(std::memory_order_acquire) == 2; },
		std::chrono::seconds(2)), "Consumers did not start");
	queue.push(json{ { "payload", "single-item" } }, "MessageQueueTest");

	require(waitUntil([&] {
		return firstConsumer.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready ||
			secondConsumer.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
		}, std::chrono::seconds(2)), "No consumer received the queued item");

	const bool firstReady = firstConsumer.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
	const bool secondReady = secondConsumer.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready;
	require(firstReady != secondReady,
		"One queue item must wake exactly one consumer");

	interruptionCode.store(edll::Code::CTRL_C_INTERRUPT, std::memory_order_release);
	queue.notify();
	require(firstConsumer.wait_for(std::chrono::seconds(2)) == std::future_status::ready &&
		secondConsumer.wait_for(std::chrono::seconds(2)) == std::future_status::ready,
		"The remaining consumer did not unblock on interruption");

	const json firstResult = firstConsumer.get();
	const json secondResult = secondConsumer.get();
	const bool firstHasPayload = firstResult.contains("payload");
	const bool secondHasPayload = secondResult.contains("payload");
	require(firstHasPayload != secondHasPayload,
		"The queued item must be delivered to exactly one consumer");
	const json& delivered = firstHasPayload ? firstResult : secondResult;
	require(delivered["payload"] == "single-item",
		"The consumer received the wrong queued item");
}

void testSessionStopUnblocksQueueWithoutStoppingService() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };
	std::atomic<bool> sessionStop{ false };
	std::promise<void> waitStarted;
	auto started = waitStarted.get_future();

	auto waiter = std::async(std::launch::async, [&] {
		waitStarted.set_value();
		return queue.waitAndPop(interruptionCode, "MessageQueueTest", &sessionStop);
	});
	StopSessionOnExit cleanup{ queue, sessionStop };
	started.wait();

	sessionStop.store(true, std::memory_order_release);
	queue.notify();
	require(waiter.wait_for(std::chrono::seconds(1)) == std::future_status::ready,
		"Session cancellation did not release the queue waiter");
	require(waiter.get().is_null(), "A cancelled queue wait should return an empty JSON value");
	require(interruptionCode.load(std::memory_order_acquire) == edll::Code::RUNNING,
		"Cancelling one client session must not stop the service");
}

void testSessionStopUnblocksPushAndWait() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };
	std::atomic<bool> sessionStop{ false };
	auto waiter = std::async(std::launch::async, [&] {
		return queue.pushAndWait(json::object(), "MessageQueueTest", interruptionCode, false, &sessionStop);
	});
	StopSessionOnExit cleanup{ queue, sessionStop };

	require(waitUntil([&] { return queue.size() == 1; }, std::chrono::seconds(2)),
		"pushAndWait did not enqueue its item");
	sessionStop.store(true, std::memory_order_release);
	queue.notify();
	require(waiter.wait_for(std::chrono::seconds(1)) == std::future_status::ready,
		"Session cancellation did not release pushAndWait");
	(void)waiter.get();
	require(interruptionCode.load(std::memory_order_acquire) == edll::Code::RUNNING,
		"Cancelling pushAndWait changed the service interruption code");
}

void testSessionStopUnblocksWaitAction() {
	MessageQueue queue;
	std::atomic<edll::INT_CODE> interruptionCode{ edll::Code::RUNNING };
	std::atomic<bool> sessionStop{ false };
	std::promise<void> waitStarted;
	auto started = waitStarted.get_future();

	auto waiter = std::async(std::launch::async, [&] {
		waitStarted.set_value();
		return queue.waitAction(interruptionCode, "MessageQueueTest", 5000, &sessionStop);
	});
	StopSessionOnExit cleanup{ queue, sessionStop };
	started.wait();

	sessionStop.store(true, std::memory_order_release);
	queue.notify();
	require(waiter.wait_for(std::chrono::seconds(1)) == std::future_status::ready,
		"Session cancellation did not release waitAction");
	require(!waiter.get(), "Session cancellation must not look like a queue change");
	require(interruptionCode.load(std::memory_order_acquire) == edll::Code::RUNNING,
		"Cancelling waitAction changed the service interruption code");
}

void testClientSessionOwnsIndependentStateAndQueues() {
	ClientSession firstSession;
	ClientSession secondSession;

	firstSession.request.push(json{ { "payload", "first-session" } }, "MessageQueueTest");
	firstSession.requestStop();

	require(firstSession.stopRequested.load(std::memory_order_acquire),
		"Requesting session shutdown did not set its stop state");
	require(secondSession.stopRequested.load(std::memory_order_acquire) == false,
		"Stopping one session changed another session's stop state");
	require(firstSession.request.size() == 1 && secondSession.request.empty(),
		"Client sessions do not own independent request queues");
}

void testWorkerCompletionGuardSignalsDuringException() {
	ClientSession session;
	try {
		SessionWorkerCompletionGuard completion(session);
		throw std::runtime_error("simulated worker failure");
	}
	catch (const std::runtime_error&) {
	}

	require(session.stopRequested.load(std::memory_order_acquire),
		"An exception did not request session cancellation");
	require(session.workerCompleted.load(std::memory_order_acquire),
		"An exception prevented worker completion from being signaled");
}

void testContinuousWorkerStopsAndJoins() {
	ClientSession session;
	std::atomic<bool> workerStarted{ false };
	std::atomic<bool> workerExited{ false };

	require(session.startMeasurementWorker([&] {
		workerStarted.store(true, std::memory_order_release);
		while (!session.measurementStopRequested.load(std::memory_order_acquire) &&
			!session.stopRequested.load(std::memory_order_acquire)) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
		workerExited.store(true, std::memory_order_release);
	}), "The continuous worker did not start");

	require(waitUntil([&] { return workerStarted.load(std::memory_order_acquire); },
		std::chrono::seconds(1)), "The continuous worker did not enter its run loop");
	require(!session.startMeasurementWorker([] {}),
		"A second continuous worker started while one was already active");
	session.requestMeasurementStop();
	session.joinMeasurementWorker();

	require(workerExited.load(std::memory_order_acquire),
		"The continuous worker did not exit after a stop request");
	require(!session.measurementWorkerRunning.load(std::memory_order_acquire),
		"The measurement controller remained marked active after join");
}

void testSendAllCompletesShortWrites() {
	const std::string payload = "complete framed response";
	std::string transmitted;
	int sendCalls = 0;
	size_t bytesSent = 0;
	int errorCode = 0;

	const bool sent = edll::detail::sendAllBytes(
		payload.data(), payload.size(),
		[&](const char* data, int length) {
			++sendCalls;
			const int shortWrite = length < 3 ? length : 3;
			transmitted.append(data, static_cast<size_t>(shortWrite));
			return shortWrite;
		},
		[] { return 0; }, bytesSent, errorCode);

	require(sent, "sendAllBytes did not complete after short writes");
	require(transmitted == payload, "sendAllBytes transmitted incorrect or incomplete bytes");
	require(bytesSent == payload.size(), "sendAllBytes reported an incorrect byte count");
	require(sendCalls > 1, "The short-write test did not exercise multiple send calls");
}

void testSendAllStopsAfterPartialWriteFailure() {
	const std::string payload = "response that cannot be completed";
	std::string transmitted;
	int sendCalls = 0;
	size_t bytesSent = 0;
	int errorCode = 0;

	const bool sent = edll::detail::sendAllBytes(
		payload.data(), payload.size(),
		[&](const char* data, int length) {
			++sendCalls;
			if (sendCalls == 1) {
				const int shortWrite = length < 4 ? length : 4;
				transmitted.append(data, static_cast<size_t>(shortWrite));
				return shortWrite;
			}
			return SOCKET_ERROR;
		},
		[] { return WSAECONNRESET; }, bytesSent, errorCode);

	require(!sent, "sendAllBytes reported success after a socket error");
	require(bytesSent == transmitted.size() && bytesSent == 4,
		"sendAllBytes reported an incorrect partial byte count");
	require(errorCode == WSAECONNRESET, "sendAllBytes did not preserve the socket error code");
	require(sendCalls == 2, "sendAllBytes continued sending after a hard socket error");
}

void testDefaultServiceConfigOmitsLegacyBufferTTL() {
	const json config = buildCoreDefaultConfigJson();
	require(!config[edll::DefaultConfig::Service::KEY].contains("bufferTTLMsgCount"),
		"Generated default configuration still contains bufferTTLMsgCount");
}

void testServiceConfigAcceptsLegacyBufferTTL() {
	json config = buildCoreDefaultConfigJson();
	config[edll::DefaultConfig::Service::KEY]["bufferTTLMsgCount"] = 5;
	require(validServiceParams(config),
		"A legacy bufferTTLMsgCount field should be ignored, not invalidate configuration");
}

void testServiceConfigDoesNotRequireBufferTTL() {
	const json config = buildCoreDefaultConfigJson();
	require(validServiceParams(config),
		"Service configuration should remain valid without bufferTTLMsgCount");
}

} // namespace

int main() {
	auto sink = std::make_shared<spdlog::sinks::null_sink_mt>();
	spdlog::logger testLogger("MessageQueueTest", sink);
	loggerPtr = &testLogger;

	try {
		testPushAndWaitTracksItsOwnMessage();
		testPushAndWaitUnblocksOnInterruption();
		testWaitActionWakesWhenQueueChanges();
		testWaitActionReturnsFalseOnTimeout();
		testWaitActionUnblocksOnInterruption();
		testWaitAndPopHasSingleConsumerForEachItem();
		testSessionStopUnblocksQueueWithoutStoppingService();
		testSessionStopUnblocksPushAndWait();
		testSessionStopUnblocksWaitAction();
		testClientSessionOwnsIndependentStateAndQueues();
		testWorkerCompletionGuardSignalsDuringException();
		testContinuousWorkerStopsAndJoins();
		testSendAllCompletesShortWrites();
		testSendAllStopsAfterPartialWriteFailure();
		testDefaultServiceConfigOmitsLegacyBufferTTL();
		testServiceConfigAcceptsLegacyBufferTTL();
		testServiceConfigDoesNotRequireBufferTTL();
		std::cout << "MessageQueue tests passed.\n";
		return 0;
	}
	catch (const std::exception& exception) {
		std::cerr << "MessageQueue test failed: " << exception.what() << '\n';
		return 1;
	}
}
