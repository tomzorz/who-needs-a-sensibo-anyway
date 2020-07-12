using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace PulseWrangler
{
    class Program
    {
        static void Main(string[] args)
        {
            //var dataPath = @"..\..\..\..\..\..\data\ir_recording_cleanup.txt";
            var dataPath = @"..\..\..\..\..\..\data\ir_recording_onoff.txt";

            using var fileStream = File.OpenRead(dataPath);
            using var textStream = new StreamReader(fileStream);

            var commands = new List<(string command, List<Signal> signal)>();

            while (!textStream.EndOfStream)
            {
                var command = textStream.ReadLine();
                var signals = textStream.ReadLine();

                // ReSharper disable once PossibleNullReferenceException
                commands.Add((command, signals.Split(',').Select(x =>
                {
                    var val = int.Parse(x);
                    return new Signal
                    {
                        SignalLength = Math.Abs(val),
                        SignalType = val < 0 ? SignalType.Pause : SignalType.Signal
                    };
                }).ToList()));

                if(textStream.EndOfStream) break;

                var el = textStream.ReadLine();

                if(string.IsNullOrWhiteSpace(el)) continue;

                throw new Exception("invalid line");
            }

            foreach (var samples in commands)
            {
                // Console.WriteLine($"{samples.command} {samples.signal.Count}");

                // if(samples.signal.Count == 142) Console.WriteLine(string.Join(' ', samples.signal.Select(x => x.SignalLength).ToList()));

                //Console.WriteLine($"{samples.command};{string.Join(';', samples.signal.Select(x => x.ToString()).ToList())}");

                Console.WriteLine($"{samples.command};{samples.signal.ToParsed(true)}");
            }

            Console.ReadLine();
        }
    }
}
