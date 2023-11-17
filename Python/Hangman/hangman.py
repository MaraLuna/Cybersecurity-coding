from random import choice

def load_words(filename, length):
    try:
        with open(filename) as f:
            return [i.strip().lower() for i in f if len(i.strip()) == length]
            words = []
            for line in f:
                if len(line.strip()) == length:
                    words.append(line.strip().lower())
            return words
    except FileNotFoundError:
        print("File not found")
        return []

def game_over(guesses_remaining, hint):
    if guesses_remaining <= 0:
        return True
    if hint.count("-") == 0:
        return True
    return False

def mask_word(word, guessed):
    """Returns word with all letters not in guessed replaced with hyphens

    Args:
        word (str): the word to mask
        guessed (set): the guessed letters

    Returns:
        str: the masked word
    """ 
    result = ""
    for character in word:
        if character in guessed:
            result += character
        else:
            result += "-"
    return result

def test_mask_word():
    word = "zymurgy"
    guesses = {"y", "m"}
    expected = "-ym---y"
    actual = mask_word(word, guesses)
    if expected != actual:
        print(f"Error mask_word({word}, {guesses}")
        print(f"Expected: {expected}")
        print(f"Actual: {actual}")

def read_input(guesses):
    while True:
        guess = input("Enter a letter: ").lower()

        if len(guess) != 1:
            print("Enter only one letter")
        elif not guess.isalpha():
            print("That isn't a letter, try again")
        elif guess in guesses:
            print("You already guessed that, dummy")
        else:
            return guess
        
def read_int():
    while True:
        try:
            length = int(input("What word length? "))
            return length
        except:
            print("That isn't a number, dummy")

def main():
    # Game setup
    length = read_int()
    cheat = length < 0
    length = abs(length)
    words = load_words("hangman_words.txt", length)
    if len(words) < 1:
        print("No words found of that length")
        return
    answer = choice(words)
    guesses_remaining = 5
    guesses = set()
    hint = mask_word(answer, guesses)

    if cheat:
        print(f"The answer is {answer}")

    # Game loop
    while not game_over(guesses_remaining, hint):
        # Print out game state
        print(f"You have {guesses_remaining} incorrect guesses remaining")
        print(f"Hint: {hint}")
        print(f"Guessed letters: {guesses}")

        # Get new input
        guess = read_input(guesses)

        # Update game state
        guesses.add(guess)
        next_hint = mask_word(answer, guesses)
        if hint != next_hint:
            # Correct guess
            print(f"Correct! {guess} is in the word!")
        else:
            print(f"I'm sorry, {guess} is not in the word")
            guesses_remaining -= 1
        hint = next_hint


    # Game end messages
    if hint.count("-") == 0:
        print(f"You win! The word was {answer}")
    else:
        print(f"You lose! The word was {answer}")

if __name__ == "__main__":
    test_mask_word()
    main()