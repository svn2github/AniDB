// Copyright (C) 2009 DvdKhl 
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.using System;

using System;
using System.Collections.Generic;
using System.Text;
using System.Linq;

namespace AVDump2CL {
	public class Average {
		private Stack<double> values;

		public Average() { values = new Stack<double>(); }
		public void Add(double value) { values.Push(value); }

		public double Calc() { return values.Average(); }
		public double Calc(int restriction) { return values.Take(restriction).Average(); }
	}
}
