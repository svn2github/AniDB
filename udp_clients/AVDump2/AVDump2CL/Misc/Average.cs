using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace AVDump2CL.Misc {
	public class Average {
		private Queue<double> values;

		public Average() { values = new Queue<double>(); }
		public void Add(double value) { values.Enqueue(value); if(values.Count > 10) values.Dequeue(); }

		public double Calc() { return values.Average(); }
		public double Calc(int restriction) {
			//return values.Take(restriction).Max();

			//int weight = 1, weightSum = 0;
			//return (from value in values orderby value ascending select value).Sum((v) => {
			//    weight = weight << 1;
			//    weightSum += weight;
			//    return v * weight;
			//}) / weightSum;

			return values.Take(restriction).Average();
		}
	}
}
