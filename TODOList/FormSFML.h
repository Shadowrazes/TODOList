#include <iostream>
#include <string>
#include <locale>

using namespace std;
using namespace sf;

namespace form {
	Font font;				// ����� ��������� �������������

	class Label {
	public:
		wstring text;	//�����
	protected:
		double x;		//���������� x
		double y;		//���������� y

		Text TXT;		//������������ �����

	public:
		Label(double _x = 0, double _y = 0, wstring _text = L"Text")
		{
			font.loadFromFile("times-new-roman.ttf");			//�������� ������ ������ ���� ������

			text = _text;
			x = _x;
			y = _y;
			TXT.setFont(font);							// ��������� �����
			TXT.setCharacterSize(20); 					// � ��������, � �� ������!
			TXT.setFillColor(Color::White);					// ������������� ���� ���������� ������
			TXT.setPosition(x, y);						// ������� ������
			//wcout << text;
			//cout << "|" << endl;
		}
		Text displayText()
		{
			TXT.setString(String::fromUtf8(text.begin(), text.end()));						// ��������� � ��������� ����� ����������� �����
			return TXT;
		}
	};

	class Button : public Label {
	protected:
		double width;		// ������ ������
		double height;		// ������ ������
		RectangleShape BOX;	// ������� ������������� � �������� width, height
	public:
		Button(double _x = 0, double _y = 0, double _width = 150, double _height = 30, wstring _text = L"Button")
		{
			font.loadFromFile("times-new-roman.ttf");			//�������� ������ ������ ���� ������

			x = _x;
			y = _y;
			width = _width;
			height = _height;
			text = _text;
			TXT.setFont(font);
			TXT.setCharacterSize(20);
			TXT.setFillColor(Color::White);
			TXT.setPosition(x, y);

			BOX.setSize(Vector2f(width, height));		// ������ ������
			BOX.setPosition(x, y);						// ���������� ������
			BOX.setFillColor(sf::Color(133, 133, 133));	// ���� ������
			BOX.setOutlineThickness(2);					// ������ X ��������
			BOX.setOutlineColor(Color(66, 66, 66));		// ���� �������
		}
		//�������� ������� �� ������
		bool select(Vector2i _mouse)
		{
			if ((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)) {
				return  true;
			}
			else {
				return false;
			}
		}
		//����� ������
		Text displayText()
		{
			TXT.setString(text);  // ��������� � ��������� ����� ����������� �����
			return TXT;
		}
		//����� ������
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
		//const wstring VoidHead = L"��������";
		//const wstring VoidDescription = L"��������";
	public:
		Input(double _x = 0, double _y = 0, double _width = 150, double _height = 30, wstring _defaultStroke = L"", wstring _text = L"")
		{
			x = _x;
			y = _y;
			width = _width;
			height = _height;
			defaultStroke = _defaultStroke;
			text = _text;								// ��������� �����
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
			if (defaultStroke == L"��������")
				TXT.setCharacterSize(35);
			else
				TXT.setCharacterSize(25);
			TXT.setPosition(x, y);

			BOX.setSize(Vector2f(width, height));		// ������ ���� �����
			BOX.setPosition(x, y);						// ���������� ���� �����
			BOX.setFillColor(Color(255, 255, 255, 0));	// ���� ������
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

		//����� .reText( char ) �������� ����� ������ ������ ���� ������ Input
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

		// �������� ������ Input
		bool select()
		{
			return focus;
		}

		// ��������� ������ Input
		bool select(Vector2i _mouse)
		{
			if (BOX.getGlobalBounds().contains(_mouse.x, _mouse.y)) { 	//���� ����� ������ ��� �������� Input...
				if (!focus)
					text += L"|";																		// � ����� ������ �������� | (��� �� �������� ��� input � ������)
				focus = true;																	   	// ����� true
				if (BOXisEmpty)
				{
					text = L"|";
					TXT.setString(text);
					TXT.setFillColor(Color::White);
				}
			}
			else {																				//...����� ���� ������� ��������� �� ��� ��������, ��...
				if (text.size() > 0) {																// �������� ���������� �������(����� ��������)
					if (text[text.size() - 1] == 124) {													// ���� ������ | ...
						text.erase(text.size() - 1);														// ... �� ������� ���
					}
				}
				if (BOXisEmpty)
				{
					text = defaultStroke;
					TXT.setString(text);
					TXT.setFillColor(Color(255, 255, 255, 128));
				}
				focus = false;																		// ������������� ����� false
			}
			return focus;
		}

		//����� ������
		Text& displayText()
		{
			//if (!BOXisEmpty)
			//	TXT.setString(text);   // ��������� � ��������� ����� ����������� �����
			//else
			//	TXT.setString(defaultStroke);
			TXT.setString(text);
			return TXT;
		}

		//����� ���� Input
		RectangleShape& displayBOX()
		{
			return BOX;
		}

		//��� ����, ��� �� ������� ����� � ���� Ipnut ���������� ���������  readText() ������� ���������� string
		wstring& readText()
		{
			//if (text.size() != 0) {
			//	if (text[text.size() - 1] == 124) { 	// ������� (���� ����) ������ |
			//		text.erase(text.size() - 1);
			//	}
			//	return text;
			//}
			return text;
		}
	};
}

