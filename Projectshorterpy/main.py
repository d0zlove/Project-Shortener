import ctypes

# Загрузка библиотеки C++
dll = ctypes.cdll.LoadLibrary("./ProjectShorter.dll")

def url(arg, longURl):
    # Определение типа возвращаемого значения для функции библиотеки C++ (const char*)
    dll.shorten_url.argtypes = ctypes.c_int,ctypes.c_wchar_p
    dll.shorten_url.restype = ctypes.c_char_p


    # Вызов функции из библиотеки C++ и получение результата в Python
    short_url_bytes = dll.shorten_url(arg,longURl)
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
    processed_value = url(1,user_value) 
    return render_template('index.html', processed_value=processed_value,user_value=user_value)

if __name__ == '__main__':
    app.run()
