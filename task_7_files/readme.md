Model Completion Comparison

Example 1
Reference: { _carService = carService; }

v1.0 Prediction: }
Jaccard: 0.0
ED: 0.04
Syntax correct rate: 0.0

v2.0 Prediction: { *carService = carService; }
Jaccard: 1.0
ED: 1.0
Syntax correct rate: 1.0


Example 2
Reference: try { super.onComplete(); } finally { MDC.clear(); }

v1.0 Prediction: super.onComplete(); MDC.clear();
Jaccard: 0.667
ED: 0.566
Syntax correct rate: 1.0

v2.0 Prediction: try { super.onComplete(); } finally { MDC.clear(); }
Jaccard: 1.0
ED: 1.0
Syntax correct rate: 1.0


Example 3
Reference: { SendAsyncCalled = true; return Task.FromResult(new HttpResponseMessage { StatusCode = _statusCode }); }

v1.0 Prediction: {
Jaccard: 0.0
ED: 0.016
Syntax correct rate: 0.0

v2.0 Prediction: { SendAsyncCalled = true; return Task.FromResult(new HttpResponseMessage(_statusCode)); }
Jaccard: 0.889
ED: 0.915
Syntax correct rate: 1.0
