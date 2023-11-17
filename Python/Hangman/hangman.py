# !/usr/bin/env python3
#  COMP 3005-2 FALL 2022
# Homework 7 Hangman
# Maria Orms

from itertools import count
from random import choice
from collections import Counter

def load_words(filename, length):
    try:
        Master_words =[]
        with open(filename) as f:
            for line in f:
                if len(line.strip()) ==length:
                    Master_words.append(line.strip().lower())
            return Master_words
    except FileNotFoundError:
            print("File not found")
            return []

def get_word_length():
    while True:
        try:
            word_length = int(input("What word length would you like? "))
            return word_length
        except ValueError as e:
            print("Please enter a numerical value for the word length.")

def get_input(guesses): 
    while True:
        guess = input("Enter a letter: ").lower()
        if len(guess) != 1:
            print("Enter only one letter")
        elif not guess.isalpha():
            print("That isn't a letter try again.")
        elif guess in guesses:
            print("You already guessed that.")
        else: 
            return guess

def game_over(guesses_remaining, hint):
    if guesses_remaining <= 0:
        return True
    if hint.count("-") ==0:
        return True
    return False

def mask_word(word, guessed):
    #print(f"Word = {word} and guessed = {guessed}")
    """Returns word with all letters not in guessed replaced with hyphens

    Args:
        word (str): the word to mask
        guessed (set): the guessed letters

    Returns:
        str: the masked word
    """ 
    result =""
    for character in word:
        if character in guessed:
            result += character
        else:
            result +="-"
    return result

def test_mask_word():
    # First test of Mask Word
    word = "zymurgy"
    guesses = {"y","m"}
    expected = "-ym---y"
    actual = mask_word(word, guesses)
    if expected != actual:
        print(f"Error mask_word {word}, {guesses}")
        print(f"Expected: {expected}")
        print(f"Actual: {actual}")
    #Second test of Mask Word
    word = "schnapps"
    guesses = {"p"}
    expected = "-----pp-"
    actual = mask_word(word, guesses)
    if expected != actual:
        print(f"Error mask_word {word}, {guesses}")
        print(f"Expected: {expected}")
        print(f"Actual: {actual}")
    #Third test of Mask Word
    word = "triphthong"
    guesses = {"p","h"}
    expected = "---ph-h---"
    actual = mask_word(word, guesses)
    if expected != actual:
        print(f"Error mask_word {word}, {guesses}")
        print(f"Expected: {expected}")
        print(f"Actual: {actual}")

def partition(words, guessed):
    """Generates the partitions of the set words based upon guessed

    Args:
        words (set): the word set
        guessed (set): the guessed letters

    Returns:
        dict: The partitions
    """
    partitions={}
    for word in words:
        x=mask_word(word, guessed)
        if x in partitions:
            partitions[x].append(word)
        else:
            partitions[x] = [word]
    return partitions

def get_max(d):
    max_key=d[0]
    for num in d:
        if max_key < num:
            max_key = num
    return max_key

def max_partitions(p):
    """Returns the hint for the largest partite set

    The maximum partite set is selected by selecting the partite set with
    1. The maximum size partite set
    2. If more than one maximum, prefer the hint with fewer revealed letters
    3. If there is still a tie, select randomly

    Args:
        partitions (dict): partitions from partition function

    Returns:
        str: hint for the largest partite set
    """
    #Find out the max number of words in each partite and number of sets
    counts ={}
    to_return = ("")
    max = 0
    for x in p: 
        if isinstance(p[x], list):
            counts[x] = int(len(p[x]))
            if counts[x] > max:
                max = counts[x]
    
    # Find out if multiple items in dict have same max value
    c = 0
    for val in counts.values():
        if val == max:
            c += 1
    #print(f"Max number occurs {c} times")

     #Find str to return
    check_dash=[]
    for k,v in counts.items():
        if c == 1:
            if counts[k] == max:
                to_return = (k)
                return to_return
        if c >1:
            l = k.count("-")
            check_dash.append(l)
            check_dash.sort(reverse = True)          
    z = get_max(check_dash)

    for k,v in counts.items():
        if k.count('-')==z:
            return k
        else:
            rando=choice(k)
            return rando

def test_max_partition():
    #First test to select the largest partion out of three
    part= {'----': ['jinx', 'onyx', 'quiz', 'waxy'], '---v': ['shiv'], '--v-': ['wave', 'wavy']}
    actual = max_partitions(part)
    expected = "----"
    if expected != actual:
        print()
        print(f"Error selecting max partition!")
        print(f"Expected: {expected}")
        print(f"Actual:   {actual}")

    # Second test to choose between 2 equal length partites
    part= {'---x': ['jinx', 'onyx'], '--v-': ['wave', 'wavy']}
    actual = max_partitions(part)
    expected = "---x"
    if expected != actual:
        print()
        print(f"Error selecting max partition!")
        print(f"Expected: {expected}")
        print(f"Actual:   {actual}")
    # Third test to choose between 2 unequal length partites
    part= {'----i--': ['buzzing', 'mystify'], '-o--i--': ['jogging']}
    actual = max_partitions(part)
    expected = "----i--"
    if expected != actual:
        print()
        print(f"Error selecting max partition!")
        print(f"Expected: {expected}")
        print(f"Actual:   {actual}")

   # Forth test to choose between 2 equal length partites
    part= {'-i--':['jinx'], '--i-':['quiz']}
    actual = max_partitions(part)
    expected = "-i--"
    expected2 = "--i-"
    if expected != actual:
        print()
        print(f"Error selecting max partition!")
        print(f"Expected: {expected}")
        print(f"Actual:   {actual}")

def main():
    length = get_word_length()
    cheat = length < 0
    length = abs(length)
    words = load_words("hangman_words.txt", length)
    if len(words) <1:
        print (f"No words found of length {length} found.")
        quit()
    guesses_remaining = 5
    guesses = set()
    word = (' ')*length
    hint = mask_word(word, guesses)
    if cheat:
        print(f"Potential words: {words}')")
        print(f"There are {len(words)} possible words")

    while not game_over(guesses_remaining, hint):
        print(f"You have {guesses_remaining} guesses remaining")
        print(f"Guessed letters: {guesses}")
        print(f"Current hint: {hint}")
        guess = get_input(guesses)

        #Update Game State
        guesses.add(guess)

        #Partition Word Set
        x=partition(words, guesses)
        if cheat:
            print(f"Partitions:")
            for item in x.items():
                print (item)
        
        #Select partition
        y = max_partitions(x)
        if cheat:
            print(f"Selected partition: {y}")
            
        
        words = x[y]
        next_hint=y

        if hint != next_hint:   
            print(f"Correct! {guess} is in the word!")
        else:
            print(f"I'm sorry, {guess} is not in the word!")
            guesses_remaining -=1
        hint = next_hint
        if cheat:
            print(f"Potential words: {words}')")
            print(f"There are {len(words)} possible words")


    #Game End Messages
    if hint.count("-") ==0:
        print(f"You win!  The word was {words}.")
        win_mouse()
    else:
        if len(words) > 1:
            lose_word=choice(words)
            print(f"You lose!  The word was {lose_word}.") 
        else:
            print(f"You lose!  The word was {words}.")
        lost_cat()


def win_mouse():
    print()
    print('()-() ')
    print(' \\"/')
    print('  . ')
    print()
    print("Yay!!!!  You got the Win Mouse ", end="")
    print("\U0001f600")
def lost_cat():
    print()
    print('  /\\_/\\')
    print(' ( o.o )')
    print('  > ^ <')
    print()
    print("Ohh so sad, you got the lost cat ", end="")
    print("\U0001F606")

if __name__=="__main__":
    test_mask_word()
    test_max_partition()
    main()