#include "AceyDucey.hpp"
#include <__algorithm/ranges_find.h>
#include <__algorithm/ranges_all_of.h>
#include <iostream>
#include <print>
#include <algorithm>
#include <cctype>

/**
 * @brief Constructs a new AceyDucey game instance.
 *
 * Initializes the player's balance to $100, copies the static CARDS
 * array into the deck, seeds the random number generator, and sets the
 * initial game state to Initialising.
 */
AceyDucey::AceyDucey()
  : balance(100),
    deck(std::ranges::begin(CARDS), std::ranges::end(CARDS)),
    rng(std::random_device{}()),
    state(State::Initialising) {}

/**
 * @brief Starts the main game loop, handling state transitions.
 */
void AceyDucey::run() {
  while (state != State::GameOver) {
    switch (state) {
      case State::Initialising:
        print_intro();
        print_instructions();
        state = State::Playing;
        break;

      case State::Playing:
      case State::BetNothing:
        play_turn();
        if (is_game_over()) {
          state = State::GameOver;
        }
        break;

      case State::GameOver:
        std::println("GAME OVER. Thanks for playing!");
        break;
    }
  }
}

/**
 * @brief Prints the game title and attribution message.
 */
void AceyDucey::print_intro() {
  std::println("{:^66}", "ACEY DUCEY CARD GAME");
  std::println("{:^66}", "CREATIVE COMPUTING  MORRISTOWN, NEW JERSEY");
}

/**
 * @brief Displays gameplay instructions to the user.
 */
void AceyDucey::print_instructions() {
  std::println("");
  std::println("ACEY-DUCEY IS PLAYED IN THE FOLLOWING MANNER");
  std::println("THE DEALER (COMPUTER) DEALS TWO CARDS FACE UP");
  std::println("YOU HAVE AN OPTION TO BET OR NOT BET DEPENDING");
  std::println("ON WHETHER OR NOT YOU FEEL THE CARD WILL HAVE");
  std::println("A VALUE BETWEEN THE FIRST TWO.");
}

/**
 * @brief Returns a randomly selected card rank from the deck.
 *
 * @return A card rank (e.g., "5", "J", "A")
 */
std::string_view AceyDucey::deal_card() {
  std::uniform_int_distribution<size_t> dist(0, deck.size() - 1);
  return deck[dist(rng)];
}

/**
 * @brief Checks whether a card rank lies strictly between two other ranks.
 *
 * @param a The first card (can be lower or higher)
 * @param b The second card (can be lower or higher)
 * @param test The card to test
 * @return true if test is strictly between a and b; false otherwise
 */
bool AceyDucey::is_between(std::string_view a, std::string_view b, std::string_view test) {
  auto index_of = [](std::string_view value) -> int {
    auto it = std::ranges::find(CARDS, value);
    return static_cast<int>(std::distance(CARDS.begin(), it));
  };

  int idx_a = index_of(a);
  int idx_b = index_of(b);
  int idx_test = index_of(test);

  if (idx_a > idx_b) std::swap(idx_a, idx_b);

  return idx_test > idx_a && idx_test < idx_b;
}

/**
 * @brief Plays one round of the game.
 *
 * Displays the player's balance, deals two cards,
 * prompts for a bet, evaluates the third card,
 * and updates the balance and game state.
 */
void AceyDucey::play_turn() {
  if (state == State::Playing) {
    std::println("YOU NOW HAVE ${} DOLLARS", balance);
  }

  std::println("HERE ARE YOUR NEXT TWO CARDS:");
  std::string_view first_pick = deal_card();
  std::string_view second_pick = deal_card();
  print_cards(first_pick, second_pick);

  std::print("WHAT IS YOUR BET ");
  int bet = get_positive_integer(get_input_line());

  if (bet <= 0) {
    std::println("CHICKEN!!");
    state = State::BetNothing;
    return;
  } else if (bet > balance) {
    std::println("SORRY, MY FRIEND, BUT YOU BET TOO MUCH.");
    std::println("YOU HAVE ONLY {} DOLLARS TO BET.", balance);
  } else {
    std::string_view third_pick = deal_card();
    std::println("{}", third_pick);

    if (is_between(first_pick, second_pick, third_pick)) {
      std::println("YOU WIN!!!");
      balance += bet;
    } else {
      std::println("SORRY, YOU LOSE");
      balance -= bet;
      if (balance <= 0) {
        std::println("SORRY, FRIEND, BUT YOU BLEW YOUR WAD.");
        std::print("TRY AGAIN (YES OR NO)? ");
        std::string response = get_input_line();
        if (response == "YES") {
          state = State::Playing;
          balance = 100;
        } else {
          state = State::GameOver;
        }
      }
    }
  }

  state = State::Playing;  // reset state at end of normal round
}

/**
 * @brief Prints two card ranks in ascending order.
 *
 * @param a First card
 * @param b Second card
 */
void AceyDucey::print_cards(std::string_view a, std::string_view b) {
  auto index_of = [](std::string_view value) -> int {
    auto it = std::ranges::find(CARDS, value);
    return it != CARDS.end() ? std::distance(CARDS.begin(), it) : -1;
  };

  int idx_a = index_of(a);
  int idx_b = index_of(b);

  if (idx_a == -1 || idx_b == -1) {
    std::cerr << "Error: Invalid card rank(s)." << std::endl;
    return;
  }

  if (idx_a > idx_b) {
    std::swap(idx_a, idx_b);
    std::swap(a, b);
  }

  std::println("{} {}", a, b);
}

/**
 * @brief Reads and returns a line of input from the user.
 *
 * Trims whitespace and converts to uppercase.
 *
 * @return Sanitized input string.
 */
std::string AceyDucey::get_input_line() {
  std::string line;
  std::getline(std::cin, line);

  auto is_not_space = [](unsigned char ch) { return !std::isspace(ch); };
  line.erase(line.begin(), std::find_if(line.begin(), line.end(), is_not_space));
  line.erase(std::find_if(line.rbegin(), line.rend(), is_not_space).base(), line.end());

  std::transform(line.begin(), line.end(), line.begin(),
                 [](unsigned char c) { return std::toupper(c); });

  return line;
}

/**
 * @brief Attempts to convert a string to a positive integer.
 *
 * @param s Input string
 * @return Parsed integer if valid, -1 otherwise
 */
int AceyDucey::get_positive_integer(const std::string &s) {
  if (!s.empty() && std::ranges::all_of(s, ::isdigit)) {
    return std::stoi(s);
  }
  return -1;
}

/**
 * @brief Determines whether the game is over due to lack of funds.
 *
 * @return true if balance is zero or below; false otherwise.
 */
bool AceyDucey::is_game_over() {
  return balance <= 0;
}