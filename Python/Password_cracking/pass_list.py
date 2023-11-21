from string import ascii_lowercase as letters
from itertools import product
import random, time, hashlib
from time import process_time
from passlib.hash import pbkdf2_sha256

# all possible passwords
all_pass_lex = [''.join(c) for c in product(letters, repeat=5)]
# copy and shuffle
list = all_pass_lex.copy()
random.shuffle(list)
size = len(list)
#test list
list2=["aaaaa","hiuoj"]

def sha256_crack(file_to_crack, file):
    atc, ttc, total_atc, total_ttc =0,0,0,0
    with open (file_to_crack) as f:
        for line in f:    
            st = time.process_time()
            parts = line.strip().split('$')
            salt = parts[0]
            hash_value = parts[1]
            salt_bytes = bytes.fromhex(salt)
            for password in file:
                salted_password = bytes.fromhex(salt) + password.encode()
                m=hashlib.sha256(salted_password)
                hashed_password_and_salt = m.hexdigest()
                if hash_value == hashed_password_and_salt:
                    atc = atc +1
                    print (password)
                    et = time.process_time()
                    ttc = et-st
                    #print('There were', atc, 'attempts. Execution time was:', (ttc),'seconds')
                    # print(atc)
                    # print(ttc)
                    total_atc = total_atc + atc
                    total_ttc = total_ttc + ttc
                    atc=0
                    ttc=0
                    break
                else:
                    atc = atc +1

    avg_atc = (total_atc/100)
    avg_ttc = (total_ttc/100)
    print ("Average attempts = " , avg_atc)
    print ("Average time to crack = ", avg_ttc)

def pbkdf_crack(file_to_crack, pw_file):
    atc, ttc, total_atc, total_ttc =0,0,0,0
    st = time.process_time()
    with open (file_to_crack) as f:
        for hash in f:
            for password in pw_file:
                #print ('First password to attempt:', password)
                x = pbkdf2_sha256.verify(password,hash)
                #print (x)
                if x == True:
                    atc = atc +1
                    print ("Success! - the password is:" + (password))
                    et = time.process_time()
                    ttc = et-st
                    print('There were', atc, 'attempts. Execution time was:', ttc,'seconds')
                    total_atc = total_atc + atc
                    total_ttc = total_ttc + ttc
                    atc=0
                    ttc=0
                    break
                else:
                    atc = atc +1

    avg_atc = (total_atc/100)
    avg_ttc = (total_ttc/100)
    print ("Average attempts = " , avg_atc)
    print ("Average time to crack = ", avg_ttc)

def main():
    print("Checking first SHA256 password file in random order")
    sha256_crack("sha256_hash1.txt", list)
    print("Checking Second SHA256 File in random order")
    sha256_crack("sha256_hash2.txt", list)
    print("Checking first SHA256 password file in lexigraphic order")
    sha256_crack("sha256_hash1.txt", all_pass_lex)
    print("Checking Second SHA256 password file in lexigraphic order")
    sha256_crack("sha256_hash2.txt", all_pass_lex)
    print("Checking first pbkdf_sha256 file  -Random ")
    pbkdf_crack("pbkdf2_hash1.txt", list)
    print("Checking first pbkdf_sha256 file lexigraphic order ")
    pbkdf_crack("pbkdf2_hash1.txt", all_pass_lex)
    print("Checking second pbkdf_sha256 file  -Random ")
    pbkdf_crack("pbkdf2_hash2.txt", list)
    print("Checking second pbkdf_sha256 file lexigraphic order ")
    pbkdf_crack("pbkdf2_hash2.txt", all_pass_lex)

if __name__ == "__main__":
    main()
