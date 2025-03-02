def djb2(text):
    hash_value = 5381
    for char in text:
        hash_value = ((hash_value << 5) + hash_value) + ord(char)
    return hash_value & 0xFFFFFFFF

while True:
    user_text = input("Input text: ")
    user_size = int(input("Input table size: "))
    hash = djb2(user_text) 
    print("Hash: " + str(hash))
    print(str(hash) + " % " + str(user_size) + " = " + str(hash % user_size))


print("Ended program.")