import ctypes

def url():
    # Загрузка библиотеки C++
    my_lib = ctypes.cdll.LoadLibrary("C:/Users/Админ/Desktop/ProjectShorter/x64/Debug/ProjectShorter.dll")

    # Определение типа возвращаемого значения для функции библиотеки C++
    my_lib.shorten_url.restype = ctypes.c_char_p

    # Вызов функции из библиотеки C++ и получение результата в Python
    popa = my_lib.shorten_url()
    popa_str = popa.decode("utf-8")

    return popa_str

from flask import Flask, render_template, request

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/result', methods=['POST'])
def result():
    processed_value = url() # пример обработки значения
    return render_template('index.html', processed_value=processed_value)

if __name__ == '__main__':
    app.run()
