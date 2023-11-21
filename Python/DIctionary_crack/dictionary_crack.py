import sys
import os
import hashlib
import time

def crack(dict_file, hash_to_crack):
    attempts = 0
    toc = time.perf_counter()

    with open(dict_file, 'r') as fileobj:
        with open(hash_to_crack, 'r') as hashfile:
            # Read all hashed values into a list
            hashed_values = [line.strip('\n') for line in hashfile]

            for line in fileobj:
                attempts += 1
                line = line.rstrip('\n')
                m = hashlib.md5()
                m.update(line.encode('utf-8'))
                line_hash = m.hexdigest()

                # print("Checking:", line_hash)
                # print("Hashed Values:", hashed_values)

                # Compare each line from the dictionary file against the list of hashed values
                if line_hash in hashed_values:
                    tic = time.perf_counter()
                    print("------------------------------------------------------------------------")
                    print("Successfully cracked the hashed password %s. It's: %s" % (line_hash, line))
                    print("It took %s attempts to crack this hash." % attempts)
                    print("The time to execute this crack is: ", round(tic - toc, 5), "seconds.")
                    print("------------------------------------------------------------------------")
                    # Remove the cracked value from the set
                    hashed_values.remove(line_hash)

                    # Check if there are no more hashed values left
                    if not hashed_values:
                        return  # Exit the function if all values are cracked

            # Check if there are no more hashed values left
            if not hashed_values:
                return  # Exit the function if all values are cracked
            else:
                print("Failed to crack the hash.")

if __name__ == "__main__":
    print("This program will crack an MD5 hash.")
    dict_file = input("Please enter a dictionary file to use: ")
    hash_to_crack = input("Please enter an MD5 hash file to decrypt: ")
    cracked = crack(dict_file, hash_to_crack)
