

def getLengthWithoutVowels(word):
    return sum([1 for letter in word if letter in "bcdfghjklmnpqrstvwxz"])

def strip_vowels(word):
    return "".join(letter for letter in word if letter in "bcdfghjklmnpqrstvwxz")

def strip_consonant(word):
    return "".join(letter if letter not in "bcdfghjklmnpqrstvwxz" else "*" for letter in word)

def haveCapitalLetter(word):
    return sum([1 for letter in word if letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ"]) != 0



def main():
    hist = [0 for i in range(20)]
    with open("liste.de.mots.francais.frgut.txt", "r") as infile:
        all_valid_words = []
        for line in infile:
            word = line.rstrip()

            length = getLengthWithoutVowels(word)
            hist[length] += 1
            if length >= 3 and length <= 5 and not haveCapitalLetter(word):
                all_valid_words.append(word)

    with open("valid_words.txt", "w") as outfile:
        for word in all_valid_words:
            outfile.write(word + "\n")


    print(len(all_valid_words))
    print(all_valid_words[0])
    print(hist)

if __name__ == "__main__":
    main()