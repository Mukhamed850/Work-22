# Практическая работа 2.2

## Знакомство с библиотекой Qt

Разработано графическое приложение для взаимодействия с программой openssl. 
Окно приложения содержит кнопку выбора файла ключа, под которой в ряд 
расположены два выпадающих списка выбора алгоритма шифрования и алгоритма 
хеширования. Ниже, друг под другом, располагаются поля для ввода значений 
запроса сертификата и кнопка. Под кнопкой располагается текстовое поле. 
При нажатии на кнопку происходит создание запроса сертификата, который 
отображается в текстовом поле.

### Примечания
В качестве основного класса окна приложения использован QWidget. 
Для размещения элементов окна задействованы QHBoxLayout и QVBoxLayout.