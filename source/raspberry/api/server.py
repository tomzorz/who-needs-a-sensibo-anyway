import subprocess
from flask import Flask
from flask import request

app = Flask(__name__)

# 1: on/off | 2: mode | 3: temp | 4: fan | 5: swing

@app.route('/')
def hello():
    return 'tmzdev.LookingGlass'

@app.route('/control')
def control():
    power = request.args.get('power', default = 'off', type = str)
    mode = request.args.get('mode', default = 'cool', type = str)
    temp = request.args.get('temp', default = '26', type = int)
    fan = request.args.get('fan', default = '2', type = int)
    swing = request.args.get('swing', default = 'off', type = str)

    subprocess.call(['sudo', '/home/pi/ir/pulsegen', power, mode, str(temp), str(fan), swing])

    return power + ' ' + mode + ' ' + str(temp) + ' ' + str(fan) + ' ' + swing

if __name__ == '__main__':
   app.run(host="0.0.0.0", port=32223)