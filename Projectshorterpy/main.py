import ctypes

# Загрузка библиотеки C++
dll = ctypes.cdll.LoadLibrary("./ProjectShorter.dll")

def url(longURl):
    # Определение типа возвращаемого значения для функции библиотеки C++ (const char*)
    dll.shorten_url.argtypes = [ctypes.c_char_p]
    dll.shorten_url.restype = ctypes.c_char_p


    # Вызов функции из библиотеки C++ и получение результата в Python
    short_url_bytes = dll.shorten_url(longURl.encode())
    short_url_str = short_url_bytes.decode("utf-8")

    return short_url_str

from flask import Flask, render_template, request

app = Flask(__name__, template_folder='front')

#создание и отображение основной страницы с вводом ссылки
@app.route('/')
def index():
    return render_template('index.html')

#создание и отображение страницы с результатом сокращения
@app.route('/result', methods=['POST'])
def result():
    user_value = request.form['value']
    if len(user_value) > 1:
        processed_value = url(user_value) 
    else:
        return render_template('index.html', processed_value = "Вы не указали ссылку")
    
    return render_template('index.html', processed_value=processed_value,user_value=user_value)

from flask import Flask, redirect
@app.route('/<short_id>', methods=['GET'])
def redirect_url(short_id):
    dll.getDataFromDatabase.argtypes = [ctypes.c_char_p]
    dll.getDataFromDatabase.restype = ctypes.c_char_p
    
    # Вызов функции из библиотеки C++ и получение результата в Python
    short_url_bytes = dll.getDataFromDatabase(("http://127.0.0.1:5000/" + short_id).encode())
    short_url_str = short_url_bytes.decode("utf-8")
    
    return redirect(short_url_str, code=302)

if __name__ == '__main__':
    app.run()
