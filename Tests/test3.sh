echo "w" > a.txt
rm b.txt
rm c.txt

echo "=== Shell command ==="
cat a.txt b.txt c.txt | tr -d "[a-i" | wc -w
echo "=== Lab program ==="
./program