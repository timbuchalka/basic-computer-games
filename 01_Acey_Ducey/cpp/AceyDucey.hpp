#pragma once

#include <array>
#include <string>
#include <string_view>
#include <vector>
#include <random>

/**
 * @brief The AceyDucey class encapsulates the core game logic for the Acey Ducey card game.
 *
 * This class manages the player's balance, game state, and game loop, and provides
 * all supporting methods for playing rounds and interacting with the user.
 */
class AceyDucey {
public:
  /**
   * @brief Constructs a new AceyDucey game and initializes balance and deck.
   */
  AceyDucey();

  /**
   * @brief Starts the main game loop.
   */
  void run();

private:
  enum class State { Initialising, Playing, BetNothing, GameOver };

  int balance;                              ///< Player's current balance
  State state;                              ///< Current game state
  std::vector<std::string_view> deck;       ///< Working deck of card ranks
  std::default_random_engine rng;           ///< Random number generator

  static constexpr std::array<std::string_view, 13> CARDS{
    "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"
  };

  // Game logic
  void play_turn();
  bool is_game_over();

  // Card handling
  std::string_view deal_card();
  static bool is_between(std::string_view a, std::string_view b, std::string_view test);
  void print_cards(std::string_view a, std::string_view b);

  // I/O
  void print_intro();
  void print_instructions();
  std::string get_input_line();
  int get_positive_integer(const std::string& s);
};