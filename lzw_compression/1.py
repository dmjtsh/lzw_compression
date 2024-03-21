import io

with io.open('source.txt', encoding='ansi') as file:
    counter = 0
    for i in file:
        for j in i:
            counter += 1

print(counter)
