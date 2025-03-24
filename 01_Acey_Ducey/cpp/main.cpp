#include <__algorithm/ranges_all_of.h>
#include <__algorithm/ranges_find.h> // for std::ranges::find
#include <array>
#include <iostream>
#include <print>  // for std::print / std::println
#include <random> // for random number generation
#include <string_view>
#include <vector>

/**
 * @brief The Player class encapsulates player state for the Acey Ducey game.
 *
 * This class maintains a virtual deck of card ranks and the player's balance.
 * It supports dealing random cards and evaluating whether a card is
 * strictly between two others in rank, according to game rules.
 */
class AceyDucey {

private:
  enum class State { Initialising, Playing, GameOver };

  // Player's current in-game balance in dollars.
  int balance;

  // Game state
  State state;

  // Fixed array of card ranks in ascending order.
  // Used for both display and rank comparisons.
  static constexpr std::array<std::string_view, 13> CARDS{
      "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

  // A working copy of the deck containing only ranks (no suits in this game).
  std::vector<std::string_view> deck;

  // Pseudo-random number generator, seeded with non-deterministic entropy.
  std::default_random_engine rng{std::random_device{}()};

public:
  /**
   * @brief Constructs a new Player with a default balance and a fresh deck.
   *
   * Initializes the player's balance to $100 and creates a vector copy
   * of the constant card rank array for use in random draws.
   */
  AceyDucey()
      : balance(100), deck(std::ranges::begin(CARDS), std::ranges::end(CARDS)),
        state(State::Initialising) {
    run();
  }

  /**
   * @brief Prints the Acey Ducey game banner and title screen.
   */
  void print_intro() {
    std::println("{:^66}", "ACEY DUCEY CARD GAME");
    std::println("{:^66}", "CREATIVE COMPUTING  MORRISTOWN, NEW JERSEY");
    std::println("");
  }

  /**
   * @brief Prints game instructions for how to play Acey Ducey.
   */
  void print_instructions() {
    std::println("");
    std::println("ACEY-DUCEY IS PLAYED IN THE FOLLOWING MANNER");
    std::println("THE DEALER (COMPUTER) DEALS TWO CARDS FACE UP");
    std::println("YOU HAVE AN OPTION TO BET OR NOT BET DEPENDING");
    std::println("ON WHETHER OR NOT YOU FEEL THE CARD WILL HAVE");
    std::println("A VALUE BETWEEN THE FIRST TWO.");
    std::println("");
  }

  /**
   * @brief Selects a random card from the current deck.
   *
   * Picks a random rank from the deck vector using a uniform distribution.
   * Since this version of the game only uses card ranks (no suits),
   * duplicates are possible, and no cards are removed.
   *
   * @return A randomly selected card rank as a std::string_view.
   */
  std::string_view deal_card() {
    std::uniform_int_distribution<size_t> dist(0, deck.size() - 1);
    return deck[dist(rng)];
  }

  /**
   * @brief Determines if a given card is strictly between two other cards in
   * rank.
   *
   * The function compares the position of three cards using their rank order,
   * as defined in the CARDS array. The third card is considered
   * "between" only if its rank is strictly greater than the lower card
   * and strictly less than the higher card. Equality is not considered
   * in-range.
   *
   * The order of the first two cards does not matter.
   *
   * @param a The first card (rank), e.g. "4"
   * @param b The second card (rank), e.g. "K"
   * @param test The third card (rank) to evaluate, e.g. "9"
   * @return true if test is strictly between a and b in rank; false otherwise
   */
  static bool is_between(std::string_view a, std::string_view b,
                         std::string_view test) {
    // Looks up the index of a rank in the CARDS array.
    auto index_of = [](std::string_view value) -> int {
      auto const it = std::ranges::find(CARDS, value);
      return static_cast<int>(std::distance(CARDS.begin(), it));
    };

    int idx_a = index_of(a);
    int idx_b = index_of(b);
    int const idx_test = index_of(test);

    // Ensure idx_a is always the lower index
    if (idx_a > idx_b)
      std::swap(idx_a, idx_b);

    // Return true if test is strictly between a and b
    return idx_test > idx_a && idx_test < idx_b;
  }

  /**
   * @brief Main game loop that manages state transitions and gameplay flow.
   */
  void run() {
    while (state != State::GameOver) {
      switch (state) {
      case State::Initialising:
        print_intro();
        print_instructions();
        state = State::Playing;
        break;

      case State::Playing:
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
   * @brief Executes a single round of gameplay logic.
   */
  void play_turn() {
    std::println("You have ${} dollars", balance);
    std::println("Here are your next two cards:");
    std::string_view first_pick = deal_card();
    std::string_view second_pick = deal_card();
    print_cards(first_pick, second_pick);
    std::print("How much do you want to bet? ");
    if (is_positive_integer(get_input_line())) {
      std::println("Positive integer entered");
    } else {
      std::println("Invalid input. Please enter a positive integer.");
    }
  }

  /**
   * @brief Displays two card ranks in ascending order.
   *
   * @param a First card rank
   * @param b Second card rank
   */
  void print_cards(std::string_view a, std::string_view b) {
    std::string_view card1 = a;
    std::string_view card2 = b;
    // Ensure card1 is always the lower card
    if (card1 > card2) {
      std::swap(card1, card2);
    }

    std::println("{} {}", card1, card2);
  }

  /**
   * @brief Reads a line of user input from standard input.
   *
   * Trims leading and trailing whitespace from the input string.
   *
   * @return A trimmed std::string containing the user's input.
   */
  std::string get_input_line() {
    std::string line;
    std::getline(std::cin, line);

    // Trim leading/trailing whitespace
    auto is_not_space = [](unsigned char ch) { return !std::isspace(ch); };
    line.erase(line.begin(),
               std::find_if(line.begin(), line.end(), is_not_space));
    line.erase(std::find_if(line.rbegin(), line.rend(), is_not_space).base(),
               line.end());

    return line;
  }

  /**
   * @brief Checks if a string contains only digits (positive integer).
   *
   * Leading zeros (e.g. "0005") are allowed.
   * Empty strings or strings with non-digit characters will return false.
   *
   * @param s The string to check.
   * @return true if the string is a positive integer; false otherwise.
   */
  bool is_positive_integer(const std::string &s) {
    return !s.empty() && std::ranges::all_of(s, ::isdigit);
  }

  /**
   * @brief Checks if the player's balance has dropped to zero.
   *
   * @return true if the game is over; false otherwise.
   */
  bool is_game_over() { return balance <= 0; }
};

/**
 * @brief Entry point for the Acey Ducey game.
 *
 * Creates a game instance and begins execution.
 */
int main() {
  AceyDucey game;
  game.run();
}