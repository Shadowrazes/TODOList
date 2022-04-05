#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>
#include "FormSFML.h"
#include "configTask.h"
#include "input.h"

using namespace sf;
using namespace std;
using namespace form;

vector<VideoMode> getWindowSize()
{
	vector<VideoMode> v = VideoMode::getFullscreenModes();
	return v;
}


const int radius = 12;
const wstring headStr = L"Список задач";    //+ to_string(a);
const wstring addStr = L"Добавить задачу";
const wstring askStr = L"Удалить выбранную задачу?";
const wstring yesStr = L"Да";
const wstring cancelStr = L"Отмена";
const wstring defaultHead = L"Название";
const wstring defaultDescription = L"Описание";
const Vector2f centerPos(getWindowSize()[0].width / 2, getWindowSize()[0].height / 2);
const int fieldWidth = getWindowSize()[0].width * 0.85;
const int fieldHeight = 40;

//Sprite backg()
//{
//	Texture bg;
//	bg.loadFromFile("bg.jpg");
//	Sprite background(bg);
//	return background;
//}
//
//const Sprite background = backg();
//const RenderWindow& window = initWindow();

//bool confirmation = false;
//Text  askText, yesText, cancelText;
//customizationOfText(askText, font, 20, askStr, Vector2f(0, 0), Color::White);
//customizationOfText(yesText, font, 20, yesStr, Vector2f(0, 0), Color::White);
//customizationOfText(cancelText, font, 20, cancelStr, Vector2f(0, 0), Color::White);
//askWindow(window, askText, yesText, cancelText, confirmation);
//if (confirmation)
//{
//	box.erase(box.begin() + index);
//	recalculatingPosOfField(box, headLineIndentY);
//	box[0].rectOfFieldPos = Vector2f(centerPos.x - box[0].rectOfField.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size() - 1));
//}

//RenderWindow& initWindow()
//{
//	ContextSettings settings;
//	settings.antialiasingLevel = 8;
//	vector<VideoMode> v = VideoMode::getFullscreenModes();
	/*for (int b = 0; b < v.size(); b++)
		cout << v[b].width << "x" << v[b].height << " " << v[b].bitsPerPixel << endl;*/
		//	RenderWindow window(VideoMode(v[0]), "ToDoList", Style::Fullscreen, settings);
		//	return window;
		//}


struct field
{
	RectangleShape rectOfField;
	Vector2f rectOfFieldPos;
	RectangleShape rectOfDelete;
	CircleShape circle;
	Input descriptionHead;
	Input description;

	bool circlePressed = false;
};

void signIn(RenderWindow& window, Sprite& background)
{
	const wstring head = L"Войдите в аккаунт";
	const wstring log = L"Логин";
	const wstring pas = L"Пароль";
	RectangleShape signInWindow(Vector2f(window.getSize().x * 0.4, window.getSize().y * 0.3));
	signInWindow.setPosition(centerPos.x - signInWindow.getGlobalBounds().width / 2, centerPos.y - signInWindow.getGlobalBounds().height / 2);
	signInWindow.setOutlineThickness(5);
	signInWindow.setFillColor(Color(0, 0, 0, 0));
	signInWindow.setOutlineColor(Color::White);

	Text entry;
	Font font;
	font.loadFromFile("times-new-roman.ttf");
	entry.setFont(font);
	entry.setString(head);
	entry.setCharacterSize(30);
	entry.setPosition(centerPos.x - entry.getGlobalBounds().width / 2, signInWindow.getPosition().y + 20);

	double dataFieldWidth = window.getSize().x * 0.3, dataFielHeight = window.getSize().y * 0.05;
	Input login = Input(centerPos.x - dataFieldWidth / 2, entry.getPosition().y + entry.getGlobalBounds().height + 30, dataFieldWidth, dataFielHeight, log);
	Input password = Input(centerPos.x - dataFieldWidth / 2, login.displayBOX().getPosition().y + login.displayBOX().getGlobalBounds().height + 40, dataFieldWidth, dataFielHeight, pas);

	login.displayBOX().setOutlineThickness(5);
	login.displayBOX().setOutlineColor(Color::White);
	password.displayBOX().setOutlineThickness(5);
	password.displayBOX().setOutlineColor(Color::White);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				login.select(mousePos);
				password.select(mousePos);
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode == 13)
					return;
				if (login.select())
				{
					if (login.displayText().getGlobalBounds().width < login.displayBOX().getSize().x || event.text.unicode == 8)
						login.reText(event.text.unicode);
				}
				if (password.select())
				{
					if (password.displayText().getGlobalBounds().width < password.displayBOX().getSize().x || event.text.unicode == 8)
						password.reText(event.text.unicode);
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape) && Keyboard::isKeyPressed(Keyboard::LShift))
			{
				window.close();
			}
		}
		window.draw(background);
		window.draw(signInWindow);
		window.draw(entry);
		window.draw(login.displayBOX());
		window.draw(login.displayText());
		window.draw(password.displayBOX());
		window.draw(password.displayText());
		window.display();
		sleep(milliseconds(1000 / 60));
	}
}

void fieldFocus(RenderWindow& window, Vector2f& rectOfFieldPos)  // отрисовка выделяющих линий при наведении
{
	RectangleShape line(Vector2f(fieldWidth, 2));
	line.setFillColor(Color::White);
	line.setPosition(rectOfFieldPos.x, rectOfFieldPos.y - 2);
	window.draw(line);
	line.setPosition(rectOfFieldPos.x, rectOfFieldPos.y + 40);
	window.draw(line);
}

void checkChange(RenderWindow& window, field& boxElem) // показать/скрыть галочку
{
	if (boxElem.circlePressed == true) boxElem.circle.setFillColor(Color::White);
	else boxElem.circle.setFillColor(Color::Black);
}

void addButtonFieldElements(RenderWindow& window, field& boxElemForAdd, Text& addText)
{
	boxElemForAdd.rectOfDelete.setRotation(90);
	boxElemForAdd.rectOfDelete.setPosition(boxElemForAdd.rectOfFieldPos.x + 22, boxElemForAdd.rectOfFieldPos.y + 7);
	window.draw(boxElemForAdd.rectOfDelete);

	boxElemForAdd.rectOfDelete.setRotation(180);
	boxElemForAdd.rectOfDelete.setPosition(boxElemForAdd.rectOfFieldPos.x + 33.5, boxElemForAdd.rectOfFieldPos.y + 22);
	window.draw(boxElemForAdd.rectOfDelete);

	addText.setPosition(boxElemForAdd.rectOfFieldPos.x + 50, boxElemForAdd.rectOfFieldPos.y);
	window.draw(addText);
}

void addingTask(RenderWindow& window, vector <field>& box)
{
	for (int i = 0; i < box.size(); i++)
	{
		Text fieldName;
		box[i].rectOfField.setPosition(box[i].rectOfFieldPos.x, box[i].rectOfFieldPos.y);
		if (i > 0)
		{
			box[i].circle.setOutlineThickness(3);
			box[i].circle.setOutlineColor(Color::Red);
			box[i].circle.setPosition(box[i].rectOfFieldPos.x + 8, box[i].rectOfFieldPos.y + 8);
			box[i].rectOfDelete.setPosition(box[i].rectOfField.getGlobalBounds().width, box[i].rectOfFieldPos.y + 3);
			if (box[i].descriptionHead.readText() != box[i].descriptionHead.displayDefaultStroke())
			{
				fieldName = box[i].descriptionHead.displayText();
				fieldName.setPosition(box[i].circle.getPosition().x + box[i].circle.getGlobalBounds().width + 15, box[i].circle.getPosition().y - 12.5);
			}
		}
		window.draw(box[i].rectOfField);
		window.draw(box[i].circle);
		window.draw(fieldName);
		window.draw(box[i].rectOfDelete);
	}
}

void insertTextWrap(Input& inputText, int textWidthBorder)
{
	int i = inputText.readText().size() - 1; // '|'
	char y = inputText.readText()[i];
	if ((inputText.displayText().findCharacterPos(i).x + inputText.displayText().getCharacterSize() / 2) > textWidthBorder)
	{
		int voidPos = inputText.readText().rfind(L" ");
		int closerWrapPos = inputText.readText().rfind(L"\n");
		if (voidPos != -1 && voidPos > closerWrapPos)     //если ближайший пробел существует и, если смотреть справа, он стоит раньше переноса
		{
			inputText.readText().insert(voidPos + 1, L"\n");
			inputText.displayText().setString(inputText.readText());
			inputText.wrapQuantity() += 1;
			return;
		}
		else
		{
			inputText.readText().insert(i - 1, L"-\n-");
			inputText.displayText().setString(inputText.readText());
			inputText.wrapQuantity() += 1;
			return;
		}
	}
}

//void deleteTask(RenderWindow& window, float& centerPosX, int& headLineIndentY, int& fieldHeight)
//{
//	wstring askStr = L"Удалить выбранную задачу?";
//	wstring yesStr = L"Да";
//	wstring cancelStr = L"Отмена";
//
//	Text  askText, yesText, cancelText;
//	customizationOfText(askText, font, 20, askStr, Vector2f(0, 0), Color::White);
//	customizationOfText(yesText, font, 20, yesStr, Vector2f(0, 0), Color::White);
//	customizationOfText(cancelText, font, 20, cancelStr, Vector2f(0, 0), Color::White);
//}

void outputDescription(RenderWindow& window, Sprite& background, RectangleShape& backButton, field& boxElem, task& task)
{
	/*boxElem.descriptionHead.displayText().setCharacterSize(35);
	boxElem.description.displayText().setCharacterSize(25);*/
	float a = window.getSize().x * 0.85, b = window.getSize().x * 0.925;
	boxElem.description.displayBOX().setSize(Vector2f(window.getSize().x * 0.85, window.getSize().y * 0.925));

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
			{
				Vector2i mousePos = Mouse::getPosition(window);
				boxElem.descriptionHead.select(mousePos);
				boxElem.description.select(mousePos);
				if (backButton.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
				{
					return;
				}
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (boxElem.descriptionHead.select())
				{
					if (boxElem.descriptionHead.displayText().getGlobalBounds().width < boxElem.descriptionHead.displayBOX().getSize().x || event.text.unicode == 8) {
						boxElem.descriptionHead.reText(event.text.unicode);

						task.descriptionHead = boxElem.descriptionHead.readText();
					}

				}
				if (boxElem.description.select())
				{
					boxElem.description.reText(event.text.unicode);
					insertTextWrap(boxElem.description, window.getSize().x * 0.925);

					task.description = boxElem.description.readText();
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape) && Keyboard::isKeyPressed(Keyboard::LShift))
			{
				window.close();
			}
		}
		window.draw(background);
		window.draw(backButton);
		window.draw(boxElem.descriptionHead.displayBOX());
		window.draw(boxElem.descriptionHead.displayText());
		window.draw(boxElem.description.displayBOX());
		window.draw(boxElem.description.displayText());
		window.display();
		sleep(milliseconds(1000 / 60));
	}
}

void askWindow(RenderWindow& window, RectangleShape& askWindowPlace, RectangleShape& yesPlace, RectangleShape& cancelPlace, Text& askText, Text& yesText, Text& cancelText, bool& confirmation)
{
	//askText.setPosition(centerPos.x - askText.getGlobalBounds().width / 2, centerPos.y - 20);

	///*yesPlace.setPosition(askWindowPlace.getPosition().x + 50, askWindowPlace.getPosition().y + askWindowPlace.getSize().y - 50);
	//yesPlace.setFillColor(Color(255, 255, 255, 0));*/
	//yesText.setPosition(centerPos.x - askText.getGlobalBounds().width / 3, centerPos.y + 20);

	///*cancelPlace.setPosition(yesPlace.getPosition().x + yesPlace.getSize().x + 50, askWindowPlace.getPosition().y + askWindowPlace.getSize().y - 50);
	//cancelPlace.setFillColor(Color(255, 255, 255, 0));*/
	//cancelText.setPosition(centerPos.x + askText.getGlobalBounds().width / 10, centerPos.y + 20);


	//askWindowPlace.setFillColor(Color(255, 255, 255, 0));
	askText.setPosition(centerPos.x - askText.getGlobalBounds().width / 2, centerPos.y - 20);

	yesPlace.setPosition(centerPos.x - askText.getGlobalBounds().width / 3, centerPos.y + 20);
	yesPlace.setFillColor(Color(0, 0, 0, 0));
	yesPlace.setOutlineThickness(3);
	yesPlace.setOutlineColor(Color(255, 255, 255, 0));

	yesText.setPosition(centerPos.x - askText.getGlobalBounds().width / 4, centerPos.y + 20);

	cancelPlace.setPosition(centerPos.x + askText.getGlobalBounds().width / 10, centerPos.y + 20);
	cancelPlace.setFillColor(Color(0, 0, 0, 0));
	cancelPlace.setOutlineThickness(3);
	cancelPlace.setOutlineColor(Color(255, 255, 255, 0));
	cancelText.setPosition(centerPos.x + askText.getGlobalBounds().width / 10, centerPos.y + 20);

	//window.draw(askWindowPlace);
	/*window.draw(yesPlace);
	window.draw(cancelPlace);*/
	window.draw(askText);
	window.draw(yesText);
	window.draw(cancelText);

	while (!confirmation)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) //Mouse::getPosition(window).x, Mouse::getPosition(window).y
			{
				if (yesText.getGlobalBounds().contains(Vector2f(Mouse::getPosition())))
					confirmation = true;

				if (cancelText.getGlobalBounds().contains(Vector2f(Mouse::getPosition())))
					return;

				if (Keyboard::isKeyPressed(Keyboard::Escape) && Keyboard::isKeyPressed(Keyboard::LShift))
					window.close();
			}
			if (event.type == Event::MouseMoved)
			{
				if (yesPlace.getGlobalBounds().contains(Vector2f(Mouse::getPosition())))
					yesPlace.setOutlineColor(Color(255, 255, 255, 255));
				else
					yesPlace.setOutlineColor(Color(255, 255, 255, 0));

				if (cancelPlace.getGlobalBounds().contains(Vector2f(Mouse::getPosition())))
					cancelPlace.setOutlineColor(Color(255, 255, 255, 255));
				else
					cancelPlace.setOutlineColor(Color(255, 255, 255, 0));
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) //event.type == event.KeyPressed && event.key.code == Keyboard::Escape
			{
				return;
			}
		}
		window.draw(yesPlace);
		window.draw(cancelPlace);
		window.display();
		sleep(milliseconds(1000 / 60));
	}

}

void recalculatingPosOfField(vector <field>& box, int& headLineIndentY)
{
	for (int j = 1; j < box.size(); j++)
	{
		box[j].rectOfFieldPos = Vector2f(centerPos.x - box[j].rectOfField.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (j - 1));
	}
}

void customizationOfText(Text& text, Font& font, int characterSize, wstring str, Vector2f pos, Color color)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setString(str);
	text.setFillColor(color);
	text.setPosition(pos);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	RenderWindow window(VideoMode(1700, 900), "ToDoList");

	vector <field> box;

	Texture check, redact, del, back, bg;
	check.loadFromFile("check.png");
	del.loadFromFile("basket.png");
	back.loadFromFile("back.png");
	bg.loadFromFile("bg.jpg");
	Sprite background(bg);

	signIn(window, background);

	RectangleShape addFieldButton(Vector2f(fieldWidth, fieldHeight));
	RectangleShape plusLines(Vector2f(26, 3));
	RectangleShape headLine(Vector2f(getWindowSize()[0].width, 3));
	RectangleShape backButton(Vector2f(40, 40));
	RectangleShape askPlace(Vector2f(300, 100));
	RectangleShape yesPlace(Vector2f(75, 25));
	RectangleShape cancelPlace(Vector2f(75, 25));
	backButton.setTexture(&back);

	Text headText, addText, descriptionHeadText, descriptionText, askText, yesText, cancelText;//, InputHead, InputDescription;
	Font font;
	font.loadFromFile("times-new-roman.ttf");
	headText.setFont(font);
	headText.setString(headStr);
	headText.setCharacterSize(40);
	int headLineIndentY = headText.getGlobalBounds().height + 30;

	//head.setString(L"Список задач"); //String::fromUtf8(m.begin(), m.end())

	headLine.setFillColor(Color::Red);
	headLine.setPosition(0, headLineIndentY);

	askPlace.setPosition(centerPos.x - askPlace.getGlobalBounds().width / 2, centerPos.y - askPlace.getGlobalBounds().height / 2);

	customizationOfText(headText, font, 40, headStr, Vector2f(centerPos.x - headText.getGlobalBounds().width / 2, 0), Color::White);
	customizationOfText(addText, font, 30, addStr, Vector2f(centerPos.x - addFieldButton.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size())), Color::White);
	customizationOfText(askText, font, 20, askStr, Vector2f(0, 0), Color::White);
	customizationOfText(yesText, font, 20, yesStr, Vector2f(0, 0), Color::White);
	customizationOfText(cancelText, font, 20, cancelStr, Vector2f(0, 0), Color::White);

	plusLines.setFillColor(Color::White);
	Vector2f addFieldPos(centerPos.x - addFieldButton.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size()));
	addFieldButton.setFillColor(Color(255, 255, 255, 0));
	field addField = { addFieldButton, addFieldPos, plusLines };
	box.push_back(addField);


	vector <task> tasks;
	tasks = readTasksFromFile();



	for (int i = 0; i < tasks.size(); i++)
	{
		RectangleShape rectOfField(Vector2f(fieldWidth, fieldHeight));
		Vector2f rectOfFieldPos(centerPos.x - rectOfField.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size() - 1));
		CircleShape circle(radius);

		rectOfField.setFillColor(Color(255, 255, 255, 0));
		circle.setFillColor(Color::Black);
		circle.setTexture(&check);

		RectangleShape rectOfRedact(Vector2f(fieldHeight, fieldHeight - 10));
		rectOfRedact.setTexture(&redact);

		RectangleShape rectOfDelete(Vector2f(fieldHeight, fieldHeight - 10));
		rectOfDelete.setTexture(&del);

		Vector2f defaultBOXSize(window.getSize().x * 0.85, 35);
		Input descriptionHead = Input(centerPos.x - defaultBOXSize.x / 2, 20, defaultBOXSize.x, defaultBOXSize.y, defaultHead, tasks[i].descriptionHead);
		Input description = Input(centerPos.x - defaultBOXSize.x / 2, 90, defaultBOXSize.x, defaultBOXSize.y, defaultDescription, tasks[i].description);

		field addedElem = { rectOfField, rectOfFieldPos, rectOfDelete, circle, descriptionHead, description, tasks[i].completed };

		box.push_back(addedElem);
		checkChange(window, box[i + 1]);
		box[0].rectOfFieldPos = Vector2f(centerPos.x - addFieldButton.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size() - 1));
	}


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || (Keyboard::isKeyPressed(Keyboard::Escape) && Keyboard::isKeyPressed(Keyboard::LShift))) //event.type == event.KeyPressed && event.key.code == Keyboard::Escape
				window.close();
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{

					Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
					if (box[0].rectOfField.getGlobalBounds().contains(mousePos))  // добавление записи
					{
						task tmp = { L"", L"", false };
						tasks.push_back(tmp);
						writeTasksInFile(tasks);

						RectangleShape rectOfField(Vector2f(fieldWidth, fieldHeight));
						Vector2f rectOfFieldPos(centerPos.x - rectOfField.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size() - 1));
						CircleShape circle(radius);

						rectOfField.setFillColor(Color(255, 255, 255, 0));
						circle.setFillColor(Color::Black);
						circle.setTexture(&check);

						RectangleShape rectOfRedact(Vector2f(fieldHeight, fieldHeight - 10));
						rectOfRedact.setTexture(&redact);

						RectangleShape rectOfDelete(Vector2f(fieldHeight, fieldHeight - 10));
						rectOfDelete.setTexture(&del);

						Vector2f defaultBOXSize(window.getSize().x * 0.85, 35);
						Input descriptionHead = Input(centerPos.x - defaultBOXSize.x / 2, 20, defaultBOXSize.x, defaultBOXSize.y, defaultHead);
						Input description = Input(centerPos.x - defaultBOXSize.x / 2, 90, defaultBOXSize.x, defaultBOXSize.y, defaultDescription);

						field addedElem = { rectOfField, rectOfFieldPos, rectOfDelete, circle, descriptionHead, description };

						box.push_back(addedElem);
						box[0].rectOfFieldPos = Vector2f(centerPos.x - addFieldButton.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size() - 1));
					}
					for (int i = 1; i < box.size(); i++) // показать/скрыть галочку
					{
						bool fieldPlace = box[i].rectOfField.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
						bool circlePlace = box[i].circle.getGlobalBounds().contains(mousePos);
						bool deleteRectPlace = box[i].rectOfDelete.getGlobalBounds().contains(mousePos);

						if (circlePlace)
						{
							box[i].circlePressed = !box[i].circlePressed;

							checkChange(window, box[i]);

							tasks[i - 1].completed = !tasks[i - 1].completed;
							writeTasksInFile(tasks);
						}
						if (deleteRectPlace)
						{
							bool confirmation = true;
							//askWindow(window, askPlace, yesPlace, cancelPlace, askText, yesText, cancelText, confirmation);
							if (confirmation)
							{
								box.erase(box.begin() + i);
								recalculatingPosOfField(box, headLineIndentY);
								box[0].rectOfFieldPos = Vector2f(centerPos.x - addFieldButton.getGlobalBounds().width / 2, headLineIndentY + 20 + (fieldHeight + 10) * (box.size() - 1));
							}

							tasks.erase(tasks.begin() + i - 1);
							writeTasksInFile(tasks);
						}
						if (fieldPlace && !circlePlace && !deleteRectPlace)
						{
							outputDescription(window, background, backButton, box[i], tasks[i - 1]);
							writeTasksInFile(tasks);
						}
					}
				}

			}
		}
		window.draw(background);
		for (int i = 0; i < box.size(); i++)  // проверяет наличие курсора в box'e
		{
			if (box[i].rectOfField.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
			{
				fieldFocus(window, box[i].rectOfFieldPos);
			}
		}
		window.draw(headText);
		window.draw(headLine);
		addingTask(window, box);
		addButtonFieldElements(window, box[0], addText);
		window.display();

		sleep(milliseconds(1000 / 60));
	}
	return 0;
}