using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace AVDump2CL {
	public class ActivityTraceContext : IDisposable {
		private TraceSource ts;
		private string operationName;
		private Guid parentActivityId;

		public ActivityTraceContext(TraceSource ts, string operationName) {
			this.operationName = operationName;
			this.ts = ts;

			Trace.CorrelationManager.StartLogicalOperation(operationName);

			parentActivityId = Trace.CorrelationManager.ActivityId;
			Guid activityId = Guid.NewGuid();

			ts.TraceTransfer(0, "Transfering to " + operationName, activityId);
			Trace.CorrelationManager.ActivityId = activityId;
			ts.TraceEvent(TraceEventType.Start, 0, operationName + " started");
		}

		public void Dispose() {
			ts.TraceTransfer(0, "Transfering from " + operationName, parentActivityId);
			ts.TraceEvent(TraceEventType.Stop, 0, operationName + " stopped");
			Trace.CorrelationManager.ActivityId = parentActivityId;

			Trace.CorrelationManager.StopLogicalOperation();
		}

	}
}
