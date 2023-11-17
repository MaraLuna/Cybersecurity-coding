# !/usr/bin/env python3


from random import shuffle, randint
from statistics import mean
from unittest import case

from pyrfc3339 import generate

def generate_decks():
    deck=[]
    for face in range(2,15):
        for suit in "DHSC":
            deck.append((face,suit))
    shuffle(deck)
    return deck[:26], deck[26:]

def play_hand(p1_deck, p2_deck):
    #print (f"Player 1 Hand is {p1_deck}")
    winner = "neither"
    common_pool = []
    
    while winner == "neither":
        if len(p1_deck) > 0 and len(p2_deck) > 0:
            p1_top=p1_deck[0][0]
            p2_top=p2_deck[0][0]
            # print (f"Player 1 top is {p1_top}")
            # print (f"Player 2 top is {p2_top}")
            if p1_top > p2_top:
                winner = "player1"
            elif p2_top > p1_top:
                winner = "player2"
            elif p1_top == p2_top:
                winner="neither"
            common_pool.append(p1_deck.pop(0))
            common_pool.append(p2_deck.pop(0))
            #print(f"Commmon pool is: {common_pool}")
            shuffle(common_pool)
            match winner:
                case "player1":
                    p1_deck.extend(common_pool)
                    common_pool=[]
                    return winner
                case "player2":
                    p2_deck.extend(common_pool)
                    common_pool=[]
                    return winner
                case "neither":
                    continue
        else:
            return winner

                
def test_play_hand_one_set():
    player1 =[(13,"H")]
    player2 = [(5, "H")]

    if play_hand(player1, player2) != 1:
        print ("Expected player1 to win!")

def test_play_hand_multiple_cards():
    player1 =[(4,"H"),(12,"H")]
    player2 = [(13, "H"),(3,"H")]
    while len(player1) != 0 and len(player2)!=0:
        play_hand(player1, player2)


def play_game():
    p1,p2 = generate_decks()
    count =0
    while len(p1) > 0 and len(p2) > 0:
        winner = play_hand(p1,p2)
        count +=1
    print (f"Winner is {winner}.")
    return count

def main():
    num_trials=1000
    hands=[]
    for i in range (num_trials):
        print(f"Game #{i+1}:")
        count = play_game()
        print (f"Took {count} hands.")
        print()
        hands.append(count)
    mean_hands=mean(hands)
    print(f"Mean: {mean_hands}")
        
main()


