#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include "configTask.h"

using namespace std;

vector<task> readTasksFromFile() {
	string jsonTasks;

	ifstream file("tasks.json");

	if (!file.is_open()) {
		cout << "Error, file 'tasks.json' not readed" << endl;
	}

	getline(file, jsonTasks);
	file.close();
	vector<task> tasks;

	nlohmann::json j = nlohmann::json::parse(jsonTasks);

	task tmp;
	for (int i = 0; i < j.size(); i++) {
		tmp.descriptionHead = j[i]["descriptionHead"].get<wstring>();
		tmp.description = j[i]["description"].get<wstring>();
		tmp.completed = j[i]["completed"].get<bool>();

		tasks.push_back(tmp);

	}
	return tasks;
}

void writeTasksInFile(vector<task> tasks) {

	nlohmann::json j{};

	for (int i = 0; i < tasks.size(); i++) {
		j[i]["descriptionHead"] = tasks[i].descriptionHead;
		j[i]["description"] = tasks[i].description;
		j[i]["completed"] = tasks[i].completed;
	}

	string jsonString = j.dump();

	ofstream file;
	file.open("tasks.json");

	if (!file.is_open()) {
		cout << "Error, file 'tasks.json' not writen" << endl;
	}

	file << jsonString;
	file.close();
}