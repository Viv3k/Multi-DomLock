echo >./output/DomLock1.txt
for j in {1..8};do
for i in {0..4};do
./a.out 1000000 16 5 $((2**$j)) $((2**$i)) s >>./output/DomLock1.txt;
done
echo >>./output/DomLock1.txt
done
