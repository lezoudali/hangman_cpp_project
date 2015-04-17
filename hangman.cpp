#include "header.h"

using namespace std;

bool single_player_mode(Player, Guess, Game);
void ai_mode(Player, AI, Guess, Game);
void draw(int);


int main(){
	Player player;
	Guess guess;
	Game game;
	AI computer(game.word_length, game.wordlist);
	char mode = '-'; 
	cout << "Enter 'A' for AI mode or 'S' for single mode: ";
	cin >> mode;

	if (mode == 'A' || mode == 'a'){

		ai_mode(player, computer, guess, game);

	}else{

		bool new_game = true;
		bool won = false;
		char retry;
		while (new_game){
			won = single_player_mode(player, guess, game);
			if (won){
				cout << "\nEnter 'Y' to play again or any other character to quit: ";
				cin >> retry;
				retry = tolower(retry);
				cout << endl;
				if (retry != 'y'){ 
					new_game = false;
				}else {
					game.choose_word();
				}
			}else {
				char retry;
				cout << "\nEnter 'R' to retry. Enter 'Y' to retry with a new secret code or any other character to quit: ";
				cin >> retry;
				retry = tolower(retry);
				if (retry == 'r'){
					new_game = true;
				} 
				else if (retry == 'y'){
					game.choose_word();;
				}
				else {
					cout << "\nGoodbye and thanks for playing :)" <<endl;
					new_game = false;
				}
			}
	  }
	}
}

bool single_player_mode(Player player, Guess guess, Game game){
	int num_tries = 7;
	bool game_won = false;

  cout << "\nWelcome to Hangman!" << endl;
  cout << "I'm thinking of a word that's " << game.word_length << " letters long." << endl;
  //cout << "secret_word: " << game.secret_word << endl;
  cout << endl;
  draw(num_tries);
  cout << endl;

  while(num_tries > 0 && !game_won){
  	cout << "-----------\n" << endl;
    if (num_tries > 1)
    	cout << "You have " << num_tries << " guesses left."<< endl;
    else
    	cout << "You have " << num_tries << " guess left."<< endl;

    string remaining_letters = guess.get_remaining();
    cout << "remaining letters: " << remaining_letters << endl;
    if (num_tries < 5 && num_tries > 1) {
   		char ans; char hint;
    	cout << "\nWould like a hint (-2 lives)? (y/n): ";
    	cin >> ans;
    	if (ans == 'y' || ans == 'Y'){
    		hint = game.hint(guess.guessed);
    		if (hint != ' '){
    			cout << "\nHint: " << hint << endl;
    			num_tries -= 2;
    		}else{
    			cout << "\nSorry, couldn't generate a hint"<< endl;
    		}
    	}
    }
    player.request_guess(remaining_letters);
    guess.set_guess(player.guess);
    cout << "\t" << game.print(guess.guessed) << '\n' << endl;

    if (game.is_guess_good(player.guess)){
    	cout << "\n\tGood guess :).\n" << endl;
			if (game.is_word_guessed(guess.guessed)){
				game_won = true;
			}
		}else {
			cout << endl;
			draw(num_tries);
			cout << "\n\tThat letter is not in my word.\n"<< endl;
	    num_tries--;
	  }
  }
	if (game_won) {
  	cout << "\nCongratulations. You've Won!!! " << endl;
  	cout << "secret word index in alphabetical order: " << game.binary_search_word () << endl;
  	return true;
  }
  cout << "secret word index: " << game.binary_search_word () << endl;
  cout << "\nYou ran out of tries :(. Thanks for playing!" << endl;
  cout << "\nsecret word index in alphabetical order: " << game.binary_search_word () << endl;
  return false;
}


void ai_mode(Player player, AI computer, Guess guess, Game game){
	bool game_won = false;
	bool player_won = false;
  cout << "\n\nWelcome to Hangman!" << endl;
  cout << "I'm thinking of a word that's " << game.word_length << " letters long." << endl;
  // cout << "secret_word: " << game.secret_word << endl;

  while(!game_won){
  	cout << "\n\n-----------\n" << endl;

    string remaining_letters = guess.get_remaining();
    cout << "remaining letters: " << remaining_letters << endl;
    player.request_guess(remaining_letters);
    guess.set_guess(player.guess);

    cout << "Player: \n\n\t";
    cout << game.print(guess.guessed);

    if (game.is_guess_good(player.guess)){
    	cout << "\n\n\tGood guess :).\n" << endl;
			if (game.is_word_guessed(guess.guessed)){
				game_won = true;
				player_won = true;
				break;
			}
		}else {
			cout << endl;
			cout << "\n\tThat letter is not in my word.\n"<< endl;
	  }
	  
	  if (game.is_word_guessed(computer.found_word)){
	  	cout << "AI guessed word: " << computer.found_word << endl;
	  	game_won = true;
	  	player_won = false;
	  } else {
	  	computer.request_guess(remaining_letters, game.print(guess.guessed));
	  	guess.set_guess(computer.guess);
		  cout << "AI: \n\n\t";
	    cout << game.print(guess.guessed);
	    cout << "\n\n\t The AI guessed: " << computer.guess << endl;

	    if (game.is_guess_good(computer.guess)){
				if (game.is_word_guessed(guess.guessed)){
					game_won = true;
					player_won = false;
				}
			}else {
				cout << endl;
				cout << "\tThat letter is not in my word.\n"<< endl;
		  }

	  }
		  
  }

	if (player_won) 
  	cout << "\nCongratulations. You've Won!!! " << endl;
  else
  	cout <<"\nThe AI won. Thanks for playing" << endl;

  cout << "\nsecret word index in alphabetical order: " << game.binary_search_word () << endl;

}

void draw(int num_tries){
	switch(num_tries){
		case 0: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |    O"    <<endl;
						cout << "   |   -|-"   <<endl;
						cout << "   |   / \\"  <<endl;
						cout << "___|\\___"    <<endl;
						break;

		case 1: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |    O"    <<endl;
						cout << "   |   -| "   <<endl;
						cout << "   |   / \\"  <<endl;
						cout << "___|\\___"   <<endl;
						break;
		case 2: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |    O"    <<endl;
						cout << "   |    | "   <<endl;
						cout << "   |   / \\"  <<endl;
						cout << "___|\\___"   <<endl;
						break;
		case 3: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |    O"    <<endl;
						cout << "   |    | "   <<endl;
						cout << "   |     \\"  <<endl;
						cout << "___|\\___"   <<endl;
						break;
		case 4: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |    O"    <<endl;
						cout << "   |    | "   <<endl;
						cout << "   |      "  <<endl;
						cout << "___|\\___"   <<endl;
						break;
		case 6: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |    O"    <<endl;
						cout << "   |      "   <<endl;
						cout << "   |      "  <<endl;
						cout << "___|\\___"   <<endl;
						break;
		case 7: cout << "   _____"     <<endl;
						cout << "   |/   |"    <<endl;
						cout << "   |     "    <<endl;
						cout << "   |     "   <<endl;
						cout << "   |     "  <<endl;
						cout << "___|\\___"   <<endl;
						break;
		case 8: cout << "   _____ "     <<endl;
						cout << "   |/    "    <<endl;
						cout << "   |     "    <<endl;
						cout << "   |     "   <<endl;
						cout << "   |     "  <<endl;
						cout << "___|\\___"   <<endl;
						break;
	}
}
