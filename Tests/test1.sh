echo "abc" > a.txt
echo "qwe" > b.txt
echo "aabR" > c.txt

echo "=== Shell command ==="
cat a.txt b.txt c.txt | tr -d "[a-i" | wc -w
echo "=== Lab program ==="
./program