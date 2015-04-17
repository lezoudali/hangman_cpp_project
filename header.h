#ifndef HEADER_H_
#define HEADER_H_


#define FILE_PATH "./wordlist.txt"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <algorithm>


//-----------------------------------------------CLASSES INIT----------------------------------------------------

class Player
{
public:
	void request_guess(std::string);
	char guess;
	
protected:
	bool is_letter_in_string(char, std::string);
};

class Guess
{
public:
	std::string get_remaining();
	std::string guessed;
	void set_guess(char);

private: 
	bool is_letter_in_string(char, std::string);
};

class Game
{
public:
	Game();
	bool is_word_guessed(std::string);
	int word_length;
	std::string print(std::string letters_guessed);
	std::vector<std::string> wordlist;
	bool is_guess_good(char);
	int binary_search_word();
	void choose_word();
	char hint(std::string letters_guessed);

private:
	void load_words();
	std::string secret_word;
	bool is_letter_in_string(char, std::string);
};

class AI : public Player
{
public:
	AI(int, std::vector<std::string>);
	void request_guess(std::string, std::string);
	std::vector<std::string> wordlist;
	std::string found_word;

private:
	std::vector<std::string> words;
	std::string good_guesses(std::string);
	int word_length;
	std::string guess_choices;
	bool match(std::string, std::string);	
};

//---------------------------------------------PLAYER CLASS METHODS-------------------------------------------------


bool Player::is_letter_in_string(char letter, std::string s){
	for (int i = 0; i < s.size(); ++i){
		if(s[i] == letter) return true;
	}
	return false;
}

void Player::request_guess(std::string remaining_letters){
	bool flag = true;
	while (flag) {
		std::cout << "Please choose from the remaining letters: ";
		std::cin >> guess;
		guess = tolower(guess);
		std::cout << '\n';

		if (is_letter_in_string(guess, remaining_letters))
			flag = false;
	}
}

//-------------------------------------------------GUESS CLASS METHODS----------------------------------------------

void Guess::set_guess(char letter){
	guessed += letter;
}

std::string Guess::get_remaining(){
	std::string letters = "abcdefghijklmnopqrstuvwxyz";
	std::string remaining_letters = "";
	for (int i = 0; i < letters.size(); ++i){
		if (!is_letter_in_string(letters[i], guessed)){
			remaining_letters += letters[i];
		}
	}
	return remaining_letters;
}

bool Guess::is_letter_in_string(char letter, std::string s){
	for (int i = 0; i < s.size(); ++i){
		if(s[i] == letter) return true;
	}
	return false;
}

//-----------------------------------------------------GAME CLASS METHODS---------------------------------------------

Game::Game() {
	load_words();
	bool admin = false; 
	std::string password;
	char ans;
	std::cout << "\nWould you like to login as an admin? (y/n): ";
	std::cin >> ans;
	if (ans == 'y' || ans == 'Y'){
		int i = 2; 
		while (i > 0 && !admin){
			std::cout << "admin password: "; 
			std::cin >> password;
			if (password != "bradnickles"){
				std::cout << "\nwrong password. try again.\n";
				i--;
			} else {
				admin = true;
			}
		}
	} 
	while (admin) {
		std::string word;
		std::cout << "Enter word: "; 
		std::cin >> word; 
		wordlist.push_back(word);
		std::cout << "Enter a new word? (y/n):";
		std::cin >> ans;
		if (ans != 'y')
			admin = false;
	}
	choose_word();
	word_length = secret_word.size();
}

void Game::load_words(){
	std::ifstream File(FILE_PATH, std::ifstream::in);
	if (File.good()){
		std::string line;
		while(!File.eof()) {
			getline(File, line);
			wordlist.push_back(line);
		}
	}
	File.close();
}

void Game::choose_word(){
	srand(time(NULL));
	int index = rand() % wordlist.size();
	secret_word = wordlist[index];
	sort(wordlist.begin(), wordlist.end());
}

bool Game::is_word_guessed(std::string letters_guessed){
	bool found;
	for (int i = 0; i < secret_word.size(); ++i) {
		char letter = secret_word[i];
		found = false;
		for (int j = 0; j < letters_guessed.size(); ++j){
			if (letter == letters_guessed[j]){
				found = true;
				break;
			}
		}
		if (!found){
			return false;
		}
	}
	return true;
}

bool Game::is_letter_in_string(char letter, std::string s){
	for (int i = 0; i < s.size(); ++i){
		if(s[i] == letter) return true;
	}
	return false;
}

std::string Game::print(std::string letters_guessed){
	std::string guessed_word = "";
	for (int i = 0; i < secret_word.size(); ++i){
		if (is_letter_in_string(secret_word[i], letters_guessed)){
			guessed_word += secret_word[i];
			guessed_word += ' ';
		}else{
			guessed_word += '_';
			guessed_word += ' ';
		}
	}
	return guessed_word;
}

bool Game::is_guess_good(char guess){
 if (is_letter_in_string(guess, secret_word))
 	return true;
 else
 	return false;
}

int Game::binary_search_word(){
	int mid, left = 0;
	int right = wordlist.size();
	while (left < right) {
		mid = left + (right - left)/2;
		if (secret_word > wordlist[mid]){
      left = mid+1;
    }else if (secret_word < wordlist[mid]){                                        
        right = mid;
    }else{
    	return mid;
    }
	}
	return 0;
}

char Game::hint (std::string letters_guessed){
	for (int i = 0; i < secret_word.size(); ++i) {
		if (!is_letter_in_string(secret_word[i], letters_guessed))
			return secret_word[i];
	}
	return ' ';
}



//--------------------------------------------------------AI CLASS METHOD---------------------------------------------------

AI::AI(int num, std::vector<std::string> list_of_words){
	word_length = num;
	guess_choices = ""; 
	wordlist = list_of_words;
	found_word = " ";

	for (int i = 0; i < wordlist.size(); ++i){
		if (wordlist[i].size() == word_length){
			words.push_back(wordlist[i]);
		}
	}
}

std::string AI::good_guesses(std::string print_format){
	std::string temp = ""; std::string guesses = "";
	for (int i = 0; i < print_format.size(); ++i){
		if (print_format[i] != ' ' && print_format[i] != '_' )
			temp += print_format[i];
	}

	char letter; bool duplicate = false;
	for (int i = 0; i < temp.size(); ++i){
		letter = temp[i];
		if (is_letter_in_string(letter, guesses))
			guesses += letter;
	}
	return guesses;
}

bool AI::match(std::string word, std::string print_format){
	for(int i = 0; i < word_length; ++i){
		if (print_format[i*2] != '_'){
			if (word[i] != print_format[i*2])
				return false;
		}
	}
	return true;
}

void AI::request_guess(std::string remaining_letters, std::string print_format){
	srand(time(NULL));
	std::string word;
	std::vector<std::string> temp_words;

	for (int i = 0; i < words.size(); ++i){
		if (match(words[i], print_format) ){ 
			temp_words.push_back(words[i]);
		}
	}

	if (temp_words.size() == 1){
		found_word = temp_words[0];
		//std::cout << "\nfound_word from class: " << found_word << std::endl;
	}

	guess_choices = "";
	for (int i = 0; i < temp_words.size(); ++i){
		word = temp_words[i];
		for (int j = 0; j < word.size(); ++j){
			if (is_letter_in_string(word[j], remaining_letters) && !is_letter_in_string(word[j], guess_choices))
				guess_choices += word[j];
		}
	}

	if (guess_choices == "")
		guess = remaining_letters[rand() % remaining_letters.size()];
	else
		guess = guess_choices[rand() % guess_choices.size()];
}
#endif