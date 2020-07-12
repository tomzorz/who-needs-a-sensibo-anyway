using System;
using System.Collections.Generic;
using System.Text;

namespace PulseWrangler
{
    public class Signal
    {
        public SignalType SignalType { get; set; }

        public int SignalLength { get; set; }

        public override string ToString() => (SignalType == SignalType.Signal ? SignalLength : SignalLength * -1).ToString();
    }
}
