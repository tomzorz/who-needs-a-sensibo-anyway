using System;
using System.Collections.Generic;
using System.Text;

namespace PulseWrangler
{
    public static class Parser
    {
        public static bool Around(this int value, int target, int epsilon)
        {
            return Math.Abs(value - target) < epsilon;
        }

        public static string ToParsed(this List<Signal> signals, bool asCsv = false)
        {
            var sb = new StringBuilder();

            var zero = asCsv ? "0;" : "0";
            var one = asCsv ? "1;" : "1";
            var start = asCsv ? "S;" : "S";
            var connection = asCsv ? "X;" : "X";
            var end = asCsv ? "E;" : "E";

            for (var i = 0; i < signals.Count; i += 2)
            {
                var s1 = signals[i];
                var s2 = signals[i+1];

                if (s1.SignalLength.Around(608, 150))
                {
                    if (s2.SignalLength.Around(608, 200)) sb.Append(zero);
                    else if (s2.SignalLength.Around(1650, 150)) sb.Append(one);
                    else if (s2.SignalLength.Around(20138, 12500)) sb.Append(connection);
                    else throw new Exception("Invalid code pair!");
                }
                else
                {
                    if (s1.SignalLength.Around(8998, 1500) && s2.SignalLength.Around(4472, 1000)) sb.Append(start);
                    else if (s1.SignalLength.Around(130625, 10000)) sb.Append(end); // no check for end, it's all over the place
                    else throw new Exception("Invalid code pair!");
                }
            }

            return sb.ToString();
        }
    }
}
