//
// Created by Jakub Janak on 9/15/24.
//

#include "Controller.h"
#include "../files/FileManager.h"

#include <iostream>
#include <random>
#include <filesystem>
#include <fstream>
#include <string>

int getRandomInteger(const int from, const int to) {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution intDist(from, to);

    return intDist(rng);
}

void Controller::run() {
    std::string userInput;

    std::cout << "**********TSP Solver**********" << std::endl;
    do {
        std::cout << "Command: ";
        std::getline(std::cin, userInput);

        this->response(userInput);
    } while (userInput != "exit");
    std::cout << "Exiting the program" << std::endl;
}

void Controller::response(const std::string &userInput) {
    if (userInput == "--help") {
        this->showHelp();
    } else if (userInput == "li") {
        // loadInstance
        this->loadInstance();
    } else if (userInput == "alis") {
        // autoLoadInstances
        this->autoLoadInstances();
    } else if (userInput == "csi") {
        // createSyntheticInstance
        this->createSyntheticInstance();
    } else if (userInput == "solve") {
        this->solve();
    } else if (userInput == "exit") {
        // do nothing as the do-while loop will end
    } else {
        std::cout << "Unknown Command: " << userInput << std::endl;
    }
}

void Controller::showHelp() {
    std::cout << "showing help" << std::endl;
}

void Controller::loadInstance() {
    std::string filePath;

    std::cout << "file path: ";
    std::getline(std::cin, filePath);

    std::unique_ptr<TSInstance> tsInstance = FileManager::readDotFile(filePath);
    this->unsolvedInstances.push(std::move(*tsInstance));
}

void Controller::autoLoadInstances() {
    std::string directoryPath = "../files/instances";
    std::cout << "Loading all instances from " << directoryPath << " automatically" << std::endl;
    for (const auto &entry: FileManager::getDotInstances(directoryPath)) {
        std::unique_ptr<TSInstance> tsInstance = FileManager::readDotFile(entry.path().string());
        this->unsolvedInstances.push(std::move(*tsInstance));
    }
}

void Controller::createSyntheticInstance() {
    std::string userInput;

    std::cout << "number of nodes: ";
    std::getline(std::cin, userInput);
    int numOfNodes = std::stoi(userInput);

    std::vector<std::unique_ptr<Node> > nodes;
    std::vector<std::unique_ptr<Edge> > edges;
    nodes.reserve(numOfNodes);
    for (int i = 0; i < numOfNodes; i++) {
        nodes.emplace_back(std::make_unique<Node>(std::to_string(i)));
    }

    long counter = 0;
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < nodes.size(); j++) {
            if (i < j) {
                auto edge1 = std::make_unique<Edge>(nodes[i].get(), nodes[j].get(), getRandomInteger(1, 10));
                nodes[i]->addEdge(edge1.get());
                auto edge2 = std::make_unique<Edge>(nodes[j].get(), nodes[i].get(), getRandomInteger(1, 10));
                nodes[j]->addEdge(edge2.get());
                edges.push_back(std::move(edge1));
                edges.push_back(std::move(edge2));
                counter++;
            }
        }
    }

    this->unsolvedInstances.emplace(std::move(nodes), std::move(edges));
}

void Controller::solve() {
    while (!this->unsolvedInstances.empty()) {
        TSInstance &instance = this->unsolvedInstances.front();
        instance.solve();
        instance.printStatistics();
        std::string userInput;
        std::cout << "Save the result graph[y for yes]: ";
        std::getline(std::cin, userInput);
        if (userInput == "y") {
            std::cout << "Name of the file: ";
            std::getline(std::cin, userInput);
            instance.saveAs(userInput);
        }
        this->unsolvedInstances.pop();
    }
}