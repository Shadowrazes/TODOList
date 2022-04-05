#include <iostream>
#include <string>
#include <locale>

using namespace std;
using namespace sf;

namespace form {
	Font font;				// Шрифт первичная инициализация

	class Label {
	public:
		wstring text;	//Текст
	protected:
		double x;		//Координата x
		double y;		//Координата y

		Text TXT;		//Отображаемый текст

	public:
		Label(double _x = 0, double _y = 0, wstring _text = L"Text")
		{
			font.loadFromFile("times-new-roman.ttf");			//передаем нашему шрифту файл шрифта

			text = _text;
			x = _x;
			y = _y;
			TXT.setFont(font);							// загружаем фрифт
			TXT.setCharacterSize(20); 					// в пикселях, а не точках!
			TXT.setFillColor(Color::White);					// устанавливаем цвет выводимого текста
			TXT.setPosition(x, y);						// позиция текста
			//wcout << text;
			//cout << "|" << endl;
		}
		Text displayText()
		{
			TXT.setString(String::fromUtf8(text.begin(), text.end()));						// загружаем в выводимый текст загружаемый текст
			return TXT;
		}
	};

	class Button : public Label {
	protected:
		double width;		// Ширина Кнопки
		double height;		// Высота Кнопки
		RectangleShape BOX;	// Создаем прямоугольник с размером width, height
	public:
		Button(double _x = 0, double _y = 0, double _width = 150, double _height = 30, wstring _text = L"Button")
		{
			font.loadFromFile("times-new-roman.ttf");			//передаем нашему шрифту файл шрифта

			x = _x;
			y = _y;
			width = _width;
			height = _height;
			text = _text;
			TXT.setFont(font);
			TXT.setCharacterSize(20);
			TXT.setFillColor(Color::White);
			TXT.setPosition(x, y);

			BOX.setSize(Vector2f(width, height));		// размер кнопки
			BOX.setPosition(x, y);						// координаты кнопки
			BOX.setFillColor(sf::Color(133, 133, 133));	// цвет кнопки
			BOX.setOutlineThickness(2);					// Контур X пикселей
			BOX.setOutlineColor(Color(66, 66, 66));		// Цвет контура
		}
		//Проверка нажатия на кнопку
		bool select(Vector2i _mouse)
		{
			if ((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)) {
				return  true;
			}
			else {
				return false;
			}
		}
		//Вывод Текста
		Text displayText()
		{
			TXT.setString(text);  // загружаем в выводимый текст загружаемый текст
			return TXT;
		}
		//Вывод кнопки
		RectangleShape displayButton()
		{
			return BOX;
		}


	};

	class Input : public Button {
	protected:
		bool focus;
		int wrapCounter;
		wstring defaultStroke;
		bool BOXisEmpty = true;
		//const wstring VoidHead = L"Название";
		//const wstring VoidDescription = L"Описание";
	public:
		Input(double _x = 0, double _y = 0, double _width = 150, double _height = 30, wstring _defaultStroke = L"", wstring _text = L"")
		{
			x = _x;
			y = _y;
			width = _width;
			height = _height;
			defaultStroke = _defaultStroke;
			text = _text;								// загружает текст
			if (text != L"")
			{
				BOXisEmpty = false;
				TXT.setFillColor(Color::White);
			}
			else
			{
				text = defaultStroke;
				TXT.setFillColor(Color(255, 255, 255, 128));
			}
			TXT.setString(text);
			focus = false;

			TXT.setFont(font);
			if (defaultStroke == L"Название")
				TXT.setCharacterSize(35);
			else
				TXT.setCharacterSize(25);
			TXT.setPosition(x, y);

			BOX.setSize(Vector2f(width, height));		// размер поля ввода
			BOX.setPosition(x, y);						// координаты поля ввода
			BOX.setFillColor(Color(255, 255, 255, 0));	// цвет кнопки
			BOX.setOutlineThickness(0);
			wrapCounter = 0;
		}
		bool& displayBOXEmpty()
		{
			return BOXisEmpty;
		}

		wstring& displayDefaultStroke()
		{
			return defaultStroke;
		}

		int& wrapQuantity()
		{
			return wrapCounter;
		}

		//метод .reText( char ) изменяет текст внутри класса Типа данных Input
		void reText(char _tmp)
		{
			text.erase(text.size() - 1);

			if (_tmp != 8)
			{
				text += _tmp;
				BOXisEmpty = false;
			}
			else
			{
				if (text.size() > 0)
				{
					wstring subs;
					if (text.size() > 2)
						subs = text.substr(text.size() - 2);
					if (subs == "-\n")
					{
						text.erase(text.size() - 2);
						wrapCounter -= 1;
					}
					else
					{
						if (text[text.size() - 1] == '\n')
						{
							text.erase(text.size() - 1);
							wrapCounter -= 1;
						}
						text.erase(text.size() - 1);
					}
				}

			}
			text += L"|";
			if (text == L"|")
			{
				BOXisEmpty = true;
			}
			TXT.setString(text);
		}

		// Проверка Фокуса Input
		bool select()
		{
			return focus;
		}

		// Установка Фокуса Input
		bool select(Vector2i _mouse)
		{
			if (BOX.getGlobalBounds().contains(_mouse.x, _mouse.y)) { 	//Если нажат клаиша над объектом Input...
				if (!focus)
					text += L"|";																		// В конец строки добаляем | (что бы понимать что input в фокусе)
				focus = true;																	   	// Фокус true
				if (BOXisEmpty)
				{
					text = L"|";
					TXT.setString(text);
					TXT.setFillColor(Color::White);
				}
			}
			else {																				//...Иначе если нажатие произошло не над объектом, то...
				if (text.size() > 0) {																// проверка последнего символа(иначе вылетает)
					if (text[text.size() - 1] == 124) {													// если символ | ...
						text.erase(text.size() - 1);														// ... то удаляем его
					}
				}
				if (BOXisEmpty)
				{
					text = defaultStroke;
					TXT.setString(text);
					TXT.setFillColor(Color(255, 255, 255, 128));
				}
				focus = false;																		// устанавливаем фокус false
			}
			return focus;
		}

		//Вывод Текста
		Text& displayText()
		{
			//if (!BOXisEmpty)
			//	TXT.setString(text);   // загружаем в выводимый текст загружаемый текст
			//else
			//	TXT.setString(defaultStroke);
			TXT.setString(text);
			return TXT;
		}

		//Вывод фона Input
		RectangleShape& displayBOX()
		{
			return BOX;
		}

		//Для того, что бы считать текст с поля Ipnut необходимо пременить  readText() который возвращает string
		wstring& readText()
		{
			//if (text.size() != 0) {
			//	if (text[text.size() - 1] == 124) { 	// удаляем (если есть) символ |
			//		text.erase(text.size() - 1);
			//	}
			//	return text;
			//}
			return text;
		}
	};
}

