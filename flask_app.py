
from flask import Flask,render_template
from flask import request

ch = "0.0$$0.0$$0.0"

app = Flask(__name__)

@app.route('/10200314038_XYZ_ESP-TEMP', methods = ['GET','POST'])
def temp():
        global ch
        if(request.method == 'GET'):
            return ch
        else:
            ch = request.form['Chat']
            return ch

@app.route('/')
def disp():
    global ch
    return render_template('project.html',deg=ch)


if __name__=="__main__":
    app.run()
