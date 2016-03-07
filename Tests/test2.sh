echo "abc abc" > a.txt
echo "" > b.txt
echo "abcdefghi abcabcdedefghi" > c.txt

echo "=== Shell command ==="
cat a.txt b.txt c.txt | tr -d "[a-i" | wc -w
echo "=== Lab program ==="
./program