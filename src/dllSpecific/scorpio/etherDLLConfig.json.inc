R"({
  "log": {
    "name": "log",
    "console": {
      "level": "trace"
    },
    "file": {
      "level": "trace",
      "path": "log.txt"
    }
  },
  "station": {
    "address": "172.24.3.15",
    "port": 3303,
    "timeout_s": 10
  },
  "service": {
      "port": 30000,
      "timeout_s": 10000,
      "sleep_ms": 100,
      "buffer_ttl_period": 5,
      "ping_period": 10,
      "ping_enable": true,
      "msg_keys" : {
          "ping" : "PING",
          "ack" : "ACK",
          "nack" : "NACK",
          "end" : "\\r\\n"
          "id" : "ID""
      }
  },
  "default": {
    "OCCRequest": {
      "durationMethod": "Fixed",
      "thresholdMethod": "Noise Riding",
      "storageTime": 1,
      "measurementTime": 1,
      "confidenceLevel": 90,
      "desiredAccuracy": 90,
      "occPrimaryThreshold": 10,
      "occupancyMinGap": 1,
      "occflags": {
        "occupancyVsChannel": true,
        "eFieldVsChannel": false,
        "occupancyVsTimeOfDay": false,
        "msglengthVsChannel": false,
        "msglengthDistribution": false,
        "spectrogram": false,
        "timegram": false
      },
      "ant": 1
    }
  }
})"