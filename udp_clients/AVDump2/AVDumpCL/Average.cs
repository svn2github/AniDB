using System;
using System.Collections.Generic;
using System.Text;

namespace AVDump2CL {
    class Average {
        List<double> values;

        public Average() { values = new List<double>(); }
        public void Add(double value) { values.Add(value); }

        public double Calc() {
            double sum = 0; int i;
            for(i = 0;i < values.Count;i++) sum += values[i];
            return sum / i;
        }

        public double Calc(int restriction) {
            double sum = 0; int i;
            int start = values.Count - restriction;
            start = start < 0 ? 0 : start;

            for(i = start;i < values.Count;i++) sum += values[i];
            return sum / (i - start);
        }
    }
}
