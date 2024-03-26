/*
Program care simuleaza functionarea unui translator finit nedeterminist cu lambda-tranziții.
Programul citește (dintr-un fișier sau de la consolă) elementele unui translator
finit nedeterminist cu lambda-tranziții oarecare (starile, starea initiala, starile finale,
alfabetul de intrare, alfabetul de iesire, tranzitiile). Programul permite citirea unui nr
oarecare de siruri peste alfabetul de intrarea al translatorului. Pentru fiecare astfel de sir
se afiseaza toate iesirile (siruri peste alfabetul de iesire) corespunzatoare (Atentie! pot
exista 0, 1 sau mai multe iesiri pentru acelasi șir de intrare).

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

class Transition {
private:
    char symbol;
    int nextState;
    char value;

public:
    Transition(char sym, int next, char value) : symbol(sym), nextState(next), value(value) {}

    char getSymbol() const { return symbol; }
    int getNextState() const { return nextState; }
    char getValue() const { return value; }

    bool operator==(const Transition& other) const {
        return symbol == other.symbol && nextState == other.nextState;
    }

    bool operator<(const Transition& other) const {
        return symbol < other.symbol || (symbol == other.symbol && nextState < other.nextState);
    }
};

class State {
private:
    set<Transition> transitions;

public:
    State() {}
    void addTransition(const Transition& transition) {
        transitions.insert(transition);
    }
    const set<Transition>& getTransitions() const {
        return transitions;
    }
};

class NFA {
private:
    set<int> states;
    set<char> inputAlphabet;
    set<char> outputAlphabet;
    map<int, State> stateTransitions;
    int initialState;

public:
    void readFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            if (line.find("states:") != string::npos) {
                parseStates(line);
            } else if (line.find("initial state:") != string::npos) {
                parseInitialState(line);
            } else if (line.find("input alphabet:") != string::npos) {
                parseInputAlphabet(line);
            } else if (line.find("output alphabet:") != string::npos) {
                parseOutputAlphabet(line);
            } else if (line.find("transitions:") != string::npos) {
                parseTransitions(file);
            }
        }

        file.close();
    }

    void parseStates(const string& line) {
        int pos = line.find(":");
        string stateList = line.substr(pos + 1);
        int state;
        for (char c : stateList) {
            if (isdigit(c)) {
                state = c - '0';
                states.insert(state);
            }
        }
    }

    void parseInitialState(const string& line) {
        int pos = line.find(":");
        string stateStr = line.substr(pos + 1);
        initialState = stoi(stateStr);
    }

    void parseInputAlphabet(const string& line) {
        int pos = line.find(":");
        string alphabet = line.substr(pos + 1);
        for (char c : alphabet) {
            inputAlphabet.insert(c);
        }
    }

    void parseOutputAlphabet(const string& line) {
        int pos = line.find(":");
        string alphabet = line.substr(pos + 1);
        for (char c : alphabet) {
            outputAlphabet.insert(c);
        }
    }

    void parseTransitions(ifstream& file) {
        string line;
        while (getline(file, line)) {
            if (line.empty())
                break;
            int fromState = line[0] - '0';
            char symbol = line[2];
            int toState = line[4] - '0';
            char value = line[6];
            Transition transition(symbol, toState, value);
            stateTransitions[fromState].addTransition(transition);
        }
    }

    set<int> getStates() const { return states; }

    set<char> getInputAlphabet() const { return inputAlphabet; }

    set<char> getOutputAlphabet() const { return outputAlphabet; }

    map<int, State> getStateTransitions() const { return stateTransitions; }

    int getInitialState() const { return initialState; }

    vector<string> simulate(const vector<string>& inputs) const {
        vector<string> outputs;
        for (const string& input : inputs) {
            string currentOutput;
            simulateHelper(initialState, input, 0, currentOutput, outputs);
        }
        return outputs;
    }

private:
    void simulateHelper(int currentState, const string& input, int index, string currentOutput, vector<string>& outputs) const {
        if (index == input.length()) {
            outputs.push_back(currentOutput);
            return;
        }

        char symbol = input[index];
        cout << "Current State: " << currentState << ", Input Symbol: " << symbol << endl;

        for (const Transition& transition : stateTransitions.at(currentState).getTransitions()) {
            if (transition.getSymbol() == symbol) {
                cout << "Transition: " << currentState << " -> " << transition.getNextState() << ", Value: " << transition.getValue() << endl;
                simulateHelper(transition.getNextState(), input, index + 1, currentOutput, outputs);
            }
        }

        // Add separator after processing each input
        if (index == input.length() - 1) {
            cout << "--------------------------------------------------------------" << endl;
        }
    }
};

int main() {
    NFA nfa;
    nfa.readFromFile("nfa_input.txt");

    // Example input strings
    vector<string> inputs = {"101100", "110001"};

    vector<string> outputs = nfa.simulate(inputs);

    // * == lambda
    // Trebuie creat un fisier nfa_input.txt cu urmatoarea structura:
    /*
    states:0,1,2,3
    initial state:0
    input alphabet:0,1
    output alphabet:0,1,2,3
    transitions:
    0 1 1 1
    0 1 3 *
    1 0 2 *
    1 1 3 *
    2 0 1 0
    2 1 1 1
    3 0 1 2
    3 1 1 3
*/

    return 0;
}
